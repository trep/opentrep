--
-- See create_optd_por_public_schema.sql
--

-- SQLite3 separator
.separator '^'

-- Insert Paris CDG details into the table
insert into optd_por_public
values ('CDG', 'LFPG', '', 'Y', 6269554, '', 'Paris - Charles-de-Gaulle', 'Paris - Charles-de-Gaulle', 49.012779, 2.55, 'S', 'AIRP', 0.651959893408, '', '', '', 'FR', '', 'France', 'Europe', 'A8', 'Île-de-France', 'Ile-de-France', '95', 'Département du Val-d''Oise', 'Departement du Val-d''Oise', '', '', '0', '119', '106', 'Europe/Paris', 1.0, 2.0, 1.0, 2008-07-09, 'PAR|C|2988507', 'Paris', 'Paris', '', '', 427, 'France', 'A', 'http://en.wikipedia.org/wiki/Paris-Charles_de_Gaulle_Airport', 'es|París - Charles de Gaulle|p=|Roissy Charles de Gaulle|');
insert into optd_por_public
values ('ORY', 'LFPO', '', 'Y', 2988500, '', 'Paris-Orly', 'Paris-Orly', 48.725278, 2.359444, 'S', 'AIRP', 0.278594625966, '', '', '', 'FR', 'FR', 'France', 'Europe', 'A8', 'Île-de-France', 'Ile-de-France', '91', 'Département de l''Essonne', 'Departement de l''Essonne', '913', '91479', '0', '88', '80', 'Europe/Paris', 1.0, 2.0, 1.0, 2012-02-27, 'PAR|C|2988507', 'Paris', 'Paris', '', '', 427, 'France', 'A', 'http://en.wikipedia.org/wiki/Orly_Airport', '|Aéroport de Paris-Orly|=|Aéroport d''Orly|=|Orly|=|Paris-Orly|=es|Aeropuerto París-Orly|p');
insert into optd_por_public
values ('PAR', 'ZZZZ', '', 'Y', 2988507, '', 'Paris', 'Paris', 48.85341, 2.3488, 'P', 'PPLC', 1.0, '', '', '', 'FR', '', 'France', 'Europe', 'A8', 'Île-de-France', 'Ile-de-France', '75', 'Paris', 'Paris', '751', '75056', '2138551', '', '42', 'Europe/Paris', 1.0, 2.0, 1.0, 2012-08-19, 'PAR|C|2988507', 'Paris', 'Paris', 'BVA,CDG,JDP,JPU,LBG,ORY,POX,VIY,XCR,XEX,XGB,XHP,XJY,XPG,XTT', '', 427, 'France', 'C', 'http://en.wikipedia.org/wiki/Paris', 'en|Paris|p=fr|Paris|p=ru|Париж|=');
insert into optd_por_public
values ('RDU', 'KRDU', '', 'Y', 4487056, '', 'Raleigh-Durham International Airport', 'Raleigh-Durham International Airport', 35.87946, -78.7871, 'S', 'AIRP', 0.0818187017848, '', '', '', 'US', '', 'United States', 'North America', 'NC', 'North Carolina', 'North Carolina', '183', 'Wake County', 'Wake County', '', '', '0', '126', '124', 'America/New_York', -5.0, -4.0, -5.0, 2011-12-11, 'RDU|C|4464368=RDU|C|4487042', 'Raleigh=Durham', 'Raleigh=Durham', '', 'NC', 36, 'North Carolina', 'A', 'http://en.wikipedia.org/wiki/Raleigh%E2%80%93Durham_International_Airport', '');
insert into optd_por_public
values ('RDU', 'ZZZZ', '', 'Y', 4464368, '', 'Durham', 'Durham', 35.99403, -78.89862, 'P', 'PPLA2', 0.0818187017848, '', '', '', 'US', '', 'United States', 'North America', 'NC', 'North Carolina', 'North Carolina', '063', 'Durham County', 'Durham County', '', '', '228330', '123', '121', 'America/New_York', -5.0, -4.0, -5.0, 2012-11-27, 'RDU', 'Raleigh', 'Raleigh', 'RDU', 'NC', 36, 'North Carolina', 'C', 'http://en.wikipedia.org/wiki/Durham%2C_North_Carolina', 'en|Durham|p=ru|Дарем|=zh|德罕|');
insert into optd_por_public
values ('RDU', 'ZZZZ', '', 'Y', 4487042, '', 'Raleigh', 'Raleigh', 35.7721, -78.63861, 'P', 'PPLA', 0.0818187017848, '', '', '', 'US', '', 'United States', 'North America', 'NC', 'North Carolina', 'North Carolina', '183', 'Wake County', 'Wake County', '', '', '403892', '96', '99', 'America/New_York', -5.0, -4.0, -5.0, 2012-11-27, 'RDU', 'Raleigh', 'Raleigh', 'RDU', 'NC', 36, 'North Carolina', 'C', 'http://en.wikipedia.org/wiki/Raleigh%2C_North_Carolina', 'en|Raleigh|p=ru|Роли|=zh|罗利|');


--
-- Sanity check
--
-- Display the details of a given POR
select iata_code, location_type, geoname_id,
       name, asciiname, date_from, date_until, moddate,
	   latitude, longitude, page_rank,
	   city_code_list, city_UTF8_name_list, city_ASCII_name_list,
	   tvl_por_list, alt_name_section
from  optd_por_public
where iata_code = 'CDG' or iata_code = 'ORY' or iata_code = 'PAR'
   or iata_code = 'RDU';

--
-- Cleaning
--
delete from optd_por_public;
