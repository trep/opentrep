#!/bin/bash
# Show current OpenTREP deployment state on CT189 (FastAPI / blue-green)

LOCK_FILE="/tmp/opentrep/lock/opentrep-index.lock"
WEBAPP_DIR="/var/www/webapps/search"
VIEWPY="${WEBAPP_DIR}/app.py"
TREP_DIR="/var/www/webapps/opentrep/trep"

depnb_from=$(grep "^deploymentNb = " ${VIEWPY} | cut -d'=' -f2 | tr -d ' ')
depnb_from=$((0 + depnb_from))
depnb_to=$(( (1 + depnb_from) % 2 ))

echo
echo "Web application (in '${WEBAPP_DIR}') deployment stage: ${depnb_from}"
echo "  Active Xapian DB : ${TREP_DIR}/traveldb${depnb_from}/"
echo "  SQL backend slot : $(grep -A2 "${depnb_from}:" ${VIEWPY} | head -1)"
echo "  Target slot      : ${depnb_to}"
echo

if [ -f "${LOCK_FILE}" ]; then
    TREP_OTH_DEPNB=$(awk -F'^' '{print $1}' ${LOCK_FILE})
    TREP_OTH_PID=$(awk  -F'^' '{print $2}' ${LOCK_FILE})
    echo "An indexing process (PID: ${TREP_OTH_PID}) is running for slot ${TREP_OTH_DEPNB}."
    echo
fi
