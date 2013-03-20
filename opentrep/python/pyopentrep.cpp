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
#include <opentrep/bom/BomJSONExport.hpp>

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
     *   <li>ORI-maintained list of POR (points of reference)</li>
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
     * Public wrapper around the search use case. 
     */
    std::string search (const std::string& iOutputFormatString,
                        const std::string& iTravelQuery) {
      const OutputFormat lOutputFormat (iOutputFormatString);
      const OutputFormat::EN_OutputFormat& lOutputFormatEnum =
        lOutputFormat.getFormat();
      return searchImpl (iTravelQuery, lOutputFormatEnum);
    }

  private:
    /**
     * Private wrapper around the indexation use case. 
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
        const OPENTREP_Service::FilePathPair_T& lFilePathPair =
          _opentrepService->getFilePaths();
        const PORFilePath_T& lPORFilePath = lFilePathPair.first;
        const TravelDBFilePath_T& lTravelDBFilePath = lFilePathPair.second;

        // Dump the results into the output string
        oPythonLogStr << lPORFilePath << ";" << lTravelDBFilePath;

        // DEBUG
        *_logOutputStream << "ORI-maintained list of POR: '" << lPORFilePath
                          << "'" << std::endl;
        *_logOutputStream << "Xapian travel database/index: '"
                          << lTravelDBFilePath << "'" << std::endl;

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
        const OPENTREP_Service::FilePathPair_T& lFilePathPair =
          _opentrepService->getFilePaths();
        const PORFilePath_T& lPORFilePath = lFilePathPair.first;
        const TravelDBFilePath_T& lTravelDBFilePath = lFilePathPair.second;

        // DEBUG
        *_logOutputStream << "ORI-maintained list of POR: '" << lPORFilePath
                          << "'" << std::endl;
        *_logOutputStream << "Xapian travel database/index: '"
                          << lTravelDBFilePath << "'" << std::endl;

        // Launch the indexation by Xapian of the ORI-maintained list of POR
        const NbOfDBEntries_T lNbOfEntries= _opentrepService->buildSearchIndex();

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
    std::string searchImpl(const std::string& iTravelQuery,
                           const OutputFormat::EN_OutputFormat& iOutputFormat) {
      std::ostringstream oNoDetailedStr;
      std::ostringstream oDetailedStr;
      std::ostringstream oJSONStr;

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
        const OPENTREP_Service::FilePathPair_T& lFilePathPair =
          _opentrepService->getFilePaths();
        const PORFilePath_T& lPORFilePath = lFilePathPair.first;
        const TravelDBFilePath_T& lTravelDBFilePath = lFilePathPair.second;

        // DEBUG
        *_logOutputStream << "Xapian travel database/index: '" << lPORFilePath
                          << "' - ORI-maintained list of POR: '"
                          << lTravelDBFilePath << "'" << std::endl;

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

        // DEBUG
        *_logOutputStream << "Short version: "
                          << oNoDetailedStr.str() << std::endl;
        *_logOutputStream << "Long version: "
                          << oDetailedStr.str() << std::endl;
        *_logOutputStream << "JSON version: "
                          << oJSONStr.str() << std::endl;

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
        return oNoDetailedStr.str();
      }

      case OutputFormat::FULL: {
        return oDetailedStr.str();
      }

      case OutputFormat::JSON: {
        return oJSONStr.str();
      }

      default: {
	// If the output format is not known, an exception is thrown by
	// the call to the OutputFormat() constructor above.
	assert (false);
      }
      }
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
               const std::string& iLogFilePath) {
      bool isEverythingOK = true;

      try {
        
        // Check that the file-path exist and are accessible
        boost::filesystem::path lXapianFilePath (iTravelDBFilePath.begin(),
                                                 iTravelDBFilePath.end());
        if (!(boost::filesystem::exists (lXapianFilePath)
              && boost::filesystem::is_directory (lXapianFilePath))) {
          isEverythingOK = false;
          return isEverythingOK;
        }

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
        _opentrepService = new OPENTREP_Service (*_logOutputStream,
                                                 lTravelDBFilePath);

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
BOOST_PYTHON_MODULE(libpyopentrep) {
  boost::python::class_<OPENTREP::OpenTrepSearcher> ("OpenTrepSearcher")
    .def ("index", &OPENTREP::OpenTrepSearcher::index)
    .def ("search", &OPENTREP::OpenTrepSearcher::search)
    .def ("getPaths", &OPENTREP::OpenTrepSearcher::getPaths)
    .def ("init", &OPENTREP::OpenTrepSearcher::init)
    .def ("finalize", &OPENTREP::OpenTrepSearcher::finalize);
}
