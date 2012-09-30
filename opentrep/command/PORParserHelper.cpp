// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// STL
#include <cassert>
#include <vector>
#include <fstream>
// Boost
#include <boost/lexical_cast.hpp>
//#define BOOST_SPIRIT_DEBUG
#define BOOST_SPIRIT_UNICODE
// OpenTREP
#include <opentrep/basic/BasParserTypes.hpp>
#include <opentrep/bom/Place.hpp>
#include <opentrep/factory/FacPlace.hpp>
#include <opentrep/service/Logger.hpp>
#include <opentrep/command/PORParserHelper.hpp>

namespace OPENTREP {

  /** Namespaces. */
  namespace bsq = boost::spirit::qi;
  //namespace bsa = boost::spirit::ascii;
  namespace bsu = boost::spirit::unicode;
  namespace bsc = boost::spirit::classic;

  namespace PorParserHelper {
      
    // //////////////////////////////////////////////////////////////////
    //  Semantic actions
    // //////////////////////////////////////////////////////////////////

    ParserSemanticAction::ParserSemanticAction (Location& ioLocation)
      : _location (ioLocation) {
    }      
   
    // //////////////////////////////////////////////////////////////////
    storeIataCode::storeIataCode (Location& ioLocation)
      : ParserSemanticAction (ioLocation) {
    }
    
    // //////////////////////////////////////////////////////////////////
    void storeIataCode::operator() (std::vector<uchar_t> iChar,
                                    bsq::unused_type, bsq::unused_type) const {

      const std::string lIataCodeStr (iChar.begin(), iChar.end());
      const OPENTREP::IATACode_T lIataCode (lIataCodeStr);
      _location.setIataCode (lIataCode);

      // Reset the values
      //_location.resetMatrix();
      //_location._itAltNameShortList.clear();

      // DEBUG
       //OPENTREP_LOG_DEBUG ( "IATA code: " << _location.getIataCode());
    }

    // //////////////////////////////////////////////////////////////////
    storeIcaoCode::storeIcaoCode (Location& ioLocation)
      : ParserSemanticAction (ioLocation) {
    }
    
    // //////////////////////////////////////////////////////////////////
    void storeIcaoCode::operator() (std::vector<uchar_t> iChar,
                                    bsq::unused_type, bsq::unused_type) const {

      const std::string lIcaoCodeStr (iChar.begin(), iChar.end());
      const OPENTREP::ICAOCode_T lIcaoCode (lIcaoCodeStr);
      _location.setIcaoCode (lIcaoCode);
       // DEBUG
       //OPENTREP_LOG_DEBUG ( "ICAO code: " << _location.getIcaoCode());
    }

    // //////////////////////////////////////////////////////////////////
    storeGeonamesID::storeGeonamesID (Location& ioLocation)
      : ParserSemanticAction (ioLocation) {
    }
    
    // //////////////////////////////////////////////////////////////////
    void storeGeonamesID::operator() (unsigned int iPorId,
                                      bsq::unused_type, bsq::unused_type) const {
      _location.setGeonamesID (iPorId);
      
      // DEBUG
      //OPENTREP_LOG_DEBUG ( "Por Id: " << _location.getPorID ());
    }
    
    // //////////////////////////////////////////////////////////////////
    storeCommonName::storeCommonName (Location& ioLocation)
      : ParserSemanticAction (ioLocation) {
    }
    
    // //////////////////////////////////////////////////////////////////
    void storeCommonName::operator() (std::vector<uchar_t> iChar,
                                      bsq::unused_type, bsq::unused_type) const {

      const std::string lCommonNameStr (iChar.begin(), iChar.end());
      const OPENTREP::CommonName_T lCommonName (lCommonNameStr);
      _location.setCommonName (lCommonName);
       // DEBUG
       //OPENTREP_LOG_DEBUG ("Common name: " << _location.getCommonName());
    }

    // //////////////////////////////////////////////////////////////////
    storeAsciiName::storeAsciiName (Location& ioLocation)
      : ParserSemanticAction (ioLocation) {
    }
    
    // //////////////////////////////////////////////////////////////////
    void storeAsciiName::operator() (std::vector<uchar_t> iChar,
                                     bsq::unused_type, bsq::unused_type) const {

      const std::string lAsciiNameStr (iChar.begin(), iChar.end());
      const OPENTREP::ASCIIName_T lAsciiName (lAsciiNameStr);
      _location.setAsciiName (lAsciiName);
       // DEBUG
       //OPENTREP_LOG_DEBUG ("ASCII name: " << _location.getAsciiName());
    }

    // //////////////////////////////////////////////////////////////////
    storeAltNameShort::storeAltNameShort (Location& ioLocation)
      : ParserSemanticAction (ioLocation) {
    }
    
    // //////////////////////////////////////////////////////////////////
    void storeAltNameShort::operator() (std::vector<uchar_t> iChar,
                                        bsq::unused_type,
                                        bsq::unused_type)const {

      const std::string lAltNameShortStr (iChar.begin(), iChar.end());
      const OPENTREP::AltNameShortListString_T lAltNameShort (lAltNameShortStr);
      _location._itAltNameShortList.push_back (lAltNameShort);
       // DEBUG
       //OPENTREP_LOG_DEBUG ("Alt name short: " << lAltNameShortStr);
    }

    // //////////////////////////////////////////////////////////////////
    storeAltNameShortListString::
    storeAltNameShortListString (Location& ioLocation)
      : ParserSemanticAction (ioLocation) {
    }
    
    // //////////////////////////////////////////////////////////////////
    void storeAltNameShortListString::
    operator() (bsq::unused_type, bsq::unused_type, bsq::unused_type)const {
      _location.consolidateAltNameShortListString();
       // DEBUG
       //OPENTREP_LOG_DEBUG ("Common name: " << _location.getAltNameShortListString());
    }

    // //////////////////////////////////////////////////////////////////
    storeLatitude::storeLatitude (Location& ioLocation)
      : ParserSemanticAction (ioLocation) {
    }
    
    // //////////////////////////////////////////////////////////////////
    void storeLatitude::operator() (double iLatitude,
                                    bsq::unused_type, bsq::unused_type) const {
      _location.setLatitude (iLatitude);
      // DEBUG
      //OPENTREP_LOG_DEBUG ("Latitude: " << _location.getLatitude());
    }

    // //////////////////////////////////////////////////////////////////
    storeLongitude::storeLongitude (Location& ioLocation)
      : ParserSemanticAction (ioLocation) {
    }
    
    // //////////////////////////////////////////////////////////////////
    void storeLongitude::operator() (double iLongitude,
                                     bsq::unused_type, bsq::unused_type) const {
      _location.setLongitude (iLongitude);
      // DEBUG
      //OPENTREP_LOG_DEBUG ("Longitude: " << _location.getLongitude());
    }

    // //////////////////////////////////////////////////////////////////
    storeFeatureClass::storeFeatureClass (Location& ioLocation)
      : ParserSemanticAction (ioLocation) {
    }

    // //////////////////////////////////////////////////////////////////
    void storeFeatureClass::operator() (std::vector<uchar_t> iChar,
                                        bsq::unused_type,
                                        bsq::unused_type) const {
      const std::string lFeatClassStr (iChar.begin(), iChar.end());
      const FeatureClass_T lFeatClass (lFeatClassStr);
      _location.setFeatureClass (lFeatClass);
      // DEBUG
      //OPENTREP_LOG_DEBUG ("Feature class: " << _location.getFeatureClass());
    }

