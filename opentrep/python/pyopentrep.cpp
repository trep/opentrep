// Python
#include <boost/python.hpp>
// STL
#include <cassert>
#include <stdexcept>
#include <fstream>
#include <sstream>
#include <string>
#include <list>
#include <vector>
// Boost Python
#include <boost/filesystem.hpp>
// OpenTREP
#include <opentrep/OPENTREP_Service.hpp>
#include <opentrep/OutputFormat.hpp>
#include <opentrep/Location.hpp>
#include <opentrep/CityDetails.hpp>
#include <opentrep/bom/BomJSONExport.hpp>
#include <opentrep/bom/LocationExchange.hpp>

//
namespace bp = boost::python;

namespace OPENTREP {

  /** 
   * @brief API wrapper around the OpenTREP C++ API, so that Python scripts
   *        can use it seamlessly.
   */
  struct OpenTrepSearcher {
  public:

    /** 
     * Get the file-paths for the:
     * <ul>
     *   <li>OPTD-maintained list of POR (points of reference)</li>
     *   <li>Xapian-based travel database/index</li>
     * </ul>
     */
    std::string getPaths() {
      return getPathsImpl();
    }

    /** 
     * Public wrapper around the indexation use case. 
     */
    std::string index() {
      return indexImpl();
    }

    /** 
     * Public wrapper around the random generation use case
     * for most of the formats.
     */
    std::string generate (const std::string& iOutputFormatString,
                          const NbOfMatches_T& iNbOfDraws) {
      const OutputFormat lOutputFormat (iOutputFormatString);
      const OutputFormat::EN_OutputFormat& lOutputFormatEnum =
        lOutputFormat.getFormat();
      return generateImpl (iNbOfDraws, lOutputFormatEnum);
    }

    /** 
     * Public wrapper around the random generation use case
     * for Protobuf.
     */
    bp::object generateToPB (const NbOfMatches_T& iNbOfDraws) {
      const OutputFormat::EN_OutputFormat lOutputFormatEnum =
        OutputFormat::PROTOBUF;
      //
      const std::string& oPBStr = generateImpl (iNbOfDraws, lOutputFormatEnum);

      const ssize_t lPBSize = oPBStr.size();
      
      // Convert to a byte array; otherwise Python considers it as a str
      // (Unicode string in Python 3), and the decoding fails
      const bp::object& oPBObj =
        bp::object (bp::handle<> (PyBytes_FromStringAndSize (oPBStr.c_str(),
                                                             lPBSize)));

      return oPBObj;
    }

    /** 
     * Public wrapper around the search use case for most of the formats.
     */
    std::string search (const std::string& iOutputFormatString,
                        const std::string& iTravelQuery) {
      const OutputFormat lOutputFormat (iOutputFormatString);
      const OutputFormat::EN_OutputFormat& lOutputFormatEnum =
        lOutputFormat.getFormat();
      return searchImpl (iTravelQuery, lOutputFormatEnum);
    }

    /** 
     * Public wrapper around the search use case for Protobuf.
     */
    bp::object searchToPB (const std::string& iTravelQuery) {
      const OutputFormat::EN_OutputFormat lOutputFormatEnum =
        OutputFormat::PROTOBUF;
      //
      const std::string& oPBStr = searchImpl (iTravelQuery, lOutputFormatEnum);
      const ssize_t lPBSize = oPBStr.size();
      
      // Convert to a byte array; otherwise Python considers it as a str
      // (Unicode string in Python 3), and the decoding fails
      const bp::object& oPBObj =
        bp::object (bp::handle<> (PyBytes_FromStringAndSize (oPBStr.c_str(),
                                                             lPBSize)));
      
      return oPBObj;
    }

