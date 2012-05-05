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
#include <opentrep/Location.hpp>

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
    boost_utf::unit_test_log.set_format (boost_utf::XML);
    boost_utf::unit_test_log.set_threshold_level (boost_utf::log_test_units);
    //boost_utf::unit_test_log.set_threshold_level (boost_utf::log_successful_tests);
  }

  /** Destructor. */
  ~UnitTestConfig() {
  }
};


// //////////// Constants for the tests ///////////////
/** Xapian database name (directory containing the index). */
const std::string K_XAPIAN_DB_NAME ("traveldb");
/** The airport/city details are specified within a MySQL database.
    The connection parameters are specified below. */
const std::string K_MYSQL_DB_USER ("geo");
const std::string K_MYSQL_DB_PASSWD ("geo");
const std::string K_MYSQL_DB_HOSTNAME ("localhost");
const std::string K_MYSQL_DB_PORT ("3306");
const std::string K_MYSQL_DB_DBNAME ("geo_trep");


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

  // Travel query
  std::string lTravelQuery ("cdg");
    
  // Set the log parameters
  std::ofstream logOutputFile;
  // Open and clean the log outputfile
  logOutputFile.open (lLogFilename.c_str());
  logOutputFile.clear();

  // SQL database parameters
  OPENTREP::DBParams lDBParams (K_MYSQL_DB_USER, K_MYSQL_DB_PASSWD,
                                K_MYSQL_DB_HOSTNAME, K_MYSQL_DB_PORT,
                                K_MYSQL_DB_DBNAME);
  
  // Initialise the context
  const OPENTREP::TravelDatabaseName_T lXapianDatabaseName (K_XAPIAN_DB_NAME);
  OPENTREP::OPENTREP_Service opentrepService  (logOutputFile, lDBParams,
                                               lXapianDatabaseName);
  
  // Query the Xapian database (index)
  OPENTREP::WordList_T lNonMatchedWordList;
  OPENTREP::LocationList_T lLocationList;
  // Launch the indexation
  const OPENTREP::NbOfDBEntries_T nbOfEntries =
    opentrepService.buildSearchIndex();

  BOOST_CHECK_MESSAGE (nbOfEntries == 11133,
                       "The Xapian index ('" << lXapianDatabaseName
                       << "') contains " << nbOfEntries
                       << " entries, whereas 11133 are expected.");

  // DEBUG
  std::cout << nbOfEntries << " entries have been created" << std::endl;

  // Close the Log outputFile
  logOutputFile.close();
}

// End the test suite
BOOST_AUTO_TEST_SUITE_END()

/*!
 * \endcode
 */

