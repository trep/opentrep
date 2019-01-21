#!/bin/sh

TREP_DEV_DIR=/root/dev/trep/opentrepgit
WWW_ROOT=/var/www/webapps
TREP_INS_DIR=${WWW_ROOT}/opentrep/trep

cp -f ${TREP_DEV_DIR}/gui/psp/*.psp .
cp -f ${TREP_DEV_DIR}/gui/psp/*.py .
cp -f ${TREP_DEV_DIR}/gui/psp/index.html .
cp -f ${TREP_DEV_DIR}/gui/icons/opentrep.png ${WWW_ROOT}/icons
chown apache.apache *.psp *.py *.html
chown -R apache.apache trep

echo "OpenTrep installation dir is in ${TREP_INS_DIR},"
echo "with lib in ${TREP_INS_DIR}/lib"
echo "and traveldb in ${TREP_INS_DIR}/traveldb"
echo "Check that ${WWW_ROOT}/opentrep/localize.py is correct."
echo