    // //////////////////////////////////////////////////////////////////
    storeFeatureCode::storeFeatureCode (Location& ioLocation)
      : ParserSemanticAction (ioLocation) {
    }

    // //////////////////////////////////////////////////////////////////
    void storeFeatureCode::operator() (std::vector<uchar_t> iChar,
                                       bsq::unused_type,
                                       bsq::unused_type) const {
      const std::string lFeatCodeStr (iChar.begin(), iChar.end());
      const FeatureClass_T lFeatCode (lFeatCodeStr);
      _location.setFeatureCode (lFeatCode);
      // DEBUG
      //OPENTREP_LOG_DEBUG ("Feature code: " << _location.getFeatureCode());
    }

    // //////////////////////////////////////////////////////////////////
    storeCountryCode::storeCountryCode (Location& ioLocation)
      : ParserSemanticAction (ioLocation) {
    }

    // //////////////////////////////////////////////////////////////////
    void storeCountryCode::operator() (std::vector<uchar_t> iChar,
                                       bsq::unused_type,
                                       bsq::unused_type) const {
      const std::string lCountryCodeStr (iChar.begin(), iChar.end());
      const CountryCode_T lCountryCode (lCountryCodeStr);
      _location.setCountryCode (lCountryCode);
      // DEBUG
      //OPENTREP_LOG_DEBUG ("Country code: " << _location.getCountryCode());
    }

    // //////////////////////////////////////////////////////////////////
    storeAltCountryCode::storeAltCountryCode (Location& ioLocation)
      : ParserSemanticAction (ioLocation) {
    }

    // //////////////////////////////////////////////////////////////////
    void storeAltCountryCode::operator() (std::vector<uchar_t> iChar,
                                          bsq::unused_type,
                                          bsq::unused_type) const {
      const std::string lCountryCodeStr (iChar.begin(), iChar.end());
      const CountryCode_T lCountryCode (lCountryCodeStr);
      _location.setAltCountryCode (lCountryCode);
      // DEBUG
      //OPENTREP_LOG_DEBUG ("Alt country code: " << _location.getAltCountryCode());
    }

    // //////////////////////////////////////////////////////////////////
    storeAdm1Code::storeAdm1Code (Location& ioLocation)
      : ParserSemanticAction (ioLocation) {
    }

    // //////////////////////////////////////////////////////////////////
    void storeAdm1Code::operator() (std::vector<uchar_t> iChar,
                                    bsq::unused_type,
                                    bsq::unused_type) const {
      const std::string lAdmCodeStr (iChar.begin(), iChar.end());
      const Admin1Code_T lAdmCode (lAdmCodeStr);
      _location.setAdmin1Code (lAdmCode);
      // DEBUG
      //OPENTREP_LOG_DEBUG ("Adm1 code: " << _location.getAdmin1Code());
    }

    // //////////////////////////////////////////////////////////////////
    storeAdm2Code::storeAdm2Code (Location& ioLocation)
      : ParserSemanticAction (ioLocation) {
    }

    // //////////////////////////////////////////////////////////////////
    void storeAdm2Code::operator() (std::vector<uchar_t> iChar,
                                    bsq::unused_type,
                                    bsq::unused_type) const {
      const std::string lAdmCodeStr (iChar.begin(), iChar.end());
      const Admin2Code_T lAdmCode (lAdmCodeStr);
      _location.setAdmin2Code (lAdmCode);
      // DEBUG
      //OPENTREP_LOG_DEBUG ("Adm2 code: " << _location.getAdmin2Code());
    }

    // //////////////////////////////////////////////////////////////////
    storeAdm3Code::storeAdm3Code (Location& ioLocation)
      : ParserSemanticAction (ioLocation) {
    }

    // //////////////////////////////////////////////////////////////////
    void storeAdm3Code::operator() (std::vector<uchar_t> iChar,
                                    bsq::unused_type,
                                    bsq::unused_type) const {
      const std::string lAdmCodeStr (iChar.begin(), iChar.end());
      const Admin3Code_T lAdmCode (lAdmCodeStr);
      _location.setAdmin3Code (lAdmCode);
      // DEBUG
      //OPENTREP_LOG_DEBUG ("Adm3 code: " << _location.getAdmin3Code());
    }

    // //////////////////////////////////////////////////////////////////
    storeAdm4Code::storeAdm4Code (Location& ioLocation)
      : ParserSemanticAction (ioLocation) {
    }

    // //////////////////////////////////////////////////////////////////
    void storeAdm4Code::operator() (std::vector<uchar_t> iChar,
                                    bsq::unused_type,
                                    bsq::unused_type) const {
      const std::string lAdmCodeStr (iChar.begin(), iChar.end());
      const Admin4Code_T lAdmCode (lAdmCodeStr);
      _location.setAdmin4Code (lAdmCode);
      // DEBUG
      //OPENTREP_LOG_DEBUG ("Adm4 code: " << _location.getAdmin4Code());
    }

    // //////////////////////////////////////////////////////////////////
    storePopulation::storePopulation (Location& ioLocation)
      : ParserSemanticAction (ioLocation) {
    }
    
    // //////////////////////////////////////////////////////////////////
    void storePopulation::operator() (unsigned int iPopulation,
                                      bsq::unused_type, bsq::unused_type) const {
      _location.setPopulation (iPopulation);
      // DEBUG
      //OPENTREP_LOG_DEBUG ("Population: " << _location.getPopulation());
    }
    
    // //////////////////////////////////////////////////////////////////
    storeElevation::storeElevation (Location& ioLocation)
      : ParserSemanticAction (ioLocation) {
    }
    
    // //////////////////////////////////////////////////////////////////
    void storeElevation::operator() (int iElevation,
                                     bsq::unused_type, bsq::unused_type) const {
      _location.setElevation (iElevation);
      // DEBUG
      //OPENTREP_LOG_DEBUG ("Elevation: " << _location.getElevation());
    }
    
    // //////////////////////////////////////////////////////////////////
    storeGTopo30::storeGTopo30 (Location& ioLocation)
      : ParserSemanticAction (ioLocation) {
    }
    
    // //////////////////////////////////////////////////////////////////
    void storeGTopo30::operator() (int iGTopo30,
                                   bsq::unused_type, bsq::unused_type) const {
      _location.setGTopo30 (iGTopo30);
      // DEBUG
      //OPENTREP_LOG_DEBUG ("GTopo30: " << _location.getGTopo30());
    }

    // //////////////////////////////////////////////////////////////////
    storeTimeZone::storeTimeZone (Location& ioLocation)
      : ParserSemanticAction (ioLocation) {
    }

    // //////////////////////////////////////////////////////////////////
    void storeTimeZone::operator() (std::vector<uchar_t> iChar,
                                    bsq::unused_type,
                                    bsq::unused_type) const {
      const std::string lTimeZoneStr (iChar.begin(), iChar.end());
      const TimeZone_T lTimeZone (lTimeZoneStr);
      _location.setTimeZone (lTimeZone);
      // DEBUG
      //OPENTREP_LOG_DEBUG ("Time-zone code: " << _location.getTimeZone());
    }

