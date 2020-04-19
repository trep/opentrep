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
#include <boost/program_options.hpp>
// OpenTREP
#include <opentrep/OPENTREP_Service.hpp>
#include <opentrep/Location.hpp>
#include <opentrep/CityDetails.hpp>
#include <opentrep/DBType.hpp>
#include <opentrep/basic/BasConst_OPENTREP_Service.hpp>
#include <opentrep/basic/Utilities.hpp>
#include <opentrep/config/opentrep-paths.hpp>


// //////// Type definitions ///////
typedef std::vector<std::string> WordList_T;


// //////// Constants //////
/**
 * Default name and location for the log file.
 */
const std::string K_OPENTREP_DEFAULT_LOG_FILENAME ("opentrep-indexer.log");

/**
 * Default flag for the including (or not) of the non-IATA-referenced POR.
 * By default, only POR which are referenced by IATA are included.
 * If the flag is set up at 1, then also the POR referenced by other
 * international organizations (such ICAO or UN/LOCODE) are included.
 *  <br>
 *  <ul>
 *    <li>0 = Do not include non-IATA-referenced POR</li>
 *    <li>1 = Include non-IATA-referenced POR (eg, POR referenced by ICAO
 *        or UN/LOCODE)</li>
 *  </ul>
 */
const bool K_OPENTREP_DEFAULT_POR_INCLUDING = false;


// ///////// Parsing of Options & Configuration /////////
/** Early return status (so that it can be differentiated from an error). */
const int K_OPENTREP_EARLY_RETURN_STATUS = 99;

/** Read and parse the command line options. */
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
                       std::ostringstream& oStr) {

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
     "POR file-path (e.g., optd_por_public.csv)")
    ("xapiandb,d",
     boost::program_options::value< std::string >(&ioXapianDBFilepath)->default_value(OPENTREP::DEFAULT_OPENTREP_XAPIAN_DB_FILEPATH),
     "Xapian database filepath (e.g., /tmp/opentrep/xapian_traveldb)")
    ("sqldbtype,t",
     boost::program_options::value< std::string >(&ioSQLDBTypeString)->default_value(OPENTREP::DEFAULT_OPENTREP_SQL_DB_TYPE),
     "SQL database type (e.g., nodb for no SQL database, sqlite for SQLite, mysql for MariaDB/MySQL)")
    ("sqldbconx,s",
     boost::program_options::value< std::string >(&ioSQLDBConnectionString),
     "SQL database connection string (e.g., ~/tmp/opentrep/sqlite_travel.db for SQLite, \"db=trep_trep user=trep password=trep\" for MariaDB/MySQL)")
    ("deploymentnb,m",
     boost::program_options::value<unsigned short>(&ioDeploymentNumber)->default_value(OPENTREP::DEFAULT_OPENTREP_DEPLOYMENT_NUMBER), 
     "Deployment number (from to N, where N=1 normally)")
    ("noniata,n",
     boost::program_options::value<bool>(&ioIncludeNonIATAPOR)->default_value(K_OPENTREP_DEFAULT_POR_INCLUDING), 
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

  std::ifstream ifs ("opentrep-indexer.cfg");
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
    oStr << "POR file-path is: " << ioPORFilepath << std::endl;
  }

  if (vm.count ("deploymentnb")) {
    ioDeploymentNumber = vm["deploymentnb"].as< unsigned short >();
    oStr << "Deployment number: " << ioDeploymentNumber << std::endl;
  }

  if (vm.count ("xapiandb")) {
    ioXapianDBFilepath = vm["xapiandb"].as< std::string >();
    oStr << "Xapian index/database filepath is: " << ioXapianDBFilepath
         << ioDeploymentNumber << std::endl;
  }

  // Parse the SQL database type, if any is given
  if (vm.count ("sqldbtype")) {
    ioSQLDBTypeString = vm["sqldbtype"].as< std::string >();
    oStr << "SQL database type is: " << ioSQLDBTypeString << std::endl;
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
      || lDBType == OPENTREP::DBType::MYSQL) {
    const std::string& lSQLDBConnString =
      OPENTREP::parseAndDisplayConnectionString (lDBType,
                                                 ioSQLDBConnectionString,
                                                 ioDeploymentNumber);
    //
    oStr << "SQL database connection string is: " << lSQLDBConnString
         << std::endl;
  }

  oStr << "Are non-IATA-referenced POR included? " << ioIncludeNonIATAPOR
       << std::endl;
  
  oStr << "Index the POR in Xapian? " << ioIndexPORInXapian << std::endl;
  
  oStr << "Add and re-index the POR in the SQL-based database? " << ioAddPORInDB
       << std::endl;
  
  if (vm.count ("log")) {
    ioLogFilename = vm["log"].as< std::string >();
    oStr << "Log filename is: " << ioLogFilename << std::endl;
  }

  return 0;
}


