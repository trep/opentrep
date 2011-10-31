// STL
#include <fstream>
#include <string>
// CPPUNIT
#include <extracppunit/CppUnitCore.hpp>
// OpenTrep
#include <opentrep/OPENTREP_Service.hpp>
#include <opentrep/Location.hpp>
// OpenTrep Test Suite
#include <test/IndexBuildingTestSuite.hpp>

// //////////////////////////////////////////////////////////////////////
void testIndexBuildingHelper() {
  // Output log File
  std::string lLogFilename ("IndexBuildingTestSuite.log");

  // Xapian database name (directory of the index)
  std::string lXapianDatabaseName ("traveldb");
  
  // Travel query
  std::string lTravelQuery ("cdg");
    
  // Set the log parameters
  std::ofstream logOutputFile;
  // Open and clean the log outputfile
  logOutputFile.open (lLogFilename.c_str());
  logOutputFile.clear();

  // SQL database parameters
  OPENTREP::DBParams lDBParams ("opentrep", "opentrep", "localhost", "3306",
                                "trep_opentrep");
  
  // Initialise the context
  OPENTREP::OPENTREP_Service opentrepService  (logOutputFile, lDBParams,
                                               lXapianDatabaseName);
  
  // Launch a simulation
  //opentrepService.buildSearchIndex();
  
  // Query the Xapian database (index)
  OPENTREP::WordList_T lNonMatchedWordList;
  OPENTREP::LocationList_T lLocationList;
  //  const OPENTREP::NbOfMatches_T nbOfMatches =
    opentrepService.interpretTravelRequest (lTravelQuery, lLocationList,
                                            lNonMatchedWordList);
  
  // Close the Log outputFile
  logOutputFile.close();
}

// //////////////////////////////////////////////////////////////////////
void IndexBuildingTestSuite::testIndexBuilding() {
  CPPUNIT_ASSERT_NO_THROW (testIndexBuildingHelper(););
}

// //////////////////////////////////////////////////////////////////////
// void IndexBuildingTestSuite::errorCase () {
//  CPPUNIT_ASSERT (false);
// }

// //////////////////////////////////////////////////////////////////////
IndexBuildingTestSuite::IndexBuildingTestSuite () {
  _describeKey << "Running test on Xapian database (index) building";  
}

// /////////////// M A I N /////////////////
CPPUNIT_MAIN()
