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
    # Logging
    if form.has_key ('data'):
	log_service.log (localize.www_log_filename, req, form['data'], place_list,
			 unrecognized)
    os.system ('cat %s >> %s' % (localize.tmp_trep_log_filename, 
                                 localize.trep_log_filename))
    return '', ''

# Extract the parameters from the incoming request
def extract_params (request, query_string = ''):
    #
    result = False
    place_list = None
    zoom_level = 5
    map_type_value = 'HYBRID'

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

    # Detect the Google Map parameters (if any)
    if search_form.has_key('zoom'):
        zoom_level = search_form['zoom']
    if search_form.has_key('map_type'):
        map_type_value = search_form['map_type']

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

    # Detect the Google Map parameters (if any)
    if search_form.has_key('zoom'):
        zoom_level = search_form['zoom']
    if search_form.has_key('map_type'):
        map_type_value = search_form['map_type']

    #
    return query_string, result, place_list, zoom_level, map_type_value

#
def index (request):
    # Extract the parameters from the incoming request
    query_string, result, place_list, zoom_level, map_type_value = extract_params (request, query_string='')

    if query_string == '':
        #
        return render (request, 'search/index.html', {
                'place_list': None,
                'place_pair_list': [{'dep': None, 'arr': None, 'dist': '0'}],
                'dist_total': '0',
                'query_string': query_string,
                'coord_for_GMap_center': '',
                'zoom_level': zoom_level, 'map_type_value': map_type_value,
                'list_markers': [], 'nbOfMarkers': 0})
    else:
        # Delegate the call to the display view
        return display (request, query_string, zoom_level, map_type_value)

#
def search (request, query_string = ''):
    # Extract the parameters from the incoming request
    query_string, result, place_list, zoom_level, map_type_value = extract_params (request, query_string)

    # Try with the query string within the URL
    if query_string != '':
        result = True

    #
    if result == False:
        return render (request, 'search/index.html', {
                'place_list': place_list,
                'place_pair_list': [{'dep': None, 'arr': None, 'dist': '0'}],
                'dist_total': '0',
                'query_string': query_string,
                'coord_for_GMap_center': '',
                'zoom_level': zoom_level, 'map_type_value': map_type_value,
                'list_markers': [], 'nbOfMarkers': 0})

    # Delegate the call to the display view
    return display (request, query_string, zoom_level, map_type_value)

#
def display (request, query_string = '',
             zoom_level = 5, map_type_value = 'HYBRID'):
    #
    if query_string == '':
        return HttpResponseRedirect(reverse('search:index'))
        

    # Initialise the OpenTrep C++ library
    xapianDBPath = "/tmp/opentrep/traveldb"
    openTrepLibrary = libpyopentrep.OpenTrepSearcher()
    initOK = openTrepLibrary.init (xapianDBPath, 'pyopentrep.log')
    if initOK == False:
        errorMsg = 'Error: The OpenTrepLibrary cannot be initialised'
        return render (request, 'search/500.html', {'error_msg': errorMsg})

    # Call the underlying OpenTREP library
    query_string_str = str(query_string)
    result = openTrepLibrary.search ('P', query_string_str)

    # Protobuf
    placeList = Travel_pb2.PlaceList()
    placeList.ParseFromString (result)

    #
    interpretedString, msg = '', ''
    list_markers = []
    coord_for_GMap_center = {'lat': 10, 'lon': 0}

    # DEBUG
    #print 'Result: ' + str(placeList)

    # Free the OpenTREP library resource
    openTrepLibrary.finalize()

    # Calling appropriate program (nothing, coordinates, or distance)
    place_list = placeList.place
    n_airports = len (place_list)
    place_pair_list = []
    dist_total = 0

    ##
    # No airport (=> no map)
    ##
    if n_airports == 0:
	if (result == True):
		msg += 'Your entry ("%s") was not recognised:<br>' % query_string_str
		msg += 'Enter a three-letter code or the name of an airport or city to find out its coordinates (e.g., JFK, or Rio de Janeiro)<br>'
		msg += 'Enter a sequence of two three-letter place_list or names to find out the distance between those airports (e.g., JFK CDG Tokyo)'

    ##
    # A single airport => a map with a single marker
    ##
    elif n_airports == 1:
	place = place_list[0]
        place_pair_list = [{'dep': place, 'arr': None, 'dist': '0'}]
	coord = por_service.get_lat_lon (place)
	if coord:
            coord_for_GMap_center = {'lat': coord[0], 'lon': coord[1]}
            city_name_utf, city_name_ascii = por_service.get_city_names (place)
            aptMarker = map_display.create_marker (place, coord[0], coord[1],
                                                   city_name_utf)
            list_markers = [aptMarker]

    ##
    # Several airports => a map with several paths (and markers)
    ##
    elif n_airports >= 2:
	# Markers
	for place in place_list:
            coord = por_service.get_lat_lon (place)
            if coord:
                city_name_utf, city_name_ascii= por_service.get_city_names(place)
                aptMarker = map_display.create_marker (place,
                                                       coord[0], coord[1],
                                                       city_name_utf)
                list_markers.append (aptMarker)

        # Iterate on two lists, one browsing elements [1, N-1],
        # the other browsing the elements [2, N]
        place_list_front = place_list[:-1]
        place_list_second = place_list[1:]
        for (place1, place2) in zip (place_list_front, place_list_second):
            dist = por_service.get_distance_km (place1, place2)
            dist_total += dist
            place_pair_list.append ({'dep': place1, 'arr': place2, 
                                     'dist': '%d' % dist})

    # Number of markers for Google Maps
    nbOfMarkers = len (list_markers)

    #
    return render (request, 'search/display.html', {
            'place_list': place_list, 'place_pair_list': place_pair_list,
            'dist_total': '%d' % dist_total,
            'query_string': query_string,
            'coord_for_GMap_center': coord_for_GMap_center,
            'zoom_level': zoom_level, 'map_type_value': map_type_value,
            'list_markers': list_markers, 'nbOfMarkers': nbOfMarkers})

