/*!
 * \page IndexBuildingTestSuite_cpp Command-Line Test to Demonstrate How To Test the OpenTREP Project
 * \code
 */
// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// STL
#include <cassert>
#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
// Boost Unit Test Framework (UTF)
#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MAIN
#define BOOST_TEST_MODULE IndexBuildingTestSuite
#include <boost/test/unit_test.hpp>
// OpenTrep
#include <opentrep/OPENTREP_Service.hpp>
#include <opentrep/basic/BasConst_OPENTREP_Service.hpp>
#include <opentrep/Location.hpp>
#include <opentrep/CityDetails.hpp>
#include <opentrep/config/opentrep-paths.hpp>

namespace boost_utf = boost::unit_test;

// (Boost) Unit Test XML Report
std::ofstream utfReportStream ("IndexBuildingTestSuite_utfresults.xml");

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
 * File-path of the POR (points of reference) file.
 */
const std::string K_POR_FILEPATH (OPENTREP_POR_DATA_DIR
                                  "/test_optd_por_public.csv");

/**
 * Xapian database/index file-path (directory containing the index).
 */
const std::string X_XAPIAN_DB_FP ("/tmp/opentrep/test_traveldb");

/**
 * SQL database connection string.
 */
const std::string X_SQL_DB_STR ("");

/**
 * Deployment number/version.
 */
const OPENTREP::DeploymentNumber_T X_DEPLOYMENT_NUMBER (0);

/**
 * Do not include non-IATA-referenced POR.
 */
const OPENTREP::shouldIndexNonIATAPOR_T K_ALL_POR = false;

/**
 * Index the POR in Xapian.
 */
const OPENTREP::shouldIndexPORInXapian_T K_XAPIAN_IDX = true;

/**
 * Do not add the POR in the SQL database.
 */
const OPENTREP::shouldAddPORInSQLDB_T K_SQLDB_ADD = false;


// /////////////// Main: Unit Test Suite //////////////

// Set the UTF configuration (re-direct the output to a specific file)
BOOST_GLOBAL_FIXTURE (UnitTestConfig);

// Start the test suite
BOOST_AUTO_TEST_SUITE (master_test_suite)

/**
 * Test the indexing by Xapian of a given travel-related database text file
 */
BOOST_AUTO_TEST_CASE (opentrep_simple_index) {
    
  // Output log File
  std::string lLogFilename ("IndexBuildingTestSuite.log");

  // Set the log parameters
  std::ofstream logOutputFile;
  // Open and clean the log outputfile
  logOutputFile.open (lLogFilename.c_str());
  logOutputFile.clear();

  // Initialise the context
  const OPENTREP::PORFilePath_T lPORFilePath (K_POR_FILEPATH);
  const OPENTREP::TravelDBFilePath_T lTravelDBFilePath (X_XAPIAN_DB_FP);
  const OPENTREP::DBType lDBType (OPENTREP::DBType::NODB);
  const OPENTREP::SQLDBConnectionString_T lSQLDBConnStr (X_SQL_DB_STR);
  const OPENTREP::DeploymentNumber_T lDeploymentNumber (X_DEPLOYMENT_NUMBER);
  const OPENTREP::shouldIndexNonIATAPOR_T lShouldIndexNonIATAPOR (K_ALL_POR);
  const OPENTREP::shouldIndexPORInXapian_T lShouldIndexPORInXapian(K_XAPIAN_IDX);
  const OPENTREP::shouldAddPORInSQLDB_T lShouldAddPORInSQLDB (K_SQLDB_ADD);
  OPENTREP::OPENTREP_Service opentrepService (logOutputFile, lPORFilePath,
                                              lTravelDBFilePath,
                                              lDBType, lSQLDBConnStr,
                                              lDeploymentNumber,
                                              lShouldIndexNonIATAPOR,
                                              lShouldIndexPORInXapian,
                                              lShouldAddPORInSQLDB);
  
  // Query the Xapian database (index)
  OPENTREP::WordList_T lNonMatchedWordList;
  OPENTREP::LocationList_T lLocationList;
  // Launch the indexation
  const OPENTREP::NbOfDBEntries_T nbOfEntries =
    opentrepService.insertIntoDBAndXapian();

  BOOST_CHECK_MESSAGE (nbOfEntries == 9,
                       "The Xapian index ('" << lTravelDBFilePath
                       << "') contains " << nbOfEntries
                       << " entries, where as 9 are expected.");

  // Close the Log outputFile
  logOutputFile.close();
}

// End the test suite
BOOST_AUTO_TEST_SUITE_END()

/*!
 * \endcode
 */

