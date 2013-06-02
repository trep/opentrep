--
-- See create_ori_por_public_schema.sql
--

-- Insert Paris CDG details into the table
insert into ori_por_public_details
values ('CDG', 'A', 6269554, 'LFPG', '', '', 'Paris - Charles-de-Gaulle', 'Paris - Charles-de-Gaulle', 49.012779, 2.55, 'S', 'AIRP', 0.651959893408, '', '', '', 'FR', '', 'France', 'Europe', 'A8', 'Île-de-France', 'Ile-de-France', '95', 'Département du Val-d''Oise', 'Departement du Val-d''Oise', '', '', '0', '119', '106', 'Europe/Paris', 1.0, 2.0, 1.0, '2008-07-09', '', 'http://en.wikipedia.org/wiki/Paris-Charles_de_Gaulle_Airport');
insert into ori_por_public_details
values ('ORY', 'A', 2988500, 'LFPO', '', '', 'Paris-Orly', 'Paris-Orly', 48.725278, 2.359444, 'S', 'AIRP', 0.278594625966, '', '', '', 'FR', 'FR', 'France', 'Europe', 'A8', 'Île-de-France', 'Ile-de-France', '91', 'Département de l''Essonne', 'Departement de l''Essonne', '913', '91479', '0', '88', '80', 'Europe/Paris', 1.0, 2.0, 1.0, '2012-02-27', '', 'http://en.wikipedia.org/wiki/Orly_Airport');

-- Insert Paris CDG alternate names into the table
insert into ori_por_public_alt_names
values ('CDG', 'A', '6269554', 'es', 'París - Charles de Gaulle', 'p');
insert into ori_por_public_alt_names
values ('CDG', 'A', '6269554', '', 'Roissy Charles de Gaulle', '');
insert into ori_por_public_alt_names
values ('ORY', 'A', 2988500, '', 'Aéroport de Paris-Orly', '');
insert into ori_por_public_alt_names
values ('ORY', 'A', 2988500, '', 'Aéroport d''Orly', '');
insert into ori_por_public_alt_names
values ('ORY', 'A', 2988500, '', 'Orly', '');
insert into ori_por_public_alt_names
values ('ORY', 'A', 2988500, '', 'Paris-Orly', '');
insert into ori_por_public_alt_names
values ('ORY', 'A', 2988500, 'es', 'Aeropuerto París-Orly', 'p');

-- Insert Paris airports served cities names into the table
insert into ori_por_public_served_cities
values ('CDG', 'A', 6269554, 'PAR', 'C', 2988507, 'Paris', 'Paris');
insert into ori_por_public_served_cities
values ('ORY', 'A', 2988500, 'PAR', 'C', 2988507, 'Paris', 'Paris');


--
-- Sanity check
--
-- Display the details of a given POR
select iata_code, location_type, geoname_id,
       name, asciiname, date_from, date_until, moddate
from  ori_por_public_details
where iata_code = 'CDG' or iata_code = 'ORY';

-- Display all the alternate names of a given POR
select dts.iata_code, dts.location_type, dts.geoname_id,
       alt.lang_code, alt.name, alt.specifiers
from  ori_por_public_details dts, ori_por_public_alt_names alt
where dts.iata_code = alt.iata_code
  and dts.location_type = alt.location_type
  and dts.geoname_id = alt.geoname_id
  and (dts.iata_code = 'CDG' or dts.iata_code = 'ORY');

-- Display all the served cities of a given POR
select dts.iata_code, dts.location_type, dts.geoname_id,
	   cty.city_iata_code, cty.city_location_type, cty.city_geoname_id,
	   cty.city_UTF8_name, cty.city_ASCII_name
from  ori_por_public_details dts, ori_por_public_served_cities cty
where dts.iata_code = cty.iata_code
  and dts.location_type = cty.location_type
  and dts.geoname_id = cty.geoname_id
  and (dts.iata_code = 'CDG' or dts.iata_code = 'ORY');

-- Display all the serving travel-related POR of a given city
select cty.city_iata_code, cty.city_location_type, cty.city_geoname_id,
	   cty.city_UTF8_name, cty.city_ASCII_name,
	   dts.iata_code, dts.location_type, dts.geoname_id, dts.name
from  ori_por_public_details dts, ori_por_public_served_cities cty
where dts.iata_code = cty.iata_code
  and dts.location_type = cty.location_type
  and dts.geoname_id = cty.geoname_id
  and cty.city_iata_code = 'PAR';

--
-- Cleaning
--
delete from ori_por_public_details;
delete from ori_por_public_alt_names;
delete from ori_por_public_served_cities;

