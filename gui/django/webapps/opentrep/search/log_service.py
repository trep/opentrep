import socket, os, datetime

def log (filename, req, query):
	req.add_common_vars()

	# Determine IP address
	remote_client_ip = req.connection.remote_ip

	# Determine hostname
	hostname = req.connection.remote_host
	if hostname == None:
		hostname = 'localhost'

	# Determine current time
	str_time = datetime.datetime.now().strftime('%y%m%d%H%M%S')

	# Determine user agent (browser type)
	agent = ''
	if req.subprocess_env.has_key("HTTP_USER_AGENT"):
		agent = req.subprocess_env["HTTP_USER_AGENT"]

	# Determine user allowed languages
	languages = ''
	if req.subprocess_env.has_key("HTTP_ACCEPT_LANGUAGE"):
		languages = req.subprocess_env["HTTP_ACCEPT_LANGUAGE"]

	# Determine user allowed character sets
	charsets = ''
	if req.subprocess_env.has_key("HTTP_ACCEPT_CHARSET"):
		charsets = req.subprocess_env["HTTP_ACCEPT_CHARSET"]

	# Write to log file
	str_out = '^'.join ([str_time,remote_client_ip, hostname, query,
			     agent, languages,
			     charsets])
	os.system ('echo "%s" >> %s' % (str_out, filename))