    // //////////////////////////////////////////////////////////////////
    storeGMTOffset::storeGMTOffset (Location& ioLocation)
      : ParserSemanticAction (ioLocation) {
    }
    
    // //////////////////////////////////////////////////////////////////
    void storeGMTOffset::operator() (float iOffset,
                                     bsq::unused_type, bsq::unused_type) const {
      _location.setGMTOffset (iOffset);
      // DEBUG
      //OPENTREP_LOG_DEBUG ("GMT offset: " << _location.getGMTOffset());
    }
    
    // //////////////////////////////////////////////////////////////////
    storeDSTOffset::storeDSTOffset (Location& ioLocation)
      : ParserSemanticAction (ioLocation) {
    }
    
    // //////////////////////////////////////////////////////////////////
    void storeDSTOffset::operator() (float iOffset,
                                     bsq::unused_type, bsq::unused_type) const {
      _location.setDSTOffset (iOffset);
      // DEBUG
      //OPENTREP_LOG_DEBUG ("DST offset: " << _location.getDSTOffset());
    }
    
    // //////////////////////////////////////////////////////////////////
    storeRawOffset::storeRawOffset (Location& ioLocation)
      : ParserSemanticAction (ioLocation) {
    }
    
    // //////////////////////////////////////////////////////////////////
    void storeRawOffset::operator() (float iOffset,
                                     bsq::unused_type, bsq::unused_type) const {
      _location.setRawOffset (iOffset);
      // DEBUG
      //OPENTREP_LOG_DEBUG ("Raw offset: " << _location.getRawOffset());
    }
    
    // //////////////////////////////////////////////////////////////////
    storeModDate::storeModDate (Location& ioLocation)
      : ParserSemanticAction (ioLocation) {
    }
    
    // //////////////////////////////////////////////////////////////////
    void storeModDate::operator() (bsq::unused_type,
                                   bsq::unused_type, bsq::unused_type) const {
      const OPENTREP::Date_T& lModDate = _location.calculateDate();
      _location.setModificationDate (lModDate);
      // DEBUG
      //OPENTREP_LOG_DEBUG ("Modification date: " << _location.getModificationDate());
    }

    // //////////////////////////////////////////////////////////////////
    storeIsAirport::storeIsAirport (Location& ioLocation)
      : ParserSemanticAction (ioLocation) {
    }
    
    // //////////////////////////////////////////////////////////////////
    void storeIsAirport::operator() (bool iBool,
                                     bsq::unused_type, bsq::unused_type) const {
      _location.setIsAirport (iBool);
       // DEBUG
       //OPENTREP_LOG_DEBUG ("Is airport? " << _location.isAirport());
    }

    // //////////////////////////////////////////////////////////////////
    storeIsCommercial::storeIsCommercial (Location& ioLocation)
      : ParserSemanticAction (ioLocation) {
    }
    
    // //////////////////////////////////////////////////////////////////
    void storeIsCommercial::operator() (bool iBool, bsq::unused_type,
                                        bsq::unused_type) const {
      _location.setIsCommercial (iBool);
       // DEBUG
       //OPENTREP_LOG_DEBUG ("Is commercial? " << _location.isAirport());
    }

    // //////////////////////////////////////////////////////////////////
    storeCityCode::storeCityCode (Location& ioLocation)
      : ParserSemanticAction (ioLocation) {
    }
    
    // //////////////////////////////////////////////////////////////////
    void storeCityCode::operator() (std::vector<uchar_t> iChar,
                                    bsq::unused_type, bsq::unused_type) const {

      const std::string lCityCodeStr (iChar.begin(), iChar.end());
      const OPENTREP::CityCode_T lCityCode (lCityCodeStr);
      _location.setCityCode (lCityCode);
       // DEBUG
       //OPENTREP_LOG_DEBUG ("City code: " << _location.getCityCode());
    }

    // //////////////////////////////////////////////////////////////////
    storeStateCode::storeStateCode (Location& ioLocation)
      : ParserSemanticAction (ioLocation) {
    }
    
    // //////////////////////////////////////////////////////////////////
    void storeStateCode::operator() (std::vector<uchar_t> iChar,
                                     bsq::unused_type, bsq::unused_type) const {

      const std::string lStateCodeStr (iChar.begin(), iChar.end());
      const OPENTREP::StateCode_T lStateCode (lStateCodeStr);
      _location.setStateCode (lStateCode);
       // DEBUG
       //OPENTREP_LOG_DEBUG ("State code: " << _location.getStateCode());
    }

    // //////////////////////////////////////////////////////////////////
    storeRegionCode::storeRegionCode (Location& ioLocation)
      : ParserSemanticAction (ioLocation) {
    }
    
    // //////////////////////////////////////////////////////////////////
    void storeRegionCode::operator() (std::vector<uchar_t> iChar,
                                      bsq::unused_type, bsq::unused_type) const {

      const std::string lRegionCodeStr (iChar.begin(), iChar.end());
      const OPENTREP::RegionCode_T lRegionCode (lRegionCodeStr);
      _location.setRegionCode (lRegionCode);
       // DEBUG
       //OPENTREP_LOG_DEBUG ("Region code: " << _location.getRegionCode());
    }

    // //////////////////////////////////////////////////////////////////
    storePORType::storePORType (Location& ioLocation)
      : ParserSemanticAction (ioLocation) {
    }

    // //////////////////////////////////////////////////////////////////
    void storePORType::operator() (std::vector<uchar_t> iChar,
                                   bsq::unused_type, bsq::unused_type) const {
      const std::string lIATATypeStr (iChar.begin(), iChar.end());
      const IATAType_T lIATAType (lIATATypeStr);
      _location.setIATAType (lIATAType);
      // DEBUG
      //OPENTREP_LOG_DEBUG ("IATA type: " << _location.getIATAType());
    }

    // //////////////////////////////////////////////////////////////////
    storeWikiLink::storeWikiLink (Location& ioLocation)
      : ParserSemanticAction (ioLocation) {
    }
    
    // //////////////////////////////////////////////////////////////////
    void storeWikiLink::operator() (std::vector<uchar_t> iChar,
                                    bsq::unused_type, bsq::unused_type) const {

      const std::string lWikiLinkStr (iChar.begin(), iChar.end());
      const OPENTREP::WikiLink_T lWikiLink (lWikiLinkStr);
      _location.setWikiLink (lWikiLink);
       // DEBUG
       // OPENTREP_LOG_DEBUG ("Wiki link: " << _location.getWikiLink());
    }

    // //////////////////////////////////////////////////////////////////
    storeAltLangCodeFull::storeAltLangCodeFull (Location& ioLocation)
      : ParserSemanticAction (ioLocation) {
    }
    
    // //////////////////////////////////////////////////////////////////
    void storeAltLangCodeFull::operator() (std::vector<uchar_t> iChar,
                                           bsq::unused_type, bsq::unused_type) const {

      const std::string lAltLangCodeStr (iChar.begin(), iChar.end());
      const OPENTREP::LanguageCode_T lAltLangCode (lAltLangCodeStr);
      _location._itLanguageCode = lAltLangCode;
       // DEBUG
       //OPENTREP_LOG_DEBUG ("Alt lang full code: " << _location._itLanguageCode);
    }

