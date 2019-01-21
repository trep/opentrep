from piston.handler import BaseHandler
import json

#
class TrepHandler (BaseHandler):	
	def read (self, request, airportCodeURL=None):
		# Default values, corresponding to the default inventory dump
		# file loaded by the inventory server
		airportCode = 'LHR'

		# Extract the requested action from the query string (if any)
		airportCodeQuery = request.GET.get ('ac')

		# If the airport parameter is given within the URL (e.g.,
		# http://localhost:8000/api/display/place/SFO)
		# take it into account
		if airportCodeQuery: airportCode = airportCodeQuery

		# If the airport parameter is given as part of the query
		# string (e.g., http://localhost:8000/api/display/place?ac=LAX)
		# take it into account. It has the precedence over the 
		# parameter defined within the URL. That is, with
		# http://localhost:8000/api/display/place/RIO?ac=FRA
		# the following parameter would be kept: FRA
		if airportCodeURL: airportCode = airportCodeURL

		# Compose a JSON Python object with the parameters
		jsonAction = json.dumps ({'display': { 'airport_code': airportCode }})
		# print 'JSON serialised request: ', jsonAction

		# TODO: do not harcode the answer. Actually search for the airport instead.
		jsonMessage = jsonAction
		message = json.loads (jsonMessage)
		
		# print 'Received reply ', request, '[', jsonMessage, ']'
		#TESTING --- UNCOMMENT FOR NORMAL OPERATION ---		
		#return message
		print 'Message: ', message
		#REMOVE OR COMMENT FOR NORMAL OPERATION ---
		return message

