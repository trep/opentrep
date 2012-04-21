--
-- Export data from the place_details table
--
select concat_ws('^', iata_code, icao_code, is_geonames, geonameid,
	    latitude, longitude, fclass, fcode,
		country_code, cc2, admin1, admin2, admin3, admin4,
		population, elevation, gtopo30,
		timezone, gmt_offset, dst_offset, raw_offset,
		moddate, is_airport, is_commercial, city_code, state_code, region_code,
		location_type)
from place_details;


