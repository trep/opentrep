#!/bin/bash
# Switch the active deployment slot and restart the FastAPI service.

LOCK_FILE="/tmp/opentrep/lock/opentrep-index.lock"
VIEWPY="/var/www/webapps/search/app.py"
TREP_DIR="/var/www/webapps/opentrep/trep"
SVC="search-webapp.service"

depnb_max=2
depnb_from=$(grep "^deploymentNb = " ${VIEWPY} | cut -d'=' -f2 | tr -d ' ')
depnb_from=$((0 + depnb_from))
depnb_to=$(( (1 + depnb_from) % depnb_max ))

if [ "$1" = "-h" ] || [ "$1" = "--help" ]; then
    echo "Usage: $0"
    echo "  Will switch from slot ${depnb_from} → ${depnb_to} and restart ${SVC}"
    exit 0
fi

if [ -f "${LOCK_FILE}" ]; then
    echo "Indexing in progress ($(cat ${LOCK_FILE})). Aborting."
    exit 1
fi

# Sanity: check target Xapian DB exists and looks populated
XAPIAN_TARGET="${TREP_DIR}/traveldb${depnb_to}"
if [ ! -f "${XAPIAN_TARGET}/postlist.glass" ] && [ ! -f "${XAPIAN_TARGET}/flintlock" ]; then
    echo "Target Xapian DB (${XAPIAN_TARGET}) does not look ready. Aborting."
    exit 1
fi

echo
echo "Switching deployment: ${depnb_from} → ${depnb_to}"
sed -i "s/^deploymentNb = ${depnb_from}/deploymentNb = ${depnb_to}/" "${VIEWPY}"

depnb_new=$(grep "^deploymentNb = " ${VIEWPY} | cut -d'=' -f2 | tr -d ' ')
if [ "$((0 + depnb_new))" != "${depnb_to}" ]; then
    echo "[Error] Failed to update deploymentNb in ${VIEWPY}. Manual check needed."
    exit 1
fi

echo "Restarting ${SVC}..."
systemctl restart "${SVC}"
sleep 2
systemctl is-active "${SVC}"
echo "Done — now serving slot ${depnb_to}."
echo
