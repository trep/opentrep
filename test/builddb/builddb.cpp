// STL
#include <iostream>
#include <istream>
#include <ostream>
#include <string>
#include <vector>
#include <exception>
// OpenTrep
#include "Airport.hpp"
// SOCI
#include <soci.h>
#include <soci-mysql.h>
#include "DbaAirport.hpp"
// Xapian
#include <xapian.h>

// /////// Type definitions ///////
typedef std::vector<std::string> AirportCodeList_T;
typedef std::vector<std::string> CityCodeList_T;
typedef std::vector<std::string> StateCodeList_T;
typedef std::vector<soci::indicator> StateCodeIndicatorList_T;
typedef std::vector<std::string> CountryCodeList_T;
typedef std::vector<std::string> OverrideList_T;

// ///////////////////////////////////////////////////
bool getName (std::string& ioCityCode) {
  std::cout << "Enter city code: ";
  return std::cin >> ioCityCode;
}

// ///////////////////////////////////////////////////
int main (int argc, char* argv[]) {

  // Database parameters
  const std::string lUserName ("opentrep");
  const std::string lPassword ("opentrep");
  const std::string lDBName ("opentrep");
  const std::string lDBPort ("3306");
  const std::string lDBHost ("localhost");
  std::ostringstream oStr;
  oStr << "db=" << lDBName << " user=" << lUserName
       << " password=" << lPassword << " port=" << lDBPort
       << " host=" << lDBHost;
  const std::string lSociSessionConnectionString (oStr.str());
  soci::session lSociSession;
  
  try {

    // Open the connection to the database
    lSociSession.open (soci::mysql, lSociSessionConnectionString);

  } catch (std::exception const& lException) {
    std::cerr << "Error while opening a connection to database: "
              << lException.what() << std::endl;
  }

  // Get the city code from the standard input
  // std::string lInputCityCode;
  // getName (lInputCityCode);

  // Instanciate an empty airport object, which will be filled from the
  // rows retrieved from the database.
  OpenTrep::Airport lAirport;
  
  try {
    
    // Instanciate a SQL statement (no request is performed at that stage)
    /* For debugging purposes
    soci::statement lStatement =
      (lSociSession.prepare
       << "select code, city_code, state_code, country_code, override "
       << "from dic_codes where city_code = :city_code",
       soci::into (lAirport), soci::use (lInputCityCode));
    */

    soci::statement lStatement =
      (lSociSession.prepare
       << "select code, classical_name, city_code, city_name, state_code, "
       << "country_code, region_code, continent_code, time_zone_grp, "
       << "longitude, latitude "
       << "from ref_city", soci::into (lAirport));

    // Execute the SQL query
    lStatement.execute();

    // Create the Xapian database
    const std::string lTravelDB ("traveldb");
    Xapian::WritableDatabase lDatabase (lTravelDB,
                                        Xapian::DB_CREATE_OR_OVERWRITE);

    // Iterate through the retrieved database rows
    int idx = 0;
    while (lStatement.fetch()) {
      // Debug
      std::cout << "[" << idx << "] " << lAirport << std::endl;

      // Build a Xapian document
      Xapian::Document lDocument;
    
      // The document contains the airport code
      lDocument.set_data (lAirport.toString());
      
      // Put the search terms into the document
      lDocument.add_posting (lAirport.airportCode, 1);
      lDocument.add_posting (lAirport.airportName, 2);
      lDocument.add_posting (lAirport.cityCode, 3);
      lDocument.add_posting (lAirport.cityName, 4);
      lDocument.add_posting (lAirport.stateCode, 5);
      lDocument.add_posting (lAirport.countryCode, 6);
      lDocument.add_posting (lAirport.regionCode, 7);
      lDocument.add_posting (lAirport.continentCode, 8);
      lDocument.add_posting (lAirport.timeZoneGroup, 9);
      std::ostringstream oLongitudeStr;
      oLongitudeStr << lAirport.longitude;
      const std::string lLongitudeString (oLongitudeStr.str());
      lDocument.add_posting (lLongitudeString, 10);
      std::ostringstream oLatitudeStr;
      oLatitudeStr << lAirport.latitude;
      const std::string lLatitudeString (oLatitudeStr.str());
      lDocument.add_posting (lLatitudeString, 11);
      
      // Add the document to the database
      lDatabase.add_document (lDocument);
      
      // Index incrementing
      ++idx;
    }
    
  } catch (const Xapian::Error& error) {
    std::cerr << "Exception: "  << error.get_msg() << std::endl;

  } catch (std::exception const& lException) {
    std::cerr << "Error: " << lException.what() << std::endl;
  }

  return 0;
}