  private:
    /**
     * Private wrapper around the file-path retrieval use case. 
     */
    std::string getPathsImpl() {
      std::ostringstream oPythonLogStr;

      // Sanity check
      if (_logOutputStream == NULL) {
        oPythonLogStr << "The log filepath is not valid." << std::endl;
        return oPythonLogStr.str();
      }
      assert (_logOutputStream != NULL);

      try {

        // DEBUG
        *_logOutputStream << "Get the file-path details" << std::endl;

        if (_opentrepService == NULL) {
          oPythonLogStr << "The OpenTREP service has not been initialised, "
                        << "i.e., the init() method has not been called "
                        << "correctly on the OpenTrepSearcher object. Please "
                        << "check that all the parameters are not empty and "
                        << "point to actual files.";
          *_logOutputStream << oPythonLogStr.str();
          return oPythonLogStr.str();
        }
        assert (_opentrepService != NULL);

        // Retrieve the underlying file-path details
        const OPENTREP_Service::FilePathSet_T& lFilePathSet =
          _opentrepService->getFilePaths();
        const PORFilePath_T& lPORFilePath = lFilePathSet.first;
        const OPENTREP_Service::DBFilePathPair_T& lDBFilePathPair =
          lFilePathSet.second;
        const TravelDBFilePath_T& lTravelDBFilePath = lDBFilePathPair.first;
        const SQLDBConnectionString_T& lSQLDBConnStr = lDBFilePathPair.second;

        // Dump the results into the output string
        oPythonLogStr << lPORFilePath << ";" << lTravelDBFilePath
                      << ";" << lSQLDBConnStr;

        // DEBUG
        *_logOutputStream << "OPTD-maintained list of POR: '" << lPORFilePath
                          << "'" << std::endl;
        *_logOutputStream << "Xapian travel database/index: '"
                          << lTravelDBFilePath << "'" << std::endl;
        *_logOutputStream << "SQL database connection string: '"
                          << lSQLDBConnStr << "'" << std::endl;

      } catch (const RootException& eOpenTrepError) {
        *_logOutputStream << "OpenTrep error: "  << eOpenTrepError.what()
                          << std::endl;

      } catch (const std::exception& eStdError) {
        *_logOutputStream << "Error: "  << eStdError.what() << std::endl;

      } catch (...) {
        *_logOutputStream << "Unknown error" << std::endl;
      }

      //
      return oPythonLogStr.str();
    }

    /**
     * Private wrapper around the indexation use case. 
     */
    std::string indexImpl() {
      std::ostringstream oPythonLogStr;

      // Sanity check
      if (_logOutputStream == NULL) {
        oPythonLogStr << "The log filepath is not valid." << std::endl;
        return oPythonLogStr.str();
      }
      assert (_logOutputStream != NULL);

      try {

        // DEBUG
        *_logOutputStream << "Indexation by Xapian" << std::endl;

        if (_opentrepService == NULL) {
          oPythonLogStr << "The OpenTREP service has not been initialised, "
                        << "i.e., the init() method has not been called "
                        << "correctly on the OpenTrepSearcher object. Please "
                        << "check that all the parameters are not empty and "
                        << "point to actual files.";
          *_logOutputStream << oPythonLogStr.str();
          return oPythonLogStr.str();
        }
        assert (_opentrepService != NULL);

        // Retrieve the underlying file-path details
        const OPENTREP_Service::FilePathSet_T& lFilePathSet =
          _opentrepService->getFilePaths();
        const PORFilePath_T& lPORFilePath = lFilePathSet.first;
        const OPENTREP_Service::DBFilePathPair_T& lDBFilePathPair =
          lFilePathSet.second;
        const TravelDBFilePath_T& lTravelDBFilePath = lDBFilePathPair.first;
        const SQLDBConnectionString_T& lSQLDBConnStr = lDBFilePathPair.second;

        // DEBUG
        *_logOutputStream << "OPTD-maintained list of POR: '" << lPORFilePath
                          << "'" << std::endl;
        *_logOutputStream << "Xapian travel database/index: '"
                          << lTravelDBFilePath << "'" << std::endl;
        *_logOutputStream << "SQL database connection string: '"
                          << lSQLDBConnStr << "'" << std::endl;

        // Launch the indexation by Xapian of the OPTD-maintained list of POR
        const NbOfDBEntries_T lNbOfEntries= _opentrepService->insertIntoDBAndXapian();

        // Dump the results into the output string
        oPythonLogStr << lNbOfEntries;

        // DEBUG
        *_logOutputStream << "Xapian indexation yielded " << lNbOfEntries
                          << " POR (points of reference) entries." << std::endl;

      } catch (const RootException& eOpenTrepError) {
        *_logOutputStream << "OpenTrep error: "  << eOpenTrepError.what()
                          << std::endl;

      } catch (const std::exception& eStdError) {
        *_logOutputStream << "Error: "  << eStdError.what() << std::endl;

      } catch (...) {
        *_logOutputStream << "Unknown error" << std::endl;
      }

      //
      return oPythonLogStr.str();
    }