    // //////////////////////////////////////////////////////////////////
    storeAltLangCode2Char::storeAltLangCode2Char (Location& ioLocation)
      : ParserSemanticAction (ioLocation) {
    }
    
    // //////////////////////////////////////////////////////////////////
    void storeAltLangCode2Char::operator() (std::vector<uchar_t> iChar,
                                            bsq::unused_type, bsq::unused_type) const {

      const std::string lAltLangCodeStr (iChar.begin(), iChar.end());
      _location._itLangCode2Char = lAltLangCodeStr;
      _location._itLangCodeExt = "";
      _location._itLangCodeHist = "";
       // DEBUG
       //OPENTREP_LOG_DEBUG ("Alt lang 2-char code: " << _location._itLangCode2Char);
    }

    // //////////////////////////////////////////////////////////////////
    storeAltLangCodeExt::storeAltLangCodeExt (Location& ioLocation)
      : ParserSemanticAction (ioLocation) {
    }
    
    // //////////////////////////////////////////////////////////////////
    void storeAltLangCodeExt::operator() (std::vector<uchar_t> iChar,
                                          bsq::unused_type, bsq::unused_type) const {

      const std::string lAltLangCodeStr (iChar.begin(), iChar.end());
      std::ostringstream oStr;
      oStr << _location._itLangCode2Char << "-" << lAltLangCodeStr;
      _location._itLangCodeExt = oStr.str();
       // DEBUG
       //OPENTREP_LOG_DEBUG ("Alt lang 2-char code: " << _location._itLangCodeExt);
    }

    // //////////////////////////////////////////////////////////////////
    storeAltLangCodeHist::storeAltLangCodeHist (Location& ioLocation)
      : ParserSemanticAction (ioLocation) {
    }
    
    // //////////////////////////////////////////////////////////////////
    void storeAltLangCodeHist::operator() (std::vector<uchar_t> iChar,
                                           bsq::unused_type, bsq::unused_type) const {

      const std::string lAltLangCodeStr (iChar.begin(), iChar.end());
      std::ostringstream oStr;
      oStr << _location._itLangCode2Char << "_" << lAltLangCodeStr;
      _location._itLangCodeHist = oStr.str();
       // DEBUG
       //OPENTREP_LOG_DEBUG ("Alt lang 2-char code: " << _location._itLangCodeHist);
    }

    // //////////////////////////////////////////////////////////////////
    storeAltLangCodeEnd::storeAltLangCodeEnd (Location& ioLocation)
      : ParserSemanticAction (ioLocation) {
    }
    
    // //////////////////////////////////////////////////////////////////
    void storeAltLangCodeEnd::operator() (bsq::unused_type,
                                          bsq::unused_type, bsq::unused_type) const {

      std::ostringstream oStr;
      oStr << _location._itLangCode2Char << _location._itLangCodeExt
           << _location._itLangCodeHist;
      const OPENTREP::Language::EN_Language iLanguageCode =
        OPENTREP::Language::getCode (oStr.str());
      _location._itLanguageCodeNum = iLanguageCode;
       // DEBUG
       //OPENTREP_LOG_DEBUG ("Alt lang code: " << OPENTREP::Language::getShortLabel (_location._itLanguageCodeNum));
    }

    // //////////////////////////////////////////////////////////////////
    storeAltName::storeAltName (Location& ioLocation)
      : ParserSemanticAction (ioLocation) {
    }
    
    // //////////////////////////////////////////////////////////////////
    void storeAltName::operator() (std::vector<uchar_t> iChar,
                                   bsq::unused_type, bsq::unused_type) const {

      const std::string lAltNameStr (iChar.begin(), iChar.end());
      const OPENTREP::AltNameShortListString_T lAltName (lAltNameStr);
      //_location.addName (_location._itLanguageCodeNum, lAltName);
      _location.addName (_location._itLanguageCode, lAltName);
      // Reset the values
      //_location._itLanguageCodeNum = OPENTREP::Language::LAST_VALUE;
      _location._itLanguageCode = LanguageCode_T ("");
       // DEBUG
       //OPENTREP_LOG_DEBUG ("Alt name: " << _location.getAltNameShortList());
    }

    
    // //////////////////////////////////////////////////////////////////
    doEndPor::doEndPor (Location& ioLocation)
      : ParserSemanticAction (ioLocation) {
    }
    
    // //////////////////////////////////////////////////////////////////
    void doEndPor::operator() (bsq::unused_type,
                               bsq::unused_type, bsq::unused_type) const {
      // DEBUG
      //OPENTREP_LOG_DEBUG ("Do End. Location structure: " << _location);

      // Generation of the Place object from the Location structure.
      //Place& lPlace = FacPlace::instance().create (_location);

      // DEBUG
      // OPENTREP_LOG_DEBUG (lPlace);
    }  

    // ///////////////////////////////////////////////////////////////////
    //
    //  Utility Parsers
    //
    // ///////////////////////////////////////////////////////////////////
    
    /** 1-digit-integer parser */
    OPENTREP::uint1_p_t uint1_p;
    
    /** 2-digit-integer parser */
    OPENTREP::uint2_p_t uint2_p;

    /** Up-to-2-digit-integer parser */
    OPENTREP::int1_2_p_t int1_2_p;

    /** 4-digit-integer parser */
    OPENTREP::uint4_p_t uint4_p;
    
    /** Up-to-5-digit-integer parser */
    OPENTREP::int1_5_p_t int1_5_p;
    OPENTREP::int0_5_p_t int0_5_p;

    /** Up-to-4-digit-integer parser */
    OPENTREP::uint1_4_p_t uint1_4_p;

    /** Up-to-9-digit-integer parser */
    OPENTREP::uint1_9_p_t uint1_9_p;
    OPENTREP::uint0_9_p_t uint0_9_p;

    /** Time element parsers. */
    OPENTREP::hour_p_t hour_p;
    OPENTREP::minute_p_t minute_p;
    OPENTREP::second_p_t second_p;

    /** Date element parsers. */
    OPENTREP::year_p_t year_p;
    OPENTREP::month_p_t month_p;
    OPENTREP::day_p_t day_p;

    /** Boolean parser */
    OPENTREP::boolean_p_t boolean_p;

