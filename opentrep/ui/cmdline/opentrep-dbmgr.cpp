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
#include <boost/regex.hpp>
#include <boost/tokenizer.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/program_options.hpp>
// GNU Readline Wrapper
#include <opentrep/ui/cmdline/SReadline.hpp>
// OpenTREP
#include <opentrep/OPENTREP_Service.hpp>
#include <opentrep/OPENTREP_exceptions.hpp>
#include <opentrep/Location.hpp>
#include <opentrep/CityDetails.hpp>
#include <opentrep/basic/BasConst_OPENTREP_Service.hpp>
#include <opentrep/basic/Utilities.hpp>
#include <opentrep/config/opentrep-paths.hpp>
#include <opentrep/service/Logger.hpp>


// //////// Type definitions ///////
typedef std::vector<std::string> WordList_T;


// //////// Constants //////
/**
 * Default name and location for the log file.
 */
const std::string K_OPENTREP_DEFAULT_LOG_FILENAME ("opentrep-dbmgr.log");


// ///////// Parsing of Options & Configuration /////////
/** Early return status (so that it can be differentiated from an error). */
const int K_OPENTREP_EARLY_RETURN_STATUS = 99;

/**
 * List of strings, representing the tokens as entered by the user on
 * a command-line.
 */
typedef std::vector<std::string> TokenList_T;

/**
 * Enumeration representing the command entered by the user on the command-line.
 */
struct Command_T {
  typedef enum {
    NOP = 0,
    QUIT,
    HELP,
    INFO,
    TUTORIAL,
    CREATE_USER,
    RESET_CONNECTION_STRING,
    TOGGLE_DEPLOYMENT_NUMBER,
    TOGGLE_NONIATA_INDEXING_FLAG,
    TOGGLE_XAPIAN_IDEXING_FLAG,
    TOGGLE_SQLDB_INSERTING_FLAG,
    CREATE_TABLES,
    CREATE_INDEXES,
    FILL_FROM_POR_FILE,
    LIST_BY_IATA,
    LIST_BY_ICAO,
    LIST_BY_FAA,
    LIST_BY_UNLOCODE,
    LIST_BY_UICCODE,
    LIST_BY_GEONAMEID,
    LIST_NB,
    LIST_ALL,
    LIST_CONT,
    LAST_VALUE
  } Type_T;
};

// ///////// Parsing of Options & Configuration /////////
// A helper function to simplify the main part.
template<class T> std::ostream& operator<< (std::ostream& os,
                                            const std::vector<T>& v) {
  std::copy (v.begin(), v.end(), std::ostream_iterator<T> (std::cout, " ")); 
  return os;
}

/**
 * Read and parse the command line options.
 */
int readConfiguration (int argc, char* argv[], 
                       std::string& ioPORFilepath, 
                       std::string& ioXapianDBFilepath,
                       std::string& ioSQLDBTypeString,
                       std::string& ioSQLDBConnectionString,
                       unsigned short& ioDeploymentNumber,
                       bool& ioIncludeNonIATAPOR,
                       bool& ioIndexPORInXapian,
                       bool& ioAddPORInDB,
                       std::string& ioLogFilename,
                       std::string& ioCommand) {

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
    ("porfile,p",
     boost::program_options::value< std::string >(&ioPORFilepath)->default_value(OPENTREP::DEFAULT_OPENTREP_POR_FILEPATH),
     "POR file-path (e.g., ori_por_public.csv)")
    ("xapiandb,d",
     boost::program_options::value< std::string >(&ioXapianDBFilepath)->default_value(OPENTREP::DEFAULT_OPENTREP_XAPIAN_DB_FILEPATH),
     "Xapian database filepath (e.g., /tmp/opentrep/xapian_traveldb)")
    ("sqldbtype,t",
     boost::program_options::value< std::string >(&ioSQLDBTypeString)->default_value(OPENTREP::DEFAULT_OPENTREP_SQL_DB_TYPE),
     "SQL database type (e.g., nodb for no SQL database, sqlite for SQLite, mysql for MariaDB/MySQL, pg for PostgreSQL)")
    ("sqldbconx,s",
     boost::program_options::value< std::string >(&ioSQLDBConnectionString),
     "SQL database connection string (e.g., ~/tmp/opentrep/sqlite_travel.db for SQLite, \"db=trep_trep user=trep password=trep\" for MariaDB/MySQL or PostgreSQL)")
    ("deploymentnb,m",
     boost::program_options::value<unsigned short>(&ioDeploymentNumber)->default_value(OPENTREP::DEFAULT_OPENTREP_DEPLOYMENT_NUMBER), 
     "Deployment number (from to N, where N=1 normally)")
    ("noniata,n",
     boost::program_options::value<bool>(&ioIncludeNonIATAPOR)->default_value(OPENTREP::DEFAULT_OPENTREP_INCLUDE_NONIATA_POR),
     "Whether or not to include POR not referenced by IATA (0 = only IATA-referenced POR, 1 = all POR are included)")
    ("xapianindex,x",
     boost::program_options::value<bool>(&ioIndexPORInXapian)->default_value(OPENTREP::DEFAULT_OPENTREP_INDEX_IN_XAPIAN),
     "Whether or not to index the POR in Xapian (0 = do not touch the Xapian index, 1 = re-index all the POR in Xapian)")
    ("dbadd,a",
     boost::program_options::value<bool>(&ioAddPORInDB)->default_value(OPENTREP::DEFAULT_OPENTREP_ADD_IN_DB),
     "Whether or not to add and index the POR in the SQL-based database (0 = do not touch the SQL-based database, 1 = add and re-index all the POR in the SQL-based database)")
    ("log,l",
     boost::program_options::value< std::string >(&ioLogFilename)->default_value(K_OPENTREP_DEFAULT_LOG_FILENAME),
     "Filepath for the logs")
    ("command,c",
     boost::program_options::value< std::string >(&ioCommand)->default_value(""),
     "Execute the given semicolon-separated dbmgr command(s) (e.g., \"list_nb ; list_by_iata nce\") in non-interactive mode, and then exit")
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

  std::ifstream ifs ("opentrep-dbmgr.cfg");
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

  if (vm.count ("porfile")) {
    ioPORFilepath = vm["porfile"].as< std::string >();
  }

  if (vm.count ("deploymentnb")) {
    ioDeploymentNumber = vm["deploymentnb"].as< unsigned short >();
    std::cout << "Deployment number " << ioDeploymentNumber << std::endl;
  }

  if (vm.count ("xapiandb")) {
    ioXapianDBFilepath = vm["xapiandb"].as< std::string >();
    std::cout << "Xapian index/database filepath is: " << ioXapianDBFilepath
              << ioDeploymentNumber << std::endl;
  }

  // Parse the SQL database type, if any is given
  if (vm.count ("sqldbtype")) {
    ioSQLDBTypeString = vm["sqldbtype"].as< std::string >();
    std::cout << "SQL database type is: " << ioSQLDBTypeString
              << std::endl;
  }

  /**
   * Derive, from the SQL database type:
   * <ul>
   *   <li>The detault connection string</li>
   *   <li>Whether or not the POR should be added to that SQL database.
   *       It is assumed that when the SQL database type is specified
   *       (and not null), the POR should be added into it, and conversely,
   *       when ``nodb`` is specified, the POR should not be added to any
   *       SQL database.</li>
   * </ul>
   */
  const OPENTREP::DBType lDBType (ioSQLDBTypeString);
  if (lDBType == OPENTREP::DBType::NODB) {
    ioAddPORInDB = false;
    ioSQLDBConnectionString = "";
    
  } else if (lDBType == OPENTREP::DBType::SQLITE3) {
    ioAddPORInDB = true;
    ioSQLDBConnectionString = OPENTREP::DEFAULT_OPENTREP_SQLITE_DB_FILEPATH;

  } else if (lDBType == OPENTREP::DBType::PG) {
    ioAddPORInDB = true;
    ioSQLDBConnectionString = OPENTREP::DEFAULT_OPENTREP_PG_CONN_STRING;

  } else if (lDBType == OPENTREP::DBType::MYSQL) {
    ioAddPORInDB = true;
    ioSQLDBConnectionString = OPENTREP::DEFAULT_OPENTREP_MYSQL_CONN_STRING;
  }

  // Set the SQL database connection string, if any is given
  if (vm.count ("sqldbconx")) {
    ioSQLDBConnectionString = vm["sqldbconx"].as< std::string >();    
  }

  // Reporting of the SQL database connection string
  if (lDBType == OPENTREP::DBType::SQLITE3
      || lDBType == OPENTREP::DBType::MYSQL
      || lDBType == OPENTREP::DBType::PG) {
    const std::string& lSQLDBConnString =
      OPENTREP::parseAndDisplayConnectionString (lDBType,
                                                 ioSQLDBConnectionString,
                                                 ioDeploymentNumber);
    //
    std::cout << "SQL database connection string is: " << lSQLDBConnString
              << std::endl;
  }

  std::cout << "Are non-IATA-referenced POR included? "
            << ioIncludeNonIATAPOR << std::endl;
  
  std::cout << "Index the POR in Xapian? "
            << ioIndexPORInXapian << std::endl;
  
  std::cout << "Add and re-index the POR in the SQL-based database? "
            << ioAddPORInDB << std::endl;
  
  if (vm.count ("log")) {
    ioLogFilename = vm["log"].as< std::string >();
  }

  if (vm.count ("command")) {
    ioCommand = vm["command"].as< std::string >();
  }

  // Information
  std::cout << "Type the 'info' command to get a few details (e.g., file-path)"
            << std::endl;

  return 0;
}