    /**
     * Private wrapper around the search use case. 
     */
    std::string searchImpl (const std::string& iTravelQuery,
                            const OutputFormat::EN_OutputFormat& iOutputFormat) {
      const std::string oEmptyStr ("");
      std::ostringstream oNoDetailedStr;
      std::ostringstream oDetailedStr;
      std::ostringstream oJSONStr;
      std::ostringstream oProtobufStr;

      // Sanity check
      if (_logOutputStream == NULL) {
        oNoDetailedStr << "The log filepath is not valid." << std::endl;
        return oNoDetailedStr.str();
      }
      assert (_logOutputStream != NULL);

      try {

        // DEBUG
        *_logOutputStream << "Travel query ('" << iTravelQuery << "'"
                          << "') search" << std::endl;

        if (_opentrepService == NULL) {
          oNoDetailedStr << "The OpenTREP service has not been initialised, "
                         << "i.e., the init() method has not been called "
                         << "correctly on the OpenTrepSearcher object. Please "
                         << "check that all the parameters are not empty and "
                         << "point to actual files.";
          *_logOutputStream << oNoDetailedStr.str();
          return oNoDetailedStr.str();
        }
        assert (_opentrepService != NULL);

        // Retrieve the underlying file-path details
        const OPENTREP_Service::FilePathSet_T& lFilePathSet =
          _opentrepService->getFilePaths();
        const PORFilePath_T& lPORFilePath = lFilePathSet.first;
        const OPENTREP_Service::DBFilePathPair_T& lDBFilePathPair =
          lFilePathSet.second;
        const TravelDBFilePath_T& lTravelDBFilePath = lDBFilePathPair.first;
        const SQLDBConnectionString_T& lSQLDBConnStr = lDBFilePathPair.second;

        // Check the directory of the Xapian database/index exists
        // and is accessible
        const OPENTREP::DeploymentNumber_T& lDeploymentNumber =
          _opentrepService->getDeploymentNumber();
        const bool lExistXapianDBDir =
          _opentrepService->checkXapianDBOnFileSystem (lTravelDBFilePath);
        if (lExistXapianDBDir == false) {
          *_logOutputStream << "Error - The file-path to the Xapian "
                            << "database/index ('" << lTravelDBFilePath
                            << "') does not exist or is not a directory."
                            << std::endl;
          *_logOutputStream << "Error - That usually means that the OpenTREP "
                            << "indexer (opentrep-indexer) has not been "
                            << "launched yet, or that it has operated on a "
                            << "different Xapian database/index file-path."
                            << std::endl;
          *_logOutputStream << "Error - For instance the Xapian database/index "
                            << "may have been created with a different "
                            << "deployment number (" << lDeploymentNumber
                            << " being the current deployment number)";
          return oNoDetailedStr.str();
        }

        // DEBUG
        *_logOutputStream << "Xapian travel database/index: '"
                          << lTravelDBFilePath
                          << "' - SQL database connection string: '"
                          << lSQLDBConnStr
                          << "' - OPTD-maintained list of POR: '"
                          << lPORFilePath << "'" << std::endl;

        // Query the Xapian database (index)
        WordList_T lNonMatchedWordList;
        LocationList_T lLocationList;
        const NbOfMatches_T nbOfMatches =
          _opentrepService->interpretTravelRequest (iTravelQuery, lLocationList,
                                                    lNonMatchedWordList);

        // DEBUG
        *_logOutputStream << "Python search for '" << iTravelQuery << "' gave "
                          << nbOfMatches << " matches." << std::endl;

	if (nbOfMatches != 0) {
          NbOfMatches_T idx = 0;

          for(LocationList_T::const_iterator itLocation = lLocationList.begin();
               itLocation != lLocationList.end(); ++itLocation, ++idx) {
            const Location& lLocation = *itLocation;

            if (idx != 0) {
              oNoDetailedStr << ",";
            }

            //
            oNoDetailedStr << lLocation.getIataCode();
            oDetailedStr << idx+1 << ". " << lLocation.toSingleLocationString()
			 << std::endl;

            // List of extra matching locations (those with the same
            // matching weight/percentage)
            const LocationList_T& lExtraLocationList =
              lLocation.getExtraLocationList();
            if (lExtraLocationList.empty() == false) {
              oDetailedStr << "  Extra matches: " << std::endl;

              NbOfMatches_T idxExtra = 0;
              for (LocationList_T::const_iterator itLoc =
                     lExtraLocationList.begin();
                   itLoc != lExtraLocationList.end(); ++itLoc, ++idxExtra) {
                oNoDetailedStr << ":";
                oDetailedStr << "    " << idx+1 << "." << idxExtra+1 << ". ";

                const Location& lExtraLocation = *itLoc;
                oNoDetailedStr << lExtraLocation.getIataCode();
                oDetailedStr << lExtraLocation << std::endl;
              }
            }

            // The matching weight/percentage is the same for the main
            // and the extra matching locations
            oNoDetailedStr << "/" << lLocation.getPercentage();
            
            // List of alternate matching locations (those with a lower
            // matching weight/percentage)
            const LocationList_T& lAlternateLocationList =
              lLocation.getAlternateLocationList();
            if (lAlternateLocationList.empty() == false) {
              oDetailedStr << "  Alternate matches: " << std::endl;

              NbOfMatches_T idxAlter = 0;
              for (LocationList_T::const_iterator itLoc =
                     lAlternateLocationList.begin();
                   itLoc != lAlternateLocationList.end(); ++itLoc, ++idxAlter) {
                oNoDetailedStr << "-";
                oDetailedStr << "    " << idx+1 << "." << idxAlter+1 << ". ";

                const Location& lAlternateLocation = *itLoc;
                oNoDetailedStr << lAlternateLocation.getIataCode()
                               << "/" << lAlternateLocation.getPercentage();
                oDetailedStr << lAlternateLocation << std::endl;
              }
            }
          }
        }

        if (lNonMatchedWordList.empty() == false) {
          oNoDetailedStr << ";";
          oDetailedStr << "Not recognised words:" << std::endl;
          NbOfMatches_T idx = 0;
          for (WordList_T::const_iterator itWord = lNonMatchedWordList.begin();
               itWord != lNonMatchedWordList.end(); ++itWord, ++idx) {
            const Word_T& lWord = *itWord;
            if (idx != 0) {
              oNoDetailedStr << ",";
              oDetailedStr << idx+1 << "." << std::endl;
            }
            oNoDetailedStr << lWord;
            oDetailedStr << lWord;
          }
        }

        // DEBUG
        *_logOutputStream << "Python search for '" << iTravelQuery
                          << "' yielded:" << std::endl;

        // Export the list of Location objects into a JSON-formatted string
        BomJSONExport::jsonExportLocationList (oJSONStr, lLocationList);

        // Export the list of Location objects into a Protobuf-formatted string
        oProtobufStr << LocationExchange::exportLocationList(lLocationList,
                                                             lNonMatchedWordList)
                     << std::flush;

      } catch (const RootException& eOpenTrepError) {
        *_logOutputStream << "OpenTrep error: "  << eOpenTrepError.what()
                          << std::endl;

      } catch (const std::exception& eStdError) {
        *_logOutputStream << "Error: "  << eStdError.what() << std::endl;

      } catch (...) {
        *_logOutputStream << "Unknown error" << std::endl;
      }

      // Return the string corresponding to the request (either with
      // or without details).
      switch (iOutputFormat) {
      case OutputFormat::SHORT: {
        // DEBUG
        const std::string& oNoDetailedString = oNoDetailedStr.str();
        *_logOutputStream << "Short version ("
                          << oNoDetailedString.size() << " char): "
                          << oNoDetailedString << std::endl;
        return oNoDetailedString;
      }

      case OutputFormat::FULL: {
        // DEBUG
        const std::string& oDetailedString = oDetailedStr.str();
        *_logOutputStream << "Long version ("
                          << oDetailedString.size() << " char): "
                          << oDetailedString << std::endl;
        return oDetailedString;
      }

      case OutputFormat::JSON: {
        // DEBUG
        const std::string& oJSONString = oJSONStr.str();
        *_logOutputStream << "JSON version ("
                          << oJSONString.size() << " char): "
                          << oJSONString << std::endl;
        return oJSONString;
      }

      case OutputFormat::PROTOBUF: {
        // DEBUG
        const std::string& oProtobufString = oProtobufStr.str();
        *_logOutputStream << "Protobuf version ("
                          << oProtobufString.size() << " char): "
                          << oProtobufString << std::endl;
        return oProtobufString;
      }

      default: {
        // If the output format is not known, an exception is thrown by
        // the call to the OutputFormat() constructor above.
        assert (false);
      }
      }

      // That line cannot be reached, but some compilers fail if no final return
      return oEmptyStr;
    }

