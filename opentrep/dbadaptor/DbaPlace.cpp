// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// STL
#include <exception>
#include <string>
#include <sstream>
// OpenTrep
#include <opentrep/basic/BasConst_General.hpp>
#include <opentrep/bom/Place.hpp>
#include <opentrep/dbadaptor/DbaPlace.hpp>
#include <opentrep/service/Logger.hpp>

namespace soci {

  // //////////////////////////////////////////////////////////////////////
  void type_conversion<OPENTREP::Place>::
  from_base (values const& iPlaceValues, indicator /* ind */,
             OPENTREP::Place& ioPlace) {
    /*
      rpd.iata_code, xapian_docid, icao_code, is_geonames, geonameid, 
      latitude, longitude, fclass, fcode, country_code, cc2,
      admin1, admin2, admin3, admin4, population, elevation, gtopo30, 
      timezone, gmt_offset, dst_offset, raw_offset, moddate, 
      is_airport, is_commercial, 
      city_code, state_code, region_code, location_type, wiki_link,
      language_code, ascii_name, utf_name, 
      alternate_name1, alternate_name2, alternate_name3, 
      alternate_name4, alternate_name5, alternate_name6, 
      alternate_name7, alternate_name8, alternate_name9, 
      alternate_name10, 
      page_rank, por_type 
      from place_names as pn, place_details as rpd 
      left join airport_pageranked pr on pr.iata_code = rpd.iata_code
    */

    /*
     * Retrieve the place key (IATA code, ICAO code, Geonames ID).
     * If ioPlace has already that key, then only the alternate names need
     * to be added.
     * Otherwise, all the fields of the Place need to be set.
     */
    // First, retrieve the key from the database fields
    const std::string lIataCodeStr =
      iPlaceValues.get<std::string> ("iata_code", "");
    const OPENTREP::IATACode_T lIataCode (lIataCodeStr);
    const std::string lIataTypeStr =
      iPlaceValues.get<std::string> ("location_type", "");
    const OPENTREP::IATAType lIataType (lIataTypeStr);
    const OPENTREP::GeonamesID_T lGeonameID =
      iPlaceValues.get<int> ("geoname_id");
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

      // The FAA code will be set to the default value (empty string)
      // when the column is null
      // ioPlace.setFaaCode (iPlaceValues.get<std::string> ("faa_code", ""));

      // Location type ('C' for city only, 'A' for airport only,
      // 'CA' for airport/city)
      const std::string lLocationType =
        iPlaceValues.get<std::string> ("location_type","");
      const std::string lPorType =
        iPlaceValues.get<std::string> ("por_type", "");

      // The city code will be set to the default value (empty string)
      // when the column is null
      ioPlace.setCityCode (iPlaceValues.get<std::string> ("city_code", ""));

      if (lLocationType == "ca" || lPorType == "c") {
        ioPlace.setCityCode ("");
      }

      // The state code will be set to the default value (empty string)
      // when the column is null
      ioPlace.setStateCode (iPlaceValues.get<std::string> ("state_code", ""));
      ioPlace.setCountryCode (iPlaceValues.get<std::string> ("country_code"));
      ioPlace.setContinentCode (iPlaceValues.get<std::string>("continent_code"));
      ioPlace.setContinentName (iPlaceValues.get<std::string>("continent_name"));
      ioPlace.setTimeZone (iPlaceValues.get<std::string> ("time_zone"));
      ioPlace.setLatitude (iPlaceValues.get<double> ("latitude"));
      ioPlace.setLongitude (iPlaceValues.get<double> ("longitude"));
      ioPlace.setPageRank (iPlaceValues.get<double> ("page_rank",
                                                     OPENTREP::K_DEFAULT_PAGE_RANK));
      // The Wikipedia link will be set to the default value (empty string)
      // when the column is null
      ioPlace.setWikiLink (iPlaceValues.get<std::string> ("wiki_link", ""));

      // Xapian document ID
      ioPlace.setDocID (iPlaceValues.get<int> ("xapian_docid"));
    }

    // List of alternate names
    const std::string& lLanguageString =
      iPlaceValues.get<std::string> ("language_code", "");
    if (lLanguageString.empty() == true) {
      std::ostringstream errorStr;
      errorStr << "The language field is empty for the place "
               << ioPlace.getIataCode();
      OPENTREP_LOG_ERROR (errorStr.str());
      throw OPENTREP::LanguageCodeNotDefinedInNameTableException (errorStr.str());
    }

    // Convert the language code string (e.g., "en") into
    // the corresponding enumeration
    const OPENTREP::Language::EN_Language lLanguageCode =
      OPENTREP::Language::getCode (lLanguageString);
    const OPENTREP::LanguageCode_T lLanguageStr (lLanguageString);
    
    // Add the ASCII and UTF8 names only when the language code is
    // standard English
    if (lLanguageCode == OPENTREP::Language::en) {
      // ASCII name (which may be null/empty)
      const std::string& lAsciiName =
        iPlaceValues.get<std::string> ("ascii_name", "");
      ioPlace.addName (lLanguageStr, lAsciiName);

      // UTF-8 name (which may be null/empty)
      const std::string& lExtendedName =
        iPlaceValues.get<std::string> ("utf_name", "");
      ioPlace.addName (lLanguageStr, lExtendedName);
    }

