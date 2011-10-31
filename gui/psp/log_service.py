#!/usr/bin/python

import socket, os, datetime

def log(filename, req, query, codes, unrecognized):
	req.add_common_vars()
	# determine ip
	remote_client_ip = req.connection.remote_ip
	# determine hostname
	hostname = req.connection.remote_host
	if hostname == None: hostname = 'localhost'
	# determine time
	str_time = datetime.datetime.now().strftime('%y%m%d%H%M%S')
	# determine user agent
	agent = ''
	if req.subprocess_env.has_key("HTTP_USER_AGENT"): agent = req.subprocess_env["HTTP_USER_AGENT"]
	# determine user allowed languages
	languages = ''
	if req.subprocess_env.has_key("HTTP_ACCEPT_LANGUAGE"): languages = req.subprocess_env["HTTP_ACCEPT_LANGUAGE"]
	# determine user allowed character sets
	charsets = ''
	if req.subprocess_env.has_key("HTTP_ACCEPT_CHARSET"): charsets = req.subprocess_env["HTTP_ACCEPT_CHARSET"]
	# write to file 
	str_out = '^'.join([str_time,remote_client_ip, hostname, query, ','.join(codes), unrecognized, agent, languages, charsets])
	os.system('echo "%s" >> %s' % (str_out, filename))
