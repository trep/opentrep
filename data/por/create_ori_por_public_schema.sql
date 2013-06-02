--
-- ORI-maintained list of POR (points of reference, i.e., airports, cities,
-- places, etc.)
-- See https://github.com/opentraveldata/optd/tree/trunk/refdata/ORI
--

--
-- Note: the index is created in a separate file, namely create_ori_indexes.sql
--
--
-- ORI-maintained list of POR:
-- ---------------------------

-- iata_code         : IATA code; varchar(3). See also:
--                     http://www.iata.org/ps/publications/Pages/code-search.aspx
-- icao_code         : ICAO code; varchar(4)
-- faa_code          : FAA code; varchar(4)
-- is_geoname        : Whether that POR is referenced in Geonames; varchar(1)
-- geoname_id        : Integer ID of record in geonames database; int(11)
-- envelope_id       : ID of the envelope. Empty when valid, i.e., for the
--                     current envelope/state. Incrementing ID when no longer
--                     valid; the validity date range then allows to derive when
--                     that POR was valid.
-- name              : Name of geographical point
--                     (UTF8) varchar(200)
-- asciiname         : Name of geographical point in plain ascii characters
--                     (ASCII) varchar(200)
-- latitude          : Latitude in decimal degrees (wgs84)
-- longitude         : Longitude in decimal degrees (wgs84)
-- feature_class     : See http://www.geonames.org/export/codes.html
--                     char(1)
-- feature_code      : See http://www.geonames.org/export/codes.html
--                     varchar(10)
-- page_rank         : PageRank value, reflecting the importance of the POR;
--                     100% is the most important one. float
-- date_from         : Beginning date of the validity period. date
-- date_until        : End date of the validity period. date
-- comment           : Comment. Mainly used for no longer valid POR.
--                     varchar(2000)
-- country_code      : ISO-3166 2-letter country code, 2 characters
-- cc2               : Alternate country codes, comma separated, ISO-3166
--                     2-letter country code, 60 characters
-- country_name      : Name of the country; varchar(200)
-- continent_name    : Name of the continent; varchar(20)
-- admin1_code       : FIPS code (subject to change to ISO code), see exceptions
--                     below. See file admin1Codes.txt for display names of
--                     this code; varchar(20)
-- admin1_UTF8_name  : UTF-8 name of the administrative level 1; varchar(200)
-- admin1_ASCII_name : ASCII name of the administrative level 1; varchar(200)
-- admin2_code       : Code for the second administrative division, a county
--                     in the US. See file admin2Codes.txt; varchar(20)
-- admin2_UTF8_name  : UTF-8 name of the administrative level 2; varchar(200)
-- admin2_ASCII_name : ASCII name of the administrative level 2; varchar(200)
-- admin3_code       : Code for third level administrative division; varchar(20)
-- admin4_code       : Code for fourth level administrative division; varchar(20)
-- population        : Number of inhabitants; int(11)
-- elevation         : Elevation, in meters; int(11)
-- gtopo30           : Digital elevation model (dem), srtm3 or gtopo30, average
--                     elevation of 3''x3'' (ca 90mx90m) or 30''x30'' (ca 900mx900m) area
--                     in meters. srtm processed by cgiar/ciat. int(11)
-- time_zone         : The time-zone ID (see file timeZone.txt); varchar(100)
-- gmt_offset        : GMT offset on 1st of January
-- dst_offset        : DST offset to GMT on 1st of July (of the current year)
-- raw_offset        : Raw Offset without DST
-- modification_date : Date of last modification in yyyy-MM-dd format
-- city_code_list    : The (list of) IATA code(s) of the related (list of)
--                     cities.
--                     varchar(100)
-- city_name_list    : UTF-8 names of the served (list of) cities
--                     varchar(500)
-- city_detail_list  : Details (Geonames ID, names) of the served (list of)
--                     cities
--                     varchar(500)
-- tvl_por_list      : list of the IATA codes of the travel-related POR serving
--                     the city.
--                     varchar(100)
-- state_code        : The ISO code of the related state; varchar(3)
-- location type     : A/APT airport; B/BUS bus/coach station; C/CITY City;
--                     G/GRD ground transport (this code is used for SK in
--                     Sweden only); H/HELI Heliport;
--                     O/OFF-PT off-line point, i.e. a city without an airport;
--                     R/RAIL railway Station; S/ASSOC a location without its
--                     own IATA code, but attached to an IATA location.
-- wiki_link         : Link onto the Wikipedia article, when existing
-- alt_name_section  : List of the alternate names, with their language and specifiers
--                     (p for preferred, s for short, c for colloquial, h for historical).
--                     The list if separated by equal ('=') signs. The fields are
--                     separated by pipe ('|') signs. varchar(2000)
--
--
-- Continents:
-- -----------
-- AF      Africa  (geonameId=6255146)
-- AS      Asia    (geonameId=6255147)
-- EU      Europe  (geonameId=6255148)
-- NA      North America   (geonameId=6255149)
-- OC      Oceania (geonameId=6255151)
-- SA      South America   (geonameId=6255150)
-- AN      Antarctica      (geonameId=6255152)
--
-- Samples:
-- CDG^LFPG^^Y^6269554^^Paris - Charles-de-Gaulle^Paris - Charles-de-Gaulle^49.012779^2.55^S^AIRP^0.651959893408^^^^FR^^France^Europe^A8^Île-de-France^Ile-de-France^95^Département du Val-d'Oise^Departement du Val-d'Oise^^^0^119^106^Europe/Paris^1.0^2.0^1.0^2008-07-09^PAR^Paris^Paris^^^A^http://en.wikipedia.org/wiki/Paris-Charles_de_Gaulle_Airport^es|París - Charles de Gaulle|p=|Roissy Charles de Gaulle|
-- ORY^LFPO^^Y^2988500^^Paris-Orly^Paris-Orly^48.725278^2.359444^S^AIRP^0.278594625966^^^^FR^FR^France^Europe^A8^Île-de-France^Ile-de-France^91^Département de l'Essonne^Departement de l'Essonne^913^91479^0^88^80^Europe/Paris^1.0^2.0^1.0^2012-02-27^PAR^Paris^Paris^^^A^http://en.wikipedia.org/wiki/Orly_Airport^|Aéroport de Paris-Orly|=|Aéroport d'Orly|=|Orly|=|Paris-Orly|=es|Aeropuerto París-Orly|p
-- PAR^ZZZZ^^Y^2988507^^Paris^Paris^48.85341^2.3488^P^PPLC^1.0^^^^FR^^France^Europe^A8^Île-de-France^Ile-de-France^75^Paris^Paris^751^75056^2138551^^42^Europe/Paris^1.0^2.0^1.0^2012-08-19^PAR^Paris^Paris^BVA,CDG,JDP,JPU,LBG,ORY,POX,VIY,XCR,XEX,XGB,XHP,XJY,XPG,XTT^^C^http://en.wikipedia.org/wiki/Paris^en|Paris|p=fr|Paris|p=ru|Париж|=
-- RDU^KRDU^^Y^4487056^^Raleigh-Durham International Airport^Raleigh-Durham International Airport^35.87946^-78.7871^S^AIRP^0.0818187017848^^^^US^^United States^North America^NC^North Carolina^North Carolina^183^Wake County^Wake County^^^0^126^124^America/New_York^-5.0^-4.0^-5.0^2011-12-11^RDU^Raleigh^Raleigh^^NC^A^http://en.wikipedia.org/wiki/Raleigh%E2%80%93Durham_International_Airport^
-- RDU^ZZZZ^^Y^4464368^^Durham^Durham^35.99403^-78.89862^P^PPLA2^0.0818187017848^^^^US^^United States^North America^NC^North Carolina^North Carolina^063^Durham County^Durham County^^^228330^123^121^America/New_York^-5.0^-4.0^-5.0^2012-11-27^RDU^Raleigh^Raleigh^RDU^NC^C^http://en.wikipedia.org/wiki/Durham%2C_North_Carolina^en|Durham|p=ru|Дарем|=zh|德罕|
-- RDU^ZZZZ^^Y^4487042^^Raleigh^Raleigh^35.7721^-78.63861^P^PPLA^0.0818187017848^^^^US^^United States^North America^NC^North Carolina^North Carolina^183^Wake County^Wake County^^^403892^96^99^America/New_York^-5.0^-4.0^-5.0^2012-11-27^RDU^Raleigh^Raleigh^RDU^NC^C^http://en.wikipedia.org/wiki/Raleigh%2C_North_Carolina^en|Raleigh|p=ru|Роли|=zh|罗利|
--