// //////////////////////////////////////////////////////////////////
void initReadline (swift::SReadline& ioInputReader) {

  // Prepare the list of my own completers
  std::vector<std::string> Completers;

  // The following is supported:
  // - "identifiers"
  // - special identifier %file - means to perform a file name completion
  Completers.push_back ("help");
  Completers.push_back ("info");
  Completers.push_back ("tutorial");
  Completers.push_back ("create_user");
  Completers.push_back ("reset_connection_string %connection_string");
  Completers.push_back ("create_tables");
  Completers.push_back ("create_indexes");
  Completers.push_back ("toggle_deployment_number");
  Completers.push_back ("toggle_noniata_indexing_flag");
  Completers.push_back ("toggle_xapian_idexing_flag");
  Completers.push_back ("toggle_sqldb_inserting_flag");
  Completers.push_back ("fill_from_por_file");
  Completers.push_back ("list_by_iata %iata_code");
  Completers.push_back ("list_by_icao %icao_code");
  Completers.push_back ("list_by_faa %faa_code");
  Completers.push_back ("list_by_unlocode %unlocode_code");
  Completers.push_back ("list_by_uiccode %uic_code");
  Completers.push_back ("list_by_geonameid %geoname_id");
  Completers.push_back ("list_nb");
  Completers.push_back ("list_all");
  Completers.push_back ("list_cont");
  Completers.push_back ("quit");

  // Now register the completers.
  // Actually it is possible to re-register another set at any time
  ioInputReader.RegisterCompletions (Completers);
}

// //////////////////////////////////////////////////////////////////
Command_T::Type_T extractCommand (TokenList_T& ioTokenList) {
  Command_T::Type_T oCommandType = Command_T::LAST_VALUE;

  // Interpret the user input
  if (ioTokenList.empty() == false) {
    TokenList_T::iterator itTok = ioTokenList.begin();
    std::string lCommand (*itTok);
    boost::algorithm::to_lower (lCommand);
    if (lCommand == "help") {
      oCommandType = Command_T::HELP;

    } else if (lCommand == "info") {
      oCommandType = Command_T::INFO;
    
    } else if (lCommand == "tutorial") {
      oCommandType = Command_T::TUTORIAL;
    
    } else if (lCommand == "create_user") {
      oCommandType = Command_T::CREATE_USER;
    
    } else if (lCommand == "reset_connection_string") {
      oCommandType = Command_T::RESET_CONNECTION_STRING;

    } else if (lCommand == "create_tables") {
      oCommandType = Command_T::CREATE_TABLES;

    } else if (lCommand == "create_indexes") {
      oCommandType = Command_T::CREATE_INDEXES;

    } else if (lCommand == "toggle_deployment_number") {
      oCommandType = Command_T::TOGGLE_DEPLOYMENT_NUMBER;

    } else if (lCommand == "toggle_noniata_indexing_flag") {
      oCommandType = Command_T::TOGGLE_NONIATA_INDEXING_FLAG;

    } else if (lCommand == "toggle_xapian_idexing_flag") {
      oCommandType = Command_T::TOGGLE_XAPIAN_IDEXING_FLAG;

    } else if (lCommand == "toggle_sqldb_inserting_flag") {
      oCommandType = Command_T::TOGGLE_SQLDB_INSERTING_FLAG;

    } else if (lCommand == "fill_from_por_file") {
      oCommandType = Command_T::FILL_FROM_POR_FILE;

    } else if (lCommand == "list_by_iata") {
      oCommandType = Command_T::LIST_BY_IATA;

    } else if (lCommand == "list_by_icao") {
      oCommandType = Command_T::LIST_BY_ICAO;

    } else if (lCommand == "list_by_faa") {
      oCommandType = Command_T::LIST_BY_FAA;

    } else if (lCommand == "list_by_unlocode") {
      oCommandType = Command_T::LIST_BY_UNLOCODE;

    } else if (lCommand == "list_by_uiccode") {
      oCommandType = Command_T::LIST_BY_UICCODE;

    } else if (lCommand == "list_by_geonameid") {
      oCommandType = Command_T::LIST_BY_GEONAMEID;

    } else if (lCommand == "list_nb") {
      oCommandType = Command_T::LIST_NB;

    } else if (lCommand == "list_all") {
      oCommandType = Command_T::LIST_ALL;

    } else if (lCommand == "list_cont") {
      oCommandType = Command_T::LIST_CONT;

    } else if (lCommand == "quit") {
      oCommandType = Command_T::QUIT;
    }

    // Remove the first token (the command), as the corresponding information
    // has been extracted in the form of the returned command type enumeration
    ioTokenList.erase (itTok);

  } else {
    oCommandType = Command_T::NOP;
  }

  return oCommandType;
}

// //////////////////////////////////////////////////////////////////
void parseConnectionString (const TokenList_T& iTokenList,
                            std::string& ioConnectionString) {
  // Interpret the user input
  if (iTokenList.empty() == false) {

    // Read the database connection string
    TokenList_T::const_iterator itTok = iTokenList.begin();
    if (itTok->empty() == false) {
      ioConnectionString = *itTok;
    }
  }
}

