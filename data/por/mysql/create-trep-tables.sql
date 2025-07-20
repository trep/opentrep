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
-- MySQL/MariaDB standard load statement (however, there is no correspondance
-- between the table and CSV file formats)
--
-- load data local infile 'optd_por_public.csv' replace into table optd_por
-- character set utf8 columns terminated by '^' ignore 1 lines;


--
-- Indexes
--
alter table optd_por add primary key (pk);
alter table optd_por add index optd_por_iata_code (iata_code asc);
alter table optd_por add index optd_por_iata_date (iata_code asc, date_from asc, date_until asc);
alter table optd_por add index optd_por_icao_code (icao_code asc);
alter table optd_por add index optd_por_geonameid (geoname_id asc);
alter table optd_por add index optd_por_unlocode_code (unlocode_code asc);
