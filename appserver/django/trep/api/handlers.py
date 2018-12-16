from sys import path
from os import environ
from piston.handler import BaseHandler
import simplejson

# Retrieve the OpenTrep parameters
traveldb_path = environ.get ('TREP_TRAVELDB', failobj='/tmp/opentrep/traveldb')
trep_log_path = environ.get ('TREP_LOG', failobj='django_trep.log')
trep_lib_dir = environ.get ('TREP_LIB', failobj='/usr/lib')

# The first time a request is handled by Django (after that latter has been
# started), the OpenTrep library has to be initialised
path.append (trep_lib_dir)

import pyopentrep
openTrepLibrary = pyopentrep.OpenTrepSearcher()
openTrepLibrary.init (traveldb_path, 'nodb', '', 0, trep_log_path)

#
class TrepHandler (BaseHandler):	
	def read (self, request, queryURL=None):
		# Default values
		query = 'SFO JFK'

		# Extract the requested action from the query string (if any)
		queryQuery = request.GET.get ('q')

		# If the query parameters are given within the URL (e.g.,
		# http://localhost:8000/trep/api/SFO)
		# take them into account
		if queryQuery: query = queryQuery

		# If the query parameters are given as part of the query
		# string (e.g., http://localhost:8000/trep/api?q=SFO)
		# take them into account. They have the precedence over the 
		# parameters defined within the URL. That is, with
		# http://localhost:8000/trep/api/LAX?q=SFO
		# the following parameters would be kept: LAX
		if queryURL: query = queryURL

		#
		query = str(query)

		# Call the underlying Trep library, which returns JSON-formatted
		# list of POR (points of reference).
		opentrepOutputFormat = 'J'
		jsonMessage = openTrepLibrary.search (opentrepOutputFormat, query)
		response = simplejson.loads (jsonMessage)	
		
		# print 'Received reply ', request, '[', jsonMessage, ']'
		#TESTING --- UNCOMMENT FOR NORMAL OPERATION ---
		return response
		#return response
		#REMOVE OR COMMENT FOR NORMAL OPERATION ---

