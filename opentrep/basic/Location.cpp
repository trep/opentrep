// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// STL
#include <sstream>
#include <string>
#include <list>
// OpenTrep
#include <opentrep/basic/BasConst_General.hpp>
#include <opentrep/Location.hpp>

namespace OPENTREP {

  // //////////////////////////////////////////////////////////////////////
  Location::Location() :
    _key (IATACode_T ("AAA"), ICAOCode_T ("AAAA"), 0),
    _commonName (CommonName_T ("NotAvailable")),_asciiName (ASCIIName_T ("NotAvailable")),
    _faaCode (FAACode_T ("AAA")), _cityCode (CityCode_T ("AAA")),
    _stateCode (StateCode_T ("NA")), _countryCode (CountryCode_T ("NA")),
    _altCountryCode (AltCountryCode_T ("NA")), _regionCode (RegionCode_T ("NA")),
    _latitude (0), _longitude (0),
    _featClass (FeatureClass_T ("Z")), _featCode (FeatureCode_T ("ZZZZ")),
    _iataType (IATAType_T ("Z")),
    _adminCode1 (AdminCode1_T ("Z")), _adminCode2 (AdminCode2_T ("Z")),
    _adminCode3 (AdminCode3_T ("Z")), _adminCode4 (AdminCode4_T ("Z")),
    _population (0), _elevation (0), _gTopo30 (0),
    _timeZone (TimeZone_T ("NA")),
    _gmtOffset (0), _dstOffset (0), _rawOffset (0),
    _modificationDate (2000, 01, 01),
    _isGeonames (false), _isAirport (false), _isCommercial (false),
    _wikiLink (WikiLink_T ("http://en.wikipedia.org")), _pageRank (K_DEFAULT_PAGE_RANK),
    _originalKeywords ("NA"), _correctedKeywords ("NA"),
    _percentage (0), _editDistance (0), _allowableEditDistance (0) {
  }

  // //////////////////////////////////////////////////////////////////////
  Location::Location (const Location& iLocation) :
    _key (iLocation._key),
    _commonName (iLocation._commonName), _asciiName (iLocation._asciiName),
    _faaCode (iLocation._faaCode), _cityCode (iLocation._cityCode),
    _stateCode (iLocation._stateCode), _countryCode (iLocation._countryCode),
    _altCountryCode (iLocation._altCountryCode), _regionCode (iLocation._regionCode),
    _latitude (iLocation._latitude), _longitude (iLocation._longitude),
    _featClass (iLocation._featClass), _featCode (iLocation._featCode),
    _iataType (iLocation._iataType),
    _adminCode1 (iLocation._adminCode1), _adminCode2 (iLocation._adminCode2),
    _adminCode3 (iLocation._adminCode3), _adminCode4 (iLocation._adminCode4),
    _population (iLocation._population), _elevation (iLocation._elevation),
    _gTopo30 (iLocation._gTopo30),
    _timeZone (iLocation._timeZone), _gmtOffset (iLocation._gmtOffset),
    _dstOffset (iLocation._dstOffset), _rawOffset (iLocation._rawOffset),
    _modificationDate (iLocation._modificationDate),
    _isGeonames (iLocation._isGeonames), _isAirport (iLocation._isAirport),
    _isCommercial (iLocation._isCommercial),
    _wikiLink (iLocation._wikiLink), _pageRank (iLocation._pageRank),
    _nameMatrix (iLocation._nameMatrix),
    _originalKeywords (iLocation._originalKeywords),
    _correctedKeywords (iLocation._correctedKeywords),
    _percentage (iLocation._percentage),
    _editDistance (iLocation._editDistance),
    _allowableEditDistance (iLocation._allowableEditDistance),
    _extraLocationList (iLocation._extraLocationList),
    _alternateLocationList (iLocation._alternateLocationList) { 
  }
    
  // //////////////////////////////////////////////////////////////////////
  Location::Location (const IATACode_T& iIataCode, const ICAOCode_T& iIcaoCode,
                      const GeonamesID_T& iGeonamesID,
                      const CommonName_T& iCommonName, const ASCIIName_T& iASCIIName,
                      const FAACode_T& iFaaCode, const CityCode_T& iCityCode,
                      const StateCode_T& iStateCode, const CountryCode_T& iCountryCode,
                      const RegionCode_T& iRegionCode, const TimeZone_T& iTimeZone,
                      const Latitude_T& iLatitude, const Longitude_T& iLongitude,
                      const PageRank_T& iPageRank, const WikiLink_T& iWikiLink,
                      const std::string& iOriginalKeywords,
                      const std::string& iCorrectedKeywords,
                      const MatchingPercentage_T& iPercentage,
                      const NbOfErrors_T& iEditDistance,
                      const NbOfErrors_T& iAllowableEditDistance) :
    _key (iIataCode, iIcaoCode, iGeonamesID),
    _commonName (iCommonName),_asciiName (iASCIIName),
    _faaCode (iFaaCode), _cityCode (iCityCode), _stateCode (iStateCode),
    _countryCode (iCountryCode), _altCountryCode (AltCountryCode_T ("NA")),
    _regionCode (iRegionCode),
    _latitude (iLatitude), _longitude (iLongitude),
    _featClass (FeatureClass_T ("Z")), _featCode (FeatureCode_T ("ZZZZ")),
    _iataType (IATAType_T ("Z")),
    _adminCode1 (AdminCode1_T ("Z")), _adminCode2 (AdminCode2_T ("Z")),
    _adminCode3 (AdminCode3_T ("Z")), _adminCode4 (AdminCode4_T ("Z")),
    _population (0), _elevation (0), _gTopo30 (0),
    _timeZone (iTimeZone),
    _gmtOffset (0), _dstOffset (0), _rawOffset (0),
    _modificationDate (2000, 01, 01),
    _isGeonames (false), _isAirport (false), _isCommercial (false),
    _wikiLink (iWikiLink), _pageRank (iPageRank),
    _originalKeywords (iOriginalKeywords),
    _correctedKeywords (iCorrectedKeywords),
    _percentage (iPercentage), _editDistance (iEditDistance),
    _allowableEditDistance (iAllowableEditDistance) {
  }

