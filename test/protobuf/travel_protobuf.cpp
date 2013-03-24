// STL
#include <iostream>
// OpenTREP
#include <opentrep/Location.hpp>
#include <opentrep/bom/LocationExchange.hpp>
#include <opentrep/service/Logger.hpp>

// ////////////// M A I N //////////////
int main (int argc, char* argv[]) {


  // Build the POR (point of reference) corresponding to Kiev Boryspil
  const OPENTREP::LocationKey lLocationKey (OPENTREP::IATACode_T ("KBP"),
                                            OPENTREP::ICAOCode_T ("UKBB"),
                                            OPENTREP::GeonamesID_T (6300952));

  // Build a Location structure to hold the POR details
  OPENTREP::Location lLocation;
  lLocation.setKey (lLocationKey);

  // Serialise the Location structure with Protobuf
  const std::string& lSerialisedLocation =
    OPENTREP::LocationExchange::exportLocation (lLocation);

  // DEBUG
  OPENTREP_LOG_DEBUG ("Serialised location: " << lSerialisedLocation);
  
  return 0;
}
