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
#include <opentrep/basic/BasConst_OPENTREP_Service.hpp>
#include <opentrep/config/opentrep-paths.hpp>


// //////// Type definitions ///////
typedef std::vector<std::string> WordList_T;


// //////// Constants //////
/**
 * Default name and location for the log file.
 */
const std::string K_OPENTREP_DEFAULT_LOG_FILENAME ("opentrep-indexer.log");


// ///////// Parsing of Options & Configuration /////////
/** Early return status (so that it can be differentiated from an error). */
const int K_OPENTREP_EARLY_RETURN_STATUS = 99;

/** Read and parse the command line options. */
int readConfiguration (int argc, char* argv[], 
                       std::string& ioPORFilepath, 
                       std::string& ioDatabaseFilepath,
                       std::string& ioLogFilename) {

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
    ("database,d",
     boost::program_options::value< std::string >(&ioDatabaseFilepath)->default_value(OPENTREP::DEFAULT_OPENTREP_XAPIAN_DB_FILEPATH),
     "Xapian database file-path (e.g., /tmp/opentrep/traveldb)")
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
    std::cout << "POR file-path is: " << ioPORFilepath << std::endl;
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

  // Call the command-line option parser
  const int lOptionParserStatus =
    readConfiguration (argc, argv, lPORFilepathStr, lXapianDBNameStr,
                       lLogFilename);

  if (lOptionParserStatus == K_OPENTREP_EARLY_RETURN_STATUS) {
    return 0;
  }

  // Set the log parameters
  std::ofstream logOutputFile;
  // open and clean the log outputfile
  logOutputFile.open (lLogFilename.c_str());
  logOutputFile.clear();

  //
  std::cout << "Creating the Xapian index/database may take a few minutes "
            << "on some architectures (and a few seconds on fastest ones)..."
            << std::endl;
    
  // Initialise the context
  const OPENTREP::PORFilePath_T lPORFilepath (lPORFilepathStr);
  const OPENTREP::TravelDBFilePath_T lXapianDBName (lXapianDBNameStr);
  OPENTREP::OPENTREP_Service opentrepService (logOutputFile, lPORFilepath,
                                              lXapianDBName);

  // Launch the indexation
  const OPENTREP::NbOfDBEntries_T lNbOfEntries =
    opentrepService.buildSearchIndex();

  // Close the Log outputFile
  logOutputFile.close();
    
  //
  std::cout << lNbOfEntries << " entries have been processed" << std::endl;

  return 0;
}
