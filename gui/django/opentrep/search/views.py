from django.shortcuts import render
from django.http import Http404
from django.http import HttpResponseRedirect, HttpResponse
from django.core.urlresolvers import reverse
import sys, getopt, os
import simplejson as json
sys.path.append ('/home/dan/dev/deliveries/opentrep-0.5.4/lib64')
sys.path.append ('/home/dan/dev/deliveries/opentrep-0.5.4/lib64/python/opentrep')
import libpyopentrep
import Travel_pb2
import log_service
import por_service
import map_display

# Parser helpers
def getMain(locations):
    return locations[:3].upper()

# Parse the OpenTREP compact-formatted result
def parseCompactResult (resultString):
    place_list = []
    form_value, unrecognized = '', ''
    if ';' in resultString:
        resultString, unrecognized = resultString.split(';')
        msg = 'Unrecognised: %s. ' % unrecognized
    if resultString != '':
        place_list = [getMain(x) for x in resultString.split(',')]
    str_value = unrecognized

    if len (place_list) > 0:
        form_value = ' '.join(place_list) + ' ' + str_value

    #
    return (place_list, form_value, unrecognized)

# Parse the OpenTREP compact-formatted result
def parseJSONResult (resultString):
    place_list = []
    form_value, unrecognized = '', ''

    #
    return (place_list, form_value, unrecognized)

# Imported from airports.psp
def travelSearch (openTrepLibrary, travelPB2):
    # Google Maps API v3
    body_for_GMap = '<body onload="initialize()">'
    html_for_GMap = '<div id="map_canvas" style="margin-left: auto; text-align:center; margin-right: auto; width:50%; height:50%"></div>'
    coord_for_GMap_center = {'lat': 10, 'lon': 0}

    # Defaults
    head = ''
    body_declaration = '<body>'
    msg = ''
    form_value, original_form_value, unrecognized = '', '', ''
    default_zoom_level = 5
    zoom_level = default_zoom_level
    map_type_list = ['ROADMAP', 'SATELLITE', 'HYBRID', 'TERRAIN']
    default_map_type = map_type_list[0] 
    map_type_value = default_map_type

    language = 'en'
    result = False
    list_markers = []

    # Parsing: recognise sequence of three-letter place_list
    place_list = []
    locations = []
    alter_locations = []

    # Detect the required action
    if form.has_key('query_text'):
	form_value = form['data']
	result = True
    elif form.has_key('show_airport'):
	place_list = [por_service.get_random_airport (openTrepLibrary, travelPB2)]
	form_value = ' '.join(place_list)
	result = True
    elif form.has_key('show_itinerary'):
	place_list = [por_service.get_random_airport (openTrepLibrary, travelPB2) for i in range(3)]
	form_value = ' '.join(place_list)
	result = True

    # Save the query,as entered by the user
    original_form_value = form_value

    # Logging
    if form.has_key ('data'):
	log_service.log (localize.www_log_filename, req, form['data'], place_list,
			 unrecognized)
    os.system ('cat %s >> %s' % (localize.tmp_trep_log_filename, 
                                 localize.trep_log_filename))
		
    # Calling appropriate program (nothing, coordinates, or distance)
    n_airports = len (place_list)

    ##
    # Re-state the initial query
    ##
    msg += '<center>Original query: "%s"</center><br>' % original_form_value

    ##
    # No airport (=> no map)
    ##
    if n_airports == 0:
	if (result == True):
		msg += '<center>'
		msg += 'Your entry ("%s") was not recognised:<br>' % original_form_value
		msg += 'Enter a three-letter code or the name of an airport or city to find out its coordinates (e.g., JFK, or Rio de Janeiro)<br>'
		msg += 'Enter a sequence of two three-letter place_list or names to find out the distance between those airports (e.g., JFK CDG Tokyo)'
		msg += '</center>'

    ##
    # A single airport => a map with a single marker
    ##
    elif n_airports == 1:
	place = place_list[0]
	coord = por_service.get_lat_lon (place)
	if coord is None:
		msg += 'Unknown: ' + place
	else:
		coord_for_GMap_center = {'lat': coord[0], 'lon': coord[1]}
		city_name = por_service.get_city_name (place)
		msg += map_display.create_msg_coordinates (place, city_name)
		aptMarker = map_display.create_marker (place, coord[0],
						       coord[1], city_name)
		list_markers = [aptMarker]
		body_declaration = body_for_GMap

    ##
    # Several airports => a map with several paths (and markers)
    ##
    elif n_airports >= 2:
	import scipy
	list_unknown, list_polylines = [], []
	list_distances, list_cities = [], []

	# Markers
	for place in place_list:
		coord = por_service.get_lat_lon (place)
		if coord is None: list_unknown.append(place)
		else:
			city_name = por_service.get_city_name (place)
			list_cities.append (city_name)
			aptMarker = map_display.create_marker (place,
							       coord[0], 
							       coord[1], 
							       city_name)
			list_markers.append (aptMarker)

	if len (list_unknown) > 0:
		msg += 'unknown: ' + ', '.join(list_unknown)
	else:
		for (place1, place2) in zip (place_list[:-1], place_list[1:]):
			dist, unknown = por_service.get_distance_km (place1,
                                                                     place2)
			list_distances.append(dist)
			polyline = {'color': '#ff0000', 'width': 2}
			for (str_i, place) in zip(('1', '2'), (place1, place2)):
				coord = por_service.get_lat_lon (place)
				polyline[str_i] = {'lat': coord[0], 'lon': coord[1]}
			list_polylines.append(polyline)

		msg += map_display.create_msg_distance (list_distances,
                                                        place_list,
							list_cities)
		body_declaration = body_for_GMap
    return body_declaration, msg

