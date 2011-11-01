/*!
 * \page SearchBuildingTestSuite_cpp Command-Line Test to Demonstrate How To Test the OpenTREP Project
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
#define BOOST_TEST_MODULE SearchBuildingTestSuite
#include <boost/test/unit_test.hpp>
// CPPUNIT
#include <extracppunit/CppUnitCore.hpp>
// OpenTrep
#include <opentrep/OPENTREP_Service.hpp>
#include <opentrep/Location.hpp>

namespace boost_utf = boost::unit_test;

// (Boost) Unit Test XML Report
std::ofstream utfReportStream ("SearchBuildingTestSuite_utfresults.xml");

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
const std::string K_MYSQL_DB_DBNAME ("geo_geonames");


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
  std::string lLogFilename ("SearchBuildingTestSuite.log");

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
  OPENTREP::OPENTREP_Service opentrepService  (logOutputFile, lDBParams,
                                               K_XAPIAN_DB_NAME);
  
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

