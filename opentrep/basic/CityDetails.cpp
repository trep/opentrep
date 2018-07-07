// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// STL
#include <cassert>
// OpenTrep
#include <opentrep/CityDetails.hpp>
#include <opentrep/service/Logger.hpp>

namespace OPENTREP {

  // //////////////////////////////////////////////////////////////////////
  CityDetails::CityDetails (const IATACode_T& iIataCode,
                            const GeonamesID_T& iGeonamesID,
                            const CityUTFName_T& iUtfName,
                            const CityASCIIName_T& iAsciiName) :
    _iataCode (iIataCode), _geonamesID (iGeonamesID),
    _utfName (iUtfName), _asciiName (iAsciiName) {
  }

  // //////////////////////////////////////////////////////////////////////
  CityDetails::CityDetails() :
    _iataCode (IATACode_T ("")), _geonamesID (0),
    _utfName (""), _asciiName ("") {
    assert (false);
  }
  
  // //////////////////////////////////////////////////////////////////////
  CityDetails::CityDetails (const CityDetails& iCityDetails) :
    _iataCode (iCityDetails._iataCode), _geonamesID (iCityDetails._geonamesID),
    _utfName (iCityDetails._utfName), _asciiName (iCityDetails._asciiName) {
  }
  
  // //////////////////////////////////////////////////////////////////////
  CityDetails::~CityDetails() {
  }

  // //////////////////////////////////////////////////////////////////////
  bool CityDetails::operator== (const CityDetails& iCityDetails) const {
    const bool areEqual = (_iataCode == iCityDetails._iataCode
                           && _geonamesID == iCityDetails._geonamesID
                           && _utfName == iCityDetails._utfName
                           && _asciiName == iCityDetails._asciiName);
    return areEqual;
  }

  // //////////////////////////////////////////////////////////////////////
  std::string CityDetails::describe() const {
    std::ostringstream oStr;
    oStr << _iataCode << "|" << _geonamesID
         << "|" << _utfName << "|" << _asciiName;

    return oStr.str();
  }

  // //////////////////////////////////////////////////////////////////////
  std::string CityDetails::toString() const {
    std::ostringstream oStr;
    oStr << describe();      
    return oStr.str();
  }   

  // //////////////////////////////////////////////////////////////////////
  void CityDetails::toStream (std::ostream& ioOut) const {
    ioOut << toString();
  }
  
  // //////////////////////////////////////////////////////////////////////
  void CityDetails::fromStream (std::istream& ioIn) {
  }
  
}
