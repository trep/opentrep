#!/usr/bin/env python
# -*- coding: utf-8 -*-

query = u'ЙЦУКЕНГШЩЗХЪ ФЫВАПРОЛДЖЭ ЯЧСМИТЬБЮ Ё ЇІЄ'
query_input = raw_input()
if query_input != '':
	query = unicode(query_input,encoding='utf-8')

print 'Original query: ' + query
print 'Lower-cased query: ' + query.lower()
print 'Upper-cased query: ' + query.upper() + '\n'

