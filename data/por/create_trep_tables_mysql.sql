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
 serialised_place varchar(8000) default NULL
);

--
-- MySQL standard load statement (however, there is no correspondance
-- between the table and CSV file formats)
--
-- load data local infile 'ori_por_public.csv' replace into table ori_por
-- character set utf8 columns terminated by '^' ignore 1 lines;


--
-- Indexes
--
alter table ori_por add primary key (pk);
alter table ori_por add index ori_por_iata_code (iata_code asc);
alter table ori_por add index ori_por_iata_date (iata_code asc, date_from asc, date_until asc);
alter table ori_por add index ori_por_icao_code (icao_code asc);
alter table ori_por add index ori_por_geonameid (geoname_id asc);
