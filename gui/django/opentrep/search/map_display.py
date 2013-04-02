import re

#
def escape_quotes_function (str_text):
	return re.sub(r'\'', "\\'", str_text)

#
def str_airport_and_city (place, city_name, escape_quotes = False):
	if escape_quotes:
		city_name = escape_quotes_function (city_name)
	out_city_name = ''
	if city_name != 'NA':
		out_city_name = ' (' + city_name + ')'
	return '%s%s' % (place.name_utf, out_city_name)

#
def create_marker (place, lat, lon, city_name):
	str_text = '<p style="font-size:small;">%s<br>lat: %.2f<br>lon: %.2f</p>' % (str_airport_and_city (place, city_name, escape_quotes=True), lat, lon)
	return {'lat': lat, 'lon': lon, 'name': city_name, 'text': str_text}

