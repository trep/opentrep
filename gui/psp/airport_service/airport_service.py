#! /usr/bin/python

from math import asin, acos, pi, sqrt, sin, cos
import sys, re, os, random


def load_db(filename):
	info = {}
	f = open(filename, 'r')
	for line in f:
		fields = line.rstrip('\n').split(',')
		if len(fields) < 8: continue
		#try:
		info[fields[0]] = {'lat': float(fields[4]), 'lon': float(fields[3]), 'city_code': fields[1], 'city_name': fields[8], 'country': fields[2], 'region': fields[6]}
		#except: pass
	f.close()
	return info

directory = os.path.dirname(__file__)
info = load_db(os.path.join(directory, 'airport_db.txt'))
apt_list = info.keys()

def get_random_airport(apt_list=apt_list): return apt_list[int(random.uniform(0, len(apt_list)))]

def great_circle_distance(lat1, lon1, lat2, lon2, degrees=True):
	if degrees: lat1, lon1, lat2, lon2 = lat1/180.0*pi, lon1/180.0*pi, lat2/180.0*pi, lon2/180.0*pi
	return 12756.0*asin(sqrt((sin((lat2-lat1)/2.0))**2.0+cos(lat1)*cos(lat2)*(sin((lon2-lon1)/2.0))**2.0))
	#colat1, colat2 = pi/2.0-lat1, pi/2.0-lat2
	#return 6378.0*acos(cos(colat1)*cos(colat2)*cos(lon1-lon2)+sin(colat1)*sin(colat2))

def great_circle_distance_acos(lat1, lon1, lat2, lon2, degrees=True):
	if degrees: lat1, lon1, lat2, lon2 = lat1/180.0*pi, lon1/180.0*pi, lat2/180.0*pi, lon2/180.0*pi
	return 6378.0*acos(cos(lat1)*cos(lon1)*cos(lat2)*cos(lon2)+cos(lat1)*sin(lon1)*cos(lat2)*sin(lon2)+sin(lat1)*sin(lat2))
	#colat1, colat2 = pi/2.0-lat1, pi/2.0-lat2
	#return 6378.0*acos(cos(colat1)*cos(colat2)*cos(lon1-lon2)+sin(colat1)*sin(colat2))

def get_distance_km(apt1, apt2, info=info, apt_list=apt_list):
	unknown = []
	if not (info.has_key(apt1) and info.has_key(apt2)):
		if not info.has_key(apt1): unknown.append(apt1)
		if not info.has_key(apt2): unknown.append(apt2)
		dist = None
	else:
		dist = great_circle_distance(info[apt1]['lat'], info[apt1]['lon'], info[apt2]['lat'], info[apt2]['lon'])
	return dist, unknown

def get_coordinates(apt, info=info, apt_list=apt_list):
	if info.has_key(apt): return {'lat': info[apt]['lat'], 'lon': info[apt]['lon']}
	return None

def get_city_name(apt, info=info, apt_list=apt_list):
	if info.has_key(apt): return info[apt]['city_name']
	return None

def get_airport_name(apt, info=info, apt_list=apt_list):
	if info.has_key(apt): return info[apt]['city_name']
	return None

def get_region(apt, info=info, apt_list=list):
	if info.has_key(apt): return info[apt]['region']
	return None
	
def main():
	pass

if __name__ == "__main__":
    main()
