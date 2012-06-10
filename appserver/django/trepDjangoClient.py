#!/bin/sh

#
AIRINV_HOSTNAME="localhost"
AIRINV_PORT=":8000"
API_URL_PREFIX="sim/airinv/api"
AIRINV_API_ACTION="display/inv"
AIRINV_BASE_URL="http://${AIRINV_HOSTNAME}${AIRINV_PORT}/${API_URL_PREFIX}/${AIRINV_API_ACTION}"
TMP_FILE="found.tmp"

# Check whether curl is installed
`type curl` > ${TMP_FILE} 2>&1
FOUND=`grep -qi "not found" ${TMP_FILE} && echo "No"`
rm -f ${TMP_FILE}

if [ "${FOUND}" = "No" ]
then
  echo
  echo "curl is not installed on your system."
  echo "Please ask your administrator to install it."
  echo
  exit -1
fi

#
displayInventory() {
    echo "Trying with the classical (GET) query string:"
    curl "${AIRINV_BASE_URL}?ac=${AIRLINE_CODE}&fn=${FLIGHT_NUMBER}&dd=${DEPARTURE_DATE}"
    echo
    echo "Trying with the typical Django URL:"
    curl "${AIRINV_BASE_URL}/${AIRLINE_CODE}/${FLIGHT_NUMBER}/${DEPARTURE_DATE}"
    echo
}

#
AIRLINE_CODE="SV"
FLIGHT_NUMBER="5"
DEPARTURE_DATE="2010-Mar-11"
displayInventory

