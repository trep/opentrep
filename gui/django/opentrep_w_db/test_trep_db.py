#!/usr/bin/env python

#
from ref_place.models import Details, Names

#
details = Details.objects.all()
names = Names.objects.all()

#
print 'Details: ', details