    /////////////////////////////////////////////////////////////////////////
    //
    //  (Boost Spirit) Grammar Definition
    //
    /////////////////////////////////////////////////////////////////////////
    /**

       -- Geonames-related part:
       -- ----------------------
       -- iata_code         : IATA code; varchar(3). See also:
       --                    http://www.iata.org/ps/publications/Pages/code-search.aspx
       -- icao_code         : ICAO code; varchar(4)
       -- geonameid         : Integer ID of record in geonames database
       -- name              : Name of geographical point
       --                     (UTF8) varchar(200)
       -- asciiname         : Name of geographical point in plain ascii characters
       --                     (ASCII) varchar(200)
       -- alternatenames    : Alternate names, comma separated
       --                     varchar(5000)
       -- latitude          : Latitude in decimal degrees (wgs84)
       -- longitude         : Longitude in decimal degrees (wgs84)
       -- feature class     : See http://www.geonames.org/export/codes.html
       --                     char(1)
       -- feature code      : See http://www.geonames.org/export/codes.html
       --                     varchar(10)
       -- country code      : ISO-3166 2-letter country code, 2 characters
       -- cc2               : Alternate country codes, comma separated, ISO-3166
       --                     2-letter country code, 60 characters
       -- admin1 code       : FIPS code (subject to change to ISO code), see exceptions
       --                     below. See file admin1Codes.txt for display names of
       --                     this code; varchar(20)
       -- admin2 code       : Code for the second administrative division, a county
       --                     in the US. See file admin2Codes.txt; varchar(80)
       -- admin3 code       : Code for third level administrative division
       --                     varchar(20)
       -- admin4 code       : Code for fourth level administrative division
       --                     varchar(20)
       -- population        : bigint (8 byte int) 
       -- elevation         : In meters, integer
       -- dem               : Digital elevation model, srtm3 or gtopo30, average
       --                     elevation of 3''x3'' (ca 90mx90m) or 30''x30''
       --                     (ca 900mx900m) area in meters, integer.
       --                     srtm processed by cgiar/ciat.
       -- timezone          : The time-zone ID (see file timeZone.txt)
       -- gmt offset        : GMT offset on 1st of January
       -- dst offset        : DST offset to GMT on 1st of July (of the current year)
       -- raw offset        : Raw Offset without DST
       -- modification date : Date of last modification in yyyy-MM-dd format
       --
       --
       -- ORI-related part:
       -- -----------------
       --
       -- is_geonames       : Whether that POR is known by Geonames; varchar(1)
       -- is_airport        : Whether that POR is an airport; varchar(1)
       -- is commercial     : Whether or not that POR hosts commercial activities
       --                     varchar(1)
       -- city_code         : The IATA code of the related city, when knwon; varchar(3)
       -- state_code        : The ISO code of the related state; varchar(3)
       -- region_code       : The code of the related region (see below); varchar(5)
       -- location type     : A/APT airport; B/BUS bus/coach station; C/CITY City;
       --                     G/GRD ground transport (this code is used for SK in
       --                     Sweden only); H/HELI Heliport;
       --                     O/OFF-PT off-line point, i.e. a city without an airport;
       --                     R/RAIL railway Station; S/ASSOC a location without its
       --                     own IATA code, but attached to an IATA location.
       --
       -- Regions:
       -- --------
       -- AFRIC / AF        : Africa (geonameId=6255146)
       -- ASIA  / AS        : Asia (geonameId=6255147)
       -- ATLAN             : Atlantic
       -- AUSTL             : Australia
       -- CAMER             : Central America
       -- CARIB             : Carribean
       -- EEURO             : Eastern Europe
       -- EURAS             : Euras
       -- EUROP / EU        : Europe (geonameId=6255148)
       -- IOCEA / OC        : Oceania (geonameId=6255151)
       -- MEAST             : Middle-East
       -- NAMER / NA        : North America (geonameId=6255149)
       -- NONE              : Non real POR
       -- PACIF             : Pacific
       -- SAMER / SA        : South America (geonameId=6255150)
       -- SEASI             : South East
       --       / AN        : Antarctica (geonameId=6255152)
       --
       -- Samples:
       -- CDG^LFPG^Y^6269554^Paris - Charles-de-Gaulle^Paris - Charles-de-Gaulle^CDG,LFPG,Paris - Charles de Gaulle,París - Charles de Gaulle,Roissy Charles de Gaulle^49.012779^2.55^S^AIRP^FR^^A8^95^^^0^119^106^Europe/Paris^1.0^2.0^1.0^2008-07-09^Y^Y^PAR^^EUROP^A^http://en.wikipedia.org/wiki/Paris-Charles_de_Gaulle_Airport^es^París - Charles de Gaulle^p^^Roissy Charles de Gaulle^
       -- PAR^ZZZZ^Y^2988507^Paris^Paris^Baariis,Bahliz,Gorad Paryzh,Lungsod ng Paris,Lutece,Lutetia,Lutetia Parisorum,PAR,Pa-ri,Paarys,Palika,Paname,Pantruche,Paraeis,Paras,Pari,Paries,Parigge,Pariggi,Parighji,Parigi,Pariis,Pariisi,Parij,Parijs,Paris,Parisi,Parixe,Pariz,Parize,Parizh,Parizh osh,Parizh',Parizo,Parizs,Pariž,Parys,Paryz,Paryzius,Paryż,Paryžius,Paräis,París,Paríž,Parîs,Parĩ,Parī,Parīze,Paříž,Páras,Párizs,Ville-Lumiere,Ville-Lumière,ba li,barys,pairisa,pali,pari,paris,parys,paryzh,perisa,pryz,pyaris,pyarisa,pyrs,pʾrys,Παρίσι,Горад Парыж,Париж,Париж ош,Парижь,Париз,Парис,Паріж,Փարիզ,פאריז,פריז,باريس,پارىژ,پاريس,پاریس,پیرس,ܦܐܪܝܣ,पॅरिस,पेरिस,पैरिस,প্যারিস,ਪੈਰਿਸ,પૅરિસ,பாரிஸ்,పారిస్,ಪ್ಯಾರಿಸ್,പാരിസ്,ปารีส,ཕ་རི།,ပ<U+102B>ရီမ<U+103C>ို့,პარიზი,ፓሪስ,ប៉ារីស,パリ,巴黎,파리^49.02^2.533^P^PPLC^FR^^A8^75^751^75056^2138551^^42^Europe/Paris^1.0^2.0^1.0^2012-08-19^N^N^PAR^^EUROP^C^http://en.wikipedia.org/wiki/Paris^la^Lutetia Parisorum^^fr^Lutece^h^fr^Ville-Lumière^c^eo^Parizo^^es^París^ps^de^Paris^^en^Paris^p^af^Parys^^als^Paris^^an^París^^ar^باريس^^ast^París^^be^Горад Парыж^^bg^Париж^^ca^París^^cs^Paříž^^cy^Paris^^da^Paris^^el^Παρίσι^^et^Pariis^^eu^Paris^^fa^پاریس^^fi^Pariisi^^fr^Paris^p^ga^Páras^^gl^París^^he^פריז^^hr^Pariz^^hu^Párizs^^id^Paris^^io^Paris^^it^Parigi^^ja^パリ^^ka^პარიზი^^kn^ಪ್ಯಾರಿಸ್^^ko^파리^^ku^Parîs^^kw^Paris^^lb^Paräis^^li^Paries^^lt^Paryžius^^lv^Parīze^^mk^Париз^^ms^Paris^^na^Paris^^nds^Paris^^nl^Parijs^^nn^Paris^^no^Paris^^oc^París^^pl^Paryż^^pt^Paris^^ro^Paris^^ru^Париж^^scn^Pariggi^^sco^Paris^^sl^Pariz^^sq^Paris^^sr^Париз^^sv^Paris^^ta^பாரிஸ்^^th^ปารีส^^tl^Paris^^tr^Paris^^uk^Париж^^vi^Paris^p^zh-CN^巴黎^^ia^Paris^^fy^Parys^^ln^Pari^^os^Париж^^pms^Paris^^sk^Paríž^^sq^Parisi^^sw^Paris^^tl^Lungsod ng Paris^^ug^پارىژ^^fr^Paname^c^fr^Pantruche^c^am^ፓሪስ^^arc^ܦܐܪܝܣ^^br^Pariz^^gd^Paris^^gv^Paarys^^hy^Փարիզ^^ksh^Paris^^lad^Paris^^lmo^Paris^^mg^Paris^^mr^पॅरिस^^tet^París^^tg^Париж^^ty^Paris^^ur^پیرس^^vls^Parys^^is^París^^vi^Pa-ri^^ml^പാരിസ്^^uz^Parij^^rue^Паріж^^ne^पेरिस^^jbo^paris^^mn^Парис^^lij^Pariggi^^vec^Parixe^^yo^Parisi^^yi^פאריז^^mrj^Париж^^hi^पैरिस^^fur^Parîs^^tt^Париж^^szl^Paryż^^mhr^Париж^^te^పారిస్^^tk^Pariž^^bn^প্যারিস^^ha^Pariis^^sah^Париж^^mzn^پاریس^^bo^ཕ་རི།^^haw^Palika^^mi^Parī^^ext^París^^ps^پاريس^^pa^ਪੈਰਿਸ^^ckb^پاریس^^cu^Парижь^^cv^Парис^^co^Parighji^^bs^Pariz^^so^Baariis^^sh^Pariz^^gu^પૅરિસ^^xmf^პარიზი^^ba^Париж^^pnb^پیرس^^arz^باريس^^la^Lutetia^^kk^Париж^^kv^Париж^^gn^Parĩ^^ky^Париж^^myv^Париж ош^^nap^Parigge^^km^ប៉ារីស^^krc^Париж^^udm^Париж^^wo^Pari^^gan^巴黎^^sc^Parigi^^za^Bahliz^^my^ပ<U+102B>ရီမ<U+103C>ို့^
       --

       iata_code          varchar(3)
       icao_code          varchar(4)
       is_geonames        varchar(1)
       geonameid          int(11)
       name               varchar(200)
       asciiname          varchar(200)
       alternatenames     varchar(4000)
       latitude           decimal(10,7)
       longitude          decimal(10,7)
       fclass             varchar(1)
       fcode              varchar(10)
       country_code       varchar(2)
       cc2                varchar(60)
       admin1             varchar(20)
       admin2             varchar(80)
       admin3             varchar(20)
       admin4             varchar(20)
       population         int(11)
       elevation          int(11)
       gtopo30            int(11)
       timezone           varchar(40)
       gmt_offset         decimal(3,1)
       dst_offset         decimal(3,1)
       raw_offset         decimal(3,1)
       moddate            date
       is_airport         varchar(1)
       is_commercial      varchar(1)
       city_code          varchar(3)
       state_code         varchar(3)
       region_code        varchar(5)
       location_type      varchar(4)
       wiki_link          varchar(200)
       lang_alt1          varchar(7)
       alt_name1          varchar(200)
       lang_alt2          varchar(7)
       alt_name2          varchar(200)
       lang_alt3          varchar(7)
       alt_name3          varchar(200)
       lang_alt4          varchar(7)
       alt_name4          varchar(200)
       lang_alt5          varchar(7)
       alt_name5          varchar(200)
       lang_alt6          varchar(7)
       alt_name6          varchar(200)
       lang_alt7          varchar(7)
       alt_name7          varchar(200)
       lang_alt8          varchar(7)
       alt_name8          varchar(200)
       lang_alt9          varchar(7)
       alt_name9          varchar(200)
       lang_alt10         varchar(7)
       alt_name10         varchar(200)

       iata_code^icao_code^is_geonames^geonameid^name^asciiname^alternatenames^
       latitude^longitude^fclass^fcode^
       country_code^cc2^admin1^admin2^admin3^admin4^
       population^elevation^gtopo30^timezone^gmt_offset^dst_offset^raw_offset^
       moddate^is_airport^is_commercial^
       city_code^state_code^region_code^location_type^wiki_link^
       lang_alt1^alt_name1^lang_alt2^alt_name2^lang_alt3^alt_name3^
       lang_alt4^alt_name4^lang_alt5^alt_name5^lang_alt6^alt_name6^
       lang_alt7^alt_name7^lang_alt8^alt_name8^lang_alt9^alt_name9^
       lang_alt10^alt_name10
    */ 

