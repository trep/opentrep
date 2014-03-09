--
-- ORI-maintained list of POR (points of reference, i.e., airports, cities,
-- places, etc.)
-- See http://github.com/opentraveldata/optd/tree/trunk/refdata/ORI
--

drop table if exists ori_por;
create table ori_por (
 pk varchar(20) NOT NULL,
 location_type varchar(4) default NULL,
 iata_code varchar(3) default NULL,
 icao_code varchar(4) default NULL,
 faa_code varchar(4) default NULL,
 is_geonames varchar(1) default NULL,
 geoname_id int(11) default NULL,
 envelope_id int(11) default NULL,
 date_from date default NULL,
 date_until date default NULL,
 serialised_place default NULL,
 primary key (pk)
);

--
-- MySQL load statement
--
-- load data local infile 'ori_por_public.csv' replace into table ori_por
-- character set utf8 columns terminated by '^' ignore 1 lines;


--
-- SQLite3 load statement
--
-- delete from ori_por;
-- .separator '^'
-- .import ori_por_public.csv ori_por


--
-- Indexes
--
--
-- SQLite
--
create index ori_por_iata_code on ori_por (iata_code);
create index ori_por_iata_date on ori_por (iata_code, date_from, date_until);
create index ori_por_icao_code on ori_por (icao_code);
create index ori_por_geonameid on ori_por (geoname_id);


--
-- MySQL
--
-- alter table ori_por add primary key (pk);
-- alter table ori_por add index ori_por_iata_code (iata_code asc);
-- alter table ori_por add index ori_por_iata_date (iata_code asc, date_from asc, date_until asc);
-- alter table ori_por add index ori_por_icao_code (icao_code asc);
-- alter table ori_por add index ori_por_geonameid (geoname_id asc);
