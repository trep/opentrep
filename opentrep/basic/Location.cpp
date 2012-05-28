// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// STL
#include <sstream>
#include <string>
#include <list>
// OpenTrep
#include <opentrep/Location.hpp>

namespace OPENTREP {

  // //////////////////////////////////////////////////////////////////////
  Location::Location () 
    : _iataCode ("AAA"), _icaoCode ("AAAA"), _geonameID (0), _faaCode ("AAA"),
      _cityCode ("AAA"), _stateCode ("NA"), _countryCode ("NA"),
      _regionCode ("NA"), _timeZoneGroup ("NA"),
      _latitude (0), _longitude (0), _pageRank (0.1),
      _nameList (std::list<std::string> ()),
      _originalKeywords ("NA"), _correctedKeywords ("NA"),
      _percentage (0), _editDistance (0), _allowableEditDistance (0) {
  }

  // //////////////////////////////////////////////////////////////////////
  Location::Location (const Location& iLocation) 
    : _iataCode (iLocation._iataCode), _icaoCode (iLocation._icaoCode),
      _geonameID (iLocation._geonameID), _faaCode (iLocation._faaCode),
      _cityCode (iLocation._cityCode), _stateCode (iLocation._stateCode),
      _countryCode (iLocation._countryCode),
      _regionCode (iLocation._regionCode),
      _timeZoneGroup (iLocation._timeZoneGroup),
      _latitude (iLocation._latitude), _longitude (iLocation._longitude),
      _pageRank (iLocation._pageRank),
      _nameList (iLocation._nameList),
      _originalKeywords (iLocation._originalKeywords),
      _correctedKeywords (iLocation._correctedKeywords),
      _percentage (iLocation._percentage),
      _editDistance (iLocation._editDistance),
      _allowableEditDistance (iLocation._allowableEditDistance),
      _extraLocationList (iLocation._extraLocationList),
      _alternateLocationList (iLocation._alternateLocationList) { 
  }
    
  // //////////////////////////////////////////////////////////////////////
  Location::Location (const std::string& iIataCode,
                      const std::string& iIcaoCode,
                      const GeonamesID_T& iGeonamesID,
                      const std::string& iFaaCode,
                      const std::string& iCityCode,
                      const std::string& iStateCode, 
                      const std::string& iCountryCode,
                      const std::string& iRegionCode, 
                      const std::string& iTimeZoneGroup, 
                      const double iLatitude, const double iLongitude,
                      const double iPageRank,
                      const LocationNameList_T& iNameList,
                      const std::string& iOriginalKeywords,
                      const std::string& iCorrectedKeywords,
                      const MatchingPercentage_T& iPercentage,
                      const NbOfErrors_T& iEditDistance,
                      const NbOfErrors_T& iAllowableEditDistance)
    : _iataCode (iIataCode), _icaoCode (iIcaoCode), _geonameID (iGeonamesID),
      _faaCode (iFaaCode), _cityCode (iCityCode), _stateCode (iStateCode),
      _countryCode (iCountryCode), _regionCode (iRegionCode),
      _timeZoneGroup (iTimeZoneGroup),
      _latitude (iLatitude), _longitude (iLongitude),
      _pageRank (iPageRank),
      _nameList (iNameList), _originalKeywords (iOriginalKeywords),
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

  // //////////////////////////////////////////////////////////////////////
  std::string Location::toBasicString() const {
    std::ostringstream oStr;
    oStr << _iataCode << ", " << _icaoCode << ", " << _geonameID
         << ", " << _faaCode << ", " << _cityCode
         << ", " << _stateCode << ", " << _countryCode << ", " << _regionCode
         << ", " << _timeZoneGroup
         << ", " << _latitude << ", " << _longitude
         << ", " << _pageRank << "%"
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
    for (LocationNameList_T::const_iterator itName = _nameList.begin();
	 itName != _nameList.end(); ++itName) {
      oStr << ", " << *itName;
    }
    return oStr.str();
  }

  // //////////////////////////////////////////////////////////////////////
  std::string Location::toString() const {
    std::ostringstream oStr;
    oStr << toSingleLocationString();

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

}
