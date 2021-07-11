// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// STL
#include <cassert>
#include <vector>
#include <fstream>
// Boost
#include <boost/lexical_cast.hpp>
//#define BOOST_SPIRIT_X3_DEBUG
#define BOOST_SPIRIT_X3_UNICODE
// OpenTREP
#include <opentrep/basic/BasParserTypes.hpp>
#include <opentrep/bom/PORParserHelper.hpp>
#include <opentrep/service/Logger.hpp>

namespace OPENTREP {

  /** Namespaces */
  namespace bs = boost::spirit;
  namespace x3 = boost::spirit::x3;
  namespace bsa = boost::spirit::x3::ascii;
  namespace bsu = boost::spirit::x3::unicode;
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
    template <typename Context>
    void storeIataCode::operator() (Context const& iCtx) const {

      const std::vector<uchar_t>& iChar = x3::_where(iCtx);
      const std::string lIataCodeStr (iChar.begin(), iChar.end());
      const OPENTREP::IATACode_T lIataCode (lIataCodeStr);
      _location.setIataCode (lIataCode);

      // Reset the values
      //_location.resetMatrix();
      //_location._itAltNameShortList.clear();

      // DEBUG
      //OPENTREP_LOG_DEBUG ("IATA code: " << _location.getIataCode());
    }

    // //////////////////////////////////////////////////////////////////
    storeIcaoCode::storeIcaoCode (Location& ioLocation)
      : ParserSemanticAction (ioLocation) {
    }
    
    // //////////////////////////////////////////////////////////////////
    template <typename Context>
    void storeIcaoCode::operator() (Context const& iCtx) const {

      const std::vector<uchar_t>& iChar = x3::_where(iCtx);
      const std::string lIcaoCodeStr (iChar.begin(), iChar.end());
      const OPENTREP::ICAOCode_T lIcaoCode (lIcaoCodeStr);
      _location.setIcaoCode (lIcaoCode);

      // DEBUG
      //OPENTREP_LOG_DEBUG ("ICAO code: " << _location.getIcaoCode());
    }

    // //////////////////////////////////////////////////////////////////
    storeFaaCode::storeFaaCode (Location& ioLocation)
      : ParserSemanticAction (ioLocation) {
    }
    
    // //////////////////////////////////////////////////////////////////
    template <typename Context>
    void storeFaaCode::operator() (Context const& iCtx) const {

      const std::vector<uchar_t>& iChar = x3::_where(iCtx);
      const std::string lFaaCodeStr (iChar.begin(), iChar.end());
      const OPENTREP::FAACode_T lFaaCode (lFaaCodeStr);
      _location.setFaaCode (lFaaCode);

      // DEBUG
      //OPENTREP_LOG_DEBUG ("FAA code: " << _location.getFaaCode());
    }

    // //////////////////////////////////////////////////////////////////
    storeUNLOCode::storeUNLOCode (Location& ioLocation)
      : ParserSemanticAction (ioLocation) {
    }
    
    // //////////////////////////////////////////////////////////////////
    template <typename Context>
    void storeUNLOCode::operator() (Context const& iCtx) const {

      const std::vector<uchar_t>& iChar = x3::_where(iCtx);
      const std::string lUNLOCodeStr (iChar.begin(), iChar.end());
      const OPENTREP::UNLOCode_T lUNLOCode (lUNLOCodeStr);
      _location.addUNLOCode (lUNLOCode);

      // DEBUG
      //OPENTREP_LOG_DEBUG ("UN/LOCODE codes: " << _location.describeUNLOCodeList());
    }

    // //////////////////////////////////////////////////////////////////
    storeUICCode::storeUICCode (Location& ioLocation)
      : ParserSemanticAction (ioLocation) {
    }
    
    // //////////////////////////////////////////////////////////////////
    template <typename Context>
    void storeUICCode::operator() (Context const& iCtx) const {
      
      const unsigned int& iUICCode = x3::_val(iCtx);
      _location.addUICCode (iUICCode);

      // DEBUG
      //OPENTREP_LOG_DEBUG ("UIC codes: " << _location.describeUICCodeList());
    }

    // //////////////////////////////////////////////////////////////////
    storeGeonamesID::storeGeonamesID (Location& ioLocation)
      : ParserSemanticAction (ioLocation) {
    }
    
    // //////////////////////////////////////////////////////////////////
    template <typename Context>
    void storeGeonamesID::operator() (Context const& iCtx) const {
      
      const unsigned int& iPorId = x3::_val(iCtx);
      _location.setGeonamesID (iPorId);
      
      // DEBUG
      //OPENTREP_LOG_DEBUG ("POR ID: " << _location.getPorID());
    }
    
    // //////////////////////////////////////////////////////////////////
    storeEnvelopeID::storeEnvelopeID (Location& ioLocation)
      : ParserSemanticAction (ioLocation) {
    }
    
    // //////////////////////////////////////////////////////////////////
    template <typename Context>
    void storeEnvelopeID::operator() (Context const& iCtx) const {
      
      const unsigned int& iEnvelopeID = x3::_val(iCtx);
      _location.setEnvelopeID (iEnvelopeID);
      
      // DEBUG
      //OPENTREP_LOG_DEBUG ("Envelope ID: " << _location.getEnvelopeID());
    }

    // //////////////////////////////////////////////////////////////////
    storeIsGeonames::storeIsGeonames (Location& ioLocation)
      : ParserSemanticAction (ioLocation) {
    }
    
    // //////////////////////////////////////////////////////////////////
    template <typename Context>
    void storeIsGeonames::operator() (Context const& iCtx) const {
      
      const std::vector<uchar_t>& iChar = x3::_where(iCtx);
      const std::string isGeonamesStr (iChar.begin(), iChar.end());
      if (isGeonamesStr == "Y") {
        _location.setIsGeonames (true);
      } else {
        _location.setIsGeonames (false);
      }
      
      // DEBUG
      //OPENTREP_LOG_DEBUG ("IsGeonames: " << _location.isGeonames());
    }
    
    // //////////////////////////////////////////////////////////////////
    storeCommonName::storeCommonName (Location& ioLocation)
      : ParserSemanticAction (ioLocation) {
    }
    
