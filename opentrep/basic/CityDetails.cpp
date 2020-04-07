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
                            const CityASCIIName_T& iAsciiName,
                            const CountryCode_T& iCountryCode,
                            const StateCode_T& iStateCode) :
    _iataCode (iIataCode), _geonamesID (iGeonamesID),
    _utfName (iUtfName), _asciiName (iAsciiName),
    _countryCode (iCountryCode), _stateCode (iStateCode) {
  }

  // //////////////////////////////////////////////////////////////////////
  CityDetails::CityDetails() :
    _iataCode (IATACode_T ("")), _geonamesID (0),
    _utfName (""), _asciiName (""), _countryCode (""), _stateCode ("") {
    assert (false);
  }
  
  // //////////////////////////////////////////////////////////////////////
  CityDetails::CityDetails (const CityDetails& iCityDetails) :
    _iataCode (iCityDetails._iataCode), _geonamesID (iCityDetails._geonamesID),
    _utfName (iCityDetails._utfName), _asciiName (iCityDetails._asciiName),
    _countryCode (iCityDetails._countryCode),
    _stateCode (iCityDetails._stateCode) {
  }
  
  // //////////////////////////////////////////////////////////////////////
  CityDetails::~CityDetails() {
  }

  // //////////////////////////////////////////////////////////////////////
  bool CityDetails::operator== (const CityDetails& iCityDetails) const {
    const bool areEqual = (_iataCode == iCityDetails._iataCode
                           && _geonamesID == iCityDetails._geonamesID
                           && _utfName == iCityDetails._utfName
                           && _asciiName == iCityDetails._asciiName
                           && _countryCode == iCityDetails._countryCode
                           && _stateCode == iCityDetails._stateCode);
    return areEqual;
  }

  // //////////////////////////////////////////////////////////////////////
  std::string CityDetails::describe() const {
    std::ostringstream oStr;
    oStr << _iataCode << "|" << _geonamesID
         << "|" << _utfName << "|" << _asciiName
         << "|" << _countryCode << "|" << _stateCode;

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
