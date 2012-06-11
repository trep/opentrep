#!/bin/sh

##
# That script works on Fedora/Redhat distributions.
# It has to be adpated to work on Debian-derived distributions (e.g., Ubuntu).
# For instance, the set up directory is /etc/apache2/mods-enabled 
# for Debian (not /etc/httpd/conf.d), and the way to restart the service is
# /etc/init.d/apache2 restart
##
SETUP_DIR="/etc/httpd/conf.d"
CURRENT_USER=`whoami`
APP_NAME="DjangoTrepApp"
SETUP_FILE="${APP_NAME}.conf"
DJANGO_WS_TO_REDIRECT="trep/api"
DJANGO_PORT="8000"
APACHE_SHORTCUT="/api"

if [ "${CURRENT_USER}" != "root" ]
then
	echo "You must be root to run this script."
	exit -1
fi

if [ ! -d ${SETUP_DIR} ]
then
	mkdir -p ${SETUP_DIR}
fi

if [ -f "${SETUP_DIR}/${SETUP_FILE}" ]
then
	echo "The ${SETUP_DIR}/${SETUP_FILE} Apache configuration file already exist."
	echo "It will be saved as ${SETUP_DIR}/${SETUP_FILE}.bak and a new one will be written."
	mv -f ${SETUP_DIR}/${SETUP_FILE} ${SETUP_DIR}/${SETUP_FILE}.bak
fi

# Write the Apache configuration file
cat > ${SETUP_DIR}/${SETUP_FILE} << _EOF

# 
ProxyRequests Off

# Map any URL matching "${APACHE_SHORTCUT}" (after the host address) towards
# the corresponding Django application service (${DJANGO_WS_TO_REDIRECT}).
# Django is assumedd to run on port ${DJANGO_PORT}.
<Location "${APACHE_SHORTCUT}">
  ProxyPass http://127.0.0.1:${DJANGO_PORT}/${DJANGO_WS_TO_REDIRECT}
  ProxyPassReverse http://127.0.0.1:${DJANGO_PORT}/${DJANGO_WS_TO_REDIRECT}
</Location>

_EOF

echo "New ${SETUP_DIR}/${SETUP_FILE} Apache configuration file created."
echo
echo "The Apache service will be restarted."

# From Fedora 16
systemctl restart httpd.service
# Up to Fedora 16
# service httpd restart

echo "See the logs above to see whether the Apache service has correctly restarted."

