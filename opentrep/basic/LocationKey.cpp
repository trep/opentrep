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
                            const ICAOCode_T& iIcaoCode,
                            const GeonamesID_T& iGeonamesID) :
    _iataCode (iIataCode), _icaoCode (iIcaoCode), _geonamesID (iGeonamesID) {
  }

  // //////////////////////////////////////////////////////////////////////
  LocationKey::LocationKey() :
    _iataCode (IATACode_T ("")), _icaoCode (ICAOCode_T ("")), _geonamesID (0) {
    assert (false);
  }
  
  // //////////////////////////////////////////////////////////////////////
  LocationKey::LocationKey (const LocationKey& iLocationKey) :
    _iataCode (iLocationKey._iataCode), _icaoCode (iLocationKey._icaoCode),
    _geonamesID (iLocationKey._geonamesID) {
  }
  
  // //////////////////////////////////////////////////////////////////////
  LocationKey::~LocationKey() {
  }

  // //////////////////////////////////////////////////////////////////////
  bool LocationKey::operator== (const LocationKey& iLocationKey) const {
    const bool areEqual = (_iataCode == iLocationKey._iataCode
                           && _icaoCode == iLocationKey._icaoCode
                           && _geonamesID == iLocationKey._geonamesID);
    return areEqual;
  }

  // //////////////////////////////////////////////////////////////////////
  std::string LocationKey::describe() const {
    std::ostringstream oStr;
    oStr << _iataCode << "-" << _icaoCode << "-" << _geonamesID;
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
