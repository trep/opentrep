// STL
#include <cassert>
#include <iostream>
#include <sstream>
#include <fstream>
#include <vector>
#include <string>
// Boost (Extended STL)
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/date_time/gregorian/gregorian.hpp>
#include <boost/tokenizer.hpp>
#include <boost/program_options.hpp>
// OpenTREP
#include <opentrep/OPENTREP_Service.hpp>
#include <opentrep/Location.hpp>
#include <opentrep/config/opentrep-paths.hpp>


// //////// Type definitions ///////
typedef std::vector<std::string> WordList_T;


// //////// Constants //////
/**
 * Default name and location for the log file.
 */
const std::string K_OPENTREP_DEFAULT_LOG_FILENAME ("opentrep-searcher.log");

/**
 * Default name and location for the Xapian database.
 */
const std::string K_OPENTREP_DEFAULT_DATABSE_FILEPATH("/tmp/opentrep/traveldb");

/**
 * Default travel query string, to be seached against the Xapian database.
 */
const std::string K_OPENTREP_DEFAULT_QUERY_STRING ("sna francicso rio de janero lso angles reykyavki");

/**
 * Default type for the search request.
 *  <br>
 *  <ul>
 *    <li>0 = Full text</li>
 *    <li>1 = Coordinates</li> 
 *  </ul>
 */
const unsigned short K_OPENTREP_DEFAULT_SEARCH_TYPE = 0;

/**
 * Default error distance for spelling corrections.
 */
const unsigned short K_OPENTREP_DEFAULT_SPELLING_ERROR_DISTANCE = 3;


// //////////////////////////////////////////////////////////////////////
void tokeniseStringIntoWordList (const std::string& iPhrase,
                                 WordList_T& ioWordList) {
  // Empty the word list
  ioWordList.clear();
  
  // Boost Tokeniser
  typedef boost::tokenizer<boost::char_separator<char> > Tokeniser_T;
  
  // Define the separators
  const boost::char_separator<char> lSepatorList(" .,;:|+-*/_=!@#$%`~^&(){}[]?'<>\"");
  
  // Initialise the phrase to be tokenised
  Tokeniser_T lTokens (iPhrase, lSepatorList);
  for (Tokeniser_T::const_iterator tok_iter = lTokens.begin();
       tok_iter != lTokens.end(); ++tok_iter) {
    const std::string& lTerm = *tok_iter;
    ioWordList.push_back (lTerm);
  }
}

// //////////////////////////////////////////////////////////////////////
std::string createStringFromWordList (const WordList_T& iWordList) {
  std::ostringstream oStr;

  unsigned short idx = iWordList.size();
  for (WordList_T::const_iterator itWord = iWordList.begin();
       itWord != iWordList.end(); ++itWord, --idx) {
    const std::string& lWord = *itWord;
    oStr << lWord;
    if (idx > 1) {
      oStr << " ";
    }
  }
  
  return oStr.str();
}


// ///////// Parsing of Options & Configuration /////////
// A helper function to simplify the main part.
template<class T> std::ostream& operator<< (std::ostream& os,
                                            const std::vector<T>& v) {
  std::copy (v.begin(), v.end(), std::ostream_iterator<T> (std::cout, " ")); 
  return os;
}

/** Early return status (so that it can be differentiated from an error). */
const int K_OPENTREP_EARLY_RETURN_STATUS = 99;

