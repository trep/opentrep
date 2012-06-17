--
--
--
set @saved_cs_client     = @@character_set_client;
set character_set_client = utf8;

--
-- Place details
-- Does not depend on language
--
drop table if exists place_details;
create table place_details (
 iata_code varchar(3) NOT NULL,
 xapian_docid integer default NULL,
 icao_code varchar(4) default NULL,
 is_geonames varchar(1) NOT NULL,
 geonameid int(11) default NULL,
 latitude decimal(10,7) default NULL,
 longitude decimal(10,7) default NULL,
 fclass varchar(1) default NULL,
 fcode varchar(10) default NULL,
 country_code varchar(2) default NULL,
 cc2 varchar(60) default NULL,
 admin1 varchar(20) default NULL,
 admin2 varchar(80) default NULL,
 admin3 varchar(20) default NULL,
 admin4 varchar(20) default NULL,
 population int(11) default NULL,
 elevation int(11) default NULL,
 gtopo30 int(11) default NULL,
 timezone varchar(40) default NULL,
 gmt_offset decimal(3,1) default NULL,
 dst_offset decimal(3,1) default NULL,
 raw_offset decimal(3,1) default NULL,
 moddate date default NULL,
 is_airport varchar(1) default NULL,
 is_commercial varchar(1) default NULL,
 city_code varchar(3) default NULL,
 state_code varchar(3) default NULL,
 region_code varchar(5) default NULL,
 location_type varchar(4) default NULL,
 wiki_link varchar(200) default NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COLLATE=utf8_unicode_ci;

--
-- Place names
-- Depends on language
--
drop table if exists place_names;
create table place_names (
  language_code char(2) not null,
  iata_code char(3) not null,
  ascii_name varchar(200) default null,
  utf_name varchar(200) collate utf8_unicode_ci default NULL,
  alternate_name1 varchar(200) collate utf8_unicode_ci,
  alternate_name2 varchar(200) collate utf8_unicode_ci,
  alternate_name3 varchar(200) collate utf8_unicode_ci,
  alternate_name4 varchar(200) collate utf8_unicode_ci,
  alternate_name5 varchar(200) collate utf8_unicode_ci,
  alternate_name6 varchar(200) collate utf8_unicode_ci,
  alternate_name7 varchar(200) collate utf8_unicode_ci,
  alternate_name8 varchar(200) collate utf8_unicode_ci,
  alternate_name9 varchar(200) collate utf8_unicode_ci,
  alternate_name10 varchar(200) collate utf8_unicode_ci
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COLLATE=utf8_unicode_ci;

--
-- Structure for the table storing airport importance (PageRank-ed thanks to
-- shcedule).
-- Note: an more up-to-date version may be available in the OpenTravelData
-- project:
-- https://github.com/opentraveldata/optd/blob/trunk/refdata/ORI/create_ori_tables.sql#L205
--
drop table if exists airport_pageranked;
create table airport_pageranked (
 iata_code char(3) NOT NULL,
 por_type char(1) NOT NULL,
 page_rank decimal(15,12) NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COLLATE=utf8_unicode_ci;


--
--
--
set character_set_client = @saved_cs_client;


