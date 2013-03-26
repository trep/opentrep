import re

def escape_quotes_function (str_text):
	return re.sub(r'\'', "\\'", str_text)

def str_airport_and_city (apt, city_name, escape_quotes=False):
	if escape_quotes:
		city_name = escape_quotes_function (city_name)
	out_city_name = ''
	if city_name != 'NA':
		out_city_name = ' (' + city_name + ')'
	return '%s%s' % (apt, out_city_name)

def create_marker (apt, lat, lon, city_name):
	str_text = '<p style="font-size:small;">%s<br>lat: %.2f<br>lon: %.2f</p>' % (str_airport_and_city (apt, city_name, escape_quotes=True), lat, lon)
	return {'lat': lat, 'lon': lon, 'name': city_name, 'text': str_text}
		
def create_msg_coordinates (apt, city_name):
	return str_airport_and_city(apt, city_name) + '<br>'

def create_msg_distance (list_distances, list_airports, list_cities):
	msg = str_airport_and_city(list_airports[0], list_cities[0])
	total_d = 0
	for d, airport, city in zip (list_distances, list_airports[1:],
				     list_cities[1:]):
		total_d += d
		msg += ' --- %d km --- %s' % (d, 
					      str_airport_and_city (airport,
								    city))
	if len(list_distances) > 1:
		msg += ' ------------ total: %d km<br>' % total_d
	return msg

