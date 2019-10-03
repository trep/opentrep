// /////////////////////////////////////////////////////////////////////////
//
// Query slice algorithm
//
// Author: Denis Arnaud
// Date:   October 2012
//
// /////////////////////////////////////////////////////////////////////////
// STL
#include <sstream>
#include <fstream>
#include <string>
#include <list>
// Boost
#include <boost/filesystem.hpp>
// Boost Unit Test Framework (UTF)
#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MAIN
#define BOOST_TEST_MODULE SliceTestSuite
#include <boost/test/unit_test.hpp>
// OpenTrep
#include <opentrep/basic/OTransliterator.hpp>
#include <opentrep/bom/QuerySlices.hpp>
#include <opentrep/basic/BasConst_OPENTREP_Service.hpp>
#include <opentrep/OPENTREP_Service.hpp>
#include <opentrep/service/Logger.hpp>

namespace boost_utf = boost::unit_test;

// (Boost) Unit Test XML Report
std::ofstream utfReportStream ("SliceTestSuite_utfresults.xml");

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
 * Test the sliceing algorithm with a small string
 */
BOOST_AUTO_TEST_CASE (slice_small_string) {

  // Output log File
  const std::string lLogFilename ("SliceTestSuite.log");

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
  
  // A few sample strings
  const std::string lLax1Str = "los angeles";
  const std::string lLax2Str = "lso angeles";
  const std::string lRio1Str = "rio de janeiro";
  const std::string lRio2Str = "rio de janero";
  const std::string lRek1Str = "reikjavik";
  const std::string lRek2Str = "rekyavik";
  const std::string lLwoIevHrk1Str = "lviv kiev kharkov";
  const std::string lSfoRio1Str = "san francisco rio de janeiro";
  const std::string lSfoRio2Str = "san francicso rio de janero";
  const std::string lSfoRio3Str = "sna francicso rio de janero";
  const std::string lChelseaStr = "chelsea municipal airport";

  /**
   * Direct access
   */

  // Open the Xapian database (the deployment number/version is added to the
  // file-path
  std::ostringstream oStr;
  oStr << lTravelDBFilePath << lDeploymentNumber;
  Xapian::Database lXapianDatabase (oStr.str());

  // Create a Unicode transliterator
  const OPENTREP::OTransliterator lTransliterator;
      
  // Create the query slices
  OPENTREP::QuerySlices lQuerySlices (lXapianDatabase, lLwoIevHrk1Str,
                                      lTransliterator);

  // DEBUG
  OPENTREP_LOG_DEBUG (lQuerySlices.size() << " slices: "
                      << lQuerySlices.describe());

  //
  BOOST_CHECK_MESSAGE (lQuerySlices.size() == 3,
                       "The query ('" << lLwoIevHrk1Str
                       << "') should contain 3 slices."
                       << " However, its size is " << lQuerySlices.size()
                       << ".");

  // Create other query slices
  OPENTREP::QuerySlices lAnotherQuerySlices (lXapianDatabase, lChelseaStr,
                                             lTransliterator);

  // DEBUG
  OPENTREP_LOG_DEBUG (lAnotherQuerySlices.size() << " slices: "
                      << lAnotherQuerySlices.describe());

  //
  BOOST_CHECK_MESSAGE (lAnotherQuerySlices.size() == 3,
                       "The query ('" << lChelseaStr
                       << "') should contain 1 slice."
                       << " However, its size is " << lAnotherQuerySlices.size()
                       << ".");

  // Close the Log outputFile
  logOutputFile.close();
}

// End the test suite
BOOST_AUTO_TEST_SUITE_END()

