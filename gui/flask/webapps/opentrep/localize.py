#!/usr/bin/python

import socket

www_log_filename = '/var/log/webapps/opentrep/www.log'
trep_log_filename = '/var/log/webapps/opentrep/opentrep.log'
tmp_trep_log_filename = '/var/log/webapps/opentrep/tmp_opentrep.log'

hostname = socket.gethostname()
main_name = hostname.split('.')[0]

traveldb_path = '/var/www/webapps/opentrep/trep/traveldb'
pyopentrep_path = '/var/www/webapps/opentrep/trep/lib64'

