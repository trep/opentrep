// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// STL
#include <cassert>
#include <sstream>
#include <string>
// OpenTrep Protobuf
#include <opentrep/Travel.pb.h>
// OpenTrep
#include <opentrep/Location.hpp>
#include <opentrep/CityDetails.hpp>
#include <opentrep/bom/LocationExchange.hpp>

namespace OPENTREP {
  
  // //////////////////////////////////////////////////////////////////////
  std::string LocationExchange::
  exportLocationList (const LocationList_T& iLocationList,
                      const WordList_T& iNonMatchedWordList) {
    std::string oStr ("");
    
    // Protobuf structure
    treppb::QueryAnswer lQueryAnswer;
    
    // //// 1. Status ////
    const bool kOKStatus = true;
    lQueryAnswer.set_ok_status (kOKStatus);

    // //// 2. Error message ////
    /** Uncomment in order to set an error message
    const std::string kEmptyMessage ("");
    treppb::ErrorMessage* lErrorMessagePtr = lQueryAnswer.mutable_error_msg();
    assert (lErrorMessagePtr != NULL);
    lErrorMessagePtr->set_msg (kEmptyMessage);
    */
    
    // //// 3. List of places ////
    treppb::PlaceList* lPlaceListPtr = lQueryAnswer.mutable_place_list();
    assert (lPlaceListPtr != NULL);

    // Browse the list of Location structures, and fill the Protobuf structure
    for (LocationList_T::const_iterator itLocation = iLocationList.begin();
         itLocation != iLocationList.end(); ++itLocation) {
      const Location& lLocation = *itLocation;

      // Create an instance of a Protobuf Place structure
      treppb::Place* lPlacePtr = lPlaceListPtr->add_place();
      assert (lPlacePtr != NULL);
      
      // Fill the Protobuf Place structure with the content of
      // the Location structure
      exportLocation (*lPlacePtr, lLocation);
    }

    // //// 4. List of un-matched keywords ////
    // Create an instance of a Protobuf UnknownKeywordList structure
    treppb::UnknownKeywordList* lUnknownKeywordListPtr =
      lQueryAnswer.mutable_unmatched_keyword_list();
      assert (lUnknownKeywordListPtr != NULL);

    // Browse the list of un-matched keywords, and fill the Protobuf structure
    for (WordList_T::const_iterator itWord = iNonMatchedWordList.begin();
         itWord != iNonMatchedWordList.end(); ++itWord) {
      const Word_T& lWord = *itWord;
      lUnknownKeywordListPtr->add_word (lWord);
    }

    // Serialize the Protobuf
    const bool pbSerialStatus = lQueryAnswer.SerializeToString (&oStr);
    if (pbSerialStatus == false) {
      std::ostringstream errStr;
      errStr << "Error - The OPTD Travel protocol buffer object cannot be "
             << "serialized into a C++ string";
      throw SerDeException (errStr.str());
    }
    
    return oStr;
  }

