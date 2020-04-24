// STL
#include <iostream>
// OpenTrep Protobuf
#include <opentrep/Travel.pb.h>
// OpenTrep
#include <opentrep/Location.hpp>
#include <opentrep/CityDetails.hpp>
#include <opentrep/bom/LocationExchange.hpp>
#include <opentrep/service/Logger.hpp>

// ////////////// M A I N //////////////
int main (int argc, char* argv[]) {

  // Verify that the version of the library that we linked against is
  // compatible with the version of the headers we compiled against.
  GOOGLE_PROTOBUF_VERIFY_VERSION;
  
  // Build the POR (point of reference) corresponding to Kiev Boryspil
  const OPENTREP::LocationKey lLocationKey (OPENTREP::IATACode_T ("KBP"),
                                            OPENTREP::IATAType ("A"),
                                            OPENTREP::GeonamesID_T (6300952));
  // Build the list of non-matching keywords
  const std::string lNonMatchedKeyword ("zorglub");
  OPENTREP::WordList_T lNonMatchedWordList;
  lNonMatchedWordList.push_back (lNonMatchedKeyword);

  // Build a Location structure to hold the POR details
  OPENTREP::Location lLocation;
  lLocation.setKey (lLocationKey);
  OPENTREP::LocationList_T lLocationList;
  lLocationList.push_back (lLocation);

  // Serialise the Location structure with Protobuf
  std::ostringstream oStr;
  oStr << OPENTREP::LocationExchange::exportLocationList (lLocationList,
                                                          lNonMatchedWordList);
  const std::string& lSerialisedLocation = oStr.str();

  // DEBUG
  OPENTREP_LOG_DEBUG ("Serialised location: " << lSerialisedLocation);
  
  // Optional:  Delete all global objects allocated by libprotobuf.
  google::protobuf::ShutdownProtobufLibrary();

  //
  return 0;
}