// //////////////////////////////////////////////////////////////////
void parsePlaceKey (const TokenList_T& iTokenList, std::string& ioPlaceKey) {
  // Interpret the user input
  if (iTokenList.empty() == false) {

    // Read the IATA code
    TokenList_T::const_iterator itTok = iTokenList.begin();
    if (itTok->empty() == false) {
      ioPlaceKey = *itTok;
    }
  }
}

// /////////////////////////////////////////////////////////
std::string toString (const TokenList_T& iTokenList) {
  std::ostringstream oStr;

  // Re-create the string with all the tokens, trimmed by read-line
  unsigned short idx = 0;
  for (TokenList_T::const_iterator itTok = iTokenList.begin();
       itTok != iTokenList.end(); ++itTok, ++idx) {
    if (idx != 0) {
      oStr << " ";
    }
    oStr << *itTok;
  }

  return oStr.str();
}

// /////////////////////////////////////////////////////////
TokenList_T extractTokenList (const TokenList_T& iTokenList,
                              const std::string& iRegularExpression) {
  TokenList_T oTokenList;

  // Re-create the string with all the tokens (which had been trimmed
  // by read-line)
  const std::string lFullLine = toString (iTokenList);

  // See the caller for the regular expression
  boost::regex expression (iRegularExpression);
  
  std::string::const_iterator start = lFullLine.begin();
  std::string::const_iterator end = lFullLine.end();

  boost::match_results<std::string::const_iterator> what;
  boost::match_flag_type flags = boost::match_default | boost::format_sed; 
  regex_search (start, end, what, expression, flags);
  
  // Put the matched strings in the list of tokens to be returned back
  // to the caller
  const unsigned short lMatchSetSize = what.size();
  for (unsigned short matchIdx = 1; matchIdx != lMatchSetSize; ++matchIdx) {
    const std::string lMatchedString (std::string (what[matchIdx].first,
                                                   what[matchIdx].second));
    //if (lMatchedString.empty() == false) {
    oTokenList.push_back (lMatchedString);
    //}
  }

  // DEBUG
  // std::cout << "After (token list): " << oTokenList << std::endl;

  return oTokenList;
}    

// /////////////////////////////////////////////////////////
TokenList_T extractTokenListForIataCode (const TokenList_T& iTokenList) {
  /**
   * Expected format:
   *   line:         iata_code
   *   iata_code:    word (alpha{3})
   */
  const std::string lRegEx ("^([[:alpha:]]{3})$");

  //
  const TokenList_T& oTokenList = extractTokenList (iTokenList, lRegEx);
  return oTokenList;
}    

// /////////////////////////////////////////////////////////
TokenList_T extractTokenListForIcaoCode (const TokenList_T& iTokenList) {
  /**
   * Expected format:
   *   line:         icao_code
   *   icao_code:    word ((alpha|digit){4})
   */
  const std::string lRegEx ("^(([[:alpha:]]|[[:digit:]]){4})$");

  //
  const TokenList_T& oTokenList = extractTokenList (iTokenList, lRegEx);
  return oTokenList;
}    

// /////////////////////////////////////////////////////////
TokenList_T extractTokenListForFaaCode (const TokenList_T& iTokenList) {
  /**
   * Expected format:
   *   line:         faa_code
   *   faa_code:     word ((alpha|digit){3,4})
   */
  const std::string lRegEx ("^(([[:alpha:]]|[[:digit:]]){3,4})$");

  //
  const TokenList_T& oTokenList = extractTokenList (iTokenList, lRegEx);
  return oTokenList;
}    

// /////////////////////////////////////////////////////////
TokenList_T extractTokenListForUNLOCode (const TokenList_T& iTokenList) {
  /**
   * Expected format:
   *   line:           unlocode_code
   *   unlocode_code:  word ((alpha|digit){5})
   */
  const std::string lRegEx ("^(([[:alpha:]]|[[:digit:]]){5})$");

  //
  const TokenList_T& oTokenList = extractTokenList (iTokenList, lRegEx);
  return oTokenList;
}    

// /////////////////////////////////////////////////////////
TokenList_T extractTokenListForUICCode (const TokenList_T& iTokenList) {
  /**
   * Expected format:
   *   line:      uic_code
   *   uic_code:  number (digit{1,11})
   */
  const std::string lRegEx ("^([[:digit:]]{1,11})$");

  //
  const TokenList_T& oTokenList = extractTokenList (iTokenList, lRegEx);
  return oTokenList;
}    

// /////////////////////////////////////////////////////////
TokenList_T extractTokenListForGeonameID (const TokenList_T& iTokenList) {
  /**
   * Expected format:
   *   line:          geoname_id
   *   geoname_id:    number (digit{1,11})
   */
  const std::string lRegEx ("^([[:digit:]]{1,11})$");

  //
  const TokenList_T& oTokenList = extractTokenList (iTokenList, lRegEx);
  return oTokenList;
}    


// //////////////////////////////////////////////////////////////////
/**
 * Check that a SQL database has been configured (i.e., that the SQL
 * database type is not 'nodb') before performing a SQL database look-up.
 *
 * When no SQL database is configured, the underlying OpenTREP service
 * methods (e.g., listByIataCode()) cannot obtain a SOCI database session,
 * and would otherwise fail with an assertion error. So, that check avoids
 * that crash, and instead reports a user-friendly error message.
 */
bool requireSQLDatabase (const OPENTREP::DBType& iDBType,
                        const std::string& iCommandName) {
  if (iDBType == OPENTREP::DBType::NODB) {
    std::cout << "The '" << iCommandName << "' command requires a SQL "
              << "database (SQLite3, MySQL/MariaDB or PostgreSQL), but no "
              << "SQL database has been configured (the SQL database type "
              << "is currently set to '" << iDBType.describe() << "'). "
              << "Restart the application with the '--sqldbtype' (-t) "
              << "option set to 'sqlite', 'mysql' or 'pg', or type 'info' "
              << "to check the current configuration." << std::endl;
    return false;
  }
  return true;
}

// //////////////////////////////////////////////////////////////////
/**
 * Split a command line into a list of semicolon-separated commands,
 * trimming white spaces around each command and discarding empty ones.
 *
 * That method is used by the non-interactive command mode (-c/--command
 * option), so that several dbmgr commands may be given at once
 * (e.g., "list_nb ; list_by_iata nce").
 */
std::vector<std::string> splitCommandLine (const std::string& iCommandLine) {
  std::vector<std::string> oCommandList;

  boost::char_separator<char> lSeparator (";");
  boost::tokenizer<boost::char_separator<char> > lTokenizer (iCommandLine,
                                                             lSeparator);
  for (boost::tokenizer<boost::char_separator<char> >::const_iterator itTok =
         lTokenizer.begin(); itTok != lTokenizer.end(); ++itTok) {
    std::string lSingleCommand (*itTok);
    boost::algorithm::trim (lSingleCommand);
    if (lSingleCommand.empty() == false) {
      oCommandList.push_back (lSingleCommand);
    }
  }

  return oCommandList;
}

// //////////////////////////////////////////////////////////////////
/**
 * Interpret and execute a single (already tokenised) dbmgr command.
 *
 * That dispatcher is shared by both the interactive (readline-based)
 * session and the non-interactive command mode (-c/--command option),
 * so that the two modes behave identically for a given command.
 */