// /////////////// M A I N /////////////////
int main (int argc, char* argv[]) {

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

  // Log stream for the introduction part
  std::ostringstream oIntroStr;

  // Call the command-line option parser
  const int lOptionParserStatus =
    readConfiguration (argc, argv, lPORFilepathStr, lXapianDBNameStr,
                       lSQLDBTypeStr, lSQLDBConnectionStr, lDeploymentNumber,
                       lIncludeNonIATAPOR, lShouldIndexPORInXapian,
                       lShouldAddPORInSQLDB, lLogFilename, oIntroStr);

  if (lOptionParserStatus == K_OPENTREP_EARLY_RETURN_STATUS) {
    return 0;
  }

  // Set the log parameters
  std::ofstream logOutputFile;
  // open and clean the log outputfile
  logOutputFile.open (lLogFilename.c_str());
  logOutputFile.clear();

  //
  oIntroStr << "Parsing and indexing the OpenTravelData POR data file (into "
            << "Xapian and/or SQL databases) may take a few tens of minutes "
            << "on some architectures (and a few minutes on fastest ones)..."
            << std::endl;
  std::cout << oIntroStr.str();
    
  // DEBUG
  // Get the current time in UTC Timezone
  boost::posix_time::ptime lTimeUTC =
    boost::posix_time::second_clock::universal_time();
  logOutputFile << "[" << lTimeUTC << "][" << __FILE__ << "#"
                << __LINE__ << "]:Parameters:" << std::endl
                <<  oIntroStr.str() << std::endl;

  // Initialise the context
  const OPENTREP::PORFilePath_T lPORFilepath (lPORFilepathStr);
  const OPENTREP::TravelDBFilePath_T lXapianDBName (lXapianDBNameStr);
  const OPENTREP::DBType lDBType (lSQLDBTypeStr);
  const OPENTREP::SQLDBConnectionString_T lSQLDBConnStr (lSQLDBConnectionStr);
  OPENTREP::OPENTREP_Service opentrepService (logOutputFile, lPORFilepath,
                                              lXapianDBName, lDBType,
                                              lSQLDBConnStr, lDeploymentNumber,
                                              lIncludeNonIATAPOR,
                                              lShouldIndexPORInXapian,
                                              lShouldAddPORInSQLDB);

  // Launch the indexation
  const OPENTREP::NbOfDBEntries_T lNbOfEntries =
    opentrepService.insertIntoDBAndXapian();

  //
  std::ostringstream oStr;
  oStr << lNbOfEntries << " entries have been processed" << std::endl;
  std::cout << oStr.str();

  // Get the current time in UTC Timezone
  lTimeUTC = boost::posix_time::second_clock::universal_time();
  logOutputFile << "[" << lTimeUTC << "][" << __FILE__ << "#"
                << __LINE__ << "]:" <<  oStr.str() << std::endl;
  
  // Close the Log outputFile
  logOutputFile.close();
  
  return 0;
}
