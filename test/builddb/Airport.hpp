#ifndef __OPENTREP_BOM_AIRPORT_HPP
#define __OPENTREP_BOM_AIRPORT_HPP

// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// STL
#include <iostream>
#include <sstream>
#include <string>

namespace OpenTrep {
  
  /** Structure modelling an airport. */
  struct Airport {
    /** Airport code. */
    std::string airportCode;
    /** Airport name. */
    std::string airportName;
    /** City code. */
    std::string cityCode;
    /** City name. */
    std::string cityName;
    /** State code. */
    std::string stateCode;
    /** Country code. */
    std::string countryCode;
    /** Region code. */
    std::string regionCode;
    /** Continent code. */
    std::string continentCode;
    /** Time-zone group. */
    std::string timeZoneGroup;
    /** Longitude. */
    double longitude;
    /** Latitude. */
    double latitude;

    /** Dump a Business Object into an output stream.
        @param ostream& the output stream. */
    void toStream (std::ostream& ioOut) const {
      ioOut << airportCode << ", " << airportName << ", "
            << cityCode << ", " << cityName << ", " << stateCode
            << ", " << countryCode << ", " << regionCode
            << ", " << continentCode << ", " << timeZoneGroup
            << ", " << longitude << ", " << latitude;
    }

    /** Read a Business Object from an input stream.
        @param istream& the input stream. */
    void fromStream (std::istream& ioIn) {
    }

    /** Get the serialised version of the Airport object. */
    std::string toString() const {
      std::ostringstream oStr;
      toStream (oStr);
      return oStr.str();
    }
  };

  // ///////////// Type definitions ////////////////////
  typedef std::vector<Airport> AirportList_T;
  
}


/**
   Piece of code given by Nicolai M. Josuttis, Section 13.12.1 "Implementing
   Output Operators" (p653) of his book "The C++ Standard Library: A Tutorial
   and Reference", published by Addison-Wesley.
*/
template <class charT, class traits>
inline
std::basic_ostream<charT, traits>&
operator<< (std::basic_ostream<charT, traits>& ioOut,
            const OpenTrep::Airport& iAirport) {
  /**
     string stream:
     - with same format
     - without special field width
  */
  std::basic_ostringstream<charT,traits> ostr;
  ostr.copyfmt (ioOut);
  ostr.width (0);

  // Fill string stream
  iAirport.toStream (ostr);

  // Print string stream
  ioOut << ostr.str();

  return ioOut;
}

/**
   Piece of code given by Nicolai M. Josuttis, Section 13.12.1 "Implementing
   Output Operators" (pp655-657) of his book "The C++ Standard Library:
   A Tutorial and Reference", published by Addison-Wesley.
*/
template <class charT, class traits>
inline
std::basic_istream<charT, traits>&
operator>> (std::basic_istream<charT, traits>& ioIn,
            OpenTrep::Airport& ioAirport) {
  // Fill Airport object with input stream
  ioAirport.fromStream (ioIn);
  return ioIn;
}
#endif // __OPENTREP_BOM_AIRPORT_HPP