Command_T::Type_T
processUserCommand (const std::string& iUserInput, TokenList_T& ioTokenList,
                    OPENTREP::OPENTREP_Service& ioOpentrepService,
                    const OPENTREP::DBType& iDBType,
                    const std::string& iLogFilename,
                    const std::string& iPORFilepathStr,
                    OPENTREP::DeploymentNumber_T& ioDeploymentNumber,
                    OPENTREP::shouldIndexNonIATAPOR_T& ioIncludeNonIATAPOR,
                    OPENTREP::shouldIndexPORInXapian_T& ioShouldIndexPORInXapian,
                    OPENTREP::shouldAddPORInSQLDB_T& ioShouldAddPORInSQLDB) {
  // Interpret the user input
  Command_T::Type_T lCommandType = extractCommand (ioTokenList);

    switch (lCommandType) {

      // ////////////////////////////// Help ////////////////////////
    case Command_T::HELP: {
      std::cout << std::endl;
      std::cout << "Commands: " << std::endl;
      std::cout << " CTRL-L (Control and L keys)" << "\t" << "Clean the screen"
                << std::endl;
      std::cout << " help" << "\t\t\t\t" << "Display this help" << std::endl;
      std::cout << " info" << "\t\t\t\t"
                << "Display details for the current session "
                << "(e.g., file-paths for the log file, SQL database)"
                << std::endl;
      std::cout << " tutorial" << "\t\t\t" << "Display examples" << std::endl;
      std::cout << " quit" << "\t\t\t\t" << "Quit the application" << std::endl;
      std::cout << " create_user" << "\t\t\t"
                << "On SQL database, create the 'trep' user and the 'trep_trep' "
                << "database. SQL database administrative rights are required."
                << std::endl;
      std::cout << " reset_connection_string" << "\t"
                << "Reset/update the connection string to a MySQL or PostgreSQL database."
                << " The connection string must be given"
                << std::endl;
      std::cout << " create_tables" << "\t\t\t"
                << "Create/reset the SQL database (eg, SQLite3, PostgreSQL, MySQL) tables"
                << std::endl;
      std::cout << " create_indexes" << "\t\t\t"
                << "Create/reset the SQL database (eg, SQLite3, PostgreSQL, MySQL) indices"
                << std::endl;
      std::cout << " toggle_deployment_number" << "\t"
                << "Toggle the deployment number/version. "
                << "To see the deployment version/number, type 'info'"
                << std::endl;
      std::cout << " toggle_noniata_indexing_flag" << "\t"
                << "Toggle the flag for the indexing (or not) of the non-IATA referenced POR."
                << " To see the flag, type 'info'"
                << std::endl;
      std::cout << " toggle_xapian_idexing_flag" << "\t"
                << "Toggle the flag for the Xapian indexing (or not) of the POR."
                << " To see the flag, type 'info'"
                << std::endl;
      std::cout << " toggle_sqldb_inserting_flag" << "\t"
                << "Toggle the flag for inserting (or not) the POR into the SQL database."
                << " To see the flag, type 'info'"
                << std::endl;
      std::cout << " fill_from_por_file" << "\t\t"
                << "Parse the file of POR and fill-in the SQL database optd_por table."
                << std::endl << "\t\t\t\t"
                << "That command (re-)creates both the Xapian index and the SQL tables (as well as the indices), if needed."
                << std::endl << "\t\t\t\t"
                << "Note that, as that command takes minutes, the connection to the SQL database may be lost and the program will exit abnormally."
                << std::endl << "\t\t\t\t"
                << "In that latter case, just re-execute the program and check how far the indexation went by executing the following command."
                << std::endl;
      std::cout << " list_nb" << "\t\t\t"
                << "Display the number of the entries of the database."
                << std::endl;
      std::cout << " list_all" << "\t\t\t"
                << "List all the entries of the database, page by page."
                << "Type the 'list_cont' command for a page down" << std::endl;
      std::cout << " list_by_iata" << "\t\t\t"
                << "List all the entries for a given IATA code"
                << std::endl;
      std::cout << " list_by_icao" << "\t\t\t"
                << "List all the entries for a given ICAO code"
                << std::endl;
      std::cout << " list_by_faa" << "\t\t\t"
                << "List all the entries for a given FAA code"
                << std::endl;
      std::cout << " list_by_unlocode" << "\t\t\t"
                << "List all the entries for a given UN/LOCODE code"
                << std::endl;
      std::cout << " list_by_uiccode" << "\t\t\t"
                << "List all the entries for a given UIC code"
                << std::endl;
      std::cout << " list_by_geonameid" << "\t\t"
                << "List all the entries for a given Geoname ID"
                << std::endl;
      std::cout << std::endl;
      break;
    }
 
      // ////////////////////////////// Information ////////////////////////
    case Command_T::INFO: {
      const OPENTREP::OPENTREP_Service::FilePathSet_T& lFPSet =
        ioOpentrepService.getFilePaths();
      const OPENTREP::OPENTREP_Service::DBFilePathPair_T& lDBFPPair =
        lFPSet.second;
      const OPENTREP::TravelDBFilePath_T& lXapianDBFP = lDBFPPair.first;
      const OPENTREP::SQLDBConnectionString_T& lSQLConnStr = lDBFPPair.second;
      std::cout << std::endl;
      std::cout << "Log file-path: " << "\t\t\t\t\t" << iLogFilename
                << std::endl;
      std::cout << "POR file-path: " << "\t\t\t\t\t" << iPORFilepathStr
                << std::endl;
      std::cout << "Xapian index/database file-path: " << "\t\t"
                << lXapianDBFP << std::endl;
      std::cout << "SQL database type: " << "\t\t\t\t" << iDBType.describe()
                << std::endl;
      std::cout << "SQL database connection string: " << "\t\t" << lSQLConnStr
                << std::endl;
      std::cout << "Deployment number/version: " << "\t\t\t"
                << ioDeploymentNumber << "/"
                << OPENTREP::DEFAULT_OPENTREP_DEPLOYMENT_NUMBER_SIZE-1
                << std::endl;
      std::cout << "Whether to index NON-IATA-referenced POR: " << "\t"
                << ioIncludeNonIATAPOR << std::endl;
      std::cout << "Whether to index the POR in Xapian: " << "\t\t"
                << ioShouldIndexPORInXapian << std::endl;
      std::cout << "Whether to insert the POR in the SQL DB: " << "\t"
                << ioShouldAddPORInSQLDB << std::endl;
      std::cout << std::endl;
      break;
    }
 
      // /////////////////////////// Help with Examples //////////////////////
    case Command_T::TUTORIAL: {
      std::cout << std::endl;
      std::cout << "Typical succession of commands" << std::endl;
      std::cout << "    --------    " << std::endl;
      std::cout << "Check with the 'info' command and adjust the various flags:"
                << std::endl;
      std::cout << " toggle_deployment_number" << std::endl;
      std::cout << " toggle_noniata_indexing_flag" << std::endl;
      std::cout << " toggle_xapian_idexing_flag" << std::endl;
      std::cout << " toggle_sqldb_inserting_flag" << std::endl;
      std::cout << std::endl;
      std::cout << "    --------    " << std::endl;
      std::cout << "Re-indexing of the POR data file:" << std::endl;
      std::cout << " fill_from_por_file" << std::endl;
      std::cout << std::endl;
      std::cout << "    --------    " << std::endl;
      std::cout << "Check the content of the SQL database:" << std::endl;
      std::cout << " list_nb" << std::endl;
      std::cout << " list_by_iata nce" << std::endl;
      std::cout << " list_by_icao lfmn" << std::endl;
      std::cout << " list_by_faa jfk" << std::endl;
      std::cout << " list_by_unlocode deham" << std::endl;
      std::cout << " list_by_uiccode 87775007" << std::endl;
      std::cout << " list_by_geonameid 6299418" << std::endl;
      std::cout << std::endl;
      std::cout << "    --------    " << std::endl;
      std::cout << "Management of the database user and database:" << std::endl;
      std::cout << "* For PostgreSQL:" << std::endl;
      std::cout <<" reset_connection_string db=postgres user=$USER password=<passwd>"
                << std::endl;
      std::cout << "* For MySQL:" << std::endl;
      std::cout <<" reset_connection_string db=mysql user=root password=<passwd>"
                << std::endl;
      std::cout << " create_user" << std::endl;
      std::cout <<" reset_connection_string db=trep_trep user=trep password=trep"
                << std::endl;
      std::cout << " create_tables" << std::endl;
      std::cout << " create_indexes" << std::endl;
      std::cout << std::endl;
      break;
    }
 
      // ////////////////////////////// Quit ////////////////////////
    case Command_T::QUIT: {
      break;
    }

      // ////////////////////////////// List Number /////////////////////////
    case Command_T::LIST_NB: {
      // Call the underlying OpenTREP service
      if (iDBType == OPENTREP::DBType::NODB) {
        const OPENTREP::NbOfDBEntries_T nbOfEntries =
          ioOpentrepService.getIndexSize();

        // Reporting
        std::cout << nbOfEntries
                  << " POR (points of reference) have been found in the Xapian "
                  << "index. Type 'info' to know where that Xapian index is "
                  "located." << std::endl;

      } else {
        const OPENTREP::NbOfDBEntries_T nbOfEntries =
          ioOpentrepService.getNbOfPORFromDB();

        // Reporting
        std::cout << nbOfEntries
                  << " POR (points of reference) have been found in the "
                  << iDBType.describe() << " database" << std::endl;
      }

      break;
    }

      // ////////////////////////////// List All /////////////////////////
    case Command_T::LIST_ALL: {
      if (requireSQLDatabase (iDBType, "list_all") == false) {
        break;
      }

      // For now, just hard code a single IATA code.
      // TODO: implement the page down process, so that the full list
      // can be retrieved and browsed.
      const std::string lIataCodeStr ("NCE");

      // Call the underlying OpenTREP service
      const OPENTREP::IATACode_T lIataCode (lIataCodeStr);
      OPENTREP::LocationList_T lLocationList;
      const OPENTREP::NbOfMatches_T nbOfMatches =
        ioOpentrepService.listByIataCode (lIataCode, lLocationList);

      //
      std::cout << nbOfMatches << " (geographical) location(s) have been found "
                << "matching the IATA code ('" << lIataCodeStr << "')."
                << std::endl;
      
      if (nbOfMatches != 0) {
        OPENTREP::NbOfMatches_T idx = 1;
        for (OPENTREP::LocationList_T::const_iterator itLocation =
               lLocationList.begin();
             itLocation != lLocationList.end(); ++itLocation, ++idx) {
          const OPENTREP::Location& lLocation = *itLocation;
          std::cout << " [" << idx << "]: " << lLocation.toString() << std::endl;
        }

      } else {
        std::cout << "List of unmatched words:" << std::endl;
        std::cout << " [" << 1 << "]: " << lIataCodeStr << std::endl;
      }

      break;
    }

      // ////////////////////////// List by IATA code ////////////////////////
    case Command_T::LIST_BY_IATA: {
      if (requireSQLDatabase (iDBType, "list_by_iata") == false) {
        break;
      }

      //
      TokenList_T lTokenList = extractTokenListForIataCode(ioTokenList);

      // Parse the parameters given by the user, giving default values
      // in case the user does not specify some (or all) of them
      std::string lIataCodeStr ("nce");
      parsePlaceKey (lTokenList, lIataCodeStr);

      // Call the underlying OpenTREP service
      const OPENTREP::IATACode_T lIataCode (lIataCodeStr);
      OPENTREP::LocationList_T lLocationList;
      const OPENTREP::NbOfMatches_T nbOfMatches =
        ioOpentrepService.listByIataCode (lIataCode, lLocationList);

      //
      std::cout << nbOfMatches << " (geographical) location(s) have been found "
                << "matching the IATA code ('" << lIataCodeStr << "')."
                << std::endl;
      
      if (nbOfMatches != 0) {
        OPENTREP::NbOfMatches_T idx = 1;
        for (OPENTREP::LocationList_T::const_iterator itLocation =
               lLocationList.begin();
             itLocation != lLocationList.end(); ++itLocation, ++idx) {
          const OPENTREP::Location& lLocation = *itLocation;
          std::cout << " [" << idx << "]: " << lLocation << std::endl;
        }

      } else {
        std::cout << "List of unmatched words:" << std::endl;
        std::cout << " [" << 1 << "]: " << lIataCodeStr << std::endl;
      }

      break;
    }

      // ////////////////////////// List by ICAO code ////////////////////////
    case Command_T::LIST_BY_ICAO: {
      if (requireSQLDatabase (iDBType, "list_by_icao") == false) {
        break;
      }

      //
      TokenList_T lTokenList = extractTokenListForIcaoCode(ioTokenList);

      // Parse the parameters given by the user, giving default values
      // in case the user does not specify some (or all) of them
      std::string lIcaoCodeStr ("lfmn");
      parsePlaceKey (lTokenList, lIcaoCodeStr);

      // Call the underlying OpenTREP service
      const OPENTREP::ICAOCode_T lIcaoCode (lIcaoCodeStr);
      OPENTREP::LocationList_T lLocationList;
      const OPENTREP::NbOfMatches_T nbOfMatches =
        ioOpentrepService.listByIcaoCode (lIcaoCode, lLocationList);

      //
      std::cout << nbOfMatches << " (geographical) location(s) have been found "
                << "matching the ICAO code ('" << lIcaoCodeStr << "')."
                << std::endl;
      
      if (nbOfMatches != 0) {
        OPENTREP::NbOfMatches_T idx = 1;
        for (OPENTREP::LocationList_T::const_iterator itLocation =
               lLocationList.begin();
             itLocation != lLocationList.end(); ++itLocation, ++idx) {
          const OPENTREP::Location& lLocation = *itLocation;
          std::cout << " [" << idx << "]: " << lLocation << std::endl;
        }

      } else {
        std::cout << "List of unmatched words:" << std::endl;
        std::cout << " [" << 1 << "]: " << lIcaoCodeStr << std::endl;
      }

      break;
    }

      // ////////////////////////// List by FAA code ////////////////////////
    case Command_T::LIST_BY_FAA: {
      if (requireSQLDatabase (iDBType, "list_by_faa") == false) {
        break;
      }

      //
      TokenList_T lTokenList = extractTokenListForFaaCode(ioTokenList);

      // Parse the parameters given by the user, giving default values
      // in case the user does not specify some (or all) of them
      std::string lFaaCodeStr ("jfk");
      parsePlaceKey (lTokenList, lFaaCodeStr);

      // Call the underlying OpenTREP service
      const OPENTREP::FAACode_T lFaaCode (lFaaCodeStr);
      OPENTREP::LocationList_T lLocationList;
      const OPENTREP::NbOfMatches_T nbOfMatches =
        ioOpentrepService.listByFaaCode (lFaaCode, lLocationList);

      //
      std::cout << nbOfMatches << " (geographical) location(s) have been found "
                << "matching the FAA code ('" << lFaaCodeStr << "')."
                << std::endl;
      
      if (nbOfMatches != 0) {
        OPENTREP::NbOfMatches_T idx = 1;
        for (OPENTREP::LocationList_T::const_iterator itLocation =
               lLocationList.begin();
             itLocation != lLocationList.end(); ++itLocation, ++idx) {
          const OPENTREP::Location& lLocation = *itLocation;
          std::cout << " [" << idx << "]: " << lLocation << std::endl;
        }

      } else {
        std::cout << "List of unmatched words:" << std::endl;
        std::cout << " [" << 1 << "]: " << lFaaCodeStr << std::endl;
      }

      break;
    }

      // //////////////////////// List by UN/LOCODE code //////////////////////
    case Command_T::LIST_BY_UNLOCODE: {
      if (requireSQLDatabase (iDBType, "list_by_unlocode") == false) {
        break;
      }

      //
      TokenList_T lTokenList = extractTokenListForUNLOCode(ioTokenList);

      // Parse the parameters given by the user, giving default values
      // in case the user does not specify some (or all) of them
      std::string lUNLOCodeStr ("deham");
      parsePlaceKey (lTokenList, lUNLOCodeStr);

      // Call the underlying OpenTREP service
      const OPENTREP::UNLOCode_T lUNLOCode (lUNLOCodeStr);
      OPENTREP::LocationList_T lLocationList;
      const OPENTREP::NbOfMatches_T nbOfMatches =
        ioOpentrepService.listByUNLOCode (lUNLOCode, lLocationList);

      //
      std::cout << nbOfMatches << " (geographical) location(s) have been found "
                << "matching the UN/LOCODE code ('" << lUNLOCodeStr << "')."
                << std::endl;
      
      if (nbOfMatches != 0) {
        OPENTREP::NbOfMatches_T idx = 1;
        for (OPENTREP::LocationList_T::const_iterator itLocation =
               lLocationList.begin();
             itLocation != lLocationList.end(); ++itLocation, ++idx) {
          const OPENTREP::Location& lLocation = *itLocation;
          std::cout << " [" << idx << "]: " << lLocation << std::endl;
        }

      } else {
        std::cout << "List of unmatched words:" << std::endl;
        std::cout << " [" << 1 << "]: " << lUNLOCodeStr << std::endl;
      }

      break;
    }

      // //////////////////////// List by UIC code //////////////////////
    case Command_T::LIST_BY_UICCODE: {
      if (requireSQLDatabase (iDBType, "list_by_uiccode") == false) {
        break;
      }

      //
      TokenList_T lTokenList = extractTokenListForUICCode(ioTokenList);

      // Parse the parameters given by the user, giving default values
      // in case the user does not specify some (or all) of them
      std::string lUICCodeStr ("87775007");
      parsePlaceKey (lTokenList, lUICCodeStr);

      // Convert the string into an integer
      OPENTREP::UICCode_T lUICCode;

      try {

        lUICCode = boost::lexical_cast<OPENTREP::UICCode_T> (lUICCodeStr);

      } catch (boost::bad_lexical_cast& eCast) {
        lUICCode = 87775007;
        std::cerr << "The UIC code ('" << lUICCodeStr
                  << "') cannot be understood. The default value ("
                  << lUICCode << ") is kept." << std::endl;
      }

      // Call the underlying OpenTREP service
      OPENTREP::LocationList_T lLocationList;
      const OPENTREP::NbOfMatches_T nbOfMatches =
        ioOpentrepService.listByUICCode (lUICCode, lLocationList);

      //
      std::cout << nbOfMatches << " (geographical) location(s) have been found "
                << "matching the UIC code ('" << lUICCodeStr << "')."
                << std::endl;
      
      if (nbOfMatches != 0) {
        OPENTREP::NbOfMatches_T idx = 1;
        for (OPENTREP::LocationList_T::const_iterator itLocation =
               lLocationList.begin();
             itLocation != lLocationList.end(); ++itLocation, ++idx) {
          const OPENTREP::Location& lLocation = *itLocation;
          std::cout << " [" << idx << "]: " << lLocation << std::endl;
        }

      } else {
        std::cout << "List of unmatched words:" << std::endl;
        std::cout << " [" << 1 << "]: " << lUICCodeStr << std::endl;
      }

      break;
    }

      // ////////////////////////// List by Geoname ID ////////////////////////
    case Command_T::LIST_BY_GEONAMEID: {
      if (requireSQLDatabase (iDBType, "list_by_geonameid") == false) {
        break;
      }

      //
      TokenList_T lTokenList =
        extractTokenListForGeonameID (ioTokenList);

      // Parse the parameters given by the user, giving default values
      // in case the user does not specify some (or all) of them
      std::string lGeonameIDStr ("6299418");
      parsePlaceKey (lTokenList, lGeonameIDStr);

      // Convert the string into an integer
      OPENTREP::GeonamesID_T lGeonameID;

      try {

        lGeonameID = boost::lexical_cast<OPENTREP::GeonamesID_T> (lGeonameIDStr);

      } catch (boost::bad_lexical_cast& eCast) {
        lGeonameID = 6299418;
        std::cerr << "The Geoname ID ('" << lGeonameIDStr
                  << "') cannot be understood. The default value ("
                  << lGeonameID << ") is kept." << std::endl;
      }

      // Call the underlying OpenTREP service
      OPENTREP::LocationList_T lLocationList;
      const OPENTREP::NbOfMatches_T nbOfMatches =
        ioOpentrepService.listByGeonameID (lGeonameID, lLocationList);

      //
      std::cout << nbOfMatches << " (geographical) location(s) have been found "
                << "matching the Geoname ID ('" << lGeonameIDStr << "')."
                << std::endl;
      
      if (nbOfMatches != 0) {
        OPENTREP::NbOfMatches_T idx = 1;
        for (OPENTREP::LocationList_T::const_iterator itLocation =
               lLocationList.begin();
             itLocation != lLocationList.end(); ++itLocation, ++idx) {
          const OPENTREP::Location& lLocation = *itLocation;
          std::cout << " [" << idx << "]: " << lLocation << std::endl;
        }

      } else {
        std::cout << "List of unmatched items:" << std::endl;
        std::cout << " [" << 1 << "]: " << lGeonameIDStr << std::endl;
      }

      break;
    }

      // ///////////////////////// Database Creation /////////////////////////
    case Command_T::CREATE_USER: {
      //
      std::cout << "Creating the 'trep' user and 'trep_trep' database"
                << std::endl;
    
      // On MySQL/MariaDB and PostgreSQL, create the 'trep' user and 'trep_trep'
      // database.
      // On SQLite, delete the directory hosting the database, and re-create it.
      // On other database types, do nothing.
      //
      // Note: on MySQL/MariaDB and PostgreSQL, that command connects to the
      // deployment-slot-suffixed database (e.g., 'trep0') in order to
      // (re-)create the 'trep' user/role and that same database. If that
      // database does not exist yet, and/or the connecting user lacks the
      // privilege to create it, the connection attempt fails and throws a
      // SQLDatabaseException (e.g., SQLDatabaseImpossibleConnectionException
      // or SQLDatabaseUserCreationException). That exception is caught here
      // so that such a (mis-)configuration is reported as a friendly error
      // message instead of aborting the whole application.
      try {
        const bool lCreationSuccessful = ioOpentrepService.createSQLDBUser();

        // Reporting
        if (lCreationSuccessful == true) {
          std::cout << "The 'trep' user and 'trep_trep' database have been created"
                    << std::endl;
        } else {
          std::cout << "The 'trep' user and 'trep_trep' database could not be "
                    << "created. See the log file ('" << iLogFilename
                    << "') for details." << std::endl;
        }

      } catch (const OPENTREP::SQLDatabaseException& eSQLDBException) {
        std::cerr << "The 'trep' user and '" << iDBType.describe()
                  << "' database could not be created: "
                  << eSQLDBException.what() << ". This may happen when the "
                  << "deployment-slot database (e.g., 'trep0') does not "
                  << "exist yet and/or the connecting user/role lacks the "
                  << "privilege to create databases. On PostgreSQL, that "
                  << "database may need to be pre-created (e.g., "
                  << "'createdb -h localhost -U trep trep0'), or the 'trep' "
                  << "role may need the CREATEDB privilege." << std::endl;
      }

      break;
    }

      // ///////////////////// Database connection string //////////////////////
    case Command_T::RESET_CONNECTION_STRING: {
      // Parse the parameters given by the user, giving default values
      // in case the user does not specify some (or all) of them
      const std::string lConnectionStringStr = toString (ioTokenList);

      //
      std::cout << "Reset the connection string" << std::endl;
    
      // Reset the connection string
      //
      // Note: the connection string format is specific to the current SQL
      // database type. For instance, PostgreSQL expects libpq-style
      // keywords (e.g., "dbname=trep user=trep password=trep"), whereas
      // MySQL/MariaDB expects "db=trep user=trep password=trep". Setting
      // the connection string triggers its parsing (so as to be able to
      // later append the deployment number/version to the database name),
      // which throws a SQLDatabaseConnectionStringParsingException when the
      // given string does not match the format expected for that database
      // type. That exception is caught here so that a malformed connection
      // string reports a friendly error message instead of aborting the
      // whole application.
      try {
        const OPENTREP::SQLDBConnectionString_T
          lConnectionString (lConnectionStringStr);
        ioOpentrepService.setSQLDBConnectString (lConnectionString);

        //
        std::cout << "The connection string has been reset" << std::endl;

      } catch (const OPENTREP::SQLDatabaseConnectionStringParsingException& eParsing) {
        std::cerr << "The connection string ('" << lConnectionStringStr
                  << "') cannot be understood for the '" << iDBType.describe()
                  << "' database type: " << eParsing.what() << ". "
                  << "The connection string has not been changed." << std::endl;
      }

      break;
    }
        
      // /////////////////// Deployment number/version /////////////////////
    case Command_T::TOGGLE_DEPLOYMENT_NUMBER: {
      // Toggle the deployment number/version
      ioDeploymentNumber = ioOpentrepService.toggleDeploymentNumber();

      // Reporting
      std::cout << "The new deployment number/version is: " << ioDeploymentNumber
                << "/" << OPENTREP::DEFAULT_OPENTREP_DEPLOYMENT_NUMBER_SIZE-1
                << std::endl;
    
      break;
    }
                                                             
      // /////////////////// Index or not non-IATA POR /////////////////////
    case Command_T::TOGGLE_NONIATA_INDEXING_FLAG: {
      // Toggle the flag
      ioIncludeNonIATAPOR = ioOpentrepService.toggleShouldIncludeAllPORFlag();

      // Reporting
      std::cout << "The new flag is: " << ioIncludeNonIATAPOR << std::endl;
    
      break;
    }
                                                             
      // ///////////////////// Index or not in Xapian ///////////////////////
    case Command_T::TOGGLE_XAPIAN_IDEXING_FLAG: {
      // Toggle the flag
      ioShouldIndexPORInXapian =
        ioOpentrepService.toggleShouldIndexPORInXapianFlag();

      // Reporting
      std::cout << "The new flag is: " << ioShouldIndexPORInXapian << std::endl;
    
      break;
    }
                                                             
      // ///////////////////// Add or not in SQL DB ///////////////////////
    case Command_T::TOGGLE_SQLDB_INSERTING_FLAG: {
      // Toggle the flag
      ioShouldAddPORInSQLDB = ioOpentrepService.toggleShouldAddPORInSQLDBFlag();

      // Reporting
      std::cout << "The new flag is: " << ioShouldAddPORInSQLDB << std::endl;
    
      break;
    }
                                                             
      // ///////////////////////// Tables Creation /////////////////////////
    case Command_T::CREATE_TABLES: {
      //
      std::cout << "Creating/resetting the " << iDBType.describe()
                << " database tables" << std::endl;
    
      // Create/reset the tables (on SQLite3, PostgreSQL, MySQL)
      //
      // Note: this connects to the deployment-slot-suffixed database (e.g.,
      // 'trep0'). If that database has not been created beforehand (see the
      // 'create_user' command) and/or the DDL statements fail (e.g., due to
      // insufficient privileges), a SQLDatabaseException is thrown. It is
      // caught here so as to report a friendly error message instead of
      // aborting the whole application.
      try {
        ioOpentrepService.createSQLDBTables();

        //
        std::cout << "The " << iDBType.describe()
                  << " tables has been created/resetted" << std::endl;

      } catch (const OPENTREP::SQLDatabaseException& eSQLDBException) {
        std::cerr << "The " << iDBType.describe() << " database tables could "
                  << "not be created/reset: " << eSQLDBException.what()
                  << ". Make sure that the '" << iDBType.describe()
                  << "' database has already been created (see the "
                  << "'create_user' command) and that the corresponding "
                  << "connection string is correct (see the 'info' command)."
                  << std::endl;
      }

      break;
    }

      // ///////////////////////// Indexes Creation /////////////////////////
    case Command_T::CREATE_INDEXES: {
      //
      std::cout << "Creating/resetting the " << iDBType.describe()
                << " database indices" << std::endl;
    
      // Create/reset the indices (on SQLite3, PostgreSQL, MySQL)
      //
      // See the note in the 'create_tables' case above: the same class of
      // exception can be thrown here (e.g., if the database does not exist,
      // or the underlying tables have not been created yet), and is caught
      // for the same reason.
      try {
        ioOpentrepService.createSQLDBIndexes();

        //
        std::cout << "The " << iDBType.describe()
                  << " indices has been created/resetted" << std::endl;

      } catch (const OPENTREP::SQLDatabaseException& eSQLDBException) {
        std::cerr << "The " << iDBType.describe() << " database indices "
                  << "could not be created/reset: " << eSQLDBException.what()
                  << ". Make sure that the '" << iDBType.describe()
                  << "' database and tables have already been created (see "
                  << "the 'create_user' and 'create_tables' commands)."
                  << std::endl;
      }

      break;
    }

      // ///////////////////////// POR File Indexing /////////////////////////
    case Command_T::FILL_FROM_POR_FILE: {
      //
      std::cout << "Indexing the POR file and filling in the SQL database may "
                << "take a few minutes on some architectures "
                << "(and a few seconds on fastest ones)..."
                << std::endl;
    
      // Launch the indexation
      const OPENTREP::NbOfDBEntries_T lNbOfEntries =
        ioOpentrepService.insertIntoDBAndXapian();

      //
      std::cout << lNbOfEntries << " entries have been processed" << std::endl;

      break;
    }

      // /////////////////////////// Default / No value ///////////////////////
    case Command_T::NOP: {
      break;
    }
 
    case Command_T::LAST_VALUE:
    default: {
      // DEBUG
      std::ostringstream oStr;
      oStr << "That command is not yet understood: '" << iUserInput
           << "' => " << ioTokenList;
      OPENTREP_LOG_DEBUG (oStr.str());
      std::cout << oStr.str() << std::endl;
    }
    }


  return lCommandType;
}

