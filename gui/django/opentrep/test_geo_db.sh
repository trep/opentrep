
# Do not execute this file. Source it instead
CURRENT_DIR=`pwd`
DJANGO_BASE_DIR=`dirname ${CURRENT_DIR}`
export DJANGO_SETTINGS_MODULE=opentrep.settings
export PYTHONPATH=${PYTHONPATH}:${DJANGO_BASE_DIR}

