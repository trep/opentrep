#
import math, re

#
def get_random_airport (openTrepLibrary, travelPB2):
  # OpenTREP
  result = openTrepLibrary.getRandom()
  # Protobuf
  place = travelPB2.Place()
  place.ParseFromString (result)
  return place

#
def get_lat_lon (place):
  return place.coord.latitude, place.coord.longitude

#
def get_lon_lat (place):
  return place.coord.longitude, place.coord.latitude

# 
def get_country_code (place):
  return place.country_code.code

# 
def get_country_name (place):
  return place.country_name

#
def get_city_code (place):
  return place.city_code.code

#
def get_city_names (place):
  return place.city_name_utf, place.city_name_ascii

#
def get_airport_names (place, nice=False):
  ugly_name_utf = place.name_utf
  ugly_name_ascii = place.city_name_ascii
  if not nice:
    return ugly_name_utf, ugly_name_ascii

  # "Ugly" names typically contain the country code (2 uppercase letters)
  # followed by a truncated repetition of the POR name. For instance:
  # - Cagnes Sur Mer FR Cagnes Sur M
  # - Antibes FR Antibes
  # We just truncate the name and keep the part before the country code.
  nice_name_utf = re.sub (r"(.*)([ ])([A-Z]{2})([ ])(.*)", "\\1", ugly_name_utf)
  nice_name_ascii = re.sub (r"(.*)([ ])([A-Z]{2})([ ])(.*)", "\\1",
                            ugly_name_ascii)
  return nice_name_utf, nice_name_ascii

#
def get_continent_code (place):
  return place.continent_code.code

#
def get_continent_name (place):
  return place.continent_name

#
def get_country_and_continent (place):
  return place.country_code.code, place.continent_code.code

# 
def great_circle_distance (lat1, lon1, lat2, lon2, degrees=True):
  if degrees:
    lat1 = lat1/180.0*math.pi
    lon1 = lon1/180.0*math.pi
    lat2 = lat2/180.0*math.pi
    lon2 = lon2/180.0*math.pi

    diameter = 12742.0
    lat_diff = (lat2-lat1) / 2.0
    lat_diff_sin = math.sin (lat_diff)
    lon_diff = (lon2-lon1) / 2.0
    lon_diff_sin = math.sin (lon_diff)
    lat_cos = math.cos (lat1) * math.cos (lat2)
    proj_dist = lat_diff_sin**2.0 + lat_cos * lon_diff_sin**2.0
    gcd = diameter * math.asin (math.sqrt (proj_dist))
  return gcd

#
def get_distance_km (place1, place2):
  lat1, lon1 = get_lat_lon (place1)
  lat2, lon2 = get_lat_lon (place2)
  dist = great_circle_distance (lat1, lon1, lat2, lon2)
  return dist

#
def get_local_local_flight_duration_hr (place1, place2):
  lat1, lon1 = get_lat_lon (place1)
  lat2, lon2 = get_lat_lon (place2)
  dist = great_circle_distance (lat1, lon1, lat2, lon2)
  travel_hr = 0.5 + dist/800.0
  time_diff_hr = (cit2['lon'] - cit1['lon']) / 15.0
  return travel_hr + time_diff_hr, unknown