    /**
     * Private wrapper around the random generation use case. 
     */
    std::string generateImpl(const NbOfMatches_T& iNbOfDraws,
                             const OutputFormat::EN_OutputFormat& iOutputFormat){
      const std::string oEmptyStr ("");
      std::ostringstream oNoDetailedStr;
      std::ostringstream oDetailedStr;
      std::ostringstream oJSONStr;
      std::ostringstream oProtobufStr;

      // Sanity check
      if (_logOutputStream == NULL) {
        oNoDetailedStr << "The log filepath is not valid." << std::endl;
        return oNoDetailedStr.str();
      }
      assert (_logOutputStream != NULL);

      try {

        // DEBUG
        *_logOutputStream << "Number of random draws: " << iNbOfDraws
                          << std::endl;

        if (_opentrepService == NULL) {
          oNoDetailedStr << "The OpenTREP service has not been initialised, "
                         << "i.e., the init() method has not been called "
                         << "correctly on the OpenTrepSearcher object. Please "
                         << "check that all the parameters are not empty and "
                         << "point to actual files.";
          *_logOutputStream << oNoDetailedStr.str();
          return oNoDetailedStr.str();
        }
        assert (_opentrepService != NULL);

        // Retrieve the underlying file-path details
        const OPENTREP_Service::FilePathSet_T& lFilePathSet =
          _opentrepService->getFilePaths();
        const PORFilePath_T& lPORFilePath = lFilePathSet.first;
        const OPENTREP_Service::DBFilePathPair_T& lDBFilePathPair =
          lFilePathSet.second;
        const TravelDBFilePath_T& lTravelDBFilePath = lDBFilePathPair.first;
        const SQLDBConnectionString_T& lSQLDBConnStr = lDBFilePathPair.second;

        // DEBUG
        *_logOutputStream << "Xapian travel database/index: '"
                          << lTravelDBFilePath
                          << "' - SQL database connection string: '"
                          << lSQLDBConnStr
                          << "' - OPTD-maintained list of POR: '"
                          << lPORFilePath << "'" << std::endl;

        // Query the Xapian database (index)
        LocationList_T lLocationList;
        const NbOfMatches_T nbOfMatches =
          _opentrepService->drawRandomLocations (iNbOfDraws, lLocationList);

        // DEBUG
        *_logOutputStream << "Python generation of " << iNbOfDraws << " gave "
                          << nbOfMatches << " documents." << std::endl;

	if (nbOfMatches != 0) {
          NbOfMatches_T idx = 0;

          for(LocationList_T::const_iterator itLocation = lLocationList.begin();
               itLocation != lLocationList.end(); ++itLocation, ++idx) {
            const Location& lLocation = *itLocation;

            if (idx != 0) {
              oNoDetailedStr << ",";
            }

            //
            oNoDetailedStr << lLocation.getIataCode();
            oDetailedStr << idx+1 << ". " << lLocation.toSingleLocationString()
			 << std::endl;

            // List of extra matching locations (those with the same
            // matching weight/percentage)
            const LocationList_T& lExtraLocationList =
              lLocation.getExtraLocationList();
            if (lExtraLocationList.empty() == false) {
              oDetailedStr << "  Extra matches: " << std::endl;

              NbOfMatches_T idxExtra = 0;
              for (LocationList_T::const_iterator itLoc =
                     lExtraLocationList.begin();
                   itLoc != lExtraLocationList.end(); ++itLoc, ++idxExtra) {
                oNoDetailedStr << ":";
                oDetailedStr << "    " << idx+1 << "." << idxExtra+1 << ". ";

                const Location& lExtraLocation = *itLoc;
                oNoDetailedStr << lExtraLocation.getIataCode();
                oDetailedStr << lExtraLocation << std::endl;
              }
            }

            // The matching weight/percentage is the same for the main
            // and the extra matching locations
            oNoDetailedStr << "/" << lLocation.getPercentage();
            
            // List of alternate matching locations (those with a lower
            // matching weight/percentage)
            const LocationList_T& lAlternateLocationList =
              lLocation.getAlternateLocationList();
            if (lAlternateLocationList.empty() == false) {
              oDetailedStr << "  Alternate matches: " << std::endl;

              NbOfMatches_T idxAlter = 0;
              for (LocationList_T::const_iterator itLoc =
                     lAlternateLocationList.begin();
                   itLoc != lAlternateLocationList.end(); ++itLoc, ++idxAlter) {
                oNoDetailedStr << "-";
                oDetailedStr << "    " << idx+1 << "." << idxAlter+1 << ". ";

                const Location& lAlternateLocation = *itLoc;
                oNoDetailedStr << lAlternateLocation.getIataCode()
                               << "/" << lAlternateLocation.getPercentage();
                oDetailedStr << lAlternateLocation << std::endl;
              }
            }
          }
        }

        // DEBUG
        *_logOutputStream << "Python generation of " << iNbOfDraws
                          << " yielded:" << std::endl;

        // Export the list of Location objects into a JSON-formatted string
        BomJSONExport::jsonExportLocationList (oJSONStr, lLocationList);

        // Export the list of Location objects into a Protobuf-formatted string
        WordList_T lNonMatchedWordList;
        oProtobufStr << LocationExchange::exportLocationList(lLocationList,
                                                             lNonMatchedWordList)
                     << std::flush;

      } catch (const RootException& eOpenTrepError) {
        *_logOutputStream << "OpenTrep error: "  << eOpenTrepError.what()
                          << std::endl;

      } catch (const std::exception& eStdError) {
        *_logOutputStream << "Error: "  << eStdError.what() << std::endl;

      } catch (...) {
        *_logOutputStream << "Unknown error" << std::endl;
      }

      // Return the string corresponding to the request (either with
      // or without details).
      switch (iOutputFormat) {
      case OutputFormat::SHORT: {
        // DEBUG
        const std::string& oNoDetailedString = oNoDetailedStr.str();
        *_logOutputStream << "Short version ("
                          << oNoDetailedString.size() << " char): "
                          << oNoDetailedString << std::endl;
        return oNoDetailedString;
      }

      case OutputFormat::FULL: {
        // DEBUG
        const std::string& oDetailedString = oDetailedStr.str();
        *_logOutputStream << "Long version ("
                          << oDetailedString.size() << " char): "
                          << oDetailedString << std::endl;
        return oDetailedString;
      }

      case OutputFormat::JSON: {
        // DEBUG
        const std::string& oJSONString = oJSONStr.str();
        *_logOutputStream << "JSON version ("
                          << oJSONString.size() << " char): "
                          << oJSONString << std::endl;
        return oJSONString;
      }

      case OutputFormat::PROTOBUF: {
        // DEBUG
        const std::string& oProtobufString = oProtobufStr.str();
        *_logOutputStream << "Protobuf version ("
                          << oProtobufString.size() << " char): "
                          << oProtobufString << std::endl;
        return oProtobufString;
      }

      default: {
        // If the output format is not known, an exception is thrown by
        // the call to the OutputFormat() constructor above.
        assert (false);
      }
      }

      // That line cannot be reached, but some compilers fail if no final return
      return oEmptyStr;
    }