    // Alternate names
    const std::string& lAlternateName1 =
      iPlaceValues.get<std::string> ("alternate_name1", "");
    ioPlace.addName (lLanguageStr, lAlternateName1);

    const std::string& lAlternateName2 =
      iPlaceValues.get<std::string> ("alternate_name2", "");
    ioPlace.addName (lLanguageStr, lAlternateName2);

    const std::string& lAlternateName3 =
      iPlaceValues.get<std::string> ("alternate_name3", "");
    ioPlace.addName (lLanguageStr, lAlternateName3);

    const std::string& lAlternateName4 =
      iPlaceValues.get<std::string> ("alternate_name4", "");
    ioPlace.addName (lLanguageStr, lAlternateName4);

    const std::string& lAlternateName5 =
      iPlaceValues.get<std::string> ("alternate_name5", "");
    ioPlace.addName (lLanguageStr, lAlternateName5);

    const std::string& lAlternateName6 =
      iPlaceValues.get<std::string> ("alternate_name6", "");
    ioPlace.addName (lLanguageStr, lAlternateName6);

    const std::string& lAlternateName7 =
      iPlaceValues.get<std::string> ("alternate_name7", "");
    ioPlace.addName (lLanguageStr, lAlternateName7);

    const std::string& lAlternateName8 =
      iPlaceValues.get<std::string> ("alternate_name8", "");
    ioPlace.addName (lLanguageStr, lAlternateName8);

    const std::string& lAlternateName9 =
      iPlaceValues.get<std::string> ("alternate_name9", "");
    ioPlace.addName (lLanguageStr, lAlternateName9);

    const std::string& lAlternateName10 =
      iPlaceValues.get<std::string> ("alternate_name10", "");
    ioPlace.addName (lLanguageStr, lAlternateName10);

    // DEBUG
    /*
    OPENTREP_LOG_DEBUG ("[Old:" << lPlaceKey << "][New:" << lNewPlaceKey << "] Lang: "
                        << lLanguageString << " => "
                        << OPENTREP::Language::getLongLabel (lLanguageCode)
                        << " -- Details: " << ioPlace);
    */
  }

  // //////////////////////////////////////////////////////////////////////
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
    const OPENTREP::IATACode_T& lCityCode = iPlace.getCityCode();
    const std::string lCityCodeStr (lCityCode);
    const indicator lCityCodeIndicator = lCityCodeStr.empty() ? i_null : i_ok;

    // State code
    const OPENTREP::StateCode_T& lStateCode = iPlace.getStateCode();
    const std::string lStateCodeStr (lStateCode);
    const indicator lStateCodeIndicator = lStateCodeStr.empty() ? i_null : i_ok;

    // Country code
    const OPENTREP::CountryCode_T& lCountryCode = iPlace.getCountryCode();
    const std::string lCountryCodeStr (lCountryCode);

    // Continent code
    const OPENTREP::ContinentCode_T& lContinentCode = iPlace.getContinentCode();
    const std::string lContinentCodeStr (lContinentCode);

    // Continent name
    const OPENTREP::ContinentName_T& lContinentName = iPlace.getContinentName();
    const std::string lContinentNameStr (lContinentName);

    // Time-zone
    const OPENTREP::TimeZone_T& lTimeZone = iPlace.getTimeZone();
    const std::string lTimeZoneStr (lTimeZone);

    // Wikipedia link
    const OPENTREP::WikiLink_T& lWikiLink = iPlace.getWikiLink();
    const std::string lWikiLinkStr (lWikiLink);

    //
    ioPlaceValues.set ("iata_code", lIataCodeStr, lIataCodeIndicator);
    ioPlaceValues.set ("icao_code", lIcaoCodeStr, lIcaoCodeIndicator);
    ioPlaceValues.set ("geoname_id", iPlace.getGeonamesID());
    ioPlaceValues.set ("faa_code", lFaaCodeStr, lFaaCodeIndicator);
    ioPlaceValues.set ("city_code", lCityCodeStr, lCityCodeIndicator);
    ioPlaceValues.set ("state_code", lStateCodeStr, lStateCodeIndicator);
    ioPlaceValues.set ("country_code", lCountryCodeStr);
    ioPlaceValues.set ("continent_code", lContinentCodeStr);
    ioPlaceValues.set ("continent_name", lContinentNameStr);
    ioPlaceValues.set ("time_zone", lTimeZoneStr);
    ioPlaceValues.set ("latitude", iPlace.getLatitude());
    ioPlaceValues.set ("longitude", iPlace.getLongitude());
    ioPlaceValues.set ("page_rank", iPlace.getPageRank());
    ioPlaceValues.set ("wiki_link", lWikiLinkStr);
    ioPlaceValues.set ("xapian_docid", iPlace.getDocID());
    ioIndicator = i_ok;
  }

}

namespace OPENTREP {

}