#
def index (request):
    #
    return render (request, 'search/index.html', {
            'place_list': None,
            'query_string': ''})

#
def search (request, query_string = ''):
    #
    place_list = []
    result = False

    # Try with the query string within the URL
    if query_string != '':
        result = True

    # Try with GET
    search_form = request.GET

    # Detect the required action
    if search_form.has_key('query_text'):
	query_string = search_form['data']
	result = True
    elif search_form.has_key('show_airport'):
        # TODO: implement that API into OpenTREP and uncomment
	# place_list = [por_service.get_random_airport (openTrepLibrary, travelPB2)]
        place_list = ['NCE']
	query_string = ' '.join(place_list)
	result = True
    elif search_form.has_key('show_itinerary'):
        # TODO: implement that API into OpenTREP and uncomment
	# place_list = [por_service.get_random_airport (openTrepLibrary, travelPB2) for i in range(3)]
        place_list = ['NCE', 'JFK', 'LAX']
	query_string = ' '.join(place_list)
	result = True

    # Try with POST
    search_form = request.POST

    # Detect the required action
    if search_form.has_key('query_text'):
	query_string = search_form['data']
	result = True
    elif search_form.has_key('show_airport'):
	# place_list = [por_service.get_random_airport (openTrepLibrary, travelPB2)]
        place_list = ['NCE']
	query_string = ' '.join(place_list)
	result = True
    elif search_form.has_key('show_itinerary'):
	# place_list = [por_service.get_random_airport (openTrepLibrary, travelPB2) for i in range(3)]
        place_list = ['NCE', 'JFK', 'LAX']
	query_string = ' '.join(place_list)
	result = True

    #
    if result == False:
        return render (request, 'search/index.html', {
                'place_list': None,
                'query_string': ''})

    # Delegate the call to the display view
    return display (request, query_string)


def display (request, query_string):
    #
    if query_string == '':
        return HttpResponseRedirect(reverse('search:index'))
        

    # Initialise the OpenTrep C++ library
    xapianDBPath = "/tmp/opentrep/traveldb"
    openTrepLibrary = libpyopentrep.OpenTrepSearcher()
    initOK = openTrepLibrary.init (xapianDBPath, 'pyopentrep.log')
    if initOK == False:
        errorMsg = 'Error: The OpenTrepLibrary cannot be initialised'
        raise Http404

    # Call the underlying OpenTREP library
    query_string_str = str(query_string)
    result = openTrepLibrary.search ('P', query_string_str)

    # Protobuf
    placeList = Travel_pb2.PlaceList()
    placeList.ParseFromString (result)

    #
    interpretedString = ''

    # DEBUG
    #print 'Result: ' + str(placeList)

    # Free the OpenTREP library resource
    openTrepLibrary.finalize()

    #
    return render (request, 'search/index.html', {
            'place_list': placeList.place,
            'query_string': query_string})

