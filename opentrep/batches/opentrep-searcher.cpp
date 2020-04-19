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
#include <opentrep/DBType.hpp>
#include <opentrep/basic/BasConst_OPENTREP_Service.hpp>
#include <opentrep/basic/Utilities.hpp>
#include <opentrep/Location.hpp>
#include <opentrep/CityDetails.hpp>
#include <opentrep/config/opentrep-paths.hpp>


// //////// Type definitions ///////
typedef std::vector<std::string> WordList_T;


// //////// Constants //////
/**
 * Default name and location for the log file.
 */
const std::string K_OPENTREP_DEFAULT_LOG_FILENAME ("opentrep-searcher.log");

/**
 * Default travel query string, to be seached against the Xapian database.
 */
const std::string K_OPENTREP_DEFAULT_QUERY_STRING ("sna francisco rio de janero los angeles reykyavki");

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
  std::copy (v.begin(), v.end(), std::ostream_iterator<T> (os, " ")); 
  return os;
}

/** Early return status (so that it can be differentiated from an error). */
const int K_OPENTREP_EARLY_RETURN_STATUS = 99;

/** Read and parse the command line options. */
int readConfiguration (int argc, char* argv[], 
                       unsigned short& ioSpellingErrorDistance, 
                       std::string& ioQueryString,
                       std::string& ioXapianDBFilepath,
                       std::string& ioSQLDBTypeString,
                       std::string& ioSQLDBConnectionString,
                       unsigned short& ioDeploymentNumber,
                       std::string& ioLogFilename,
                       unsigned short& ioSearchType,
                       std::ostringstream& oStr) {

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
    ("xapiandb,d",
     boost::program_options::value< std::string >(&ioXapianDBFilepath)->default_value(OPENTREP::DEFAULT_OPENTREP_XAPIAN_DB_FILEPATH),
     "Xapian database filepath (e.g., /tmp/opentrep/xapian_traveldb)")
    ("sqldbtype,t",
     boost::program_options::value< std::string >(&ioSQLDBTypeString)->default_value(OPENTREP::DEFAULT_OPENTREP_SQL_DB_TYPE),
     "SQL database type (e.g., nodb for no SQL database, sqlite for SQLite, mysql for MariaDB/MySQL)")
    ("sqldbconx,s",
     boost::program_options::value< std::string >(&ioSQLDBConnectionString),
     "SQL database connection string (e.g., ~/tmp/opentrep/sqlite_travel.db for SQLite, "
     "\"db=trep_trep user=trep password=trep\" for MariaDB/MySQL)")
    ("deploymentnb,m",
     boost::program_options::value<unsigned short>(&ioDeploymentNumber)->default_value(OPENTREP::DEFAULT_OPENTREP_DEPLOYMENT_NUMBER), 
     "Deployment number (from to N, where N=1 normally)")
    ("log,l",
     boost::program_options::value< std::string >(&ioLogFilename)->default_value(K_OPENTREP_DEFAULT_LOG_FILENAME),
     "Filepath for the logs")
    ("type,y",
     boost::program_options::value<unsigned short>(&ioSearchType)->default_value(K_OPENTREP_DEFAULT_SEARCH_TYPE), 
     "Type of search request (0 = full text, 1 = coordinates)")
    ("query,q",
     boost::program_options::value< WordList_T >(&lWordList)->multitoken(),
     "Travel query word list (e.g. sna francisco rio de janero los angeles reykyavki), "
     "which sould be located at the end of the command line (otherwise, "
     "the other options would be interpreted as part of that travel query word list)")
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

  if (vm.count ("deploymentnb")) {
    ioDeploymentNumber = vm["deploymentnb"].as< unsigned short >();
    oStr << "Deployment number: " << ioDeploymentNumber << std::endl;
  }

  if (vm.count ("xapiandb")) {
    ioXapianDBFilepath = vm["xapiandb"].as< std::string >();
    oStr << "Xapian database filepath is: " << ioXapianDBFilepath
         << ioDeploymentNumber << std::endl;
  }

  if (vm.count ("sqldbtype")) {
    ioSQLDBTypeString = vm["sqldbtype"].as< std::string >();
    oStr << "SQL database type is: " << ioSQLDBTypeString << std::endl;
  }

  // Derive the detault connection string depending on the SQL database type
  const OPENTREP::DBType lDBType (ioSQLDBTypeString);
  if (lDBType == OPENTREP::DBType::NODB) {
    ioSQLDBConnectionString = "";
    
  } else if (lDBType == OPENTREP::DBType::SQLITE3) {
    ioSQLDBConnectionString = OPENTREP::DEFAULT_OPENTREP_SQLITE_DB_FILEPATH;

  } else if (lDBType == OPENTREP::DBType::MYSQL) {
    ioSQLDBConnectionString = OPENTREP::DEFAULT_OPENTREP_MYSQL_CONN_STRING;
  }

  // Set the SQL database connection string, if any is given
  if (vm.count ("sqldbconx")) {
    ioSQLDBConnectionString = vm["sqldbconx"].as< std::string >();
  }

  // Reporting of the SQL database connection string
  if (lDBType == OPENTREP::DBType::SQLITE3
      || lDBType == OPENTREP::DBType::MYSQL) {
    const std::string& lSQLDBConnString =
      OPENTREP::parseAndDisplayConnectionString (lDBType,
                                                 ioSQLDBConnectionString,
                                                 ioDeploymentNumber);
    //
    oStr << "SQL database connection string is: " << lSQLDBConnString
         << std::endl;
  }

  if (vm.count ("log")) {
    ioLogFilename = vm["log"].as< std::string >();
    oStr << "Log filename is: " << ioLogFilename << std::endl;
  }

  oStr << "The type of search is: " << ioSearchType << std::endl;
  
  oStr << "The spelling error distance is: " << ioSpellingErrorDistance
            << std::endl;

  ioQueryString = createStringFromWordList (lWordList);
  oStr << "The travel query string is: " << ioQueryString << std::endl;
  
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
       << lNonMatchedWordList.size() << " word(s) was/were left unmatched."
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

  // SQL database type
  std::string lSQLDBTypeStr;

  // SQL database connection string
  std::string lSQLDBConnectionStr;

  // Deployment number/version
  OPENTREP::DeploymentNumber_T lDeploymentNumber;
  
  // Log stream for the introduction part
  std::ostringstream oIntroStr;

  // Call the command-line option parser
  const int lOptionParserStatus = 
    readConfiguration (argc, argv, lSpellingErrorDistance, lTravelQuery,
                       lXapianDBNameStr, lSQLDBTypeStr, lSQLDBConnectionStr,
                       lDeploymentNumber, lLogFilename, lSearchType, oIntroStr);

  if (lOptionParserStatus == K_OPENTREP_EARLY_RETURN_STATUS) {
    return 0;
  }
    
  // Set the log parameters
  std::ofstream logOutputFile;
  // open and clean the log outputfile
  logOutputFile.open (lLogFilename.c_str());
  logOutputFile.clear();

  // Report the parameters
  std::cout << oIntroStr.str();

  // DEBUG
  // Get the current time in UTC Timezone
  boost::posix_time::ptime lTimeUTC =
    boost::posix_time::second_clock::universal_time();
  logOutputFile << "[" << lTimeUTC << "][" << __FILE__ << "#"
                << __LINE__ << "]:Parameters:" << std::endl
                <<  oIntroStr.str() << std::endl;

  //
  std::ostringstream oStr;
  if (lSearchType == 0) {
    // Initialise the context
    const OPENTREP::TravelDBFilePath_T lXapianDBName (lXapianDBNameStr);
    const OPENTREP::DBType lDBType (lSQLDBTypeStr);
    const OPENTREP::SQLDBConnectionString_T lSQLDBConnStr (lSQLDBConnectionStr);
    OPENTREP::OPENTREP_Service opentrepService (logOutputFile, lXapianDBName,
                                                lDBType, lSQLDBConnStr,
                                                lDeploymentNumber);

    // Check the directory of the Xapian database/index exists and is accessible
    const OPENTREP::OPENTREP_Service::FilePathSet_T& lFPSet =
      opentrepService.getFilePaths();
    const OPENTREP::TravelDBFilePath_T& lActualXapianDBDir= lFPSet.second.first;
    const bool lExistXapianDBDir =
      opentrepService.checkXapianDBOnFileSystem (lActualXapianDBDir);
    if (lExistXapianDBDir == false) {
      std::ostringstream errorStr;
      errorStr << "Error - The file-path to the Xapian database/index ('"
               << lActualXapianDBDir
               << "') does not exist or is not a directory." << std::endl;
      errorStr << "\tThat usually means that the OpenTREP indexer "
               << "(opentrep-indexer) has not been launched yet, "
               << "or that it has operated on a different Xapian "
               << "database/index file-path." << std::endl;
      errorStr << "\tFor instance the Xapian database/index may have been "
               << "created with a different deployment number ("
               << lDeploymentNumber << " being the current deployment number)";
      std::cerr << errorStr.str() << std::endl;
      return -1;
    }
    
    // Parse the query and retrieve the places from Xapian only
    const std::string& lOutput = parseQuery (opentrepService, lTravelQuery);
    oStr << lOutput;

  } else {
    oStr << "Finding the airports closest to: " << lTravelQuery << std::endl;
  }
  
  //  
  std::cout << oStr.str();

  // Get the current time in UTC Timezone
  lTimeUTC = boost::posix_time::second_clock::universal_time();
  logOutputFile << "[" << lTimeUTC << "][" << __FILE__ << "#"
                << __LINE__ << "]:Results:" << std::endl
                <<  oStr.str() << std::endl;

  // Close the Log outputFile
  logOutputFile.close();

  return 0;
}
