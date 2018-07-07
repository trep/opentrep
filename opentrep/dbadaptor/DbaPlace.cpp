// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// STL
#include <exception>
#include <string>
#include <sstream>
#include <ctime>
// OpenTrep
#include <opentrep/basic/BasConst_General.hpp>
#include <opentrep/bom/Place.hpp>
#include <opentrep/dbadaptor/DbaPlace.hpp>
#include <opentrep/service/Logger.hpp>

namespace soci {

  // //////////////////////////////////////////////////////////////////////
  // TODO: that method is obviously deprecated. Remove it.
  void type_conversion<OPENTREP::Place>::
  from_base (values const& iPlaceValues, indicator /* ind */,
             OPENTREP::Place& ioPlace) {
    /*
         select dts.iata_code, dts.location_type, dts.geoname_id,
         icao_code, faa_code,
         envelope_id, dts.name as utf_name, asciiname, latitude, longitude,
         fclass, fcode, page_rank,
         date(date_from) as date_from, date(date_until) as date_until,
         comment,
         country_code, cc2, country_name, continent_name,
         admin1_code, admin1_UTF8_name, admin1_ASCII_name,
         admin2_code, admin2_UTF8_name, admin2_ASCII_name,
         admin3_code, admin4_code,
         population, elevation, gtopo30,
         time_zone, gmt_offset, dst_offset, raw_offset,
         date(moddate) as moddate,
         state_code, wac, wac_name, wiki_link,
         alt.lang_code as alt_lang_code, alt.name as alt_name,
         alt.specifiers as alt_spec,
         city_iata_code, city_location_type, city_geoname_id,
         city_UTF8_name, city_ASCII_name
         from optd_por_public_details dts, optd_por_public_alt_names alt,
              optd_por_public_served_cities cty
         where dts.iata_code = alt.iata_code
           and dts.location_type = alt.location_type
           and dts.geoname_id = alt.geoname_id
           and dts.iata_code = cty.iata_code
           and dts.location_type = cty.location_type
           and dts.geoname_id = cty.geoname_id
         order by dts.iata_code, dts.location_type, dts.geoname_id
         ;
    */

    /*
     * Retrieve the place key (IATA code, location type, Geonames ID).
     * If ioPlace has already that key, then only the alternate names need
     * to be added.
     * Otherwise, all the fields of the Place need to be set.
     */
    // First, retrieve the key from the database fields
    //   IATA code
    const std::string lIataCodeStr =
      iPlaceValues.get<std::string> ("iata_code", "");
    const OPENTREP::IATACode_T lIataCode (lIataCodeStr);
    //   Location type ('C' for city only, 'A' for airport only,
    //   'CA' for airport/city)
    const std::string lIataTypeStr =
      iPlaceValues.get<std::string> ("location_type", "");
    const OPENTREP::IATAType lIataType (lIataTypeStr);
    //   Geonames ID
    const OPENTREP::GeonamesID_T lGeonameID =
      iPlaceValues.get<int> ("geoname_id", 0);
    //
    const OPENTREP::LocationKey lNewPlaceKey (lIataCode, lIataType, lGeonameID);

    // Then, retrieve the key from the previous Place object
    const OPENTREP::LocationKey lPlaceKey = ioPlace.getKey();

    // Compare the keys
    if (lNewPlaceKey != lPlaceKey) {
      // The place keys are different: the current place retrieved from
      // the database is not the same as the one retrieved previously

      // Re-set the (STL) sets of terms for the Xapian index, spelling
      // dictionary, etc.
      ioPlace.resetIndexSets();
    
      // Reset the list of names of the given Place object
      ioPlace.resetMatrix();

      // The IATA code will be set to the default value (empty string)
      // when the column is null
      ioPlace.setIataCode (lIataCode);

      // The ICAO code will be set to the default value (empty string)
      // when the column is null
      ioPlace.setIataType (lIataTypeStr);

      // Geonames ID
      ioPlace.setGeonamesID (lGeonameID);

      // Envelope ID
      const OPENTREP::EnvelopeID_T lEnvelopeID =
        iPlaceValues.get<int> ("envelope_id");
      ioPlace.setEnvelopeID (lEnvelopeID);

      // Beginning date of the validity range (empty when indefinitely valid)
      const std::string lDateFromStr =
        iPlaceValues.get<std::string> ("date_from", OPENTREP::DEFAULT_DATE_STR);
      const OPENTREP::Date_T lDateFrom (boost::gregorian::from_string (lDateFromStr));
      ioPlace.setDateFrom (lDateFrom);

      // End date of the validity range (empty when indefinitely valid)
      const std::string lDateUntilStr =
        iPlaceValues.get<std::string> ("date_until", OPENTREP::DEFAULT_DATE_STR);
      const OPENTREP::Date_T lDateUntil (boost::gregorian::from_string (lDateUntilStr));
      ioPlace.setDateEnd (lDateUntil);

      // The comment will be set to the default value (empty string)
      // when the column is null
      ioPlace.setComment (iPlaceValues.get<std::string> ("comment", ""));

      // The UTF8 name will be set to the default value (empty string)
      // when the column is null
      ioPlace.setCommonName (iPlaceValues.get<std::string> ("utf_name", ""));

      // The ASCII name will be set to the default value (empty string)
      // when the column is null
      ioPlace.setAsciiName (iPlaceValues.get<std::string> ("asciiname", ""));

      // The FAA code will be set to the default value (empty string)
      // when the column is null
      ioPlace.setFaaCode (iPlaceValues.get<std::string> ("faa_code", ""));

      // Feature class
      const std::string lFeatClass = iPlaceValues.get<std::string>("fclass", "");
      ioPlace.setFeatureClass (lFeatClass);
      // Feature code
      const std::string lFeatCode = iPlaceValues.get<std::string> ("fcode", "");
      ioPlace.setFeatureCode (lFeatCode);

      // The state code will be set to the default value (empty string)
      // when the column is null
      ioPlace.setStateCode (iPlaceValues.get<std::string> ("state_code", ""));
      ioPlace.setCountryCode (iPlaceValues.get<std::string> ("country_code"));
      ioPlace.setCountryName (iPlaceValues.get<std::string> ("country_name"));
      ioPlace.setAltCountryCode (iPlaceValues.get<std::string> ("cc2", ""));
      ioPlace.setContinentName (iPlaceValues.get<std::string>("continent_name"));
      ioPlace.setLatitude (iPlaceValues.get<double> ("latitude"));
      ioPlace.setLongitude (iPlaceValues.get<double> ("longitude"));
      ioPlace.setPageRank (iPlaceValues.get<double> ("page_rank",
                                                     OPENTREP::K_DEFAULT_PAGE_RANK));

      // The administrative level 1 details will be set to the default value
      // (empty string) when the column is null
      ioPlace.setAdmin1Code (iPlaceValues.get<std::string> ("admin1_code", ""));
      ioPlace.setAdmin1UtfName (iPlaceValues.get<std::string> ("admin1_UTF8_name", ""));
      ioPlace.setAdmin1AsciiName (iPlaceValues.get<std::string> ("admin1_ASCII_name", ""));

      // The administrative level 1 details will be set to the default value
      // (empty string) when the column is null
      ioPlace.setAdmin2Code (iPlaceValues.get<std::string> ("admin2_code", ""));
      ioPlace.setAdmin2UtfName (iPlaceValues.get<std::string> ("admin2_UTF8_name", ""));
      ioPlace.setAdmin2AsciiName (iPlaceValues.get<std::string> ("admin2_ASCII_name", ""));

      // The administrative level 3 code will be set to the default value
      // (empty string) when the column is null
      ioPlace.setAdmin3Code (iPlaceValues.get<std::string> ("admin3_code", ""));

      // The administrative level 4 code will be set to the default value
      // (empty string) when the column is null
      ioPlace.setAdmin4Code (iPlaceValues.get<std::string> ("admin4_code", ""));

      // Population
      const OPENTREP::Population_T lPopulation =
        iPlaceValues.get<int> ("population");
      ioPlace.setPopulation (lPopulation);

      // Elevation
      const OPENTREP::Elevation_T lElevation =
        iPlaceValues.get<int> ("elevation");
      ioPlace.setElevation (lElevation);

      // GTopo30
      const OPENTREP::GTopo30_T lGTopo30 = iPlaceValues.get<int> ("gtopo30");
      ioPlace.setGTopo30 (lGTopo30);

      // The time-zone code will be set to the default value (empty string)
      // when the column is null
      ioPlace.setTimeZone (iPlaceValues.get<std::string> ("time_zone", ""));

      // The time off-sets will be set to the default value (empty string)
      // when the column is null
      const OPENTREP::GMTOffset_T lGMTOffset =
        iPlaceValues.get<int> ("gmt_offset", 0.0);
      ioPlace.setGMTOffset (lGMTOffset);
      const OPENTREP::DSTOffset_T lDSTOffset =
        iPlaceValues.get<int> ("dst_offset", 0.0);
      ioPlace.setDSTOffset (lDSTOffset);
      const OPENTREP::RawOffset_T lRawOffset =
        iPlaceValues.get<int> ("raw_offset", 0.0);
      ioPlace.setRawOffset (lRawOffset);

      // Modification date
      const std::string lModDateStr =
        iPlaceValues.get<std::string> ("moddate", OPENTREP::DEFAULT_DATE_STR);
      const OPENTREP::Date_T lModDate (boost::gregorian::from_string (lModDateStr));
      ioPlace.setModificationDate (lModDate);

      // The US DOT World Area Code (WAC) will be set to the default value (zero)
      // when the column is null
      ioPlace.setWAC (iPlaceValues.get<int> ("wac", 0));

      // The US DOT World Area Code (WAC) name will be set to the default value
      // (empty string) when the column is null
      ioPlace.setWACName (iPlaceValues.get<std::string> ("wac_name", ""));

      // The Wikipedia link will be set to the default value (empty string)
      // when the column is null
      ioPlace.setWikiLink (iPlaceValues.get<std::string> ("wiki_link", ""));
    }

    // Alternate name
    const std::string& lLanguageString =
      iPlaceValues.get<std::string> ("alt_lang_code", "");
    /*
    if (lLanguageString.empty() == true) {
      std::ostringstream errorStr;
      errorStr << "The language field is empty for the place "
               << ioPlace.getIataCode();
      OPENTREP_LOG_ERROR (errorStr.str());
      throw OPENTREP::LanguageCodeNotDefinedInNameTableException(errorStr.str());
    }
    */

    // TODO: check that that alternate name does not already exist
    // Convert the language code string (e.g., "en") into
    // the corresponding enumeration
    //const OPENTREP::Language::EN_Language lLanguageCode =
    // OPENTREP::Language::getCode (lLanguageString);
    const OPENTREP::LanguageCode_T lLanguageStr (lLanguageString);
    
    // Add the (UTF8) name only when the language code is standard English
    //if (lLanguageCode == OPENTREP::Language::en) {
    // UTF-8 name (which may be null/empty)
    const std::string& lUtfName =
      iPlaceValues.get<std::string> ("alt_name", "");
    ioPlace.addName (lLanguageStr, lUtfName);
    //}

    // TODO: check that that city code/name does not already exist, and add it
    // The city code will be set to the default value (empty string)
    // when the column is null
    //ioPlace.setCityCode (iPlaceValues.get<std::string> ("city_iata_code", ""));
    // The city UTF8 name will be set to the default value (empty string)
    // when the column is null
    //ioPlace.setCityUtfName (iPlaceValues.get<std::string>("city_UTF8_name", ""));
    // The city ASCII name will be set to the default value (empty string)
    // when the column is null
    //ioPlace.setCityAsciiName (iPlaceValues.get<std::string> ("city_ASCII_name", ""));


    // DEBUG
    /*
    OPENTREP_LOG_DEBUG ("[Old:" << lPlaceKey << "][New:" << lNewPlaceKey << "] Lang: "
                        << lLanguageString << " => "
                        << OPENTREP::Language::getLongLabel (lLanguageCode)
                        << " -- Details: " << ioPlace);
    */
  }

