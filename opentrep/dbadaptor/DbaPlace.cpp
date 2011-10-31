// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// STL
#include <string>
#include <exception>
// OpenTrep
#include <opentrep/bom/Place.hpp>
#include <opentrep/dbadaptor/DbaPlace.hpp>
#include <opentrep/service/Logger.hpp>

namespace soci {

  // //////////////////////////////////////////////////////////////////////
  void type_conversion<OPENTREP::Place>::
  from_base (values const& iPlaceValues, indicator /* ind */,
             OPENTREP::Place& ioPlace) {
    /*
      code, city_code, xapian_docid, is_airport, is_city,
      is_main, is_commercial, state_code, country_code, region_code,
      continent_code, time_zone_grp, longitude, latitude, language_code,
      classical_name, extended_name, alternate_name1, alternate_name2,
      alternate_name3, alternate_name4, alternate_name5, alternate_name6,
      alternate_name7, alternate_name8, alternate_name9, alternate_name10      
    */
    ioPlace.setPlaceCode (iPlaceValues.get<std::string> ("code"));
    // The city code will be set to the default value (empty string)
    // when the column is null
    ioPlace.setCityCode (iPlaceValues.get<std::string> ("city_code", ""));
    // The state code will be set to the default value (empty string)
    // when the column is null
    ioPlace.setStateCode (iPlaceValues.get<std::string> ("state_code", ""));
    ioPlace.setCountryCode (iPlaceValues.get<std::string> ("country_code"));
    ioPlace.setRegionCode (iPlaceValues.get<std::string> ("region_code"));
    ioPlace.setContinentCode (iPlaceValues.get<std::string> ("continent_code"));
    ioPlace.setTimeZoneGroup (iPlaceValues.get<std::string> ("time_zone_grp"));
    ioPlace.setLongitude (iPlaceValues.get<double> ("longitude"));
    ioPlace.setLatitude (iPlaceValues.get<double> ("latitude"));
    ioPlace.setDocID (iPlaceValues.get<int> ("xapian_docid"));
    
    // Names
    const std::string& lLanguageString =
      iPlaceValues.get<std::string> ("language_code", "");
    if (lLanguageString.empty() == true) {
      throw new OPENTREP::LanguageCodeNotDefinedInNameTableException();
    }

    const OPENTREP::Language::EN_Language lLanguageCode =
      OPENTREP::Language::getCode (lLanguageString);

    const std::string& lClassicalName =
      iPlaceValues.get<std::string> ("classical_name", "");
    ioPlace.addName (lLanguageCode, lClassicalName);

    const std::string& lExtendedName =
      iPlaceValues.get<std::string> ("extended_name", "");
    ioPlace.addName (lLanguageCode, lExtendedName);

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
  }

  // //////////////////////////////////////////////////////////////////////
  void type_conversion<OPENTREP::Place>::
  to_base (const OPENTREP::Place& iPlace, values& ioPlaceValues,
           indicator& ioIndicator) {
    const indicator lCityCodeIndicator =
      iPlace.getCityCode().empty() ? i_null : i_ok;
    const indicator lStateCodeIndicator =
      iPlace.getStateCode().empty() ? i_null : i_ok;
    ioPlaceValues.set ("code", iPlace.getPlaceCode());
    ioPlaceValues.set ("city_code", iPlace.getCityCode(), lCityCodeIndicator);
    ioPlaceValues.set ("state_code", iPlace.getStateCode(),
                       lStateCodeIndicator);
    ioPlaceValues.set ("country_code", iPlace.getCountryCode());
    ioPlaceValues.set ("region_code", iPlace.getRegionCode());
    ioPlaceValues.set ("continent_code", iPlace.getContinentCode());
    ioPlaceValues.set ("time_zone_grp", iPlace.getTimeZoneGroup());
    ioPlaceValues.set ("longitude", iPlace.getLongitude());
    ioPlaceValues.set ("latitude", iPlace.getLatitude());
    ioPlaceValues.set ("xapian_docid", iPlace.getDocID());
    ioIndicator = i_ok;
  }

}

namespace OPENTREP {

}
