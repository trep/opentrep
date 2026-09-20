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
#include <opentrep/basic/Utilities.hpp>
#include <opentrep/bom/PORParserHelper.hpp>
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
                                  "/csv/test-optd-por-public.csv");

/**
 * File-path of a single real-world OPTD POR record (QSW, As-Suwayda,
 * Syria) whose Japanese ('ja') alternate name contains a literal '='
 * character ('アス=スワイダ'), used as a regression fixture (see
 * opentrep_alt_name_with_embedded_equals_and_valid_lang_code below).
 */
const std::string K_POR_FILEPATH_QSW (OPENTREP_POR_DATA_DIR
                                      "/csv/test-optd-por-qsw-as-suwayda.csv");

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

/**
 * Check that the default PostgreSQL connection targets the provisioned
 * deployment-slot database (trep0 for deployment 0).
 */
BOOST_AUTO_TEST_CASE (opentrep_default_pg_connection_string) {
  const OPENTREP::SQLDBConnectionString_T lDefaultConnection (
    OPENTREP::DEFAULT_OPENTREP_PG_CONN_STRING);
  const OPENTREP::StringMap_T lDefaultParams =
    OPENTREP::parsePGConnectionString (lDefaultConnection);

  BOOST_REQUIRE (lDefaultParams.find ("dbname") != lDefaultParams.end());
  BOOST_REQUIRE (lDefaultParams.find ("host") != lDefaultParams.end());
  BOOST_CHECK_EQUAL (lDefaultParams.find ("dbname")->second, "trep");
  BOOST_CHECK_EQUAL (lDefaultParams.find ("host")->second, "localhost");

  const OPENTREP::SQLDBConnectionString_T lDeploymentConnection =
    OPENTREP::buildPGConnectionString (lDefaultParams, 0);
  const OPENTREP::StringMap_T lDeploymentParams =
    OPENTREP::parsePGConnectionString (lDeploymentConnection);

  BOOST_REQUIRE (lDeploymentParams.find ("dbname") != lDeploymentParams.end());
  BOOST_REQUIRE (lDeploymentParams.find ("host") != lDeploymentParams.end());
  BOOST_CHECK_EQUAL (lDeploymentParams.find ("dbname")->second, "trep0");
  BOOST_CHECK_EQUAL (lDeploymentParams.find ("host")->second, "localhost");
}

/**
 * Alternate names may legitimately contain '=' characters. They must not be
 * mistaken for separators between language/name entries.
 */
BOOST_AUTO_TEST_CASE (opentrep_alt_name_with_equals_sign) {
  std::ifstream lPORFile (K_POR_FILEPATH.c_str());
  BOOST_REQUIRE (lPORFile.good());

  std::string lHeader;
  std::string lPORRecord;
  std::getline (lPORFile, lHeader);
  std::getline (lPORFile, lPORRecord);

  const std::string lOriginalAltName ("ja|ケプラヴィーク国際空港|");
  const std::string lAltNameWithEquals ("ja|ケプラ=ヴィーク国際空港|");
  const std::string::size_type lAltNamePos =
    lPORRecord.find (lOriginalAltName);
  BOOST_REQUIRE (lAltNamePos != std::string::npos);
  lPORRecord.replace (lAltNamePos, lOriginalAltName.size(),
                      lAltNameWithEquals);

  OPENTREP::PORStringParser lPORParser (lPORRecord);
  const OPENTREP::Location& lLocation = lPORParser.generateLocation();
  BOOST_CHECK_EQUAL (lLocation.getIataCode(), "KEF");
}

/**
 * Real-world regression: the QSW (As-Suwayda, Syria) OPTD POR record has
 * a Japanese ('ja') alternate name containing a literal '=' character
 * ('アス=スワイダ'). Unlike the synthetic case above, here the '='
 * follows a *valid* two-letter ASCII language code ('ja') and a '|'
 * separator, so this exercises the alt_name_details/alt_name_section
 * boundary (list-splitting on '=') just as much as the alt_name rule's
 * own embedded-'=' guard. This record was seen to crash indexing in
 * production with the pre-fix grammar (opentraveldata/opentraveldata
 * has since also been notified, but this is a parser robustness issue,
 * not a data error).
 */
