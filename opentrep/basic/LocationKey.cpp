// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// STL
#include <cassert>
// OpenTrep
#include <opentrep/LocationKey.hpp>
#include <opentrep/service/Logger.hpp>

namespace OPENTREP {

  // //////////////////////////////////////////////////////////////////////
  LocationKey::LocationKey (const IATACode_T& iIataCode,
                            const IATAType& iIataType,
                            const GeonamesID_T& iGeonamesID) :
    _iataCode (iIataCode), _iataType (iIataType), _geonamesID (iGeonamesID) {
    if (_geonamesID == 0) {
      _isGeonames = false;
    } else {
      _isGeonames = true;
    }
  }

  // //////////////////////////////////////////////////////////////////////
  LocationKey::LocationKey() :
    _iataCode (IATACode_T ("")), _iataType (IATAType::LAST_VALUE),
    _geonamesID (0), _isGeonames (false) {
    assert (false);
  }
  
  // //////////////////////////////////////////////////////////////////////
  LocationKey::LocationKey (const LocationKey& iLocationKey) :
    _iataCode (iLocationKey._iataCode), _iataType (iLocationKey._iataType),
    _geonamesID (iLocationKey._geonamesID),
    _isGeonames (iLocationKey._isGeonames) {
  }
  
  // //////////////////////////////////////////////////////////////////////
  LocationKey::~LocationKey() {
  }

  // //////////////////////////////////////////////////////////////////////
  bool LocationKey::operator== (const LocationKey& iLocationKey) const {
    const bool areEqual = (_iataCode == iLocationKey._iataCode
                           && _iataType == iLocationKey._iataType
                           && _geonamesID == iLocationKey._geonamesID);
    return areEqual;
  }

  // //////////////////////////////////////////////////////////////////////
  std::string LocationKey::describe() const {
    std::ostringstream oStr;
    oStr << _iataCode << "-" << _iataType.getTypeAsString()
         << "-" << _geonamesID;

    return oStr.str();
  }

  // //////////////////////////////////////////////////////////////////////
  std::string LocationKey::toString() const {
    std::ostringstream oStr;
    oStr << describe();      
    return oStr.str();
  }   

  // //////////////////////////////////////////////////////////////////////
  void LocationKey::toStream (std::ostream& ioOut) const {
    ioOut << toString();
  }
  
  // //////////////////////////////////////////////////////////////////////
  void LocationKey::fromStream (std::istream& ioIn) {
  }
  
}