  // //////////////////////////////////////////////////////////////////////
  Location::~Location() {
  }

  // //////////////////////////////////////////////////////////////////////
  void Location::toStream (std::ostream& ioOut) const {
    ioOut << toString();
  }

  // //////////////////////////////////////////////////////////////////////
  void Location::fromStream (std::istream&) {
  }

  /*
    LocationKey <IATACode_T _iataCode, ICAOCode_T _icaoCode, GeonamesID_T _geonameID>;
    CommonName_T _commonName;
    ASCIIName_T _asciiName;
    FAACode_T _faaCode;
    CityCode_T _cityCode;
    StateCode_T _stateCode;
    CountryCode_T _countryCode;
    AltCountryCode_T _altCountryCode;
    RegionCode_T _regionCode;
    Latitude_T _latitude;
    Longitude_T _longitude;
    FeatureClass_T _featClass;
    FeatureCode_T _featCode;
    IATAType_T _iataType;
    AdminCode1_T _adminCode1;
    AdminCode2_T _adminCode2;
    AdminCode3_T _adminCode3;
    AdminCode4_T _adminCode4;
    Population_T _population;
    Elevation_T _elevation;
    GTopo30_T _gTopo30;
    TimeZone_T _timeZone;
    GMTOffset_T _gmtOffset;
    DSTOffset_T _dstOffset;
    RawOffset_T _rawOffset;
    Date_T _modificationDate;
    IsGeonames_T _isGeonames;
    IsAirport_T _isAirport;
    IsCommercial_T _isCommercial;
    WikiLink_T _wikiLink;
    NameMatrix _nameMatrix;
  */

  // //////////////////////////////////////////////////////////////////////
  std::string Location::describeShortKey() const {
    std::ostringstream oStr;
    oStr << _key.describe();
    return oStr.str();
  }
  
  // //////////////////////////////////////////////////////////////////////
  std::string Location::describeKey() const {
    return describeShortKey();
  }

  // //////////////////////////////////////////////////////////////////////
  std::string Location::toBasicString() const {
    std::ostringstream oStr;
    oStr << describeShortKey();

    oStr << ", " << _pageRank << "%"
         << ", " << _commonName << ", " << _asciiName
         << ", " << _faaCode << ", " << _cityCode << ", " << _stateCode
         << ", " << _countryCode << ", " << _altCountryCode
         << ", " << _regionCode
         << ", " << _latitude << ", " << _longitude
         << ", " << _featClass << ", " << _featCode << ", " << _iataType
         << ", " << _adminCode1 << ", " << _adminCode2
         << ", " << _adminCode3 << ", " << _adminCode4
         << ", " << _population << ", " << _elevation << ", " << _gTopo30
         << ", " << _timeZone
         << ", " << _gmtOffset << ", " << _dstOffset << ", " << _rawOffset
         << ", " << _modificationDate
         << ", " << _isGeonames << ", " << _isAirport << ", " << _isCommercial
         << ", " << _wikiLink
         << ", " << _originalKeywords << ", " << _correctedKeywords
         << ", " << _percentage << "%"
         << ", " << _editDistance << ", " << _allowableEditDistance;

    return oStr.str();
  }
    
  // //////////////////////////////////////////////////////////////////////
  std::string Location::toShortString() const {
    std::ostringstream oStr;
    oStr << toBasicString();

    if (_extraLocationList.empty() == false) {
      oStr << " with " << _extraLocationList.size() << " extra match(es)";
    }
      
    if (_alternateLocationList.empty() == false) {
      oStr << " with " << _alternateLocationList.size()
           << " alternate match(es)";
    }

    return oStr.str();
  }
    
  // //////////////////////////////////////////////////////////////////////
  std::string Location::toSingleLocationString() const {
    std::ostringstream oStr;
    oStr << toBasicString();
    return oStr.str();
  }

  // //////////////////////////////////////////////////////////////////////
  std::string Location::toString() const {
    std::ostringstream oStr;
    oStr << toSingleLocationString();
    oStr << "; name matrix {" << _nameMatrix.describe() << "}";

    if (_extraLocationList.empty() == false) {
      oStr << "; Extra matches: {";
      unsigned short idx = 0;
      for (LocationList_T::const_iterator itLoc = _extraLocationList.begin();
           itLoc != _extraLocationList.end(); ++itLoc, ++idx) {
        if (idx != 0) {
          oStr << ". ";
        }
        const Location& lExtraLocation = *itLoc;
        oStr << lExtraLocation.toShortString();
      }
      oStr << "}";
    }

    if (_alternateLocationList.empty() == false) {
      oStr << "; Alternate matches: {";
      unsigned short idx = 0;
      for (LocationList_T::const_iterator itLoc =
             _alternateLocationList.begin();
           itLoc != _alternateLocationList.end(); ++itLoc, ++idx) {
        if (idx != 0) {
          oStr << ". ";
        }
        const Location& lAlternateLocation = *itLoc;
        oStr << lAlternateLocation.toShortString();
      }
      oStr << "}";
    }

    return oStr.str();
  }

  // ////////////////////////////////////////////////////////////////////
  Date_T Location::calculateDate() const {
    _itYear.check(); _itMonth.check(); _itDay.check();
    return Date_T (_itYear._value, _itMonth._value, _itDay._value);
  }

}