// /////////////// M A I N /////////////////
int main (int argc, char* argv[]) {

  // Readline history
  const unsigned int lHistorySize (100);
  const std::string lHistoryFilename ("opentrep-dbmgr.hist");
  const std::string lHistoryBackupFilename ("opentrep-dbmgr.hist.bak");

  // Output log File
  std::string lLogFilename;

  // File-path of POR (points of reference)
  std::string lPORFilepathStr;

  // Xapian database name (directory of the index)
  std::string lXapianDBNameStr;

  // SQL database type
  std::string lSQLDBTypeStr;

  // SQL database connection string
  std::string lSQLDBConnectionStr;

  // Deployment number/version
  OPENTREP::DeploymentNumber_T lDeploymentNumber;
  
  // Whether or not to include non-IATA-referenced POR
  OPENTREP::shouldIndexNonIATAPOR_T lIncludeNonIATAPOR;

  // Whether or not to index the POR in Xapian
  OPENTREP::shouldIndexPORInXapian_T lShouldIndexPORInXapian;
  
  // Whether or not to insert the POR in the SQL database
  OPENTREP::shouldAddPORInSQLDB_T lShouldAddPORInSQLDB;

  // Semicolon-separated command(s) to execute in non-interactive mode
  // (-c/--command option)
  std::string lCommandStr;

  // Call the command-line option parser
  const int lOptionParserStatus =
    readConfiguration (argc, argv, lPORFilepathStr, lXapianDBNameStr,
                       lSQLDBTypeStr, lSQLDBConnectionStr, lDeploymentNumber,
                       lIncludeNonIATAPOR, lShouldIndexPORInXapian,
                       lShouldAddPORInSQLDB, lLogFilename, lCommandStr);

  if (lOptionParserStatus == K_OPENTREP_EARLY_RETURN_STATUS) {
    return 0;
  }

  // Set the log parameters
  std::ofstream logOutputFile;
  // open and clean the log outputfile
  logOutputFile.open (lLogFilename.c_str());
  logOutputFile.clear();

  // Initialise the context
  const OPENTREP::PORFilePath_T lPORFilepath (lPORFilepathStr);
  const OPENTREP::TravelDBFilePath_T lXapianDBName (lXapianDBNameStr);
  const OPENTREP::DBType lDBType (lSQLDBTypeStr);
  const OPENTREP::SQLDBConnectionString_T lSQLDBConnStr (lSQLDBConnectionStr);
  OPENTREP::OPENTREP_Service opentrepService (logOutputFile, lPORFilepath,
                                              lXapianDBName,
                                              lDBType, lSQLDBConnStr,
                                              lDeploymentNumber,
                                              lIncludeNonIATAPOR,
                                              lShouldIndexPORInXapian,
                                              lShouldAddPORInSQLDB);

  // If one (or several) command(s) have been given on the command-line
  // (-c/--command option), execute them in a non-interactive way (i.e.,
  // without prompting the user, nor using the GNU readline library),
  // and then exit.
  if (lCommandStr.empty() == false) {
    // DEBUG
    OPENTREP_LOG_DEBUG ("==================================================");
    OPENTREP_LOG_DEBUG ("=     Beginning of the non-interactive session   =");
    OPENTREP_LOG_DEBUG ("==================================================");

    const TokenList_T lCommandLineList = splitCommandLine (lCommandStr);
    Command_T::Type_T lNonInteractiveCommandType (Command_T::NOP);

    for (TokenList_T::const_iterator itCommand = lCommandLineList.begin();
         itCommand != lCommandLineList.end()
           && lNonInteractiveCommandType != Command_T::QUIT; ++itCommand) {
      const std::string& lSingleCommandStr = *itCommand;

      // Echo the command being executed, so that the (non-interactive)
      // session remains easy to follow (e.g., when scripted)
      std::cout << "opentrep> " << lSingleCommandStr << std::endl;

      // Tokenise that single command, the same way the GNU readline
      // library would have done for the interactive session
      TokenList_T lTokenList;
      SplitTokens (lSingleCommandStr, lTokenList);

      // Interpret and execute the command, re-using the very same
      // dispatcher as the interactive session
      lNonInteractiveCommandType =
        processUserCommand (lSingleCommandStr, lTokenList, opentrepService,
                           lDBType, lLogFilename, lPORFilepathStr,
                           lDeploymentNumber, lIncludeNonIATAPOR,
                           lShouldIndexPORInXapian, lShouldAddPORInSQLDB);
    }

    // DEBUG
    OPENTREP_LOG_DEBUG ("End of the session. Exiting.");
    std::cout << "End of the session. Exiting." << std::endl;

    // Close the Log outputFile
    logOutputFile.close();

    return 0;
  }

  // DEBUG
  OPENTREP_LOG_DEBUG ("====================================================");
  OPENTREP_LOG_DEBUG ("=       Beginning of the interactive session       =");
  OPENTREP_LOG_DEBUG ("====================================================");

  // Initialise the GNU readline wrapper
  swift::SReadline lReader (lHistoryFilename, lHistorySize);
  initReadline (lReader);

  // Now we can ask user for a line
  std::string lUserInput;
  bool EndOfInput (false);
  Command_T::Type_T lCommandType (Command_T::NOP);
  
  while (lCommandType != Command_T::QUIT && EndOfInput == false) {
    // Prompt
    std::ostringstream oPromptStr;
    oPromptStr << "opentrep> ";

    // Call read-line, which will fill the list of tokens
    TokenList_T lTokenListByReadline;
    lUserInput = lReader.GetLine (oPromptStr.str(), lTokenListByReadline,
                                  EndOfInput);

    // The history can be saved to an arbitrary file at any time
    lReader.SaveHistory (lHistoryBackupFilename);

    // The end-of-input typically corresponds to a CTRL-D typed by the user
    if (EndOfInput) {
      std::cout << std::endl;
      break;
    }

    // Interpret and execute the user command, reusing the same dispatcher
    // as the non-interactive command mode (-c/--command option)
    lCommandType =
      processUserCommand (lUserInput, lTokenListByReadline, opentrepService,
                         lDBType, lLogFilename, lPORFilepathStr,
                         lDeploymentNumber, lIncludeNonIATAPOR,
                         lShouldIndexPORInXapian, lShouldAddPORInSQLDB);
  }

  // DEBUG
  OPENTREP_LOG_DEBUG ("End of the session. Exiting.");
  std::cout << "End of the session. Exiting." << std::endl;

  // Close the Log outputFile
  logOutputFile.close();
    
  return 0;
}
