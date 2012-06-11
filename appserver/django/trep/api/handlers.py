from piston.handler import BaseHandler
import json

# The first time a request is handled by Django (after that latter has been
# started), the OpenTrep library has to be initialised

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

		# Call the underlying Trep library, which returns JSON-formatted list of POR
		jsonMessage = "SFO"
		#message = json.loads (jsonMessage)	
		message = jsonMessage	
		
		# print 'Received reply ', request, '[', jsonMessage, ']'
		#TESTING --- UNCOMMENT FOR NORMAL OPERATION ---		
		#return message
		print 'Message: ', message
		#REMOVE OR COMMENT FOR NORMAL OPERATION ---

