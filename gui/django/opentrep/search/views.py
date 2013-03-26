from django.shortcuts import render_to_response
from django.http import Http404
import sys, getopt, os
import simplejson as json
sys.path.append ('/home/dan/dev/deliveries/opentrep-0.5.4/lib64')
sys.path.append ('/home/dan/dev/deliveries/opentrep-0.5.4/lib64/python/opentrep')
import libpyopentrep
import Travel_pb2

def search (request, query_string = None):
    #
    if query_string is None:
        query_string = 'rio de janero sna francisco'

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
    return render_to_response ('search.html',
                               {'place_list': placeList.place,
                                'query_string': query_string})

