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
      code, xapian_docid, icao_code, is_geonames, geonameid, 
      latitude, longitude, fclass, fcode, country_code, cc2,
      admin1, admin2, admin3, admin4, population, elevation, gtopo30, 
      timezone, gmt_offset, dst_offset, raw_offset, moddate, 
      is_airport, is_commercial,
      city_code, state_code, region_code, location_type, 
      language_code, ascii_name, utf_name, 
      alternate_name1, alternate_name2, alternate_name3,
      alternate_name4, alternate_name5, alternate_name6,
      alternate_name7, alternate_name8, alternate_name9,
      alternate_name10,
      page_rank, wiki_link
    */

    /*
     * Retrieve the place key (IATA code, ICAO code, Geonames ID).
     * If ioPlace has already that key, then only the alternate names need to be added.
     * Otherwise, all the fields of the Place need to be set.
     */
    // First, retrieve the key from the database fields
    const std::string lIataCode = iPlaceValues.get<std::string> ("iata_code", "");
    const std::string lIcaoCode = iPlaceValues.get<std::string> ("icao_code", "");
    const OPENTREP::GeonamesID_T lGeonameID = iPlaceValues.get<int> ("geonameid");
    //
    const OPENTREP::PlaceKey lNewPlaceKey (lIataCode, lIcaoCode, lGeonameID);

    // Then, retrieve the key from the previous Place object
    const OPENTREP::PlaceKey lPlaceKey = ioPlace.getKey();

    // Compare the keys
    if (lNewPlaceKey != lPlaceKey) {
      // The place keys are different: the current place retrieved from the database
      // is not the same as the one retrieved previously

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
      ioPlace.setIcaoCode (lIcaoCode);
      // Geonames ID
      ioPlace.setGeonamesID (lGeonameID);

      // The FAA code will be set to the default value (empty string)
      // when the column is null
      // ioPlace.setFaaCode (iPlaceValues.get<std::string> ("faa_code", ""));

      // The city code will be set to the default value (empty string)
      // when the column is null
      ioPlace.setCityCode (iPlaceValues.get<std::string> ("city_code", ""));

      // The state code will be set to the default value (empty string)
      // when the column is null
      ioPlace.setStateCode (iPlaceValues.get<std::string> ("state_code", ""));
      ioPlace.setCountryCode (iPlaceValues.get<std::string> ("country_code"));
      ioPlace.setRegionCode (iPlaceValues.get<std::string> ("region_code"));
      ioPlace.setTimeZoneGroup (iPlaceValues.get<std::string> ("timezone"));
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
      errorStr << "The language field is empty for the place " << ioPlace.getIataCode();
      OPENTREP_LOG_ERROR (errorStr.str());
      throw OPENTREP::LanguageCodeNotDefinedInNameTableException (errorStr.str());
    }

    // Convert the language code string (e.g., "en") into the corresponding enumeration
    const OPENTREP::Language::EN_Language lLanguageCode =
      OPENTREP::Language::getCode (lLanguageString);

    // Add the ASCII and UTF8 names only when the language code is standard English
    if (lLanguageCode == OPENTREP::Language::en) {
      // ASCII name (which may be null/empty)
      const std::string& lAsciiName = iPlaceValues.get<std::string> ("ascii_name", "");
      ioPlace.addName (lLanguageCode, lAsciiName);

      // UTF-8 name (which may be null/empty)
      const std::string& lExtendedName = iPlaceValues.get<std::string> ("utf_name", "");
      ioPlace.addName (lLanguageCode, lExtendedName);
    }

    // Alternate names
    const std::string& lAlternateName1 =
      iPlaceValues.get<std::string> ("alternate_name1", "");
    ioPlace.addName (lLanguageCode, lAlternateName1);

    const std::string& lAlternateName2 =
      iPlaceValues.get<std::string> ("alternate_name2", "");
    ioPlace.addName (lLanguageCode, lAlternateName2);

    const std::string& lAlternateName3 =
      iPlaceValues.get<std::string> ("alternate_name3", "");
    ioPlace.addName (lLanguageCode, lAlternateName3);

    const std::string& lAlternateName4 =
      iPlaceValues.get<std::string> ("alternate_name4", "");
    ioPlace.addName (lLanguageCode, lAlternateName4);

    const std::string& lAlternateName5 =
      iPlaceValues.get<std::string> ("alternate_name5", "");
    ioPlace.addName (lLanguageCode, lAlternateName5);

    const std::string& lAlternateName6 =
      iPlaceValues.get<std::string> ("alternate_name6", "");
    ioPlace.addName (lLanguageCode, lAlternateName6);

    const std::string& lAlternateName7 =
      iPlaceValues.get<std::string> ("alternate_name7", "");
    ioPlace.addName (lLanguageCode, lAlternateName7);

    const std::string& lAlternateName8 =
      iPlaceValues.get<std::string> ("alternate_name8", "");
    ioPlace.addName (lLanguageCode, lAlternateName8);

    const std::string& lAlternateName9 =
      iPlaceValues.get<std::string> ("alternate_name9", "");
    ioPlace.addName (lLanguageCode, lAlternateName9);

    const std::string& lAlternateName10 =
      iPlaceValues.get<std::string> ("alternate_name10", "");
    ioPlace.addName (lLanguageCode, lAlternateName10);

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
    const indicator lIataCodeIndicator =
      iPlace.getIataCode().empty() ? i_null : i_ok;
    const indicator lIcaoCodeIndicator =
      iPlace.getIcaoCode().empty() ? i_null : i_ok;
    const indicator lCityCodeIndicator =
      iPlace.getCityCode().empty() ? i_null : i_ok;
    const indicator lStateCodeIndicator =
      iPlace.getStateCode().empty() ? i_null : i_ok;
    ioPlaceValues.set ("iata_code", iPlace.getIataCode(), lIataCodeIndicator);
    ioPlaceValues.set ("icao_code", iPlace.getIcaoCode(), lIcaoCodeIndicator);
    ioPlaceValues.set ("geonameid", iPlace.getGeonamesID());
    //ioPlaceValues.set ("faa_code", iPlace.getFaaCode());
    ioPlaceValues.set ("city_code", iPlace.getCityCode(), lCityCodeIndicator);
    ioPlaceValues.set ("state_code", iPlace.getStateCode(),
                       lStateCodeIndicator);
    ioPlaceValues.set ("country_code", iPlace.getCountryCode());
    ioPlaceValues.set ("region_code", iPlace.getRegionCode());
    ioPlaceValues.set ("timezone", iPlace.getTimeZoneGroup());
    ioPlaceValues.set ("latitude", iPlace.getLatitude());
    ioPlaceValues.set ("longitude", iPlace.getLongitude());
    ioPlaceValues.set ("page_rank", iPlace.getPageRank());
    ioPlaceValues.set ("wiki_link", iPlace.getWikiLink());
    ioPlaceValues.set ("xapian_docid", iPlace.getDocID());
    ioIndicator = i_ok;
  }

}

namespace OPENTREP {

}
