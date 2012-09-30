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
    _commonName (CommonName_T ("NotAvailable")),
    _asciiName (ASCIIName_T ("NotAvailable")),
    _altNameShortListString (AltNameShortListString_T ("NotAvailable")),
    _faaCode (FAACode_T ("")), _cityCode (CityCode_T ("ZZZ")),
    _stateCode (StateCode_T ("NA")), _countryCode (CountryCode_T ("NA")),
    _altCountryCode (AltCountryCode_T ("")),
    _regionCode (RegionCode_T ("NOTAVL")),
    _latitude (0), _longitude (0),
    _featClass (FeatureClass_T ("Z")), _featCode (FeatureCode_T ("ZZZZ")),
    _iataType (IATAType_T ("Z")),
    _admin1Code (Admin1Code_T ("Z")), _admin2Code (Admin2Code_T ("Z")),
    _admin3Code (Admin3Code_T ("Z")), _admin4Code (Admin4Code_T ("Z")),
    _population (0), _elevation (0), _gTopo30 (0),
    _timeZone (TimeZone_T ("NA")),
    _gmtOffset (0), _dstOffset (0), _rawOffset (0),
    _modificationDate (2000, 01, 01),
    _isAirport (false), _isCommercial (false),
    _wikiLink (WikiLink_T ("http://en.wikipedia.org")),
    _pageRank (K_DEFAULT_PAGE_RANK),
    _originalKeywords ("NA"), _correctedKeywords ("NA"),
    _percentage (0), _editDistance (0), _allowableEditDistance (0),
    _rawDataString ("NA"),
    _itLanguageCodeNum (Language::LAST_VALUE),
    _itLanguageCode (LanguageCode_T ("")) {
  }

  // //////////////////////////////////////////////////////////////////////
  Location::Location (const Location& iLocation) :
    _key (iLocation._key),
    _commonName (iLocation._commonName), _asciiName (iLocation._asciiName),
    _altNameShortListString (iLocation._altNameShortListString),
    _faaCode (iLocation._faaCode), _cityCode (iLocation._cityCode),
    _stateCode (iLocation._stateCode), _countryCode (iLocation._countryCode),
    _altCountryCode (iLocation._altCountryCode),
    _regionCode (iLocation._regionCode),
    _latitude (iLocation._latitude), _longitude (iLocation._longitude),
    _featClass (iLocation._featClass), _featCode (iLocation._featCode),
    _iataType (iLocation._iataType),
    _admin1Code (iLocation._admin1Code), _admin2Code (iLocation._admin2Code),
    _admin3Code (iLocation._admin3Code), _admin4Code (iLocation._admin4Code),
    _population (iLocation._population), _elevation (iLocation._elevation),
    _gTopo30 (iLocation._gTopo30),
    _timeZone (iLocation._timeZone), _gmtOffset (iLocation._gmtOffset),
    _dstOffset (iLocation._dstOffset), _rawOffset (iLocation._rawOffset),
    _modificationDate (iLocation._modificationDate),
    _isAirport (iLocation._isAirport),
    _isCommercial (iLocation._isCommercial),
    _wikiLink (iLocation._wikiLink), _pageRank (iLocation._pageRank),
    _nameMatrix (iLocation._nameMatrix),
    _originalKeywords (iLocation._originalKeywords),
    _correctedKeywords (iLocation._correctedKeywords),
    _percentage (iLocation._percentage),
    _editDistance (iLocation._editDistance),
    _allowableEditDistance (iLocation._allowableEditDistance),
    _extraLocationList (iLocation._extraLocationList),
    _alternateLocationList (iLocation._alternateLocationList),
    _rawDataString (iLocation._rawDataString),
    _itLanguageCodeNum (iLocation._itLanguageCodeNum),
    _itLanguageCode (LanguageCode_T ("")) { 
  }
    
  // //////////////////////////////////////////////////////////////////////
  Location::Location (const IATACode_T& iIataCode, const ICAOCode_T& iIcaoCode,
                      const GeonamesID_T& iGeonamesID,
                      const CommonName_T& iCommonName,
                      const ASCIIName_T& iASCIIName,
                      const FAACode_T& iFaaCode, const CityCode_T& iCityCode,
                      const StateCode_T& iStateCode,
                      const CountryCode_T& iCountryCode,
                      const AltCountryCode_T& iAltCountryCode,
                      const RegionCode_T& iRegionCode,
                      const Latitude_T& iLatitude, const Longitude_T& iLongitude,
                      const FeatureClass_T& iFeatureClass,
                      const FeatureCode_T& iFeatureCode,
                      const IATAType_T& iIATAType,
                      const Admin1Code_T& iAdmin1Code,
                      const Admin2Code_T& iAdmin2Code,
                      const Admin3Code_T& iAdmin3Code,
                      const Admin4Code_T& iAdmin4Code,
                      const Population_T& iPopulation,
                      const Elevation_T& iElevation, const GTopo30_T& iGTopo30,
                      const TimeZone_T& iTimeZone,
                      const GMTOffset_T& iGMTOffset,
                      const DSTOffset_T& iDSTOffset,
                      const RawOffset_T& iRawOffset,
                      const Date_T& iModDate,
                      const bool isAirport, const bool isCommercial,
                      const WikiLink_T& iWikiLink, const PageRank_T& iPageRank,
                      const std::string& iOriginalKeywords,
                      const std::string& iCorrectedKeywords,
                      const MatchingPercentage_T& iPercentage,
                      const NbOfErrors_T& iEditDistance,
                      const NbOfErrors_T& iAllowableEditDistance,
                      const RawDataString_T& iRawDataString) :
    _key (iIataCode, iIcaoCode, iGeonamesID),
    _commonName (iCommonName),_asciiName (iASCIIName),
    _altNameShortListString (AltNameShortListString_T ("NotAvailable")),
    _faaCode (iFaaCode), _cityCode (iCityCode), _stateCode (iStateCode),
    _countryCode (iCountryCode), _altCountryCode (iAltCountryCode),
    _regionCode (iRegionCode),
    _latitude (iLatitude), _longitude (iLongitude),
    _featClass (iFeatureClass), _featCode (iFeatureCode), _iataType (iIATAType),
    _admin1Code (iAdmin1Code), _admin2Code (iAdmin2Code),
    _admin3Code (iAdmin3Code), _admin4Code (iAdmin4Code),
    _population (iPopulation), _elevation (iElevation), _gTopo30 (iGTopo30),
    _timeZone (iTimeZone),
    _gmtOffset (iGMTOffset), _dstOffset (iDSTOffset), _rawOffset (iRawOffset),
    _modificationDate (iModDate),
    _isAirport (isAirport), _isCommercial (isCommercial),
    _wikiLink (iWikiLink), _pageRank (iPageRank),
    _originalKeywords (iOriginalKeywords),
    _correctedKeywords (iCorrectedKeywords),
    _percentage (iPercentage), _editDistance (iEditDistance),
    _allowableEditDistance (iAllowableEditDistance),
    _rawDataString (iRawDataString),
    _itLanguageCodeNum (Language::LAST_VALUE),
    _itLanguageCode (LanguageCode_T ("")) {
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

    const std::string isAirportString = (_isAirport)?"Y":"N";
    const std::string isCommercialString = (_isCommercial)?"Y":"N";
    
    oStr << ", " << _pageRank << "%"
         << ", " << _commonName << ", " << _asciiName
         << ", " << _faaCode << ", " << _cityCode << ", " << _stateCode
         << ", " << _countryCode << ", " << _altCountryCode
         << ", " << _regionCode
         << ", " << _latitude << ", " << _longitude
         << ", " << _featClass << ", " << _featCode << ", " << _iataType
         << ", " << _admin1Code << ", " << _admin2Code
         << ", " << _admin3Code << ", " << _admin4Code
         << ", " << _population << ", " << _elevation << ", " << _gTopo30
         << ", " << _timeZone
         << ", " << _gmtOffset << ", " << _dstOffset << ", " << _rawOffset
         << ", " << _modificationDate
         << ", " << isAirportString << ", " << isCommercialString
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

  // ////////////////////////////////////////////////////////////////////
  void Location::consolidateAltNameShortListString() {
    std::ostringstream oStr;
    unsigned short idx = 0;
    for (AltNameShortList_T::const_iterator itName = _itAltNameShortList.begin();
         itName != _itAltNameShortList.end(); ++itName, ++idx) {
      if (idx != 0) {
        oStr << ",";
      }
      const AltNameShortListString_T& lAlternateLocation = *itName;
      oStr << lAlternateLocation;
    }
  }

}
