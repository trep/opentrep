#!/usr/bin/python

import socket

www_log_filename = '/var/log/opentrep/www.log'
trep_log_filename = '/var/log/opentrep/opentrep.log'
tmp_trep_log_filename = '/var/log/opentrep/tmp_opentrep.log'

hostname = socket.gethostname()
main_name = hostname.split('.')[0]

traveldb_path = '/var/www/opentrep/traveldb'
libpyopentrep_path = '/tmp/opentrep/lib'
opentrep_dbparams = {'user': 'opentrep', 'password': 'opentrep', 'host': 'localhost', 'port': '3306', 'db': 'trep_opentrep'}