  public:
    /** 
     * Default constructor. 
     */
    OpenTrepSearcher() : _opentrepService (NULL), _logOutputStream (NULL) {
    }

    /**
     * Default copy constructor. 
     */
    OpenTrepSearcher (const OpenTrepSearcher& iOpenTrepSearcher)
      : _opentrepService (iOpenTrepSearcher._opentrepService),
        _logOutputStream (iOpenTrepSearcher._logOutputStream) {
    }

    /** 
     * Default constructor. 
     */
    ~OpenTrepSearcher() {
      _opentrepService = NULL;
      _logOutputStream = NULL;
    }

    /** 
     * Wrapper around the search use case. 
     */
    bool init (const std::string& iTravelDBFilePath,
               const std::string& iSQLDBTypeStr,
               const std::string& iSQLDBConnStr,
               const DeploymentNumber_T& iDeploymentNumber,
               const std::string& iLogFilePath) {
      bool isEverythingOK = true;

      try {

        // Add the deployment number/version to the Xapian file-path
        std::ostringstream oXFP;
        oXFP << iTravelDBFilePath << iDeploymentNumber;
        const std::string lXapianFP = oXFP.str();
        
        // Set the log parameters
        _logOutputStream = new std::ofstream;
        assert (_logOutputStream != NULL);

        // Open and clean the log outputfile
        _logOutputStream->open (iLogFilePath.c_str());
        _logOutputStream->clear();

        // DEBUG
        *_logOutputStream << "Python wrapper initialisation" << std::endl;
        
        // Initialise the context
        const OPENTREP::TravelDBFilePath_T lTravelDBFilePath (iTravelDBFilePath);
        const OPENTREP::DBType lSQLDBType (iSQLDBTypeStr);
        const OPENTREP::SQLDBConnectionString_T lSQLDBConnStr (iSQLDBConnStr);
        const OPENTREP::DeploymentNumber_T lDeploymentNumber (iDeploymentNumber);

        _opentrepService = new OPENTREP_Service (*_logOutputStream,
                                                 lTravelDBFilePath,
                                                 lSQLDBType, lSQLDBConnStr,
                                                 lDeploymentNumber);

        // DEBUG
        *_logOutputStream << "Python wrapper initialised" << std::endl;

      } catch (const RootException& eOpenTrepError) {
        *_logOutputStream << "OpenTrep error: "  << eOpenTrepError.what()
                          << std::endl;

      } catch (const std::exception& eStdError) {
        *_logOutputStream << "Error: "  << eStdError.what() << std::endl;

      } catch (...) {
        *_logOutputStream << "Unknown error" << std::endl;
      }

      return isEverythingOK;
    }