/** Read and parse the command line options. */
int readConfiguration (int argc, char* argv[], 
                       unsigned short& ioSpellingErrorDistance, 
                       std::string& ioQueryString,
                       std::string& ioDatabaseFilepath,
                       std::string& ioLogFilename,
                       unsigned short& ioSearchType) {

  // Initialise the travel query string, if that one is empty
  if (ioQueryString.empty() == true) {
    ioQueryString = K_OPENTREP_DEFAULT_QUERY_STRING;
  }
  
  // Transform the query string into a list of words (STL strings)
  WordList_T lWordList;
  tokeniseStringIntoWordList (ioQueryString, lWordList);

  // Declare a group of options that will be allowed only on command line
  boost::program_options::options_description generic ("Generic options");
  generic.add_options()
    ("prefix", "print installation prefix")
    ("version,v", "print version string")
    ("help,h", "produce help message");
    
  // Declare a group of options that will be allowed both on command
  // line and in config file
  boost::program_options::options_description config ("Configuration");
  config.add_options()
    ("error,e",
     boost::program_options::value< unsigned short >(&ioSpellingErrorDistance)->default_value(K_OPENTREP_DEFAULT_SPELLING_ERROR_DISTANCE), 
     "Spelling error distance (e.g., 3)")
    ("database,d",
     boost::program_options::value< std::string >(&ioDatabaseFilepath)->default_value(K_OPENTREP_DEFAULT_DATABSE_FILEPATH),
     "Xapian database filepath (e.g., /tmp/opentrep/traveldb)")
    ("log,l",
     boost::program_options::value< std::string >(&ioLogFilename)->default_value(K_OPENTREP_DEFAULT_LOG_FILENAME),
     "Filepath for the logs")
    ("type,t",
     boost::program_options::value<unsigned short>(&ioSearchType)->default_value(K_OPENTREP_DEFAULT_SEARCH_TYPE), 
     "Type of search request (0 = full text, 1 = coordinates)")
    ("query,q",
     boost::program_options::value< WordList_T >(&lWordList)->multitoken(),
     "Travel query word list (e.g. sna francicso rio de janero lso anglese reykyavki), which sould be located at the end of the command line (otherwise, the other options would be interpreted as part of that travel query word list)")
    ;

  // Hidden options, will be allowed both on command line and
  // in config file, but will not be shown to the user.
  boost::program_options::options_description hidden ("Hidden options");
  hidden.add_options()
    ("copyright",
     boost::program_options::value< std::vector<std::string> >(),
     "Show the copyright (license)");
        
  boost::program_options::options_description cmdline_options;
  cmdline_options.add(generic).add(config).add(hidden);

  boost::program_options::options_description config_file_options;
  config_file_options.add(config).add(hidden);

  boost::program_options::options_description visible ("Allowed options");
  visible.add(generic).add(config);
        
  boost::program_options::positional_options_description p;
  p.add ("copyright", -1);
        
  boost::program_options::variables_map vm;
  boost::program_options::
    store (boost::program_options::command_line_parser (argc, argv).
           options (cmdline_options).positional(p).run(), vm);

  std::ifstream ifs ("opentrep-searcher.cfg");
  boost::program_options::store (parse_config_file (ifs, config_file_options),
                                 vm);
  boost::program_options::notify (vm);
    
  if (vm.count ("help")) {
    std::cout << visible << std::endl;
    return K_OPENTREP_EARLY_RETURN_STATUS;
  }

  if (vm.count ("version")) {
    std::cout << PACKAGE_NAME << ", version " << PACKAGE_VERSION << std::endl;
    return K_OPENTREP_EARLY_RETURN_STATUS;
  }

  if (vm.count ("prefix")) {
    std::cout << "Installation prefix: " << PREFIXDIR << std::endl;
    return K_OPENTREP_EARLY_RETURN_STATUS;
  }

  if (vm.count ("database")) {
    ioDatabaseFilepath = vm["database"].as< std::string >();
    std::cout << "Xapian database filepath is: " << ioDatabaseFilepath
              << std::endl;
  }

  if (vm.count ("log")) {
    ioLogFilename = vm["log"].as< std::string >();
    std::cout << "Log filename is: " << ioLogFilename << std::endl;
  }

  std::cout << "The type of search is: " << ioSearchType << std::endl;
  
  std::cout << "The spelling error distance is: " << ioSpellingErrorDistance
            << std::endl;

  ioQueryString = createStringFromWordList (lWordList);
  std::cout << "The travel query string is: " << ioQueryString << std::endl;
  
  return 0;
}

/**
 * Helper function
 */
