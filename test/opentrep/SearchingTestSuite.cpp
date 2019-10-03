/*!
 * \page SearchingTestSuite_cpp Command-Line Test to Demonstrate How To Test the OpenTREP Project
 * \code
 */
// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// STL
#include <sstream>
#include <fstream>
#include <string>
// Boost Unit Test Framework (UTF)
#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MAIN
#define BOOST_TEST_MODULE SearchingTestSuite
#include <boost/test/unit_test.hpp>
// OpenTrep
#include <opentrep/OPENTREP_Service.hpp>
#include <opentrep/basic/BasConst_OPENTREP_Service.hpp>
#include <opentrep/Location.hpp>
#include <opentrep/CityDetails.hpp>

namespace boost_utf = boost::unit_test;

// (Boost) Unit Test XML Report
std::ofstream utfReportStream ("SearchingTestSuite_utfresults.xml");

/**
 * Configuration for the Boost Unit Test Framework (UTF)
 */
struct UnitTestConfig {
  /** Constructor. */
  UnitTestConfig() {
    boost_utf::unit_test_log.set_stream (utfReportStream);
#if defined(BOOST_VERSION) && BOOST_VERSION >= 105900
    boost_utf::unit_test_log.set_format (boost_utf::OF_XML);
#else // BOOST_VERSION
    boost_utf::unit_test_log.set_format (boost_utf::XML);
#endif // BOOST_VERSION    
    boost_utf::unit_test_log.set_threshold_level (boost_utf::log_test_units);
    //boost_utf::unit_test_log.set_threshold_level (boost_utf::log_successful_tests);
  }

  /** Destructor. */
  ~UnitTestConfig() {
  }
};


// //////////// Constants for the tests ///////////////
/**
 * Xapian database/index file-path (directory containing the index).
 */
const std::string X_XAPIAN_DB_FP ("/tmp/opentrep/test_traveldb");

/**
 * SQL database connection string.
 */
const std::string X_SQL_DB_STR ("");

/*
 * Deployment number/version.
 */
const OPENTREP::DeploymentNumber_T X_DEPLOYMENT_NUMBER (0);

// /////////////// Main: Unit Test Suite //////////////

// Set the UTF configuration (re-direct the output to a specific file)
BOOST_GLOBAL_FIXTURE (UnitTestConfig);

// Start the test suite
BOOST_AUTO_TEST_SUITE (master_test_suite)

/**
 * Test a simple travel search on the Xapian index just created above
 */
BOOST_AUTO_TEST_CASE (opentrep_simple_search) {
    
  // Output log File
  std::string lLogFilename ("SearchingTestSuite.log");

  // Travel query
  std::string lTravelQuery ("nce");
    
  // Set the log parameters
  std::ofstream logOutputFile;
  // Open and clean the log outputfile
  logOutputFile.open (lLogFilename.c_str());
  logOutputFile.clear();

  // Initialise the context
  const OPENTREP::TravelDBFilePath_T lTravelDBFilePath (X_XAPIAN_DB_FP);
  const OPENTREP::DBType lDBType (OPENTREP::DBType::NODB);
  const OPENTREP::SQLDBConnectionString_T lSQLDBConnStr (X_SQL_DB_STR);
  const OPENTREP::DeploymentNumber_T lDeploymentNumber (X_DEPLOYMENT_NUMBER);
  OPENTREP::OPENTREP_Service opentrepService (logOutputFile, lTravelDBFilePath,
                                              lDBType, lSQLDBConnStr,
                                              lDeploymentNumber);
  
  // Query the Xapian database (index)
  OPENTREP::WordList_T lNonMatchedWordList;
  OPENTREP::LocationList_T lLocationList;
  const OPENTREP::NbOfMatches_T nbOfMatches =
    opentrepService.interpretTravelRequest (lTravelQuery, lLocationList,
                                            lNonMatchedWordList);
  BOOST_CHECK_MESSAGE (nbOfMatches == 1,
                       "The travel query ('" << lTravelQuery
                       << "') matches with " << nbOfMatches
                       << " key-words, whereas 1 is expected.");
  
  // Close the Log outputFile
  logOutputFile.close();
}

// End the test suite
BOOST_AUTO_TEST_SUITE_END()

/*!
 * \endcode
 */