    /**
     * Grammar for the Por-Rule parser.
     */
    template <typename Iterator>	
    struct LocationParser : public bsq::grammar<Iterator, bsu::blank_type> {

      LocationParser (Location& ioPORRule) : 
        LocationParser::base_type(start), _location(ioPORRule) {

        start = bsq::eps
          >> *(header | por_rule);

        header = bsq::lit("iata_code") >> +(bsu::char_ - bsq::eoi - bsq::eol)
                                       >> (bsq::eoi | bsq::eol);

        por_rule = por_key
          >> '^' >> por_details
          >> -alt_name_section
          >> por_rule_end[doEndPor(_location)];
        // >> +( '^' >> segment )

        por_rule_end = bsq::eps;

        por_key = iata_code
          >> '^' >> icao_code
          >> '^' >> is_geonames
          >> '^' >> geonameid
          ;

        por_details = common_name
          >> '^' >> ascii_name
          >> '^' >> -alt_name_short_list[storeAltNameShortListString(_location)]
          >> '^' >> -latitude
          >> '^' >> -longitude
          >> '^' >> feat_class
          >> '^' >> feat_code
          >> '^' >> country_code
          >> '^' >> -country_code2
          >> '^' >> -adm1_code
          >> '^' >> -adm2_code
          >> '^' >> -adm3_code
          >> '^' >> -adm4_code
          >> '^' >> -population
          >> '^' >> -elevation
          >> '^' >> -gtopo30
          >> '^' >> -time_zone
          >> '^' >> -gmt_offset
          >> '^' >> -dst_offset
          >> '^' >> -raw_offset
          >> '^' >> (mod_date | bsq::lit("-1"))
          >> '^' >> is_airport
          >> '^' >> is_commercial
          >> '^' >> city_code
          >> '^' >> -state_code
          >> '^' >> -region_code
          >> '^' >> por_type
          >> '^' >> -wiki_link
          ;

        iata_code =
          bsq::repeat(3)[bsu::char_('A', 'Z')][storeIataCode(_location)];

        icao_code =
          bsq::repeat(4)[bsu::char_("A-Z0-9")][storeIcaoCode(_location)];

        geonameid = uint1_9_p[storeGeonamesID(_location)];

        is_geonames = boolean_p;

        common_name =
          (bsq::no_skip[+~bsu::char_('^')]
           - (bsq::eoi|bsq::eol))[storeCommonName(_location)]
          ;

        ascii_name =
          (bsq::no_skip[+~bsu::char_('^')]
           - (bsq::eoi|bsq::eol))[storeAsciiName(_location)]
          ;

        alt_name_short_list = alt_name_short % ',';

        alt_name_short =
          (bsq::no_skip[+~bsu::char_("^,")]
           - (bsq::eoi|bsq::eol))[storeAltNameShort(_location)]
          ;

        latitude = bsq::double_[storeLatitude(_location)];

        longitude = bsq::double_[storeLongitude(_location)];

        feat_class =
          bsq::repeat(1)[bsu::char_("A-Z")][storeFeatureClass(_location)]
          ;

        feat_code =
          bsq::repeat(2,5)[bsu::char_("A-Z1-5")][storeFeatureCode(_location)]
          ;

        country_code =
          bsq::repeat(2,3)[bsu::char_("A-Z")][storeCountryCode(_location)]
          ;

        country_code2 =
          (bsq::no_skip[+~bsu::char_('^')]
           - (bsq::eoi|bsq::eol))[storeAltCountryCode(_location)]
          ;

        adm1_code =
          (bsq::no_skip[+~bsu::char_('^')]
           - (bsq::eoi|bsq::eol))[storeAdm1Code(_location)]
          ;

        adm2_code =
          (bsq::no_skip[+~bsu::char_('^')]
           - (bsq::eoi|bsq::eol))[storeAdm2Code(_location)]
          ;

        adm3_code =
          (bsq::no_skip[+~bsu::char_('^')]
           - (bsq::eoi|bsq::eol))[storeAdm3Code(_location)]
          ;

        adm4_code =
          (bsq::no_skip[+~bsu::char_('^')]
           - (bsq::eoi|bsq::eol))[storeAdm4Code(_location)]
          ;

        population = uint1_9_p[storePopulation(_location)];

        elevation = int1_5_p[storeElevation(_location)];

        gtopo30 = int1_5_p[storeGTopo30(_location)];

        time_zone =
          (bsq::no_skip[+~bsu::char_('^')]
           - (bsq::eoi|bsq::eol))[storeTimeZone(_location)]
          ;

        gmt_offset = bsq::float_[storeGMTOffset(_location)];

        dst_offset = bsq::float_[storeDSTOffset(_location)];

        raw_offset = bsq::float_[storeRawOffset(_location)];

        mod_date = date[storeModDate(_location)];

        date = bsq::lexeme
          [year_p[boost::phoenix::ref(_location._itYear) = bsq::labels::_1]
           >> '-'
           >> month_p[boost::phoenix::ref(_location._itMonth) = bsq::labels::_1]
           >> '-'
           >> day_p[boost::phoenix::ref(_location._itDay) = bsq::labels::_1] ];

        is_airport = boolean_p[storeIsAirport(_location)];

        is_commercial = boolean_p[storeIsCommercial(_location)];
      
        city_code =
          bsq::repeat(3)[bsu::char_('A', 'Z')][storeCityCode(_location)]
          ;

        state_code =
          (bsq::no_skip[+~bsu::char_('^')]
           - (bsq::eoi|bsq::eol))[storeStateCode(_location)]
          ;

        region_code =
          (bsq::no_skip[+~bsu::char_('^')]
           - (bsq::eoi|bsq::eol))[storeRegionCode(_location)]
          ;

        por_type =
          bsq::repeat(1,3)[bsu::char_("ABCGHOPRZ")][storePORType(_location)]
          ;
      
        wiki_link =
          (bsq::no_skip[+~bsu::char_('^')]
           - (bsq::eoi|bsq::eol))[storeWikiLink(_location)]
          ;

        alt_name_section = +('^' >>  alt_name_details);

        //alt_name_details = -alt_lang_code_ftd[storeAltLangCodeEnd(_location)]
        alt_name_details =
          -alt_lang_code
          >> '^' >> alt_name
          >> '^' >> -alt_name_qualifiers        
          ;

        alt_lang_code =
          (+~bsu::char_('^') - (bsq::eoi|bsq::eol))[storeAltLangCodeFull(_location)]
          ;

        alt_lang_code_ftd = lang_code_2char >> lang_code_opt;

        lang_code_opt = -(lang_code_ext | lang_code_hist);

        lang_code_2char =
          bsq::repeat(2,4)[bsu::char_("a-z")][storeAltLangCode2Char(_location)]
          ;

        lang_code_ext =
          '-' >> bsq::repeat(1,4)[bsu::char_('A', 'Z')][storeAltLangCodeExt(_location)];

        lang_code_hist =
          '_' >> bsq::repeat(1,4)[bsu::char_("a-z0-9")][storeAltLangCodeHist(_location)];

        alt_name =
          (bsq::no_skip[+~bsu::char_('^')]
           - (bsq::eoi|bsq::eol))[storeAltName(_location)]
          ;

        alt_name_qualifiers =
          bsq::repeat(1,4)[bsu::char_("shpc")]
          ;

        //BOOST_SPIRIT_DEBUG_NODE (LocationParser);
        BOOST_SPIRIT_DEBUG_NODE (start);
        BOOST_SPIRIT_DEBUG_NODE (header);
        BOOST_SPIRIT_DEBUG_NODE (por_rule);
        BOOST_SPIRIT_DEBUG_NODE (por_rule_end);
        BOOST_SPIRIT_DEBUG_NODE (por_key);
        BOOST_SPIRIT_DEBUG_NODE (por_details);
        BOOST_SPIRIT_DEBUG_NODE (iata_code);
        BOOST_SPIRIT_DEBUG_NODE (icao_code);
        BOOST_SPIRIT_DEBUG_NODE (geonameid);
        BOOST_SPIRIT_DEBUG_NODE (is_geonames);
        BOOST_SPIRIT_DEBUG_NODE (common_name);
        BOOST_SPIRIT_DEBUG_NODE (ascii_name);      
        BOOST_SPIRIT_DEBUG_NODE (alt_name_short_list);
        BOOST_SPIRIT_DEBUG_NODE (alt_name_short);
        BOOST_SPIRIT_DEBUG_NODE (alt_name_sep);
        BOOST_SPIRIT_DEBUG_NODE (latitude);
        BOOST_SPIRIT_DEBUG_NODE (longitude);
        BOOST_SPIRIT_DEBUG_NODE (feat_class);
        BOOST_SPIRIT_DEBUG_NODE (feat_code);
        BOOST_SPIRIT_DEBUG_NODE (country_code);
        BOOST_SPIRIT_DEBUG_NODE (country_code2);
        BOOST_SPIRIT_DEBUG_NODE (adm1_code);
        BOOST_SPIRIT_DEBUG_NODE (adm2_code);
        BOOST_SPIRIT_DEBUG_NODE (adm3_code);
        BOOST_SPIRIT_DEBUG_NODE (adm4_code);
        BOOST_SPIRIT_DEBUG_NODE (population);
        BOOST_SPIRIT_DEBUG_NODE (elevation);
        BOOST_SPIRIT_DEBUG_NODE (gtopo30);
        BOOST_SPIRIT_DEBUG_NODE (time_zone);
        BOOST_SPIRIT_DEBUG_NODE (gmt_offset);
        BOOST_SPIRIT_DEBUG_NODE (raw_offset);
        BOOST_SPIRIT_DEBUG_NODE (dst_offset);
        BOOST_SPIRIT_DEBUG_NODE (mod_date);
        BOOST_SPIRIT_DEBUG_NODE (date);
        BOOST_SPIRIT_DEBUG_NODE (city_code);
        BOOST_SPIRIT_DEBUG_NODE (state_code);
        BOOST_SPIRIT_DEBUG_NODE (region_code);
        BOOST_SPIRIT_DEBUG_NODE (is_airport);
        BOOST_SPIRIT_DEBUG_NODE (is_commercial);
        BOOST_SPIRIT_DEBUG_NODE (por_type);
        BOOST_SPIRIT_DEBUG_NODE (wiki_link);
        BOOST_SPIRIT_DEBUG_NODE (alt_name_section);
        BOOST_SPIRIT_DEBUG_NODE (alt_name_details);
        BOOST_SPIRIT_DEBUG_NODE (alt_lang_code);
        BOOST_SPIRIT_DEBUG_NODE (alt_lang_code_ftd);
        BOOST_SPIRIT_DEBUG_NODE (alt_name);
        BOOST_SPIRIT_DEBUG_NODE (alt_name_qualifiers);
        BOOST_SPIRIT_DEBUG_NODE (lang_code_opt);
        BOOST_SPIRIT_DEBUG_NODE (lang_code_2char);
        BOOST_SPIRIT_DEBUG_NODE (lang_code_ext);
        BOOST_SPIRIT_DEBUG_NODE (lang_code_hist);
      }