std::string parseQuery (OPENTREP::OPENTREP_Service& ioOpentrepService,
                        const OPENTREP::TravelQuery_T& iTravelQuery) {
  std::ostringstream oStr;

  // Query the Xapian database (index)
  OPENTREP::WordList_T lNonMatchedWordList;
  OPENTREP::LocationList_T lLocationList;
  const OPENTREP::NbOfMatches_T nbOfMatches =
    ioOpentrepService.interpretTravelRequest (iTravelQuery, lLocationList,
                                              lNonMatchedWordList);

  oStr << nbOfMatches << " (geographical) location(s) have been found "
       << "matching your query (`" << iTravelQuery << "'). "
       << lNonMatchedWordList.size() << " words were left unmatched."
       << std::endl;
      
  if (nbOfMatches != 0) {
    OPENTREP::NbOfMatches_T idx = 1;
    for (OPENTREP::LocationList_T::const_iterator itLocation =
           lLocationList.begin();
         itLocation != lLocationList.end(); ++itLocation, ++idx) {
      const OPENTREP::Location& lLocation = *itLocation;
      oStr << " [" << idx << "]: " << lLocation << std::endl;
    }
  }

  if (lNonMatchedWordList.empty() == false) {
    oStr << "List of unmatched words:" << std::endl;

    OPENTREP::NbOfMatches_T idx = 1;
    for (OPENTREP::WordList_T::const_iterator itWord =
           lNonMatchedWordList.begin();
         itWord != lNonMatchedWordList.end(); ++itWord, ++idx) {
      const OPENTREP::Word_T& lWord = *itWord;
      oStr << " [" << idx << "]: " << lWord << std::endl;
    }
  }

  return oStr.str();
}

// /////////////// M A I N /////////////////
int main (int argc, char* argv[]) {

  /*
    const OPENTREP::NbOfLetters_T lScaleArray[5] = {3, 6, 9, 14, 19};

    const OPENTREP::DistanceErrorScaleArray_T lScaleBoostArray =
    { {3, 6, 9, 14, 19} };
    
    OPENTREP::DistanceErrorRule lScale (5, lScaleArray);
    OPENTREP::DistanceErrorRule lScaleBoost (lScaleBoostArray);

    std::cout << "Standard array: " << lScale << std::endl;
    std::cout << "Boost array: " << lScaleBoost << std::endl;

    for (int idx = 0; idx != 20; ++idx) {
    std::cout << "For " << idx << " letters => "
    << lScale.getAllowedDistanceError(idx) << std::endl;
    }
    
    return 0;
  */
    
  // Travel query
  OPENTREP::TravelQuery_T lTravelQuery;

  // Output log File
  std::string lLogFilename;

  // Xapian database name (directory of the index)
  std::string lXapianDBNameStr;

  // Type of search
  unsigned short lSearchType;
      
  // Xapian spelling error distance
  unsigned short lSpellingErrorDistance;

  // Call the command-line option parser
  const int lOptionParserStatus = 
    readConfiguration (argc, argv, lSpellingErrorDistance, lTravelQuery,
                       lXapianDBNameStr, lLogFilename, lSearchType);

  if (lOptionParserStatus == K_OPENTREP_EARLY_RETURN_STATUS) {
    return 0;
  }
    
  // Set the log parameters
  std::ofstream logOutputFile;
  // open and clean the log outputfile
  logOutputFile.open (lLogFilename.c_str());
  logOutputFile.clear();

  if (lSearchType == 0) {
    // Initialise the context
    const OPENTREP::TravelDatabaseName_T lXapianDBName (lXapianDBNameStr);
    OPENTREP::OPENTREP_Service opentrepService (logOutputFile, lXapianDBName);

    // Parse the query and retrieve the places from Xapian only
    const std::string& lOutput = parseQuery (opentrepService, lTravelQuery);
    std::cout << lOutput;

  } else {
    std::cout << "Finding the airports closest to: " << lTravelQuery
              << std::endl;
  }
    
  // Close the Log outputFile
  logOutputFile.close();

  return 0;
}
