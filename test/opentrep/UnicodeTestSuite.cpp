// /////////////////////////////////////////////////////////////////////////
//
// Unicode-based transformations
//
// Author: Denis Arnaud
// Date:   September 2012
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
#define BOOST_TEST_MODULE UnicodeTestSuite
#include <boost/test/unit_test.hpp>
// OpenTrep
#include <opentrep/basic/OTransliterator.hpp>

namespace boost_utf = boost::unit_test;

// (Boost) Unit Test XML Report
std::ofstream utfReportStream ("UnicodeTestSuite_utfresults.xml");

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
 * Test the Unicode transformation algorithm
 */
BOOST_AUTO_TEST_CASE (unicode_strings) {

  // Output log File
  std::string lLogFilename ("UnicodeTestSuite.log");

  // Set the log parameters
  std::ofstream logOutputFile;
  // Open and clean the log outputfile
  logOutputFile.open (lLogFilename.c_str());
  logOutputFile.clear();

  // Unicode transliterator
  OPENTREP::OTransliterator lTransliterator;

  // European
  const std::string lFRStr = "À côté de Nice Côte d'Azur";
  const std::string lFRTranslitStr = lTransliterator.normalise (lFRStr);
  const std::string lFRTargetStr = "a cote de nice cote d azur";
  const std::string lHUStr = "San Franciscó-i nemzetközi repülőtér";
  const std::string lHUTranslitStr = lTransliterator.normalise (lHUStr);
  const std::string lHUTargetStr = "san francisco i nemzetkozi repuloter";
  const std::string lGRStr = "Αλφαβητικός Κατάλογος";
  const std::string lGRTranslitStr = lTransliterator.normalise (lGRStr);
  const std::string lGRTargetStr = "alphabetikos katalogos";
  const std::string lRUStr = "Аэропорт «Аннаба», Биологическом";
  const std::string lRUTranslitStr = lTransliterator.normalise (lRUStr);
  const std::string lRUTargetStr = "aeroport annaba biologiceskom";
  const std::string lUKStr = "Биологїческом";
  const std::string lUKTranslitStr = lTransliterator.normalise (lUKStr);
  const std::string lUKTargetStr = "biologiceskom";
  const std::string lTRStr = "San Francisco Uluslararası Havalimanı";
  const std::string lTRTranslitStr = lTransliterator.normalise (lTRStr);
  const std::string lTRTargetStr = "san francisco uluslararası havalimanı";

  // Africa - Middle-East
  const std::string lARStr = "مطار سان فرانسيسكو الدولي";
  const std::string lARTranslitStr = lTransliterator.normalise (lARStr);
  const std::string lARTargetStr = "mtar san fransyskw aldwly";
  const std::string lFAStr = "فرودگاه بین المللی سانفرانسیسکو";
  const std::string lFATranslitStr = lTransliterator.normalise (lFAStr);
  const std::string lFATargetStr = "frwdgah byn almlly sanfransyskw";

  // Asia
  const std::string lZHStr = "舊金山國際機場";
  const std::string lZHTranslitStr = lTransliterator.normalise (lZHStr);
  const std::string lZHTargetStr = "jiu jin shan guo ji ji chang";
  const std::string lJPStr = "サンフランシスコ国際空港";
  const std::string lJPTranslitStr = lTransliterator.normalise (lJPStr);
  const std::string lJPTargetStr = "sanfuranshisuko guo ji kong gang";
  const std::string lKOStr = "샌프란시스코 국제공항";
  const std::string lKOTranslitStr = lTransliterator.normalise (lKOStr);
  const std::string lKOTargetStr = "saenpeulansiseuko gugjegonghang";
  const std::string lTHStr = "ท่าอากาศยานนานาชาติซานฟรานซิสโก";
  const std::string lTHTranslitStr = lTransliterator.normalise (lTHStr);
  const std::string lTHTargetStr = "tha xakasyan nana chat san f ran ss ko";
  const std::string lVIStr = "Sân bay quốc tế San Francisco";
  const std::string lVITranslitStr = lTransliterator.normalise (lVIStr);
  const std::string lVITargetStr = "san bay quoc te san francisco";

  // French (accentuated Latin)
  logOutputFile << "[FR] " << lFRStr << " -> " << lFRTranslitStr << std::endl;
  BOOST_CHECK_MESSAGE (lFRTranslitStr == lFRTargetStr,
                       "The French transliterated string for '" << lFRStr
                       << "' should be '" << lFRTargetStr << "'. "
                       << "However, it is '" << lFRTranslitStr << "'.");

  // Hungarian (accentuated Latin)
  logOutputFile << "[HU] " << lHUStr << " -> " << lHUTranslitStr << std::endl;
  BOOST_CHECK_MESSAGE (lHUTranslitStr == lHUTargetStr,
                       "The Hungarian transliterated string for '" << lHUStr
                       << "' should be '" << lHUTargetStr << "'. "
                       << "However, it is '" << lHUTranslitStr << "'.");

  // Greek
  logOutputFile << "[GR] " << lGRStr << " -> " << lGRTranslitStr << std::endl;
  BOOST_CHECK_MESSAGE (lGRTranslitStr == lGRTargetStr,
                       "The Greek transliterated string for '" << lGRStr
                       << "' should be '" << lGRTargetStr << "'. "
                       << "However, it is '" << lGRTranslitStr << "'.");

  // Russian (Cyrillic)
  logOutputFile << "[RU] " << lRUStr << " -> " << lRUTranslitStr << std::endl;
  BOOST_CHECK_MESSAGE (lRUTranslitStr == lRUTargetStr,
                       "The Russian transliterated string for '" << lRUStr
                       << "' should be '" << lRUTargetStr << "'. "
                       << "However, it is '" << lRUTranslitStr << "'.");

  // Ukrainian (Cyrillic)
  logOutputFile << "[UK] " << lUKStr << " -> " << lUKTranslitStr << std::endl;
  BOOST_CHECK_MESSAGE (lUKTranslitStr == lUKTargetStr,
                       "The Ukrainian transliterated string for '" << lUKStr
                       << "' should be '" << lUKTargetStr << "'. "
                       << "However, it is '" << lUKTranslitStr << "'.");

  // Turkish
  logOutputFile << "[TR] " << lTRStr << " -> " << lTRTranslitStr << std::endl;
  BOOST_CHECK_MESSAGE (lTRTranslitStr == lTRTargetStr,
                       "The Turkish transliterated string for '" << lTRStr
                       << "' should be '" << lTRTargetStr << "'. "
                       << "However, it is '" << lTRTranslitStr << "'.");

  // Arabic
  logOutputFile << "[AR] " << lARStr << " -> " << lARTranslitStr << std::endl;
  BOOST_CHECK_MESSAGE (lARTranslitStr == lARTargetStr,
                       "The Arabic transliterated string for '" << lARStr
                       << "' should be '" << lARTargetStr << "'. "
                       << "However, it is '" << lARTranslitStr << "'.");

  // Iranian (Farsi)
  logOutputFile << "[FA] " << lFAStr << " -> " << lFATranslitStr << std::endl;
  BOOST_CHECK_MESSAGE (lFATranslitStr == lFATargetStr,
                       "The Arabic transliterated string for '" << lFAStr
                       << "' should be '" << lFATargetStr << "'. "
                       << "However, it is '" << lFATranslitStr << "'.");

  // Chinese (Han)
  logOutputFile << "[ZH] " << lZHStr << " -> " << lZHTranslitStr << std::endl;
  BOOST_CHECK_MESSAGE (lZHTranslitStr == lZHTargetStr,
                       "The Chinese transliterated string for '" << lZHStr
                       << "' should be '" << lZHTargetStr << "'. "
                       << "However, it is '" << lZHTranslitStr << "'.");

  // Japanese (Katakana)
  logOutputFile << "[JP] " << lJPStr << " -> " << lJPTranslitStr << std::endl;
  BOOST_CHECK_MESSAGE (lJPTranslitStr == lJPTargetStr,
                       "The Japanese transliterated string for '" << lJPStr
                       << "' should be '" << lJPTargetStr << "'. "
                       << "However, it is '" << lJPTranslitStr << "'.");

  // Korean (Hangul)
  logOutputFile << "[KO] " << lKOStr << " -> " << lKOTranslitStr << std::endl;
  BOOST_CHECK_MESSAGE (lKOTranslitStr == lKOTargetStr,
                       "The Korean transliterated string for '" << lKOStr
                       << "' should be '" << lKOTargetStr << "'. "
                       << "However, it is '" << lKOTranslitStr << "'.");

  // Thai
  logOutputFile << "[TH] " << lTHStr << " -> " << lTHTranslitStr << std::endl;
  BOOST_CHECK_MESSAGE (lTHTranslitStr == lTHTargetStr,
                       "The Korean transliterated string for '" << lTHStr
                       << "' should be '" << lTHTargetStr << "'. "
                       << "However, it is '" << lTHTranslitStr << "'.");

  // Vietnamese
  logOutputFile << "[VI] " << lVIStr << " -> " << lVITranslitStr << std::endl;
  BOOST_CHECK_MESSAGE (lVITranslitStr == lVITargetStr,
                       "The Korean transliterated string for '" << lVIStr
                       << "' should be '" << lVITargetStr << "'. "
                       << "However, it is '" << lVITranslitStr << "'.");


  // Close the Log outputFile
  logOutputFile.close();
}

// End the test suite
BOOST_AUTO_TEST_SUITE_END()

