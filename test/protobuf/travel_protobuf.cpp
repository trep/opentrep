// STL
#include <iostream>
// OpenTREP
#include <opentrep/Location.hpp>
#include <opentrep/bom/Travel.hpp>
#include <opentrep/service/Logger.hpp>

// ////////////// M A I N //////////////
int main (int argc, char* argv[]) {


  //
  OPENTREP::Location lLocation;
  OPENTREP::fillLocation (lLocation);

  // DEBUG
  OPENTREP_LOG_DEBUG ("Location: " << lLocation);
  
  return 0;
}
