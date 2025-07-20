
--
-- OPTD-maintained list of POR (points of reference, i.e., airports, cities,
-- places, etc.)
-- See http://github.com/opentraveldata/opentraveldata/tree/master/opentraveldata
--

drop table if exists optd_por;
create table optd_por (
 pk varchar(20) NOT NULL,
 location_type varchar(4) default NULL,
 iata_code varchar(3) default NULL,
 icao_code varchar(4) default NULL,
 faa_code varchar(4) default NULL,
 unlocode_code varchar(5) default NULL,
 is_geonames varchar(1) default NULL,
 geoname_id int(11) default NULL,
 envelope_id int(11) default NULL,
 date_from date default NULL,
 date_until date default NULL,
 serialised_place varchar(12000) default NULL
);

--
-- SQLite3 standard load statement (however, there is no correspondance
-- between the table and CSV file formats)
--
-- delete from optd_por;
-- .separator '^'
-- .import optd_por_public.csv optd_por


--
-- Indexes
--
create index optd_por_iata_code on optd_por (iata_code);
create index optd_por_iata_date on optd_por (iata_code, date_from, date_until);
create index optd_por_icao_code on optd_por (icao_code);
create index optd_por_geonameid on optd_por (geoname_id);
create index optd_por_unlocode_code on optd_por (unlocode_code);