      // Instantiation of rules
      bsq::rule<Iterator, bsu::blank_type>
      start, header, por_rule, por_rule_end, por_key, por_details,
        iata_code, icao_code, geonameid,
        is_geonames, is_airport, is_commercial,
        common_name, ascii_name,
        alt_name_short_list, alt_name_short, alt_name_sep,
        latitude, longitude, feat_class, feat_code,
        country_code, country_code2,
        adm1_code, adm2_code, adm3_code, adm4_code,
        population, elevation, gtopo30,
        time_zone, gmt_offset, dst_offset, raw_offset,
        mod_date, date,
        city_code, state_code, region_code,
        por_type, wiki_link,
        alt_name_section, alt_name_details,
        alt_lang_code, alt_lang_code_ftd, alt_name, alt_name_qualifiers,
        lang_code_opt, lang_code_2char, lang_code_ext, lang_code_hist,
        destination;
      
      // Parser Context
      Location& _location;
    };    
  }


  /////////////////////////////////////////////////////////////////////////
  //
  //  Entry class for the string parser
  //
  /////////////////////////////////////////////////////////////////////////
    
  // //////////////////////////////////////////////////////////////////////
  PORStringParser::PORStringParser (const std::string& iString)
    : _string (iString) {
    init();
  }

  // //////////////////////////////////////////////////////////////////////
  void PORStringParser::init() {
  }
    
