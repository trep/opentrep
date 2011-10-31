#!/usr/bin/env python

import sys, getopt
sys.path.append('/home/www/webapps/opentrep/trep/lib')

# Default search string
defaultSearchString = 'sna francicso rio de janero lso angles reykyavki'

# Parser helpers
def getMain(locations): return locations[:3]

# Usage
def usage(script_name):
	print
	print "Usage: ./%s [options] search string" % script_name
	print
	print "The default output is short."
	print
	print "Options:"
	print "    -h, --help              : outputs this help and exits"
	print "    -l, --long              : long output version"
	print

# Handle opt
def handle_opt():
	try:
		opts, args = getopt.getopt(sys.argv[1:], "hl", ["help", "long"])
	except getopt.GetoptError, err:
		# print help information and exit:
		print str(err) # will print something like "option -a not recognized"
		usage()
		sys.exit(2)

	# search string
	searchString = "" # default
	if len(args) >= 1: searchString = ' '.join(args)

	# options
	needDetails = False # default
	for o, a in opts:
		if o in ("-l", "--long"):
			needDetails = True
		elif o in ("-h", "--help"):
			usage(sys.argv[0])
			sys.exit()
		else:
			assert False, "unhandled option"
	return needDetails, searchString

needDetails, searchString = handle_opt()

# Initialise the OpenTrep C++ library
import libpyopentrep
openTrepLibrary = libpyopentrep.OpenTrepSearcher()
openTrepLibrary.init('/home/www/webapps/opentrep/trep/traveldb', 'pyopentrep.log', 'opentrep', 'opentrep', 'localhost', '3306', 'trep_opentrep')

# If no search string was supplied as arguments of the command-line,
# ask the user for some
if searchString == '':
    # Ask for the user input
    searchString = raw_input('Enter a search string, or just Enter for the default one (' + defaultSearchString + '): ')
if searchString == '' : searchString = defaultSearchString

# DEBUG
print "searchString: " + searchString

# Call the OpenTrep C++ library
if needDetails == True:
	result = openTrepLibrary.searchWithFullDetails (searchString)
else:
	result = openTrepLibrary.search (searchString)

print 'Raw result from the OpenTrep library:'
print result

# If we have requested the detailed display, the result string is
# potentially big and complex, and is not aimed to be parsed. So, it
# is better to stop here.
if needDetails == True:
	quit()

# As we have requested no details, the result string is aimed to be
# parsed, so as to get the whole meaning of it.

# Defaults
msg, form_value, original_form_value, unrecognized = '', '', '', ''

# Sample of result string to be parsed:
# 'nce/100,sfo/100-emb/98-jcc/97,yvr/100-cxh/83-xea/83-ydt/83;niznayou'
# parsing: recognize sequence of three-letter codes
codes = []
locations = []
alter_locations = []

# Delegate the interpretation to the underlying
# OpenTrep library
str_matches = result

# Parsing begins
# 1. First, strip out the unrecongised keywords
if ';' in str_matches:
    str_matches, unrecognized = str_matches.split(';')

msg = 'unrecognized: %s. ' % unrecognized
str_value = unrecognized

if str_matches != '':
    # 2. Then, for each matching location, the
    #    alternate matches have to be stored aside
    alter_locations = [x for x in str_matches.split(',')]
    locations = [getMain(x) for x in alter_locations]

    for alter_location_list in alter_locations:
        alter_location_list = [x for x in alter_location_list.split('-')]
        for extra_location_list in alter_location_list:
            extra_location_list = [x for x in extra_location_list.split(':')]
                
            codes = [x[:3].upper() for x in alter_locations]
            if len(codes) > 0:
                form_value = ' '.join(codes)
            if str_value != '':
                form_value += ' ' + str_value

print "Parsed entries:"
print form_value

#print "Locations:"
#print locations

# Free the OpenTREP library resource
openTrepLibrary.finalize()