    // //////////////////////////////////////////////////////////////////
    template <typename Context>
    void storeCommonName::operator() (Context const& iCtx) const {

      const std::vector<uchar_t>& iChar = x3::_where(iCtx);
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
    template <typename Context>
    void storeAsciiName::operator() (Context const& iCtx) const {

      const std::vector<uchar_t>& iChar = x3::_where(iCtx);
      const std::string lAsciiNameStr (iChar.begin(), iChar.end());
      const OPENTREP::ASCIIName_T lAsciiName (lAsciiNameStr);
      _location.setAsciiName (lAsciiName);

      // DEBUG
       //OPENTREP_LOG_DEBUG ("ASCII name: " << _location.getAsciiName());
    }

    // //////////////////////////////////////////////////////////////////
    storeCityDetailList::storeCityDetailList (Location& ioLocation)
      : ParserSemanticAction (ioLocation) {
    }
    
    // //////////////////////////////////////////////////////////////////
    template <typename Context>
    void storeCityDetailList::operator() (Context const& iCtx) const {
      _location.consolidateCityDetailsList();

      // DEBUG
       //OPENTREP_LOG_DEBUG ("List of served cities: " << _location.describeCityDetailsList());
    }
    
    // //////////////////////////////////////////////////////////////////
    storeAltNameShort::storeAltNameShort (Location& ioLocation)
      : ParserSemanticAction (ioLocation) {
    }
    
    // //////////////////////////////////////////////////////////////////
    template <typename Context>
    void storeAltNameShort::operator() (Context const& iCtx) const {

      const std::vector<uchar_t>& iChar = x3::_where(iCtx);
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
    template <typename Context>
    void storeAltNameShortListString::operator() (Context const& iCtx) const {

      _location.consolidateAltNameShortListString();

      // DEBUG
       //OPENTREP_LOG_DEBUG ("Alternative name short list: " << _location.getAltNameShortListString());
    }

    // //////////////////////////////////////////////////////////////////
    storeTvlPORCode::storeTvlPORCode (Location& ioLocation)
      : ParserSemanticAction (ioLocation) {
    }
    
    // //////////////////////////////////////////////////////////////////
    template <typename Context>
    void storeTvlPORCode::operator() (Context const& iCtx) const {

      const std::vector<uchar_t>& iChar = x3::_where(iCtx);
      const std::string lTvlPORCodeStr (iChar.begin(), iChar.end());
      const OPENTREP::IATACode_T lTvlPORCode (lTvlPORCodeStr);
      _location._itTvlPORList.push_back (lTvlPORCode);

      // DEBUG
       //OPENTREP_LOG_DEBUG ("Travel-related IATA code: " << lTvlPORCodeStr);
    }

    // //////////////////////////////////////////////////////////////////
    storeTvlPORListString::storeTvlPORListString (Location& ioLocation)
      : ParserSemanticAction (ioLocation) {
    }
    
    // //////////////////////////////////////////////////////////////////
    template <typename Context>
    void storeTvlPORListString::operator() (Context const& iCtx) const {

      _location.consolidateTvlPORListString();

      // DEBUG
       //OPENTREP_LOG_DEBUG ("Travel-related POR list: " << _location.getTvlPORListString());
    }

    // //////////////////////////////////////////////////////////////////
    storeLatitude::storeLatitude (Location& ioLocation)
      : ParserSemanticAction (ioLocation) {
    }
    
    // //////////////////////////////////////////////////////////////////
    template <typename Context>
    void storeLatitude::operator() (Context const& iCtx) const {

      const double& iLatitude = x3::_val(iCtx);
      _location.setLatitude (iLatitude);

      // DEBUG
      //OPENTREP_LOG_DEBUG ("Latitude: " << _location.getLatitude());
    }

    // //////////////////////////////////////////////////////////////////
    storeLongitude::storeLongitude (Location& ioLocation)
      : ParserSemanticAction (ioLocation) {
    }
    
    // //////////////////////////////////////////////////////////////////
    template <typename Context>
    void storeLongitude::operator() (Context const& iCtx) const {

      const double& iLongitude = x3::_val(iCtx);
      _location.setLongitude (iLongitude);

      // DEBUG
      //OPENTREP_LOG_DEBUG ("Longitude: " << _location.getLongitude());
    }

    // //////////////////////////////////////////////////////////////////
    storeFeatureClass::storeFeatureClass (Location& ioLocation)
      : ParserSemanticAction (ioLocation) {
    }

    // //////////////////////////////////////////////////////////////////
    template <typename Context>
    void storeFeatureClass::operator() (Context const& iCtx) const {

      const std::vector<uchar_t>& iChar = x3::_where(iCtx);
      const std::string lFeatClassStr (iChar.begin(), iChar.end());
      const OPENTREP::FeatureClass_T lFeatClass (lFeatClassStr);
      _location.setFeatureClass (lFeatClass);

      // DEBUG
      //OPENTREP_LOG_DEBUG ("Feature class: " << _location.getFeatureClass());
    }

    // //////////////////////////////////////////////////////////////////
    storeFeatureCode::storeFeatureCode (Location& ioLocation)
      : ParserSemanticAction (ioLocation) {
    }

    // //////////////////////////////////////////////////////////////////
    template <typename Context>
    void storeFeatureCode::operator() (Context const& iCtx) const {

      const std::vector<uchar_t>& iChar = x3::_where(iCtx);
      const std::string lFeatCodeStr (iChar.begin(), iChar.end());
      const OPENTREP::FeatureClass_T lFeatCode (lFeatCodeStr);
      _location.setFeatureCode (lFeatCode);

      // DEBUG
      //OPENTREP_LOG_DEBUG ("Feature code: " << _location.getFeatureCode());
    }

    // //////////////////////////////////////////////////////////////////
    storePageRank::storePageRank (Location& ioLocation)
      : ParserSemanticAction (ioLocation) {
    }
    
    // //////////////////////////////////////////////////////////////////
    template <typename Context>
    void storePageRank::operator() (Context const& iCtx) const {

      const double& iPageRank = x3::_val(iCtx);
      _location.setPageRank (100.0 * iPageRank);

      // DEBUG
      //OPENTREP_LOG_DEBUG ("PageRank: " << _location.getPageRank());
    }

    // //////////////////////////////////////////////////////////////////
    storeDateYear::storeDateYear (Location& ioLocation)
      : ParserSemanticAction (ioLocation) {
    }
    
    // //////////////////////////////////////////////////////////////////
    template <typename Context>
    void storeDateYear::operator() (Context const& iCtx) const {

      const unsigned int& iDateElement = x3::_val(iCtx);
      _location._itYear = iDateElement;

      // DEBUG
      //OPENTREP_LOG_DEBUG ("Year: " << _location._itYear);
    }

    // //////////////////////////////////////////////////////////////////
    storeDateMonth::storeDateMonth (Location& ioLocation)
      : ParserSemanticAction (ioLocation) {
    }
    
    // //////////////////////////////////////////////////////////////////
    template <typename Context>
    void storeDateMonth::operator() (Context const& iCtx) const {

      const unsigned int& iDateElement = x3::_val(iCtx);
      _location._itMonth = iDateElement;

      // DEBUG
      //OPENTREP_LOG_DEBUG ("Year: " << _location._itMonth);
    }

    // //////////////////////////////////////////////////////////////////
    storeDateDay::storeDateDay (Location& ioLocation)
      : ParserSemanticAction (ioLocation) {
    }
    
    // //////////////////////////////////////////////////////////////////
    template <typename Context>
    void storeDateDay::operator() (Context const& iCtx) const {

      const unsigned int& iDateElement = x3::_val(iCtx);
      _location._itMonth = iDateElement;

      // DEBUG
      //OPENTREP_LOG_DEBUG ("Year: " << _location._itDay);
    }

    // //////////////////////////////////////////////////////////////////
    storeDateFrom::storeDateFrom (Location& ioLocation)
      : ParserSemanticAction (ioLocation) {
    }
    
    // //////////////////////////////////////////////////////////////////
    template <typename Context>
    void storeDateFrom::operator() (Context const& iCtx) const {

      const OPENTREP::Date_T& lDateFrom = _location.calculateDate();
      _location.setDateFrom (lDateFrom);

      // DEBUG
      //OPENTREP_LOG_DEBUG ("Date from: " << _location.getDateFrom());
    }

    // //////////////////////////////////////////////////////////////////
    storeDateEnd::storeDateEnd (Location& ioLocation)
      : ParserSemanticAction (ioLocation) {
    }
    
    // //////////////////////////////////////////////////////////////////
    template <typename Context>
    void storeDateEnd::operator() (Context const& iCtx) const {

      const OPENTREP::Date_T& lDateEnd = _location.calculateDate();
      _location.setDateEnd (lDateEnd);

      // DEBUG
      //OPENTREP_LOG_DEBUG ("Date end: " << _location.getDateEnd());
    }

    // //////////////////////////////////////////////////////////////////
    storeComments::storeComments (Location& ioLocation)
      : ParserSemanticAction (ioLocation) {
    }

    // //////////////////////////////////////////////////////////////////
    template <typename Context>
    void storeComments::operator() (Context const& iCtx) const {

      /*
      const std::vector<uchar_t>& iChar = x3::_where(iCtx);
      const std::string lCommentsStr (iChar.begin(), iChar.end());
      const CountryCode_T lComments (lCommentsStr);
      _location.setComments (lComments);
      */

      // DEBUG
      //OPENTREP_LOG_DEBUG ("Comments: " << _location.getComments());
    }

    // //////////////////////////////////////////////////////////////////
    storeCountryCode::storeCountryCode (Location& ioLocation)
      : ParserSemanticAction (ioLocation) {
    }

    // //////////////////////////////////////////////////////////////////
    template <typename Context>
    void storeCountryCode::operator() (Context const& iCtx) const {

      const std::vector<uchar_t>& iChar = x3::_where(iCtx);
      const std::string lCountryCodeStr (iChar.begin(), iChar.end());
      const OPENTREP::CountryCode_T lCountryCode (lCountryCodeStr);
      _location.setCountryCode (lCountryCode);

      // DEBUG
      //OPENTREP_LOG_DEBUG ("Country code: " << _location.getCountryCode());
    }

    // //////////////////////////////////////////////////////////////////
    storeAltCountryCode::storeAltCountryCode (Location& ioLocation)
      : ParserSemanticAction (ioLocation) {
    }

    // //////////////////////////////////////////////////////////////////
    template <typename Context>
    void storeAltCountryCode::operator() (Context const& iCtx) const {

      const std::vector<uchar_t>& iChar = x3::_where(iCtx);
      const std::string lCountryCodeStr (iChar.begin(), iChar.end());
      const CountryCode_T lCountryCode (lCountryCodeStr);
      _location.setAltCountryCode (lCountryCode);

      // DEBUG
      //OPENTREP_LOG_DEBUG ("Alt country code: " << _location.getAltCountryCode());
    }

    // //////////////////////////////////////////////////////////////////
    storeCountryName::storeCountryName (Location& ioLocation)
      : ParserSemanticAction (ioLocation) {
    }

    // //////////////////////////////////////////////////////////////////
    template <typename Context>
    void storeCountryName::operator() (Context const& iCtx) const {

      const std::vector<uchar_t>& iChar = x3::_where(iCtx);
      const std::string lCountryNameStr (iChar.begin(), iChar.end());
      const CountryName_T lCountryName (lCountryNameStr);
      _location.setCountryName (lCountryName);

      // DEBUG
      //OPENTREP_LOG_DEBUG ("Country name: " << _location.getCountryName());
    }

    // //////////////////////////////////////////////////////////////////
    storeContinentName::storeContinentName (Location& ioLocation)
      : ParserSemanticAction (ioLocation) {
    }

    // //////////////////////////////////////////////////////////////////
    template <typename Context>
    void storeContinentName::operator() (Context const& iCtx) const {

      const std::vector<uchar_t>& iChar = x3::_where(iCtx);
      const std::string lContinentNameStr (iChar.begin(), iChar.end());
      const ContinentName_T lContinentName (lContinentNameStr);
      _location.setContinentName (lContinentName);

      // DEBUG
      //OPENTREP_LOG_DEBUG ("Continent name: " << _location.getContinentName());
    }

    // //////////////////////////////////////////////////////////////////
    storeAdm1Code::storeAdm1Code (Location& ioLocation)
      : ParserSemanticAction (ioLocation) {
    }

    // //////////////////////////////////////////////////////////////////
    template <typename Context>
    void storeAdm1Code::operator() (Context const& iCtx) const {

      const std::vector<uchar_t>& iChar = x3::_where(iCtx);
      const std::string lAdmCodeStr (iChar.begin(), iChar.end());
      const Admin1Code_T lAdmCode (lAdmCodeStr);
      _location.setAdmin1Code (lAdmCode);

      // DEBUG
      //OPENTREP_LOG_DEBUG ("Adm1 code: " << _location.getAdmin1Code());
    }

    // //////////////////////////////////////////////////////////////////
    storeAdm1UtfName::storeAdm1UtfName (Location& ioLocation)
      : ParserSemanticAction (ioLocation) {
    }

    // //////////////////////////////////////////////////////////////////
    template <typename Context>
    void storeAdm1UtfName::operator() (Context const& iCtx) const {

      const std::vector<uchar_t>& iChar = x3::_where(iCtx);
      const std::string lAdmNameStr (iChar.begin(), iChar.end());
      const Admin1UTFName_T lAdmName (lAdmNameStr);
      _location.setAdmin1UtfName (lAdmName);

      // DEBUG
      //OPENTREP_LOG_DEBUG ("Adm1 UTF8 name: " << _location.getAdmin1UtfName());
    }

    // //////////////////////////////////////////////////////////////////
    storeAdm1AsciiName::storeAdm1AsciiName (Location& ioLocation)
      : ParserSemanticAction (ioLocation) {
    }

    // //////////////////////////////////////////////////////////////////
    template <typename Context>
    void storeAdm1AsciiName::operator() (Context const& iCtx) const {

      const std::vector<uchar_t>& iChar = x3::_where(iCtx);
      const std::string lAdmNameStr (iChar.begin(), iChar.end());
      const Admin1ASCIIName_T lAdmName (lAdmNameStr);
      _location.setAdmin1AsciiName (lAdmName);

      // DEBUG
      //OPENTREP_LOG_DEBUG("Adm1 ASCII name: "<< _location.getAdmin1AsciiName());
    }

    // //////////////////////////////////////////////////////////////////
    storeAdm2Code::storeAdm2Code (Location& ioLocation)
      : ParserSemanticAction (ioLocation) {
    }

    // //////////////////////////////////////////////////////////////////
    template <typename Context>
    void storeAdm2Code::operator() (Context const& iCtx) const {

      const std::vector<uchar_t>& iChar = x3::_where(iCtx);
      const std::string lAdmCodeStr (iChar.begin(), iChar.end());
      const Admin2Code_T lAdmCode (lAdmCodeStr);
      _location.setAdmin2Code (lAdmCode);

      // DEBUG
      //OPENTREP_LOG_DEBUG ("Adm2 code: " << _location.getAdmin2Code());
    }

    // //////////////////////////////////////////////////////////////////
    storeAdm2UtfName::storeAdm2UtfName (Location& ioLocation)
      : ParserSemanticAction (ioLocation) {
    }

    // //////////////////////////////////////////////////////////////////
    template <typename Context>
    void storeAdm2UtfName::operator() (Context const& iCtx) const {

      const std::vector<uchar_t>& iChar = x3::_where(iCtx);
      const std::string lAdmNameStr (iChar.begin(), iChar.end());
      const Admin2UTFName_T lAdmName (lAdmNameStr);
      _location.setAdmin2UtfName (lAdmName);

      // DEBUG
      //OPENTREP_LOG_DEBUG ("Adm2 UTF8 name: " << _location.getAdmin2UtfName());
    }

    // //////////////////////////////////////////////////////////////////
    storeAdm2AsciiName::storeAdm2AsciiName (Location& ioLocation)
      : ParserSemanticAction (ioLocation) {
    }

    // //////////////////////////////////////////////////////////////////
    template <typename Context>
    void storeAdm2AsciiName::operator() (Context const& iCtx) const {

      const std::vector<uchar_t>& iChar = x3::_where(iCtx);
      const std::string lAdmNameStr (iChar.begin(), iChar.end());
      const Admin2ASCIIName_T lAdmName (lAdmNameStr);
      _location.setAdmin2AsciiName (lAdmName);

      // DEBUG
      //OPENTREP_LOG_DEBUG("Adm2 ASCII name: "<< _location.getAdmin2AsciiName());
    }

    // //////////////////////////////////////////////////////////////////
    storeAdm3Code::storeAdm3Code (Location& ioLocation)
      : ParserSemanticAction (ioLocation) {
    }

    // //////////////////////////////////////////////////////////////////
    template <typename Context>
    void storeAdm3Code::operator() (Context const& iCtx) const {

      const std::vector<uchar_t>& iChar = x3::_where(iCtx);
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
    template <typename Context>
    void storeAdm4Code::operator() (Context const& iCtx) const {

      const std::vector<uchar_t>& iChar = x3::_where(iCtx);
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
    template <typename Context>
    void storePopulation::operator() (Context const& iCtx) const {

      const unsigned int& iPopulation = x3::_val(iCtx);
      _location.setPopulation (iPopulation);

      // DEBUG
      //OPENTREP_LOG_DEBUG ("Population: " << _location.getPopulation());
    }
    
    // //////////////////////////////////////////////////////////////////
    storeElevation::storeElevation (Location& ioLocation)
      : ParserSemanticAction (ioLocation) {
    }
    
    // //////////////////////////////////////////////////////////////////
    template <typename Context>
    void storeElevation::operator() (Context const& iCtx) const {

      const int& iElevation = x3::_val(iCtx);
      _location.setElevation (iElevation);

      // DEBUG
      //OPENTREP_LOG_DEBUG ("Elevation: " << _location.getElevation());
    }
    
    // //////////////////////////////////////////////////////////////////
    storeGTopo30::storeGTopo30 (Location& ioLocation)
      : ParserSemanticAction (ioLocation) {
    }
    
    // //////////////////////////////////////////////////////////////////
    template <typename Context>
    void storeGTopo30::operator() (Context const& iCtx) const {

      const int& iGTopo30 = x3::_val(iCtx);
      _location.setGTopo30 (iGTopo30);

      // DEBUG
      //OPENTREP_LOG_DEBUG ("GTopo30: " << _location.getGTopo30());
    }

    // //////////////////////////////////////////////////////////////////
    storeTimeZone::storeTimeZone (Location& ioLocation)
      : ParserSemanticAction (ioLocation) {
    }

    // //////////////////////////////////////////////////////////////////
    template <typename Context>
    void storeTimeZone::operator() (Context const& iCtx) const {

      const std::vector<uchar_t>& iChar = x3::_where(iCtx);
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
    template <typename Context>
    void storeGMTOffset::operator() (Context const& iCtx) const {

      const float& iOffset = x3::_val(iCtx);
      _location.setGMTOffset (iOffset);

      // DEBUG
      //OPENTREP_LOG_DEBUG ("GMT offset: " << _location.getGMTOffset());
    }
    
    // //////////////////////////////////////////////////////////////////
    storeDSTOffset::storeDSTOffset (Location& ioLocation)
      : ParserSemanticAction (ioLocation) {
    }
    
    // //////////////////////////////////////////////////////////////////
    template <typename Context>
    void storeDSTOffset::operator() (Context const& iCtx) const {

      const float& iOffset = x3::_val(iCtx);
      _location.setDSTOffset (iOffset);

      // DEBUG
      //OPENTREP_LOG_DEBUG ("DST offset: " << _location.getDSTOffset());
    }
    
    // //////////////////////////////////////////////////////////////////
    storeRawOffset::storeRawOffset (Location& ioLocation)
      : ParserSemanticAction (ioLocation) {
    }
    
    // //////////////////////////////////////////////////////////////////
    template <typename Context>
    void storeRawOffset::operator() (Context const& iCtx) const {

      const float& iOffset = x3::_val(iCtx);
      _location.setRawOffset (iOffset);

      // DEBUG
      //OPENTREP_LOG_DEBUG ("Raw offset: " << _location.getRawOffset());
    }
    
    // //////////////////////////////////////////////////////////////////
    storeModDate::storeModDate (Location& ioLocation)
      : ParserSemanticAction (ioLocation) {
    }
    
    // //////////////////////////////////////////////////////////////////
    template <typename Context>
    void storeModDate::operator() (Context const& iCtx) const {

      const OPENTREP::Date_T& lModDate = _location.calculateDate();
      _location.setModificationDate (lModDate);

      // DEBUG
      //OPENTREP_LOG_DEBUG ("Modification date: " << _location.getModificationDate());
    }

    // //////////////////////////////////////////////////////////////////
    storeCityCode::storeCityCode (Location& ioLocation)
      : ParserSemanticAction (ioLocation) {
    }
    
    // //////////////////////////////////////////////////////////////////
    template <typename Context>
    void storeCityCode::operator() (Context const& iCtx) const {

      const std::vector<uchar_t>& iChar = x3::_where(iCtx);
      const std::string lCityCodeStr (iChar.begin(), iChar.end());
      const OPENTREP::CityCode_T lCityCode (lCityCodeStr);
      _location._itCityIataCode = lCityCode;

      // DEBUG
       //OPENTREP_LOG_DEBUG ("City code: " << _location._itCityIataCode);
    }

    // //////////////////////////////////////////////////////////////////
    storeCityGeonamesID::storeCityGeonamesID (Location& ioLocation)
      : ParserSemanticAction (ioLocation) {
    }
    
    // //////////////////////////////////////////////////////////////////
    template <typename Context>
    void storeCityGeonamesID::operator() (Context const& iCtx) const {

      const unsigned int& iCtyId = x3::_val(iCtx);
      _location._itCityGeonamesID = iCtyId;

      // DEBUG
       //OPENTREP_LOG_DEBUG("City Geonames ID: " << _location._itCityGeonamesID);
    }

    // //////////////////////////////////////////////////////////////////
    storeCityUtfName::storeCityUtfName (Location& ioLocation)
      : ParserSemanticAction (ioLocation) {
    }
    
    // //////////////////////////////////////////////////////////////////
    template <typename Context>
    void storeCityUtfName::operator() (Context const& iCtx) const {

      const std::vector<uchar_t>& iChar = x3::_where(iCtx);
      const std::string lCityUtfNameStr (iChar.begin(), iChar.end());
      const OPENTREP::CityUTFName_T lCityUtfName (lCityUtfNameStr);
      _location._itCityUtfName = lCityUtfName;

      // DEBUG
       //OPENTREP_LOG_DEBUG ("City UTF8 name: " << _location._itCityUtfName);
    }

    // //////////////////////////////////////////////////////////////////
    storeCityAsciiName::storeCityAsciiName (Location& ioLocation)
      : ParserSemanticAction (ioLocation) {
    }
    
    // //////////////////////////////////////////////////////////////////
    template <typename Context>
    void storeCityAsciiName::operator() (Context const& iCtx) const {

      const std::vector<uchar_t>& iChar = x3::_where(iCtx);
      const std::string lCityAsciiNameStr (iChar.begin(), iChar.end());
      const OPENTREP::CityASCIIName_T lCityAsciiName (lCityAsciiNameStr);
      _location._itCityAsciiName = lCityAsciiName;

      // DEBUG
       //OPENTREP_LOG_DEBUG("City ASCII name: " << _location._itCityAsciiName);
    }

    // //////////////////////////////////////////////////////////////////
    storeCityCountryCode::storeCityCountryCode (Location& ioLocation)
      : ParserSemanticAction (ioLocation) {
    }

    // //////////////////////////////////////////////////////////////////
    template <typename Context>
    void storeCityCountryCode::operator() (Context const& iCtx) const {
      
      const std::vector<uchar_t>& iChar = x3::_where(iCtx);
      const std::string lCityCountryCodeStr (iChar.begin(), iChar.end());
      const OPENTREP::CountryCode_T lCityCountryCode (lCityCountryCodeStr);
      _location._itCityCountryCode = lCityCountryCode;

      // DEBUG
      //OPENTREP_LOG_DEBUG("City country code: "<< _location._itCityCountryCode);
    }

    // //////////////////////////////////////////////////////////////////
    storeCityStateCode::storeCityStateCode (Location& ioLocation)
      : ParserSemanticAction (ioLocation) {
    }
    
    // //////////////////////////////////////////////////////////////////
    template <typename Context>
    void storeCityStateCode::operator() (Context const& iCtx) const {

      const std::vector<uchar_t>& iChar = x3::_where(iCtx);
      const std::string lCityStateCodeStr (iChar.begin(), iChar.end());
      const OPENTREP::StateCode_T lCityStateCode (lCityStateCodeStr);
      _location._itCityStateCode = lCityStateCode;

      // DEBUG
       //OPENTREP_LOG_DEBUG ("City state code: " << _location._itCityStateCode);
    }
 
    // //////////////////////////////////////////////////////////////////
    storeStateCode::storeStateCode (Location& ioLocation)
      : ParserSemanticAction (ioLocation) {
    }
    
    // //////////////////////////////////////////////////////////////////
    template <typename Context>
    void storeStateCode::operator() (Context const& iCtx) const {

      const std::vector<uchar_t>& iChar = x3::_where(iCtx);
      const std::string lStateCodeStr (iChar.begin(), iChar.end());
      const OPENTREP::StateCode_T lStateCode (lStateCodeStr);
      _location.setStateCode (lStateCode);

      // DEBUG
       //OPENTREP_LOG_DEBUG ("State code: " << _location.getStateCode());
    }

    // //////////////////////////////////////////////////////////////////
    storeWAC::storeWAC (Location& ioLocation)
      : ParserSemanticAction (ioLocation) {
    }
    
    // //////////////////////////////////////////////////////////////////
    template <typename Context>
    void storeWAC::operator() (Context const& iCtx) const {

      const unsigned int& iWAC = x3::_val(iCtx);
      _location.setWAC (iWAC);

      // DEBUG
      //OPENTREP_LOG_DEBUG ("WAC: " << _location.getWAC());
    }
    
    // //////////////////////////////////////////////////////////////////
    storeWACName::storeWACName (Location& ioLocation)
      : ParserSemanticAction (ioLocation) {
    }
    
    // //////////////////////////////////////////////////////////////////
    template <typename Context>
    void storeWACName::operator() (Context const& iCtx) const {

      const std::vector<uchar_t>& iChar = x3::_where(iCtx);
      const std::string lWACNameStr (iChar.begin(), iChar.end());
      const OPENTREP::WACName_T lWACName (lWACNameStr);
      _location.setWACName (lWACName);

      // DEBUG
      //OPENTREP_LOG_DEBUG ("WAC name: " << _location.getWACName());
    }
    
    // //////////////////////////////////////////////////////////////////
    storeCurrencyCode::storeCurrencyCode (Location& ioLocation)
      : ParserSemanticAction (ioLocation) {
    }
    
    // //////////////////////////////////////////////////////////////////
    template <typename Context>
    void storeCurrencyCode::operator() (Context const& iCtx) const {

      const std::vector<uchar_t>& iChar = x3::_where(iCtx);
      const std::string lCurrencyCodeStr (iChar.begin(), iChar.end());
      const OPENTREP::CurrencyCode_T lCurrencyCode (lCurrencyCodeStr);
      _location.setCurrencyCode (lCurrencyCode);

      // DEBUG
      //OPENTREP_LOG_DEBUG ("Currency code: " << _location.getCurrencyCode());
    }
    
    // //////////////////////////////////////////////////////////////////
    storePORType::storePORType (Location& ioLocation)
      : ParserSemanticAction (ioLocation) {
    }

    // //////////////////////////////////////////////////////////////////
    template <typename Context>
    void storePORType::operator() (Context const& iCtx) const {

      const std::vector<uchar_t>& iChar = x3::_where(iCtx);
      const std::string lIATATypeStr (iChar.begin(), iChar.end());
      const IATAType lIATAType (lIATATypeStr);
      _location.setIataType (lIATAType);

      // DEBUG
      //OPENTREP_LOG_DEBUG ("IATA type: " << _location.getIataType());
    }

    // //////////////////////////////////////////////////////////////////
    storeWikiLink::storeWikiLink (Location& ioLocation)
      : ParserSemanticAction (ioLocation) {
    }
    
    // //////////////////////////////////////////////////////////////////
    template <typename Context>
    void storeWikiLink::operator() (Context const& iCtx) const {

      const std::vector<uchar_t>& iChar = x3::_where(iCtx);
      const std::string lWikiLinkStr (iChar.begin(), iChar.end());
      const OPENTREP::WikiLink_T lWikiLink (lWikiLinkStr);
      _location.setWikiLink (lWikiLink);

      // DEBUG
       // OPENTREP_LOG_DEBUG ("Wiki link: " << _location.getWikiLink());
    }

    // //////////////////////////////////////////////////////////////////
    storeGeonameLatitude::storeGeonameLatitude (Location& ioLocation)
      : ParserSemanticAction (ioLocation) {
    }
    
    // //////////////////////////////////////////////////////////////////
    template <typename Context>
    void storeGeonameLatitude::operator() (Context const& iCtx) const {

      const double& iLatitude = x3::_val(iCtx);
      _location.setGeonameLatitude (iLatitude);

      // DEBUG
      //OPENTREP_LOG_DEBUG ("Geoname latitude: " << _location.getGeonameLatitude());
    }

    // //////////////////////////////////////////////////////////////////
    storeGeonameLongitude::storeGeonameLongitude (Location& ioLocation)
      : ParserSemanticAction (ioLocation) {
    }
    
    // //////////////////////////////////////////////////////////////////
    template <typename Context>
    void storeGeonameLongitude::operator() (Context const& iCtx) const {

      const double& iLongitude = x3::_val(iCtx);
      _location.setGeonameLongitude (iLongitude);

      // DEBUG
      //OPENTREP_LOG_DEBUG ("Geoname longitude: " << _location.getGeonameLongitude());
    }

    // //////////////////////////////////////////////////////////////////
    storeAltLangCodeFull::storeAltLangCodeFull (Location& ioLocation)
      : ParserSemanticAction (ioLocation) {
    }
    
    // //////////////////////////////////////////////////////////////////
    template <typename Context>
    void storeAltLangCodeFull::operator() (Context const& iCtx) const {

      const std::vector<boost::uint32_t>& iCode = x3::_where(iCtx);
      const std::string lAltLangCodeStr (iCode.begin(), iCode.end());
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
    template <typename Context>
    void storeAltLangCode2Char::operator() (Context const& iCtx) const {

      const std::vector<boost::uint32_t>& iCode = x3::_where(iCtx);
      const std::string lAltLangCodeStr (iCode.begin(), iCode.end());
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
    template <typename Context>
    void storeAltLangCodeExt::operator() (Context const& iCtx) const {

      const std::vector<boost::uint32_t>& iCode = x3::_where(iCtx);
      const std::string lAltLangCodeStr (iCode.begin(), iCode.end());
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
    template <typename Context>
    void storeAltLangCodeHist::operator() (Context const& iCtx) const {

      const std::vector<boost::uint32_t>& iCode = x3::_where(iCtx);
      const std::string lAltLangCodeStr (iCode.begin(), iCode.end());
      std::ostringstream oStr;
      oStr << _location._itLangCode2Char << "_" << lAltLangCodeStr;
      _location._itLangCodeHist = oStr.str();

      // DEBUG
       //OPENTREP_LOG_DEBUG ("Alt lang 2-char code: " << _location._itLangCodeHist);
    }

    // //////////////////////////////////////////////////////////////////
    storeAltName::storeAltName (Location& ioLocation)
      : ParserSemanticAction (ioLocation) {
    }
    
    // //////////////////////////////////////////////////////////////////
    template <typename Context>
    void storeAltName::operator() (Context const& iCtx) const {

      const std::vector<boost::uint32_t>& iCode = x3::_where(iCtx);
      const std::string lAltNameStr (iCode.begin(), iCode.end());
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
    template <typename Context>
    void doEndPor::operator() (Context const& iCtx) const {

      // DEBUG
      //OPENTREP_LOG_DEBUG ("Do End. Location structure: " << _location);
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

    /////////////////////////////////////////////////////////////////////////
    //
    //  (Boost Spirit) Grammar Definition
    //
    /////////////////////////////////////////////////////////////////////////
    /**

       -- Geonames-related part:
       -- ----------------------
       -- iata_code         : IATA code; varchar(3). See also:
       --                     http://www.iata.org/ps/publications/Pages/code-search.aspx
       -- icao_code         : ICAO code; varchar(4)
       -- faa_code          : FAA code; varchar(4)
       -- geoname_id        : Integer ID of record in geonames database
       -- name              : Name of geographical point;
       --                     (UTF8) varchar(200)
       -- asciiname         : Name of geographical point in plain ASCII
       --                     characters;
       --                     (ASCII) varchar(200)
       -- alternate-names   : Alternate names, comma separated
       --                     varchar(5000)
       -- latitude          : Latitude in decimal degrees (WGS84)
       -- longitude         : Longitude in decimal degrees (WGS84)
       -- feature class     : See http://www.geonames.org/export/codes.html
       --                     char(1)
       -- feature code      : See http://www.geonames.org/export/codes.html
       --                     varchar(10)
       -- country code      : ISO-3166 2-letter country code, 2 characters
       -- cc2               : Alternate country codes, comma separated,
       --                     ISO-3166 2-letter country code, 60 characters
       -- country name      : Name of the country, as found in the Geonames
       --                     countryInfo.txt data file
       --                     varchar(80)
       -- continent name    : The name of the related continent, as found in
       --                     the Geonames continentCodes.txt data file;
       --                     see also below the 'Continents' section
       --                     varchar(80)
       -- admin1 code       : FIPS code (subject to change to ISO code),
       --                     see exceptions below. See file admin1Codes.txt
       --                     for display names of this code;
       --                     varchar(20)
       -- admin1 UTF8 name  : UTF8 name for the first administrative division
       -- admin1 ASCII name : ASCII name for the first administrative division
       -- admin2 code       : Code for the second administrative division,
       --                     a county in the US. See file admin2Codes.txt;
       --                     varchar(80)
       -- admin2 UTF8 name  : UTF8 name for the second administrative division
       -- admin2 ASCII name : ASCII name for the second administrative division
       -- admin3 code       : Code for third level administrative division;
       --                     varchar(20)
       -- admin4 code       : Code for fourth level administrative division;
       --                     varchar(20)
       -- population        : bigint (8 byte int) 
       -- elevation         : In meters, integer
       -- dem               : Digital elevation model, srtm3 or gtopo30, average
       --                     elevation of 3''x3'' (ca 90mx90m) or 30''x30''
       --                     (ca 900mx900m) area in meters, integer.
       --                     srtm processed by cgiar/ciat.
       -- timezone          : The time-zone ID (see file timeZone.txt)
       -- gmt offset        : GMT offset on 1st of January
       -- dst offset        : DST offset to GMT on 1st of July (of the
       --                     current year)
       -- raw offset        : Raw Offset without DST
       -- modification date : Date of last modification in yyyy-MM-dd format
       --
       --
       -- OPTD-related part:
       -- ------------------
       --
       -- is_geonames       : Whether that POR is known by Geonames; varchar(1)
       --                     varchar(1)
       -- envelope_id       : ID of the envelope. Empty when valid, i.e., for the
       --                     current envelope/state. Incrementing ID when no
       --                     longer valid; the validity date range then allows
       --                     to derive when that POR was valid.
       -- city_code_list    : The (list of) IATA code(s) of the related (list of)
       --                     cities.
       --                     varchar(100)
       -- city_name_list    : UTF-8 names of the served (list of) cities
       --                     varchar(500)
       -- city_detail_list  : Details (Geonames ID, names) of the served
       --                     (list of) cities
       --                     varchar(500)
       -- tvl_por_list      : list of the IATA codes of the travel-related POR
       --                     serving the city.
       --                     varchar(100)
       -- state code        : The ISO code of the related state; varchar(3)
       -- location type     : A/APT airport; B/BUS bus/coach station;
       --                     C/CITY City;
       --                     G/GRD ground transport (this code is used for SK in
       --                     Sweden only); H/HELI Heliport;
       --                     O/OFF-PT off-line point, i.e. a city without
       --                     an airport;
       --                     R/RAIL railway Station;
       --                     S/ASSOC a location without its own IATA code,
       --                     but attached to an IATA location.
       -- wac               : The US DOT World Area Code (WAC)
       -- wac_name          : The US DOT world area name (of the country/state)
       -- ccy_code          : Currency code (of the country)
       -- unlc_list         : List of UN/LOCODE codes. varchar(100)
       -- uic_list          : List of UIC codes. varchar(100)
       --
       -- Continents:
       -- -----------
       -- AF Africa  6255146
       -- AS Asia    6255147
       -- EU Europe  6255148
       -- NA North America   6255149
       -- OC Oceania 6255151
       -- SA South America   6255150
       -- AN Antarctica      6255152
       --
       -- Samples:
       -- CDG^LFPG^^Y^6269554^^Paris Charles de Gaulle Airport^Paris Charles de Gaulle Airport^49.01278^2.55^S^AIRP^0.4212379291256919^^^^FR^^France^Europe^11^Île-de-France^Ile-de-France^95^Département du Val-d’Oise^Departement du Val-d'Oise^952^95527^0^119^106^Europe/Paris^1.0^2.0^1.0^2018-04-05^PAR^Paris^PAR|2988507|Paris|Paris^^IDF^A^http://en.wikipedia.org/wiki/Charles_de_Gaulle_Airport^es|París - Charles de Gaulle|p=|Roissy Charles de Gaulle|^427^France^FRCDG|^
       -- PAR^^^Y^2988507^^Paris^Paris^48.85341^2.3488^P^PPLC^0.6748395156936545^^^^FR^^France^Europe^11^Île-de-France^Ile-de-France^75^Paris^Paris^^^2138551^^42^Europe/Paris^1.0^2.0^1.0^2018-11-06^PAR^Paris^PAR|2988507|Paris|Paris^BVA,CDG,JDP,JPU,LBG,ORY,POX,TNF,VIY,XCR,XED,XEX,XGB,XHP,XJY,XPG,XTT^IDF^C^http://en.wikipedia.org/wiki/Paris^la|Lutetia Parisorum|=fr|Lutece|h=fr|Ville-Lumière|c=eo|Parizo|=es|París|ps=de|Paris|=en|Paris|p=af|Parys|=als|Paris|=fr|Paris|p^427^France^FRPAR|^
       -- HDQ^^^Y^2637052^^Stansted Mountfitchet^Stansted Mountfitchet^51.9^0.2^P^PPL^^^^^GB^^United Kingdom^Europe^ENG^England^England^E4^Essex^Essex^22UQ^22UQ046^5429^^83^Europe/London^0.0^1.0^0.0^2018-07-03^HDQ^Stansted Mountfitchet^HDQ|2637052|Stansted Mountfitchet|Stansted Mountfitchet^HDQ^ENG^O^http://en.wikipedia.org/wiki/Stansted_Mountfitchet^|Stansted|=en|Stansted Mountfitchet|p=en|Headquarters|=|Stansted Mountfitchet|^493^United Kingdom^GBP^^
       -- ^LFLG^^Y^6694498^^Grenoble-Le Versoud^Grenoble-Le Versoud^45.219^5.85^S^AIRF^^^^^FR^^France^Europe^84^Auvergne-Rhône-Alpes^Auvergne-Rhone-Alpes^38^Département de l'Isère^Departement de l'Isere^381^38538^0^^216^Europe/Paris^1.0^2.0^1.0^2016-02-18^^^^^ARA^A^http://en.wikipedia.org/wiki/Grenoble_%E2%80%93_Le_Versoud_Aerodrome^^427^France^EUR^^
       -- ^^^Y^8288215^^Gare de Grenoble^Gare de Grenoble^45.19149^5.71422^S^RSTN^^^^^FR^^France^Europe^84^Auvergne-Rhône-Alpes^Auvergne-Rhone-Alpes^38^Département de l'Isère^Departement de l'Isere^381^38185^0^^217^Europe/Paris^1.0^2.0^1.0^2018-10-13^^^^^ARA^R^http://en.wikipedia.org/wiki/Gare_de_Grenoble^fr|Gare de Grenoble|=en|Gare de Grenoble|=zh|格勒诺布尔站|=nl|Station Grenoble|=it|Stazione di Grenoble|=wkdt|Q2545346|^427^France^EUR^FRGNB|^87747006|
       --

       iata_code          varchar(3)
       icao_code          varchar(4)
       faa_code           varchar(4)
       is_geonames        varchar(1)
       geoname_id         int(11)
       envelope_id        int(11)
       name               varchar(200)
       asciiname          varchar(200)
       latitude           decimal(10,7)
       longitude          decimal(10,7)
       fclass             varchar(1)
       fcode              varchar(10)
       page_rank          decimal(15,12)
       date_from          date
       date_end         date
       comments           varchar(4000)
       country_code       varchar(2)
       cc2                varchar(60)
       country_name       varchar(200)
       continent_name     varchar(80)
       adm1_code          varchar(20)
       adm1_name_utf      varchar(400)
       adm1_name_ascii    varchar(400)
       adm2_code          varchar(80)
       adm2_name_utf      varchar(400)
       adm2_name_ascii    varchar(400)
       adm3_code          varchar(20)
       adm4_code          varchar(20)
       population         int(11)
       elevation          int(11)
       gtopo30            int(11)
       timezone           varchar(40)
       gmt_offset         decimal(3,1)
       dst_offset         decimal(3,1)
       raw_offset         decimal(3,1)
       moddate            date
       city_code_list     varchar(100)
       city_name_list     varchar(500)
       city_detail_list   varchar(500)
       tvl_por_list       varchar(100)
       state_code         varchar(3)
       location_type      varchar(4)
       wiki_link          varchar(200)
       alt_name_section   text
       wac                int(3)
       wac_name           varchar(200)
       ccy_code           varchar(3)
       unlc_list          varchar(100)
       uic_list           varchar(100)
       geoname_lat        decimal(10,7)
       geoname_lon        decimal(10,7)

       iata_code^icao_code^faa_code^is_geonames^geoname_id^envelope_id^
       name^asciiname^
       latitude^longitude^fclass^fcode^
       page_rank^date_from^date_end^comment^
       country_code^cc2^country_name^continent_name^
       adm1_code^adm1_name_utf^adm1_name_ascii^
       adm2_code^adm2_name_utf^adm2_name_ascii^
       adm3_code^adm4_code^
       population^elevation^gtopo30^
       timezone^gmt_offset^dst_offset^raw_offset^moddate^
       city_code_list^city_name_list^city_detail_list^tvl_por_list^
       state_code^location_type^wiki_link^
       alt_name_section^
       wac^wac_name^ccy_code^
       unlc_list^uic_list^
       geoname_lat^geoname_lon
    */ 

    /**
     * Grammar for the Por-Rule parser.
     */
    template <typename Iterator>	
    struct LocationParser {

      LocationParser (Location& ioPORRule) : _location(ioPORRule) {

        start = x3::eps
          >> *(header | por_rule);

        header = x3::lit("iata_code") >> +(x3::char_ - x3::eoi - x3::eol)
                                       >> (x3::eoi | x3::eol);

        por_rule = por_key
          >> '^' >> por_details
          >> '^' >> -alt_name_section
          >> '^' >> por_details_additional
          >> por_rule_end[doEndPor(_location)];

        por_rule_end = x3::eps;

        por_key = -iata_code
          >> '^' >> -icao_code
          >> '^' >> -faa_code
          >> '^' >> is_geonames
          >> '^' >> geoname_id
          >> '^' >> -envelope_id
          ;

        por_details = common_name
          >> '^' >> ascii_name
          >> '^' >> -latitude
          >> '^' >> -longitude
          >> '^' >> feat_class
          >> '^' >> feat_code
          >> '^' >> -page_rank
          >> '^' >> -date_from
          >> '^' >> -date_end
          >> '^' >> -comments
          >> '^' >> country_code
          >> '^' >> -country_code2
          >> '^' >> country_name
          >> '^' >> -continent_name
          >> '^' >> -adm1_code
          >> '^' >> -adm1_name_utf
          >> '^' >> -adm1_name_ascii
          >> '^' >> -adm2_code
          >> '^' >> -adm2_name_utf
          >> '^' >> -adm2_name_ascii
          >> '^' >> -adm3_code
          >> '^' >> -adm4_code
          >> '^' >> -population
          >> '^' >> -elevation
          >> '^' >> -gtopo30
          >> '^' >> -time_zone
          >> '^' >> -gmt_offset
          >> '^' >> -dst_offset
          >> '^' >> -raw_offset
          >> '^' >> (mod_date | x3::lit("-1"))
          >> '^' >> -city_code_list
          >> '^' >> -city_name_list
          >> '^' >> -city_detail_list
          >> '^' >> -tvl_por_code_list[storeTvlPORListString(_location)]
          >> '^' >> -state_code
          >> '^' >> por_type
          >> '^' >> -wiki_link
          ;
        // >> '^' >> -alt_name_short_list[storeAltNameShortListString(_location)]

        por_details_additional =
          wac >> '^' >> wac_name >> '^' >> -ccy_code
              >> '^' >> -unlc_section >> '^' >> -uic_section
              >> '^' >> -geoname_lat >> '^' >> -geoname_lon
          ;

        iata_code =
          x3::repeat(3)[x3::char_('A', 'Z')][storeIataCode(_location)];

        icao_code =
          x3::repeat(4)[x3::char_("A-Z0-9")][storeIcaoCode(_location)];

        faa_code =
          x3::repeat(1,4)[x3::char_("A-Z0-9")][storeFaaCode(_location)];

        geoname_id = uint1_9_p[storeGeonamesID(_location)];

        envelope_id = uint1_4_p[storeEnvelopeID(_location)];

        is_geonames = (x3::char_('Y') | x3::char_('N') | x3::char_('Z'))[storeIsGeonames(_location)];

        common_name =
          (x3::no_skip[+~x3::char_('^')]
           - (x3::eoi|x3::eol))[storeCommonName(_location)]
          ;

        ascii_name =
          (x3::no_skip[+~x3::char_('^')]
           - (x3::eoi|x3::eol))[storeAsciiName(_location)]
          ;

        alt_name_short_list = alt_name_short % ',';

        alt_name_short =
          (x3::no_skip[+~x3::char_("^,")]
           - (x3::eoi|x3::eol))[storeAltNameShort(_location)]
          ;

        tvl_por_code_list = tvl_por_code % ',';

        tvl_por_code =
          (x3::no_skip[+~x3::char_("^,")]
           - (x3::eoi|x3::eol))[storeTvlPORCode(_location)]
          ;

        latitude = x3::double_[storeLatitude(_location)];

        longitude = x3::double_[storeLongitude(_location)];

        feat_class =
          x3::repeat(1)[x3::char_("A-Z")][storeFeatureClass(_location)]
          ;

        feat_code =
          x3::repeat(2,5)[x3::char_("A-Z1-5")][storeFeatureCode(_location)]
          ;

        page_rank = x3::double_[storePageRank(_location)];

        date_from = date[storeDateFrom(_location)];

        date_end = date[storeDateEnd(_location)];

        comments =
          (x3::no_skip[+~x3::char_('^')]
           - (x3::eoi|x3::eol))[storeComments(_location)]
          ;

        country_code =
          x3::repeat(2,3)[x3::char_("A-Z")][storeCountryCode(_location)]
          ;

        country_code2 =
          (x3::no_skip[+~x3::char_('^')]
           - (x3::eoi|x3::eol))[storeAltCountryCode(_location)]
          ;

        country_name =
          (x3::no_skip[+~x3::char_('^')]
           - (x3::eoi|x3::eol))[storeCountryName(_location)]
          ;

        continent_name =
          (x3::no_skip[+~x3::char_('^')]
           - (x3::eoi|x3::eol))[storeContinentName(_location)]
          ;

        adm1_code =
          (x3::no_skip[+~x3::char_('^')]
           - (x3::eoi|x3::eol))[storeAdm1Code(_location)]
          ;

        adm1_name_utf =
          (x3::no_skip[+~x3::char_('^')]
           - (x3::eoi|x3::eol))[storeAdm1UtfName(_location)]
          ;

        adm1_name_ascii =
          (x3::no_skip[+~x3::char_('^')]
           - (x3::eoi|x3::eol))[storeAdm1AsciiName(_location)]
          ;

        adm2_code =
          (x3::no_skip[+~x3::char_('^')]
           - (x3::eoi|x3::eol))[storeAdm2Code(_location)]
          ;

        adm2_name_utf =
          (x3::no_skip[+~x3::char_('^')]
           - (x3::eoi|x3::eol))[storeAdm2UtfName(_location)]
          ;

        adm2_name_ascii =
          (x3::no_skip[+~x3::char_('^')]
           - (x3::eoi|x3::eol))[storeAdm2AsciiName(_location)]
          ;

        adm3_code =
          (x3::no_skip[+~x3::char_('^')]
           - (x3::eoi|x3::eol))[storeAdm3Code(_location)]
          ;

        adm4_code =
          (x3::no_skip[+~x3::char_('^')]
           - (x3::eoi|x3::eol))[storeAdm4Code(_location)]
          ;

        population = uint1_9_p[storePopulation(_location)];

        elevation = int1_5_p[storeElevation(_location)];

        gtopo30 = int1_5_p[storeGTopo30(_location)];

        time_zone =
          (x3::no_skip[+~x3::char_('^')]
           - (x3::eoi|x3::eol))[storeTimeZone(_location)]
          ;

        gmt_offset = x3::float_[storeGMTOffset(_location)];

        dst_offset = x3::float_[storeDSTOffset(_location)];

        raw_offset = x3::float_[storeRawOffset(_location)];

        mod_date = date[storeModDate(_location)];

        date =
          year_p[storeDateYear(_location)]
          >> '-'
          >> month_p[storeDateMonth(_location)]
          >> '-'
          >> day_p[storeDateDay(_location)];

        city_code_list = city_code % ',';

        city_code =
          x3::repeat(3)[x3::char_('A', 'Z')][storeCityCode(_location)]
          ;

        city_name_list = city_name_utf % '=';

        city_name_utf =
          (x3::no_skip[+~x3::char_("^|=")]
           - (x3::eoi|x3::eol))[storeCityUtfName(_location)]
          ;

        city_name_ascii =
          (x3::no_skip[+~x3::char_("^|=")]
           - (x3::eoi|x3::eol))[storeCityAsciiName(_location)]
          ;

        city_country_code =
          x3::repeat(2,3)[x3::char_("A-Z")][storeCityCountryCode(_location)]
          ;
      
        city_state_code =
          (x3::no_skip[+~x3::char_('^')]
           - (x3::eoi|x3::eol))[storeCityStateCode(_location)]
          ;      

        city_detail_list = city_details[storeCityDetailList(_location)] % '=';

        city_details =
          city_code
          >> '|' >> city_geoname_id
          >> '|' >> city_name_utf
          >> '|' >> city_name_ascii
          >> '|' >> -city_country_code
          >> '|' >> -city_state_code
          ;

        city_geoname_id = uint1_9_p[storeCityGeonamesID(_location)];

        state_code =
          (x3::no_skip[+~x3::char_('^')]
           - (x3::eoi|x3::eol))[storeStateCode(_location)]
          ;

        wac = uint1_4_p[storeWAC(_location)];

        wac_name =
          (x3::no_skip[+~x3::char_('^')]
           - (x3::eoi|x3::eol))[storeWACName(_location)]
          ;

        ccy_code =
          (x3::no_skip[+~x3::char_('^')]
           - (x3::eoi|x3::eol))[storeCurrencyCode(_location)]
          ;

        unlc_section = unlc_details % '=';

        unlc_details =
          unlocode_code >> '|' >> -unlc_qualifiers
          ;

        unlocode_code =
          x3::repeat(5)[x3::char_("A-Z0-9")][storeUNLOCode(_location)];

        unlc_qualifiers =
          x3::repeat(1,2)[x3::char_("hp")]
          ;

        uic_section = uic_details % '=';

        uic_details =
          uic_code >> '|' >> -uic_qualifiers
          ;

        uic_code = uint1_9_p[storeUICCode(_location)];

        uic_qualifiers =
          x3::repeat(1,2)[x3::char_("hp")]
          ;

        geoname_lat = x3::double_[storeGeonameLatitude(_location)];

        geoname_lon = x3::double_[storeGeonameLongitude(_location)];        

        por_type =
          x3::repeat(1,3)[x3::char_("ABCGHOPRZ")][storePORType(_location)]
          ;
      
        wiki_link =
          (x3::no_skip[+~x3::char_('^')]
           - (x3::eoi|x3::eol))[storeWikiLink(_location)]
          ;

        alt_name_section = alt_name_details % '=';

        alt_name_details =
          -alt_lang_code
          >> '|' >> alt_name
          >> '|' >> -alt_name_qualifiers
          ;

        alt_lang_code =
          (+~x3::char_("|=")
           - (x3::eoi|x3::eol))[storeAltLangCodeFull(_location)]
          ;

        alt_lang_code_ftd = lang_code_2char >> lang_code_opt;

        lang_code_opt = -(lang_code_ext | lang_code_hist);

        lang_code_2char =
          x3::repeat(2,4)[x3::char_("a-z")][storeAltLangCode2Char(_location)]
          ;

        lang_code_ext =
          '-' >> x3::repeat(1,4)[x3::char_('A', 'Z')][storeAltLangCodeExt(_location)];

        lang_code_hist =
          '_' >> x3::repeat(1,4)[x3::char_("a-z0-9")][storeAltLangCodeHist(_location)];

        alt_name =
          (x3::no_skip[+~x3::char_("|=")]
           - (x3::eoi|x3::eol))[storeAltName(_location)]
          ;

        alt_name_qualifiers =
          x3::repeat(1,4)[x3::char_("shpc")]
          ;
      }

      // Instantiation of rules
      x3::rule<Iterator, bsu::blank_type>
      start, header, por_rule, por_rule_end, por_key, por_details,
        iata_code, icao_code, faa_code, geoname_id, envelope_id, is_geonames,
        common_name, ascii_name,
        alt_name_short_list, alt_name_short, alt_name_sep,
        tvl_por_code_list, tvl_por_code, tvl_por_sep,
        latitude, longitude, feat_class, feat_code,
        page_rank, date_from, date_end, comments,
        country_code, country_code2, country_name, continent_name,
        adm1_code, adm1_name_utf, adm1_name_ascii,
        adm2_code, adm2_name_utf, adm2_name_ascii, adm3_code, adm4_code,
        population, elevation, gtopo30,
        time_zone, gmt_offset, dst_offset, raw_offset,
        mod_date, date,
        city_code_list, city_code,
        city_name_list, city_name_utf, city_name_ascii,
        city_country_code, city_state_code,
        city_detail_list, city_details, city_geoname_id,
        state_code,
        por_type, wiki_link,
        alt_name_section, alt_name_details,
        alt_lang_code, alt_lang_code_ftd, alt_name, alt_name_qualifiers,
        lang_code_opt, lang_code_2char, lang_code_ext, lang_code_hist,
        por_details_additional, wac, wac_name, ccy_code,
        unlc_section, unlc_details, unlocode_code, unlc_qualifiers,
        uic_section, uic_details, uic_code, uic_qualifiers,
        geoname_lat, geoname_lon;
      
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
    // Store the raw data string
    _location.setRawDataString (_string);
  }
    
  // //////////////////////////////////////////////////////////////////////
  PORStringParser::~PORStringParser() {
  }

  // //////////////////////////////////////////////////////////////////////
  const Location& PORStringParser::generateLocation() {
    // DEBUG
    // OPENTREP_LOG_DEBUG ("Parsing POR string: '" << _string << "'");

    // String to be parsed
    //std::istringstream stringToBeParsed (_string);
    auto const iStrBegin = _string.begin();
    auto const iStrEnd = _string.end();
    
    // Initialise the parser (grammar) with the helper/staging structure.
    PorParserHelper::LocationParser<std::string> lPORParser (_location);
      
    // Launch the parsing of the file and, thanks to the doEndPor
    // call-back structure, the building of the whole BomRoot BOM
    bool hasParsingBeenSuccesful = false;
    try {

      hasParsingBeenSuccesful = x3::phrase_parse (iStrBegin, iStrEnd,
                                                   lPORParser, x3::space);

    } catch (const x3::expectation_failure<std::string>& e) {
      auto const pos = e.where();
      std::ostringstream oStr;
      oStr << "Parse error on POR string '" << _string << "'." << std::endl
           << "Message: " << e.which() << "'" << std::endl;
      OPENTREP_LOG_ERROR (oStr.str());
      throw PorFileParsingException (oStr.str());
    }
      
    if (hasParsingBeenSuccesful == false) {
      OPENTREP_LOG_ERROR ("Parsing of POR input string: '" << _string
                          << "' failed");
      throw PorFileParsingException ("Parsing of POR input string: '"
                                     + _string + "' failed");
    }
    
    if  (iStrBegin != iStrEnd) {
      OPENTREP_LOG_ERROR ("Parsing of POR input string: '" << _string
                          << "' failed");
      throw PorFileParsingException ("Parsing of POR input file: '"
                                     + _string + "' failed");
    }
    
    //
    if (hasParsingBeenSuccesful == true && iStrBegin == iStrEnd) {
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
      
    OPENTREP_LOG_DEBUG ("Parsing the POR input file: " << _filename);

    // File to be parsed
    const char* lFilenameStr = _filename.c_str();
    std::ifstream fileToBeParsed (lFilenameStr, std::ios_base::in);

    // Check if the filename exist and can be open
    if (fileToBeParsed.is_open() == false) {
      OPENTREP_LOG_ERROR ("The POR file ('" << _filename << "') can not be open."
                          << std::endl);

      throw FileNotFoundException ("The '" + _filename
                                   + "' file does not exist or can not be read");
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
       x3::phrase_parse (start, end, lPORParser, bsu::blank);
      
    if (hasParsingBeenSuccesful == false) {
      OPENTREP_LOG_ERROR ("Parsing of POR input file ('" << _filename
                          << "') failed");
      throw PorFileParsingException ("Parsing of POR input file ('"
                                     + _filename + "') failed");
    }
    
    if  (start != end) {
      OPENTREP_LOG_ERROR ("Parsing of POR input file ('" << _filename
                          << "') failed");
      throw PorFileParsingException ("Parsing of POR input file ('"
                                     + _filename + "') failed");
    }
    
    if (hasParsingBeenSuccesful == true && start == end) {
      OPENTREP_LOG_DEBUG ("Parsing of POR input file ('" << _filename
                          << "') succeeded");
    } 
  }

}