drop table if exists ori_por_public;
create table ori_por_public (
 iata_code varchar(3) NOT NULL,
 icao_code varchar(4) default NULL,
 faa_code varchar(4) default NULL,
 is_geonames varchar(1) NOT NULL,
 geoname_id int(11) default NULL,
 envelope_id int(11) default NULL,
 name varchar(200) default NULL,
 asciiname varchar(200) default NULL,
 latitude decimal(10,7) default NULL,
 longitude decimal(10,7) default NULL,
 fclass varchar(1) default NULL,
 fcode varchar(10) default NULL,
 page_rank decimal(15,12) default NULL,
 date_from date default NULL,
 date_until date default NULL,
 comment varchar(500) default NULL,
 country_code varchar(2) default NULL,
 cc2 varchar(60) default NULL,
 country_name varchar(200) default NULL,
 continent_name varchar(20) default NULL,
 admin1_code varchar(20) default NULL,
 admin1_UTF8_name varchar(200) default NULL,
 admin1_ASCII_name varchar(200) default NULL,
 admin2_code varchar(80) default NULL,
 admin2_UTF8_name varchar(200) default NULL,
 admin2_ASCII_name varchar(200) default NULL,
 admin3_code varchar(20) default NULL,
 admin4_code varchar(20) default NULL,
 population int(11) default NULL,
 elevation int(11) default NULL,
 gtopo30 int(11) default NULL,
 time_zone varchar(100) default NULL,
 gmt_offset decimal(3,1) default NULL,
 dst_offset decimal(3,1) default NULL,
 raw_offset decimal(3,1) default NULL,
 moddate date default NULL,
 city_code_list varchar(100) default NULL,
 city_name_list varchar(500) default NULL,
 city_detail_list varchar(500) default NULL,
 tvl_por_list varchar(100) default NULL,
 state_code varchar(3) default NULL,
 location_type varchar(4) default NULL,
 wiki_link varchar(200) default NULL,
 alt_name_section varchar(4000) default NULL
);

--
-- MySQL load statement
--
-- load data local infile 'ori_por_public.csv' replace into table ori_por_public
-- character set utf8 columns terminated by '^' ignore 1 lines;


--
-- SQLite3 load statement
--
-- delete from ori_por_public;
-- .separator '^'
-- .import ori_por_public.csv ori_por_public

--
-- Structure for the table storing airport importance (PageRank-ed thanks to
-- schedule)
--

drop table if exists ori_por_pagerank;
create table ori_por_pagerank (
 pk varchar(6) NOT NULL,
 iata_code varchar(3) NOT NULL,
 page_rank decimal(15,12) NOT NULL
);

