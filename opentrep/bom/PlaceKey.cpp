// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// C
#include <cassert>
// OpenTrep BOM
#include <opentrep/bom/PlaceKey.hpp>
#include <opentrep/service/Logger.hpp>

namespace OPENTREP {

  // //////////////////////////////////////////////////////////////////////
  PlaceKey::PlaceKey (const std::string& iIataCode, const std::string& iIcaoCode,
                      const GeonamesID_T& iGeonamesID) :
    _iataCode (iIataCode), _icaoCode (iIcaoCode), _geonamesID (iGeonamesID) {
  }

  // //////////////////////////////////////////////////////////////////////
  PlaceKey::PlaceKey() :
    _iataCode (""), _icaoCode (""), _geonamesID (0) {
    assert (false);
  }
  
  // //////////////////////////////////////////////////////////////////////
  PlaceKey::PlaceKey (const PlaceKey& iPlaceKey) :
    _iataCode (iPlaceKey._iataCode), _icaoCode (iPlaceKey._icaoCode),
    _geonamesID (iPlaceKey._geonamesID) {
  }
  
  // //////////////////////////////////////////////////////////////////////
  PlaceKey::~PlaceKey() {
  }

  // //////////////////////////////////////////////////////////////////////
  bool PlaceKey::operator== (const PlaceKey& iPlaceKey) const {
    const bool areEqual = (_iataCode == iPlaceKey._iataCode
                           && _icaoCode == iPlaceKey._icaoCode
                           && _geonamesID == iPlaceKey._geonamesID);
    return areEqual;
  }

  // //////////////////////////////////////////////////////////////////////
  std::string PlaceKey::describe() const {
    std::ostringstream oStr;
    oStr << _iataCode << "-" << _icaoCode << "-" << _geonamesID;
    return oStr.str();
  }

  // //////////////////////////////////////////////////////////////////////
  std::string PlaceKey::toString() const {
    std::ostringstream oStr;
    oStr << describe();      
    return oStr.str();
  }   

  // //////////////////////////////////////////////////////////////////////
  void PlaceKey::toStream (std::ostream& ioOut) const {
    ioOut << toString();
  }
  
  // //////////////////////////////////////////////////////////////////////
  void PlaceKey::fromStream (std::istream& ioIn) {
  }
  
}
