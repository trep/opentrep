--
-- @brief SQL script creating the indexes for the OpenTrep-related tables.
--        See also create_trep_tables.sql in the same directory.
-- @author Denis Arnaud <denis.arnaud_ori@m4x.org>
--

--
-- Index structure for table place_details (points of reference)
-- 

ALTER TABLE place_details ADD PRIMARY KEY (`iata_code`);

ALTER TABLE place_details ADD INDEX (`icao_code`);

ALTER TABLE place_details ADD INDEX (`geonameid`);

ALTER TABLE place_details ADD INDEX (`latitude`);

ALTER TABLE place_details ADD INDEX (`longitude`);

ALTER TABLE place_details ADD INDEX (`fcode`);

ALTER TABLE place_details ADD INDEX (`admin1`);
ALTER TABLE place_details ADD INDEX (`admin2`);

ALTER TABLE place_details ADD INDEX (`city_code`);

ALTER TABLE place_details ADD INDEX (`state_code`);

ALTER TABLE place_details ADD INDEX (`country_code`);

ALTER TABLE place_details ADD INDEX (`region_code`);

ALTER TABLE place_details ADD INDEX (`location_type`);

ALTER TABLE place_details ADD INDEX (`population`);

ALTER TABLE place_details ADD INDEX (`elevation`);
-- ALTER TABLE place_details ADD INDEX (`gtopo30`);

ALTER TABLE place_details ADD INDEX (`timezone`);

ALTER TABLE place_details ADD INDEX (`moddate`);


--
-- Index structure for table place_names
--

ALTER TABLE place_names ADD PRIMARY KEY (`language_code`, `iata_code`);

ALTER TABLE place_names ADD INDEX (`iata_code`);

--
-- Index structure for table airport importance
--
-- iata_code, page_rank

ALTER TABLE airport_pageranked ADD PRIMARY KEY (`iata_code`);
