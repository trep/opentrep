// /////////////////////////////////////////////////////////////////////////
//
// String partition algorithm
//
// Author: Denis Arnaud
// Date:   April 2012
//
// /////////////////////////////////////////////////////////////////////////
// STL
#include <sstream>
#include <fstream>
#include <string>
#include <list>
// Boost Unit Test Framework (UTF)
#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MAIN
#define BOOST_TEST_MODULE PartitionTestSuite
#include <boost/test/unit_test.hpp>
// OpenTrep
#include <opentrep/bom/StringPartition.hpp>

namespace boost_utf = boost::unit_test;

// (Boost) Unit Test XML Report
std::ofstream utfReportStream ("PartitionTestSuite_utfresults.xml");

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


// /////////////// Main: Unit Test Suite //////////////

// Set the UTF configuration (re-direct the output to a specific file)
BOOST_GLOBAL_FIXTURE (UnitTestConfig);

// Start the test suite
BOOST_AUTO_TEST_SUITE (master_test_suite)

/**
 * Test the partitioning algorithm with a small string
 */
BOOST_AUTO_TEST_CASE (partition_small_string) {

  // Output log File
  std::string lLogFilename ("PartitionTestSuite.log");

  // Set the log parameters
  std::ofstream logOutputFile;
  // Open and clean the log outputfile
  logOutputFile.open (lLogFilename.c_str());
  logOutputFile.clear();

  const std::string lLax1Str = "los angeles";
  const std::string lLax2Str = "lso angeles";
  const std::string lRio1Str = "rio de janeiro";
  const std::string lRio2Str = "rio de janero";
  const std::string lRek1Str = "reikjavik";
  const std::string lRek2Str = "rekyavik";
  const std::string lSfoRio1Str = "san francisco rio de janeiro";
  const std::string lSfoRio2Str = "san francicso rio de janero";
  const std::string lSfoRio3Str = "sna francicso rio de janero";
  const std::string lChelseaStr = "chelsea municipal airport";

  //
  OPENTREP::StringPartition lStringPartition (lSfoRio3Str);
  logOutputFile << lStringPartition << std::endl;

  BOOST_CHECK_MESSAGE (lStringPartition.size() == 16,
                       "The partition, for '" << lRio2Str
                       << "', should contain a single list."
                       << " However, its size is " << lStringPartition.size()
                       << ".");

  //
  OPENTREP::StringPartition lAnotherStringPartition (lChelseaStr);
  logOutputFile << lAnotherStringPartition << std::endl;

  // Close the Log outputFile
  logOutputFile.close();
}

// End the test suite
BOOST_AUTO_TEST_SUITE_END()

