#ifndef __OPENTREP_DBA_DBAAIRPORT_HPP
#define __OPENTREP_DBA_DBAAIRPORT_HPP

// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// STL
#include <string>
#include <vector>
#include <exception>
// OpenTrep
#include "Airport.hpp"
// SOCI
#include <soci.h>

// /////////////////////////////////////////////////////////////////
namespace soci {

  /** Specify how the Airport class can be converted to (resp. from) values
      stored into (resp. retrieved from) database, using the SOCI framework. */
  template <>
  struct type_conversion<OpenTrep::Airport> {

    typedef values base_type;

    /** Fill an Airport object from the database values. */
    static void from_base (values const& iAirportValues,
                           indicator /* ind */,
                           OpenTrep::Airport& ioAirport) {
      ioAirport.airportCode = iAirportValues.get<std::string> ("code");
      ioAirport.airportName = iAirportValues.get<std::string> ("classical_name");
      ioAirport.cityCode = iAirportValues.get<std::string> ("city_code");
      ioAirport.cityName = iAirportValues.get<std::string> ("city_name");
      ioAirport.countryCode = iAirportValues.get<std::string> ("country_code");
      ioAirport.regionCode = iAirportValues.get<std::string> ("region_code");
      ioAirport.continentCode = iAirportValues.get<std::string> ("continent_code");
      ioAirport.timeZoneGroup = iAirportValues.get<std::string> ("time_zone_grp");
      ioAirport.longitude = iAirportValues.get<double> ("longitude");
      ioAirport.latitude = iAirportValues.get<double> ("latitude");

      // ioAirport.stateCode will be set to the default value "unknown"
      // when the column is null
      ioAirport.stateCode =
        iAirportValues.get<std::string> ("state_code", "NA");
      if (ioAirport.stateCode.empty()) {
        ioAirport.stateCode = "NA";
      }
    }


    /** Fill the database values from an Airport object. */
    static void to_base (const OpenTrep::Airport& iAirport,
                         values& ioAirportValues,
                         indicator& ioIndicator) {
      const indicator lStateCodeIndicator =
        iAirport.stateCode.empty() ? i_null : i_ok;
      ioAirportValues.set ("code", iAirport.airportCode);
      ioAirportValues.set ("classical_name", iAirport.airportName);
      ioAirportValues.set ("city_code", iAirport.cityCode);
      ioAirportValues.set ("city_name", iAirport.cityName);
      ioAirportValues.set ("state_code", iAirport.stateCode,
                           lStateCodeIndicator);
      ioAirportValues.set ("country_code", iAirport.countryCode);
      ioAirportValues.set ("region_code", iAirport.regionCode);
      ioAirportValues.set ("continent_code", iAirport.continentCode);
      ioAirportValues.set ("time_zone_grp", iAirport.timeZoneGroup);
      ioAirportValues.set ("longitude", iAirport.longitude);
      ioAirportValues.set ("latitude", iAirport.latitude);
      ioIndicator = i_ok;
    }
  };
}
#endif // __OPENTREP_DBA_DBAAIRPORT_HPP
