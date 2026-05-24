#!/bin/bash
# Index OPTD POR data into the inactive deployment slot (Xapian + SQL).

LOCK_DIR="/tmp/opentrep/lock"
LOCK_FILE="${LOCK_DIR}/opentrep-index.lock"
mkdir -p "${LOCK_DIR}"

VIEWPY="/var/www/webapps/search/app.py"
TREP_DIR="/var/www/webapps/opentrep/trep"
TREP_DATA_DIR="${TREP_DIR}/share/opentrep/data/por"
LOG_DIR="/var/log/webapps/search"

depnb_max=2
depnb_from=$(grep "^deploymentNb = " ${VIEWPY} | cut -d'=' -f2 | tr -d ' ')
depnb_from=$((0 + depnb_from))
depnb_to=$(( (1 + depnb_from) % depnb_max ))
POR_FILE="${TREP_DATA_DIR}/optd_por_public_${depnb_to}.csv"

# SQL backend config per slot
SQL_TYPE_0="sqlite"
SQL_CONN_0="${TREP_DIR}/sqlite_travel.db"
SQL_TYPE_1="pg"
SQL_CONN_1="dbname=trep_trep user=trep password=trep host=localhost"

eval SQL_TYPE=\$SQL_TYPE_${depnb_to}
eval SQL_CONN=\$SQL_CONN_${depnb_to}

# Whether to include non-IATA POR (default: yes for all.csv)
INC_NO_IATA=1
if [ "$1" = "--iata-only" ]; then INC_NO_IATA=0; fi

if [ "$1" = "-h" ] || [ "$1" = "--help" ]; then
    echo "Usage: $0 [--iata-only]"
    echo "  Current slot : ${depnb_from}"
    echo "  Target slot  : ${depnb_to}"
    echo "  POR file     : ${POR_FILE}"
    echo "  SQL backend  : ${SQL_TYPE} / ${SQL_CONN}"
    exit 0
fi

if [ -f "${LOCK_FILE}" ]; then
    echo "Another indexing process is already running ($(cat ${LOCK_FILE})). Aborting."
    exit 1
fi
echo "${depnb_to}^$$" > "${LOCK_FILE}"

if [ ! -f "${POR_FILE}" ]; then
    echo "POR file not found: ${POR_FILE}"
    echo "Run ~/bin/trep-update-por-data.sh first."
    rm -f "${LOCK_FILE}"
    exit 1
fi

mkdir -p "${TREP_DIR}/traveldb${depnb_to}"

echo
echo "Indexing slot ${depnb_to} (current: ${depnb_from})"
echo "  POR file    : ${POR_FILE}"
echo "  Xapian DB   : ${TREP_DIR}/traveldb${depnb_to}/"
echo "  SQL         : ${SQL_TYPE} / ${SQL_CONN}"
echo "  Non-IATA    : ${INC_NO_IATA}"
echo

time opentrep-indexer \
    -p "${POR_FILE}" \
    -d "${TREP_DIR}/traveldb" \
    -m "${depnb_to}" \
    -t "${SQL_TYPE}" \
    -s "${SQL_CONN}" \
    -a 1 -x 1 \
    -n "${INC_NO_IATA}" \
    -l "${LOG_DIR}/opentrep-indexer-dep${depnb_to}.log"

echo
echo "Indexing complete. Xapian DB:"
ls -lh "${TREP_DIR}/traveldb${depnb_to}/"
echo
echo "Next step: ~/bin/trep-switch-deployment.sh"

rm -f "${LOCK_FILE}"
