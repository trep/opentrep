#!/usr/bin/env python

#
#   AirInv client in Python
#   Connects REQ socket to tcp://localhost:5555
#   Sends JSON-ified parameters of a flight-date to display to the AirInv 
#   server, and expects the full JSON-ified details for that given flight-date.
#
import getopt, sys, json, zmq

#------------------------------------------------------------------------------	
def usage():
    print
    print
    print "-h, --help               : outputs help and quits"
    print "-a <airline_code>        : Code of the owner airline"
    print "-f <flight_number>       : Number of the flight to display"
    print "-d <departure_date>      : Departure date of the flight to display"
    print "[[[<airline_code>] [<flight_number>]] [<departure_date>]] : parameters of the flight to display (if not already provided as option)"
    print
	
#------------------------------------------------------------------------------	
def handle_opt():
    try:
        opts, args = getopt.getopt (sys.argv[1:], "h:a:f:d:", ["help", "airline_code", "flight_number", "departure_date"])
    except getopt.GetoptError, err:
        # Will print something like "option -z not recognized"
        print str(err)
        usage()
        sys.exit(2)
	
    # Default options/parameters
    airlineCode = 'SV'
    flightNumber = 5
    departureDate = '2010-Mar-11'

    # Handling parameters
    if len (args) >= 1:
        airlineCode = args[0]
    if len (args) >= 2:
        flightNumber = args[1]
    if len (args) >= 3:
        departureDate = args[2]

    # Handling options
    for optionName, optionValue in opts:
        if optionName in ("-h", "--help"):
            usage()
            sys.exit()
        elif optionName in ("-a", "--airline_code"):
            airlineCode = optionValue
        elif optionName in ("-f", "--flight_number"):
            flightNumber = optionValue
        elif optionName in ("-d", "--departure_date"):
            departureDate = optionValue
        else:
            assert False, "Unhandled option"

    return airlineCode, flightNumber, departureDate

#------------------------------------------------------------------------------	
def main():

    # Parse command options
    airlineCode, flightNumber, departureDate = handle_opt()
    jsonAction = {'flight_date': { 'departure_date': departureDate, 'airline_code': airlineCode, 'flight_number': flightNumber }}
    jsonActionString = json.dumps (jsonAction)

    #  Socket to talk to server
    print "Connecting to hello world server..."
    context = zmq.Context()
    socket = context.socket (zmq.REQ)
    socket.connect ("tcp://localhost:5555")
    print "... done"

    #  Send the flight-date parameters to the AirInv server
    print "Requesting flight-date ", jsonActionString, " from the AirInv server..."
    socket.send (jsonActionString)
    
    #  Get the flight-date details from the AirInv server
    flightDateDetails = socket.recv()
    print "... received reply: '", flightDateDetails, "'"


#-------------------------------------------------------------------------------
if __name__ == "__main__":
    main()