BOOST_AUTO_TEST_CASE (opentrep_alt_name_with_embedded_equals_and_valid_lang_code) {
  std::ifstream lPORFile (K_POR_FILEPATH_QSW.c_str());
  BOOST_REQUIRE (lPORFile.good());

  std::string lHeader;
  std::string lPORRecord;
  std::getline (lPORFile, lHeader);
  std::getline (lPORFile, lPORRecord);
  BOOST_REQUIRE (lPORRecord.find ("ja|アス=スワイダ|") != std::string::npos);

  OPENTREP::PORStringParser lPORParser (lPORRecord);
  const OPENTREP::Location& lLocation = lPORParser.generateLocation();
  BOOST_CHECK_EQUAL (lLocation.getIataCode(), "QSW");
}

/**
 * An empty alternate-name section must stop at its '^' field delimiter,
 * even when a later field contains '=' separators.
 */
BOOST_AUTO_TEST_CASE (opentrep_empty_alt_name_before_equals_sign) {
  std::ifstream lPORFile (K_POR_FILEPATH.c_str());
  BOOST_REQUIRE (lPORFile.good());

  std::string lHeader;
  std::string lPORRecord;
  std::getline (lPORFile, lHeader);
  std::getline (lPORFile, lPORRecord);

  std::vector<std::string> lFields;
  std::stringstream lRecordStream (lPORRecord);
  std::string lField;
  while (std::getline (lRecordStream, lField, '^')) {
    lFields.push_back (lField);
  }
  BOOST_REQUIRE (lFields.size() == 51);
  lFields[43] = "";
  lFields[47] = "AUBWS|=AUWW3|";

  std::ostringstream lModifiedRecord;
  for (std::vector<std::string>::const_iterator itField = lFields.begin();
       itField != lFields.end(); ++itField) {
    if (itField != lFields.begin()) {
      lModifiedRecord << '^';
    }
    lModifiedRecord << *itField;
  }

  OPENTREP::PORStringParser lPORParser (lModifiedRecord.str());
  const OPENTREP::Location& lLocation = lPORParser.generateLocation();
  BOOST_CHECK_EQUAL (lLocation.getIataCode(), "KEF");
}

/**
 * Xapian spelling keys include an internal one-byte prefix and therefore
 * cannot store UTF-8 terms longer than 254 bytes.
 */
BOOST_AUTO_TEST_CASE (opentrep_long_spelling_term) {
  std::ifstream lPORFile (K_POR_FILEPATH.c_str());
  BOOST_REQUIRE (lPORFile.good());

  std::string lHeader;
  std::string lPORRecord;
  std::getline (lPORFile, lHeader);
  std::getline (lPORFile, lPORRecord);

  const std::string lOriginalAltName ("ja|ケプラヴィーク国際空港|");
  const std::string lLongAltName ("ja|" + std::string (300, 'a') + "|");
  const std::string::size_type lAltNamePos =
    lPORRecord.find (lOriginalAltName);
  BOOST_REQUIRE (lAltNamePos != std::string::npos);
  lPORRecord.replace (lAltNamePos, lOriginalAltName.size(), lLongAltName);

  const std::string lPORFilepath ("/tmp/opentrep-long-spelling-term.csv");
  std::ofstream lModifiedPORFile (lPORFilepath.c_str());
  BOOST_REQUIRE (lModifiedPORFile.good());
  lModifiedPORFile << lHeader << '\n' << lPORRecord << '\n';
  lModifiedPORFile.close();

  std::ofstream lLogFile ("/tmp/opentrep-long-spelling-term.log");
  OPENTREP::OPENTREP_Service lService (
    lLogFile, OPENTREP::PORFilePath_T (lPORFilepath),
    OPENTREP::TravelDBFilePath_T ("/tmp/opentrep-long-spelling-term-xapian"),
    OPENTREP::DBType (OPENTREP::DBType::NODB),
    OPENTREP::SQLDBConnectionString_T (""), 0, true, true, false);

  BOOST_CHECK_EQUAL (lService.insertIntoDBAndXapian(), 1);
}

// End the test suite
BOOST_AUTO_TEST_SUITE_END()

/*!
 * \endcode
 */