  // //////////////////////////////////////////////////////////////////////
  // TODO: that method is obviously deprecated. Remove it.
  void type_conversion<OPENTREP::Place>::
  to_base (const OPENTREP::Place& iPlace, values& ioPlaceValues,
           indicator& ioIndicator) {
    // IATA code
    const OPENTREP::IATACode_T& lIataCode = iPlace.getIataCode();
    const std::string lIataCodeStr (lIataCode);
    const indicator lIataCodeIndicator = lIataCodeStr.empty() ? i_null : i_ok;

    // ICAO code
    const OPENTREP::ICAOCode_T& lIcaoCode = iPlace.getIcaoCode();
    const std::string lIcaoCodeStr (lIcaoCode);
    const indicator lIcaoCodeIndicator = lIcaoCodeStr.empty() ? i_null : i_ok;

    // FAA code
    const OPENTREP::FAACode_T& lFaaCode = iPlace.getFaaCode();
    const std::string lFaaCodeStr (lFaaCode);
    const indicator lFaaCodeIndicator = lFaaCodeStr.empty() ? i_null : i_ok;

    // City IATA code
    //const OPENTREP::IATACode_T& lCityCode = iPlace.getCityCode();
    //const std::string lCityCodeStr (lCityCode);
    //const indicator lCityCodeIndicator = lCityCodeStr.empty() ? i_null : i_ok;

    // State code
    const OPENTREP::StateCode_T& lStateCode = iPlace.getStateCode();
    const std::string lStateCodeStr (lStateCode);
    const indicator lStateCodeIndicator = lStateCodeStr.empty() ? i_null : i_ok;

    // Country code
    const OPENTREP::CountryCode_T& lCountryCode = iPlace.getCountryCode();
    const std::string lCountryCodeStr (lCountryCode);

    // Country name
    const OPENTREP::CountryName_T& lCountryName = iPlace.getCountryName();
    const std::string lCountryNameStr (lCountryName);

    // Continent name
    const OPENTREP::ContinentName_T& lContinentName = iPlace.getContinentName();
    const std::string lContinentNameStr (lContinentName);

    // Time-zone
    const OPENTREP::TimeZone_T& lTimeZone = iPlace.getTimeZone();
    const std::string lTimeZoneStr (lTimeZone);

    // US DOT World Area Code (WAC) name
    const OPENTREP::WACName_T& lWACName = iPlace.getWACName();
    const std::string lWACNameStr (lWACName);

    // Wikipedia link
    const OPENTREP::WikiLink_T& lWikiLink = iPlace.getWikiLink();
    const std::string lWikiLinkStr (lWikiLink);

    //
    ioPlaceValues.set ("iata_code", lIataCodeStr, lIataCodeIndicator);
    ioPlaceValues.set ("icao_code", lIcaoCodeStr, lIcaoCodeIndicator);
    ioPlaceValues.set ("geoname_id", iPlace.getGeonamesID());
    ioPlaceValues.set ("faa_code", lFaaCodeStr, lFaaCodeIndicator);
    //ioPlaceValues.set ("city_code", lCityCodeStr, lCityCodeIndicator);
    ioPlaceValues.set ("state_code", lStateCodeStr, lStateCodeIndicator);
    ioPlaceValues.set ("country_code", lCountryCodeStr);
    ioPlaceValues.set ("country_name", lCountryNameStr);
    ioPlaceValues.set ("continent_name", lContinentNameStr);
    ioPlaceValues.set ("time_zone", lTimeZoneStr);
    ioPlaceValues.set ("latitude", iPlace.getLatitude());
    ioPlaceValues.set ("longitude", iPlace.getLongitude());
    ioPlaceValues.set ("page_rank", iPlace.getPageRank());
    ioPlaceValues.set ("wac", iPlace.getWAC());
    ioPlaceValues.set ("wac_name", lWACNameStr);
    ioPlaceValues.set ("wiki_link", lWikiLinkStr);
    ioIndicator = i_ok;
  }

}

namespace OPENTREP {

}
