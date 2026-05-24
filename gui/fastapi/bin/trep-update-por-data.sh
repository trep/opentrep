#!/bin/bash
# Download the latest OPTD POR all.csv and stage it for the next deployment slot.

LOCK_FILE="/tmp/opentrep/lock/opentrep-index.lock"
VIEWPY="/var/www/webapps/search/app.py"
GH_DIR="${HOME}/dev/opentraveldata"
TREP_DATA_DIR="/var/www/webapps/opentrep/trep/share/opentrep/data/por"

depnb_max=2
depnb_from=$(grep "^deploymentNb = " ${VIEWPY} | cut -d'=' -f2 | tr -d ' ')
depnb_from=$((0 + depnb_from))
depnb_to=$(( (1 + depnb_from) % depnb_max ))
POR_TARGET="${TREP_DATA_DIR}/optd_por_public_${depnb_to}.csv"

if [ "$1" = "-h" ] || [ "$1" = "--help" ]; then
    echo "Usage: $0"
    echo "  Current slot: ${depnb_from} → Target slot: ${depnb_to}"
    echo "  Output: ${POR_TARGET}"
    exit 0
fi

if [ -f "${LOCK_FILE}" ]; then
    echo "Indexing in progress ($(cat ${LOCK_FILE})). Aborting."
    exit 1
fi

echo "Pulling latest OPTD data from GitHub..."
pushd "${GH_DIR}" > /dev/null
git pull
POR_DATE=$(git log -1 --pretty="format:%ci" opentraveldata/optd_por_public_all.csv | cut -d' ' -f1)
echo "Last modification date of optd_por_public_all.csv: ${POR_DATE}"
cp -a opentraveldata/optd_por_public_all.csv "${POR_TARGET}"
popd > /dev/null

echo "Staged: ${POR_TARGET} ($(wc -l < ${POR_TARGET}) lines)"
echo
echo "Next step: ~/bin/trep-index.sh"
