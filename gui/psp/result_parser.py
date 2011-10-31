#!/usr/bin/env python

import sys

# Parser helpers
def getMain(locations):
	return locations[:3].upper()

# Default result string
defaultResultString = 'yvr:xea/98-xtw/87,sfo/100,led:dft:htl/96;niznayou'

# If no result string was supplied as arguments of the command-line,
# ask the user for some
resultString = ','.join(sys.argv[1:])
if resultString == '' : resultString = defaultResultString

# Function to parse the result string
def parseResultString(iResultString):
    form_value, unrecognized = '', ''
    msg = '(parsing successful)'
    str_matches = iResultString
    locations = []
    alter_locations = []

    if ';' in str_matches:
        str_matches, unrecognized = str_matches.split(';')
        msg = '(unrecognized: %s)' % unrecognized
    str_value = unrecognized

    if str_matches != '':
        alter_locations = str_matches.split(',')
        locations = [getMain(x) for x in alter_locations]

    print 'locations: ', locations
    print 'alter_locations: ', alter_locations

    idx1 = 0
    while idx1 != len(alter_locations):

#       print 'Before - alter_locations['+str(idx1)+']: ', alter_locations[idx1]
        alter_locations[idx1] = alter_locations[idx1].split('-')
#       print 'After - alter_locations['+str(idx1)+']: ', alter_locations[idx1], alter_locations

        idx2 = 0
        while idx2 != len(alter_locations[idx1]):

            alter_locations[idx1][idx2] = alter_locations[idx1][idx2].split(':')

            idx3 = 0
            while idx3 != len(alter_locations[idx1][idx2]):

                alter_locations[idx1][idx2][idx3] = alter_locations[idx1][idx2][idx3].split('/')
                idx3 += 1

            idx2 += 1

        idx1 += 1
				
#    codes = [x.upper() for x in alter_locations]
#    if len(codes) > 0: form_value = ' '.join(codes)
    if str_value != '': form_value += ' ' + str_value

    print 'After - alter_locations: ', alter_locations

    print 'Result: ' + ' '.join(locations) + ' ' + msg + ':'
    return form_value

# Main
print 'Before: ' + resultString
resultString = parseResultString(resultString)
print 'After: ' + resultString