  // //////////////////////////////////////////////////////////////////////
  void LocationExchange::exportLocation (treppb::Place& ioPlace,
                                         const Location& iLocation) {
    // Retrieve the primary key
    const LocationKey& lLocationKey = iLocation.getKey();

    // Retrieve and set the travel-related IATA code (part of the primary key)
    const IATACode_T& lIataCode = lLocationKey.getIataCode();
    treppb::IATACode* lIataAirportPtr = ioPlace.mutable_tvl_code();
    assert (lIataAirportPtr != NULL);
    lIataAirportPtr->set_code (lIataCode);

    // Retrieve and set the location type
    const IATAType& lLocationType = lLocationKey.getIataType();
    const treppb::PlaceType& lPlaceType = lLocationType.getTypeAsPB();
    const treppb::PlaceType_LocationType& lPlaceTypeEnum = lPlaceType.type();
    treppb::PlaceType* lPlaceTypePtr = ioPlace.mutable_loc_type();
    assert (lPlaceTypePtr != NULL);
    lPlaceTypePtr->set_type (lPlaceTypeEnum);

    // Retrieve and set the Geonames ID
    const GeonamesID_T& lGeonamesID = lLocationKey.getGeonamesID();
    treppb::GeonamesID* lGeonamesIDPtr = ioPlace.mutable_geonames_id();
    assert (lGeonamesIDPtr != NULL);
    lGeonamesIDPtr->set_id (lGeonamesID);

    // Retrieve and set the ICAO code
    const ICAOCode_T& lIcaoCode = iLocation.getIcaoCode();
    treppb::ICAOCode* lIcaoCodePtr = ioPlace.mutable_icao_code();
    assert (lIcaoCodePtr != NULL);
    lIcaoCodePtr->set_code (lIcaoCode);

    // Retrieve and set the FAA code
    const FAACode_T& lFaaCode = iLocation.getFaaCode();
    treppb::FAACode* lFaaCodePtr = ioPlace.mutable_faa_code();
    assert (lFaaCodePtr != NULL);
    lFaaCodePtr->set_code (lFaaCode);

    // Retrieve and set the UN/LOCODE code list
    const UNLOCodeList_T& lUNLOCodeList = iLocation.getUNLOCodeList();
    treppb::UNLOCodeList* lUNLOCodeListPtr = ioPlace.mutable_unlocode_list();
    assert (lUNLOCodeListPtr != NULL);
    //
    for (UNLOCodeList_T::const_iterator itUNLOCode = lUNLOCodeList.begin();
         itUNLOCode != lUNLOCodeList.end(); ++itUNLOCode) {
      const UNLOCode_T& lUNLOCode = *itUNLOCode;
      treppb::UNLOCode* lUNLOCodePtr = lUNLOCodeListPtr->add_unlocode();
      assert (lUNLOCodePtr != NULL);
      lUNLOCodePtr->set_code (lUNLOCode);
    }    
    
    // Retrieve and set the names
    const CommonName_T& lUtfName = iLocation.getCommonName();
    ioPlace.set_name_utf (lUtfName);
    const ASCIIName_T& lAsciiName = iLocation.getAsciiName();
    ioPlace.set_name_ascii (lAsciiName);

    // Retrieve and set the feature class and code
    const FeatureClass_T& lFeatClass = iLocation.getFeatureClass();
    const FeatureCode_T& lFeatCode = iLocation.getFeatureCode();
    treppb::FeatureType* lFeatTypePtr = ioPlace.mutable_feat_type();
    assert (lFeatTypePtr != NULL);
    treppb::FeatureClass* lFeatClassPtr = lFeatTypePtr->mutable_fclass();
    assert (lFeatClassPtr != NULL);
    treppb::FeatureCode* lFeatCodePtr = lFeatTypePtr->mutable_fcode();
    assert (lFeatCodePtr != NULL);
    lFeatClassPtr->set_code (lFeatClass);
    lFeatCodePtr->set_code (lFeatCode);

    // Retrieve and set the geographical coordinates
    const Latitude_T& lLatitude = iLocation.getLatitude();
    const Longitude_T& lLongitude = iLocation.getLongitude();
    treppb::GeoPoint* lPointPtr = ioPlace.mutable_coord();
    assert (lPointPtr != NULL);
    lPointPtr->set_latitude (lLatitude);
    lPointPtr->set_longitude (lLongitude);

    // Retrieve and set the list of served city details
    const CityDetailsList_T& lCityList = iLocation.getCityList();
    treppb::CityList* lCityListPtr = ioPlace.mutable_city_list();
    assert (lCityListPtr != NULL);
    //
    for (CityDetailsList_T::const_iterator itCity = lCityList.begin();
         itCity != lCityList.end(); ++itCity) {
      const CityDetails& lCity = *itCity;
      treppb::City* lCityPtr = lCityListPtr->add_city();
      assert (lCityPtr != NULL);

      // IATA code of the served city
      const IATACode_T& lIataCode = lCity.getIataCode();
      treppb::IATACode* lIataCodePtr = lCityPtr->mutable_code();
      assert (lIataCodePtr != NULL);
      lIataCodePtr->set_code (lIataCode);

      // Geonames ID of the served city
      const GeonamesID_T& lGeonamesID = lCity.getGeonamesID();
      treppb::GeonamesID* lGeonamesIDPtr = lCityPtr->mutable_geonames_id();
      assert (lGeonamesIDPtr != NULL);
      lGeonamesIDPtr->set_id (lGeonamesID);

      // City UTF8 name
      const CityUTFName_T& lCityUtfName = lCity.getUtfName();
      lCityPtr->set_name_utf (lCityUtfName);

      // City ASCII name
      const CityASCIIName_T& lCityAsciiName = lCity.getAsciiName();
      lCityPtr->set_name_ascii (lCityAsciiName);
    }

    // Retrieve and set the state code
    const StateCode_T& lStateCode = iLocation.getStateCode();
    treppb::StateCode* lStateCodePtr = ioPlace.mutable_state_code();
    assert (lStateCodePtr != NULL);
    lStateCodePtr->set_code (lStateCode);

    // Retrieve and set the country code
    const CountryCode_T& lCountryCode = iLocation.getCountryCode();
    treppb::CountryCode* lCountryCodePtr = ioPlace.mutable_country_code();
    assert (lCountryCodePtr != NULL);
    lCountryCodePtr->set_code (lCountryCode);

    // Retrieve and set the alternative country code
    const AltCountryCode_T& lAltCountryCode = iLocation.getAltCountryCode();
    treppb::AltCountryCode* lAltCountryCodePtr =
      ioPlace.mutable_alt_country_code();
    assert (lAltCountryCodePtr != NULL);
    lAltCountryCodePtr->set_code (lAltCountryCode);

    // Retrieve and set the country name
    const CountryName_T& lCountryName = iLocation.getCountryName();
    ioPlace.set_country_name (lCountryName);

    // Retrieve and set the US DOT World Area Code (WAC)
    const WAC_T& lWAC = iLocation.getWAC();
    treppb::WorldAreaCode* lWorldAreaCodePtr = ioPlace.mutable_wac_code();
    assert (lWorldAreaCodePtr != NULL);
    lWorldAreaCodePtr->set_code (lWAC);

    // Retrieve and set the US DOT World Area Code (WAC) name
    const WACName_T& lWACName = iLocation.getWACName();
    ioPlace.set_wac_name (lWACName);

    // Retrieve and set the currency code
    const CurrencyCode_T& lCurrencyCode = iLocation.getCurrencyCode();
    treppb::CurrencyCode* lCurrencyCodePtr = ioPlace.mutable_currency_code();
    assert (lCurrencyCodePtr != NULL);
    lCurrencyCodePtr->set_code (lCurrencyCode);

    // Retrieve and set the continent code
    const ContinentCode_T& lContinentCode = iLocation.getContinentCode();
    treppb::ContinentCode* lContinentCodePtr = ioPlace.mutable_continent_code();
    assert (lContinentCodePtr != NULL);
    lContinentCodePtr->set_code (lContinentCode);

    // Retrieve and set the continent name
    const ContinentName_T& lContinentName = iLocation.getContinentName();
    ioPlace.set_continent_name (lContinentName);

    // Retrieve and set the admin level 1 code
    const Admin1Code_T& lAdm1Code = iLocation.getAdmin1Code();
    treppb::Admin1Code* lAdm1CodePtr = ioPlace.mutable_adm1_code();
    assert (lAdm1CodePtr != NULL);
    lAdm1CodePtr->set_code (lAdm1Code);

    // Retrieve and set the admin level 1 names
    const Admin1UTFName_T& lAdm1UtfName = iLocation.getAdmin1UtfName();
    ioPlace.set_adm1_name_utf (lAdm1UtfName);
    const Admin1ASCIIName_T& lAdm1AsciiName = iLocation.getAdmin1AsciiName();
    ioPlace.set_adm1_name_ascii (lAdm1AsciiName);

    // Retrieve and set the admin level 2 code
    const Admin2Code_T& lAdm2Code = iLocation.getAdmin2Code();
    treppb::Admin2Code* lAdm2CodePtr = ioPlace.mutable_adm2_code();
    assert (lAdm2CodePtr != NULL);
    lAdm2CodePtr->set_code (lAdm2Code);

    // Retrieve and set the admin level 2 names
    const Admin2UTFName_T& lAdm2UtfName = iLocation.getAdmin2UtfName();
    ioPlace.set_adm2_name_utf (lAdm2UtfName);
    const Admin2ASCIIName_T& lAdm2AsciiName = iLocation.getAdmin2AsciiName();
    ioPlace.set_adm2_name_ascii (lAdm2AsciiName);

    // Retrieve and set the admin level 3 code
    const Admin3Code_T& lAdm3Code = iLocation.getAdmin3Code();
    treppb::Admin3Code* lAdm3CodePtr = ioPlace.mutable_adm3_code();
    assert (lAdm3CodePtr != NULL);
    lAdm3CodePtr->set_code (lAdm3Code);

    // Retrieve and set the admin level 4 code
    const Admin4Code_T& lAdm4Code = iLocation.getAdmin4Code();
    treppb::Admin4Code* lAdm4CodePtr = ioPlace.mutable_adm4_code();
    assert (lAdm4CodePtr != NULL);
    lAdm4CodePtr->set_code (lAdm4Code);

    // Retrieve and set the population
    const Population_T& lPopulation = iLocation.getPopulation();
    treppb::Population* lPopulationPtr = ioPlace.mutable_population();
    assert (lPopulationPtr != NULL);
    lPopulationPtr->set_value (lPopulation);

    // Retrieve and set the elevation
    const Elevation_T& lElevation = iLocation.getElevation();
    treppb::Elevation* lElevationPtr = ioPlace.mutable_elevation();
    assert (lElevationPtr != NULL);
    lElevationPtr->set_value (lElevation);

    // Retrieve and set the geo topology 30
    const GTopo30_T& lGTopo30 = iLocation.getGTopo30();
    treppb::GTopo30* lGTopo30Ptr = ioPlace.mutable_gtopo30();
    assert (lGTopo30Ptr != NULL);
    lGTopo30Ptr->set_value (lGTopo30);

    // Retrieve and set the PageRank value
    const PageRank_T& lPageRank = iLocation.getPageRank();
    treppb::PageRank* lPageRankPtr = ioPlace.mutable_page_rank();
    assert (lPageRankPtr != NULL);
    lPageRankPtr->set_rank (lPageRank);

    // Retrieve and set the time-zone
    const TimeZone_T& lTimeZone = iLocation.getTimeZone();
    treppb::TimeZone* lTimeZonePtr = ioPlace.mutable_tz();
    assert (lTimeZonePtr != NULL);
    lTimeZonePtr->set_tz (lTimeZone);

    // Retrieve and set the GMT offset
    const GMTOffset_T& lGMTOffset = iLocation.getGMTOffset();
    treppb::TZOffSet* lGMTOffsetPtr = ioPlace.mutable_gmt_offset();
    assert (lGMTOffsetPtr != NULL);
    lGMTOffsetPtr->set_offset (lGMTOffset);

    // Retrieve and set the DST offset
    const DSTOffset_T& lDSTOffset = iLocation.getDSTOffset();
    treppb::TZOffSet* lDSTOffsetPtr = ioPlace.mutable_dst_offset();
    assert (lDSTOffsetPtr != NULL);
    lDSTOffsetPtr->set_offset (lDSTOffset);

    // Retrieve and set the RAW offset
    const RawOffset_T& lRAWOffset = iLocation.getRawOffset();
    treppb::TZOffSet* lRAWOffsetPtr = ioPlace.mutable_raw_offset();
    assert (lRAWOffsetPtr != NULL);
    lRAWOffsetPtr->set_offset (lRAWOffset);

    // Retrieve and set the modification date (within Geonames)
    const Date_T& lGeonameModDate = iLocation.getModificationDate();
    treppb::Date* lGeonameModDatePtr = ioPlace.mutable_mod_date();
    assert (lGeonameModDatePtr != NULL);
    lGeonameModDatePtr->set_date (boost::gregorian::to_iso_extended_string(lGeonameModDate));

    // Retrieve and set the list of the travel-related POR IATA codes
    const TvlPORListString_T& lTvlPORList = iLocation.getTvlPORListString();
    treppb::TravelRelatedList* lTvlPORListPtr = ioPlace.mutable_tvl_por_list();
    assert (lTvlPORListPtr != NULL);
    lTvlPORListPtr->add_tvl_code (lTvlPORList);

    // Retrieve and set the list of the Wikipedia links (URLs)
    const WikiLink_T& lWikiLink = iLocation.getWikiLink();
    treppb::WikiLinkList* lWikiLinkListPtr = ioPlace.mutable_link_list();
    assert (lWikiLinkListPtr != NULL);
    treppb::WikiLink* lWikiLinkPtr = lWikiLinkListPtr->add_link();
    assert (lWikiLinkPtr != NULL);
    treppb::LanguageCode* lLangCodePtr = lWikiLinkPtr->mutable_lang();
    assert (lLangCodePtr != NULL);
    lLangCodePtr->set_code ("en");
    lWikiLinkPtr->set_link (lWikiLink);

    // Retrieve and set the beginning date of the validity period
    const Date_T& lDateFrom = iLocation.getDateFrom();
    treppb::Date* lDateFromPtr = ioPlace.mutable_date_from();
    assert (lDateFromPtr != NULL);
    lDateFromPtr->set_date (boost::gregorian::to_iso_extended_string(lDateFrom));

    // Retrieve and set the end date of the validity period
    const Date_T& lDateEnd = iLocation.getDateEnd();
    treppb::Date* lDateEndPtr = ioPlace.mutable_date_end();
    assert (lDateEndPtr != NULL);
    lDateEndPtr->set_date (boost::gregorian::to_iso_extended_string(lDateEnd));

    // Retrieve and set the commentaries
    const Comment_T& lComment = iLocation.getComment();
    treppb::Comment* lCommentPtr = ioPlace.mutable_comment();
    assert (lCommentPtr != NULL);
    lCommentPtr->set_text (lComment);

    // Retrieve and set the list of alternate names
    const NameMatrix& lNameMatrixRef = iLocation.getNameMatrix();
    treppb::AltNameList* lAltNameListPtr = ioPlace.mutable_alt_name_list();
    assert (lAltNameListPtr != NULL);
    //
    const NameMatrix_T lNameMatrix = lNameMatrixRef.getNameMatrix();
    for (NameMatrix_T::const_iterator itNameList = lNameMatrix.begin();
         itNameList != lNameMatrix.end(); ++itNameList) {
      const Names& lNameListRef = itNameList->second;
      const LanguageCode_T& lLangCode = lNameListRef.getLanguageCode();
      const NameList_T& lNameList = lNameListRef.getNameList();
      for (NameList_T::const_iterator itName = lNameList.begin();
           itName != lNameList.end(); ++itName) {
        const std::string& lName = *itName;
        //
        treppb::AltName* lAltNamePtr = lAltNameListPtr->add_name();
        assert (lAltNamePtr != NULL);
        //
        treppb::LanguageCode* lLangCodePtr = lAltNamePtr->mutable_lang();
        assert (lLangCodePtr != NULL);
        lLangCodePtr->set_code (lLangCode);
        lAltNamePtr->set_name (lName);
      }
    }

    // Retrieve and set the matching percentage value
    const MatchingPercentage_T& lPercentage = iLocation.getPercentage();
    treppb::MatchingPercentage* lPercentagePtr =
      ioPlace.mutable_matching_percentage();
    assert (lPercentagePtr != NULL);
    lPercentagePtr->set_percentage (lPercentage);

    // Retrieve and set the list of the original keywords
    /**
     * \TODO: split the keyword list and create single keywords within
     *        the Protobuf list.
     */
    const std::string& lOriginalKeywords = iLocation.getOriginalKeywords();
    treppb::KeywordList* lOriginalKeywordListPtr =
      ioPlace.mutable_original_keyword_list();
    assert (lOriginalKeywordListPtr != NULL);
    lOriginalKeywordListPtr->add_word (lOriginalKeywords);

    // Retrieve and set the list of the corrected keywords
    /**
     * \TODO: split the keyword list and create single keywords within
     *        the Protobuf list.
     */
    const std::string& lCorrectedKeywords = iLocation.getCorrectedKeywords();
    treppb::KeywordList* lCorrectedKeywordListPtr =
      ioPlace.mutable_corrected_keyword_list();
    assert (lCorrectedKeywordListPtr != NULL);
    lCorrectedKeywordListPtr->add_word (lCorrectedKeywords);

    // Retrieve and set the actual edit distance
    const NbOfErrors_T& lEditDistanceActual = iLocation.getEditDistance();
    treppb::EditDistance* lEditDistanceActualPtr =
      ioPlace.mutable_edit_distance_actual();
    assert (lEditDistanceActualPtr != NULL);
    lEditDistanceActualPtr->set_dist (lEditDistanceActual);

    // Retrieve and set the allowable edit distance
    const NbOfErrors_T& lEditDistanceAllowable =
      iLocation.getAllowableEditDistance();
    treppb::EditDistance* lEditDistanceAllowablePtr =
      ioPlace.mutable_edit_distance_actual();
    assert (lEditDistanceAllowablePtr != NULL);
    lEditDistanceAllowablePtr->set_dist (lEditDistanceAllowable);

    // Iterate on the extra list of locations
    const LocationList_T& lExtraLocationList = iLocation.getExtraLocationList();
    treppb::PlaceList* lExtraPlaceListPtr = ioPlace.mutable_extra_place_list();
    assert (lExtraPlaceListPtr != NULL);
    for (LocationList_T::const_iterator itLoc = lExtraLocationList.begin();
         itLoc != lExtraLocationList.end(); ++itLoc) {
      const Location& lExtraLocation = *itLoc;
      //
      treppb::Place* lPlacePtr = lExtraPlaceListPtr->add_place();
      assert (lPlacePtr != NULL);
      //
      exportLocation (*lPlacePtr, lExtraLocation);
    }

    // Iterate on the alternative list of locations
    const LocationList_T& lAltLocationList= iLocation.getAlternateLocationList();
    treppb::PlaceList* lAltPlaceListPtr = ioPlace.mutable_alt_place_list();
    assert (lAltPlaceListPtr != NULL);
    for (LocationList_T::const_iterator itLoc = lAltLocationList.begin();
         itLoc != lAltLocationList.end(); ++itLoc) {
      const Location& lAlternateLocation = *itLoc;
      //
      treppb::Place* lPlacePtr = lAltPlaceListPtr->add_place();
      assert (lPlacePtr != NULL);
      //
      exportLocation (*lPlacePtr, lAlternateLocation);
    }
  }

}

