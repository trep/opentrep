#!/usr/bin/env python

#
from geonames.models import POR, AlternateName

# Retrieving the Geonames entries for a few airports
nice_airport = POR.objects.get (geonameid=6299418)
kiev_airport = POR.objects.get (geonameid=6300952)
# And their related names
#nice_names = AlternateName.objects.filter (geonameid=6299418)
nice_names = nice_airport.alternatename_set.all()
#kiev_names = AlternateName.objects.filter (geonameid=6300952)
kiev_names = kiev_airport.alternatename_set.all()

#
print 'Nice airport: ', nice_airport, nice_names
print 'Kiev (Boryspil) airport: ', kiev_airport, kiev_names