    /** 
     * Wrapper around the search use case. 
     */
    bool finalize () {
      bool isEverythingOK = true;

      try {
        
        // Finalize the context
        if (_opentrepService != NULL) {
          delete _opentrepService; _opentrepService = NULL;
        }

        // Close the output stream
        if (_logOutputStream != NULL) {
          // DEBUG
          *_logOutputStream << "Python wrapper finalization" << std::endl;
          _logOutputStream->close();
          delete _logOutputStream; _logOutputStream = NULL;
        }

      } catch (...) {
      }

      return isEverythingOK;
    }

  private:
    /**
     * Handle on the OpenTREP services (API). 
     */
    OPENTREP_Service* _opentrepService;
    std::ofstream* _logOutputStream;
  };

}

// /////////////////////////////////////////////////////////////
BOOST_PYTHON_MODULE(pyopentrep) {
  boost::python::class_<OPENTREP::OpenTrepSearcher> ("OpenTrepSearcher")
    .def ("index", &OPENTREP::OpenTrepSearcher::index)
    .def ("search", &OPENTREP::OpenTrepSearcher::search)
    .def ("searchToPB", &OPENTREP::OpenTrepSearcher::searchToPB)
    .def ("generate", &OPENTREP::OpenTrepSearcher::generate)
    .def ("generateToPB", &OPENTREP::OpenTrepSearcher::generateToPB)
    .def ("getPaths", &OPENTREP::OpenTrepSearcher::getPaths)
    .def ("init", &OPENTREP::OpenTrepSearcher::init)
    .def ("finalize", &OPENTREP::OpenTrepSearcher::finalize);
}
