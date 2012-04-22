--
-- Export data from the place_names table
--
select concat_ws('^', language_code, iata_code, ascii_name, utf_name, 
	   alternate_name1, alternate_name2, alternate_name3, alternate_name4, 
	   alternate_name5, alternate_name6, alternate_name7, alternate_name8, 
	   alternate_name9, alternate_name10)
from place_names;