  // //////////////////////////////////////////////////////////////////////
  PORStringParser::~PORStringParser() {
  }

  // //////////////////////////////////////////////////////////////////////
  const Location& PORStringParser::generateLocation() {
    // DEBUG
    // OPENTREP_LOG_DEBUG ("Parsing POR string: '" << _string << "'");

    // String to be parsed
    std::istringstream stringToBeParsed (_string);
    
    // Create an input iterator
    OPENTREP::base_iterator_t iStr (stringToBeParsed);

    // Convert input iterator to an iterator usable by spirit parser  
    OPENTREP::iterator_t fwd_start(boost::spirit::make_default_multi_pass(iStr));
    OPENTREP::iterator_t fwd_end;

    // Initialise the positional iterators
    OPENTREP::pos_iterator_t pos_start (fwd_start, fwd_end, _string);
    OPENTREP::pos_iterator_t pos_end;

    // Initialise the parser (grammar) with the helper/staging structure.
    PorParserHelper::LocationParser<OPENTREP::iterator_t> lPORParser (_location);
      
    // Launch the parsing of the file and, thanks to the doEndPor
    // call-back structure, the building of the whole BomRoot BOM
    bool hasParsingBeenSuccesful = false;
    try {

      hasParsingBeenSuccesful = bsq::phrase_parse (fwd_start, fwd_end,
                                                   lPORParser, bsu::blank);

    } catch (const bsq::expectation_failure<pos_iterator_t>& e) {
      const bsc::file_position_base<std::string>& pos = e.first.get_position();
      std::ostringstream oStr;
      oStr << "Parse error on POR string '" << _string
           << "', position " << pos.column << std::endl
           << "'" << e.first.get_currentline() << "'" << std::endl
           << std::setw(pos.column) << " " << "^- here";
      OPENTREP_LOG_ERROR (oStr.str());
      throw PorFileParsingException (oStr.str());
    }
      
    if (hasParsingBeenSuccesful == false) {
      OPENTREP_LOG_ERROR ("Parsing of POR input string: '" << _string
                          << "' failed");
      throw PorFileParsingException ("Parsing of POR input string: '"
                                     + _string + "' failed");
    }
    
    if  (fwd_start != fwd_end) {
      OPENTREP_LOG_ERROR ("Parsing of POR input string: '" << _string
                          << "' failed");
      throw PorFileParsingException ("Parsing of POR input file: '"
                                     + _string + "' failed");
    }
    
    //
    if (hasParsingBeenSuccesful == true && fwd_start == fwd_end) {
      // DEBUG
      /*
      OPENTREP_LOG_DEBUG ("Parsing of POR input string: '" << _string
                          << "' succeeded");
      */
    }

    return _location;
  }


  /////////////////////////////////////////////////////////////////////////
  //
  //  Entry class for the file parser
  //
  /////////////////////////////////////////////////////////////////////////
    
  // //////////////////////////////////////////////////////////////////////
  PORFileParser::PORFileParser (const PORFilePath_T& iFilename)
    : _filename (iFilename) {
    init();
  }

  // //////////////////////////////////////////////////////////////////////
  void PORFileParser::init() {
  }
    
  // //////////////////////////////////////////////////////////////////////
  void PORFileParser::generateLocations() {
      
    OPENTREP_LOG_DEBUG ("Parsing por input file: " << _filename);

    // File to be parsed
    const char* lFilenameStr = _filename.c_str();
    std::ifstream fileToBeParsed (lFilenameStr, std::ios_base::in);

    // Check if the filename exist and can be open
    if (fileToBeParsed.is_open() == false) {
      OPENTREP_LOG_ERROR ("The por file " << _filename << " can not be open."
                          << std::endl);

      throw FileNotFoundException ("The file " + _filename
                                   + " does not exist or can not be read");
    }
    
    // Create an input iterator
    OPENTREP::base_iterator_t iStr (fileToBeParsed);

    // Convert input iterator to an iterator usable by spirit parser  
    OPENTREP::iterator_t start (boost::spirit::make_default_multi_pass (iStr));
    OPENTREP::iterator_t end;

    // Initialise the parser (grammar) with the helper/staging structure.
    PorParserHelper::LocationParser<OPENTREP::iterator_t> lPORParser (_location);
      
    // Launch the parsing of the file and, thanks to the doEndPor
    // call-back structure, the building of the whole BomRoot BOM
    const bool hasParsingBeenSuccesful = 
       bsq::phrase_parse (start, end, lPORParser, bsu::blank);
      
    if (hasParsingBeenSuccesful == false) {
      OPENTREP_LOG_ERROR ("Parsing of por input file: " << _filename
                          << " failed");
      throw PorFileParsingException ("Parsing of por input file: "
                                     + _filename + " failed");
    }
    
    if  (start != end) {
      OPENTREP_LOG_ERROR ("Parsing of por input file: " << _filename
                          << " failed");
      throw PorFileParsingException ("Parsing of por input file: "
                                     + _filename + " failed");
    }
    
    if (hasParsingBeenSuccesful == true && start == end) {
      OPENTREP_LOG_DEBUG ("Parsing of por input file: " << _filename
                          << " succeeded");
    } 
  }

}
