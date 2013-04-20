// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// STL
#include <cassert>
#include <ostream>
// Boost
#include <boost/date_time/gregorian/gregorian.hpp>
#include <boost/date_time/posix_time/ptime.hpp>
// OpenTrep
#include <opentrep/basic/BasConst_OPENTREP_Service.hpp>
#include <opentrep/basic/BasChronometer.hpp>
#include <opentrep/factory/FacWorld.hpp>
#include <opentrep/command/IndexBuilder.hpp>
#include <opentrep/command/XapianIndexManager.hpp>
#include <opentrep/command/RequestInterpreter.hpp>
#include <opentrep/factory/FacOpenTrepServiceContext.hpp>
#include <opentrep/service/OPENTREP_ServiceContext.hpp>
#include <opentrep/service/ServiceUtilities.hpp>
#include <opentrep/service/Logger.hpp>
#include <opentrep/OPENTREP_Service.hpp>

namespace OPENTREP {

  // //////////////////////////////////////////////////////////////////////
  OPENTREP_Service::
  OPENTREP_Service (std::ostream& ioLogStream, const PORFilePath_T& iPORFilepath,
                    const TravelDBFilePath_T& iTravelDBFilePath)
    : _opentrepServiceContext (NULL) {
    init (ioLogStream, iPORFilepath, iTravelDBFilePath);
  }

  // //////////////////////////////////////////////////////////////////////
  OPENTREP_Service::
  OPENTREP_Service (std::ostream& ioLogStream,
                    const TravelDBFilePath_T& iTravelDBFilePath)
    : _opentrepServiceContext (NULL) {
    init (ioLogStream, iTravelDBFilePath);
  }

  // //////////////////////////////////////////////////////////////////////
  OPENTREP_Service::OPENTREP_Service() : _opentrepServiceContext (NULL) {
    assert (false);
  }

  // //////////////////////////////////////////////////////////////////////
  OPENTREP_Service::OPENTREP_Service (const OPENTREP_Service& iService) {
    assert (false);
  }

  // //////////////////////////////////////////////////////////////////////
  OPENTREP_Service::~OPENTREP_Service() {
    // Delete/Clean all the objects from memory
    finalise();
  }

  // //////////////////////////////////////////////////////////////////////
  void logInit (const LOG::EN_LogLevel iLogLevel,
                std::ostream& ioLogOutputFile) {
    Logger::instance().setLogParameters (iLogLevel, ioLogOutputFile);
  }

  // //////////////////////////////////////////////////////////////////////
  void OPENTREP_Service::init (std::ostream& ioLogStream,
                               const TravelDBFilePath_T& iTravelDBFilePath) {
    // Set the log file
    logInit (LOG::DEBUG, ioLogStream);

    // Initialise the context
    OPENTREP_ServiceContext& lOPENTREP_ServiceContext = 
      FacOpenTrepServiceContext::instance().create (iTravelDBFilePath);
    _opentrepServiceContext = &lOPENTREP_ServiceContext;

    // Instanciate an empty World object
    World& lWorld = FacWorld::instance().create();
    lOPENTREP_ServiceContext.setWorld (lWorld);
  }
  
  // //////////////////////////////////////////////////////////////////////
  void OPENTREP_Service::init (std::ostream& ioLogStream,
                               const PORFilePath_T& iPORFilepath,
                               const TravelDBFilePath_T& iTravelDBFilePath) {
    // Set the log file
    logInit (LOG::DEBUG, ioLogStream);

    // Initialise the context
    OPENTREP_ServiceContext& lOPENTREP_ServiceContext = 
      FacOpenTrepServiceContext::instance().create (iPORFilepath,
                                                    iTravelDBFilePath);
    _opentrepServiceContext = &lOPENTREP_ServiceContext;

    // Instanciate an empty World object
    World& lWorld = FacWorld::instance().create();
    lOPENTREP_ServiceContext.setWorld (lWorld);
  }
  
  // //////////////////////////////////////////////////////////////////////
  void OPENTREP_Service::finalise() {
  }

  // //////////////////////////////////////////////////////////////////////
  OPENTREP_Service::FilePathPair_T OPENTREP_Service::getFilePaths() const {
    if (_opentrepServiceContext == NULL) {
      throw NonInitialisedServiceException ("The OpenTREP service has not been"
                                            " initialised");
    }
    assert (_opentrepServiceContext != NULL);
    OPENTREP_ServiceContext& lOPENTREP_ServiceContext = *_opentrepServiceContext;

    // Retrieve the file-path of the POR (points of reference) file
    const PORFilePath_T& lPORFilePath= lOPENTREP_ServiceContext.getPORFilePath();
      
    // Retrieve the Xapian database name (directorty of the index)
    const TravelDBFilePath_T& lTravelDBFilePath =
      lOPENTREP_ServiceContext.getTravelDBFilePath();
      
    FilePathPair_T oFilePathPair (lPORFilePath, lTravelDBFilePath);
    return oFilePathPair;
  }

  // //////////////////////////////////////////////////////////////////////
  NbOfDBEntries_T OPENTREP_Service::getIndexSize() {
    NbOfDBEntries_T oNbOfEntries = 0;
    
    if (_opentrepServiceContext == NULL) {
      throw NonInitialisedServiceException ("The OpenTREP service has not been"
                                            " initialised");
    }
    assert (_opentrepServiceContext != NULL);
    OPENTREP_ServiceContext& lOPENTREP_ServiceContext = *_opentrepServiceContext;

    // Retrieve the Xapian database name (directorty of the index)
    const TravelDBFilePath_T& lTravelDBFilePath =
      lOPENTREP_ServiceContext.getTravelDBFilePath();
      
    // Delegate the query execution to the dedicated command
    BasChronometer lIndexSizeChronometer; lIndexSizeChronometer.start();
    oNbOfEntries = XapianIndexManager::getSize (lTravelDBFilePath);
    const double lIndexSizeMeasure = lIndexSizeChronometer.elapsed();
      
    // DEBUG
    OPENTREP_LOG_DEBUG ("Size retrieval of the Xapian database (index): "
                        << lIndexSizeMeasure << " - "
                        << lOPENTREP_ServiceContext.display());

    return oNbOfEntries;
  }

  // //////////////////////////////////////////////////////////////////////
  NbOfMatches_T OPENTREP_Service::
  drawRandomLocations (const NbOfMatches_T& iNbOfDraws,
                       LocationList_T& ioLocationList) {
    NbOfMatches_T oNbOfMatches = 0;

    if (_opentrepServiceContext == NULL) {
      throw NonInitialisedServiceException ("The OpenTREP service has not been"
                                            " initialised");
    }
    assert (_opentrepServiceContext != NULL);
    OPENTREP_ServiceContext& lOPENTREP_ServiceContext= *_opentrepServiceContext;

    // Retrieve the Xapian database name (directorty of the index)
    const TravelDBFilePath_T& lTravelDBFilePath =
      lOPENTREP_ServiceContext.getTravelDBFilePath();
      
    // Delegate the query execution to the dedicated command
    BasChronometer lRandomGetChronometer; lRandomGetChronometer.start();
    oNbOfMatches = XapianIndexManager::drawRandomLocations (lTravelDBFilePath,
                                                            iNbOfDraws,
                                                            ioLocationList);
    const double lRandomGetMeasure = lRandomGetChronometer.elapsed();
      
    // DEBUG
    OPENTREP_LOG_DEBUG ("Random retrieval of locations (index): "
                        << lRandomGetMeasure << " - "
                        << lOPENTREP_ServiceContext.display());

    return oNbOfMatches;
  }

  // //////////////////////////////////////////////////////////////////////
  NbOfDBEntries_T OPENTREP_Service::buildSearchIndex() {
    NbOfDBEntries_T oNbOfEntries = 0;
    
    if (_opentrepServiceContext == NULL) {
      throw NonInitialisedServiceException ("The OpenTREP service has not been"
                                            " initialised");
    }
    assert (_opentrepServiceContext != NULL);
    OPENTREP_ServiceContext& lOPENTREP_ServiceContext = *_opentrepServiceContext;

    // Retrieve the file-path of the POR (points of reference) file
    const PORFilePath_T& lPORFilePath= lOPENTREP_ServiceContext.getPORFilePath();
      
    // Retrieve the Xapian database name (directorty of the index)
    const TravelDBFilePath_T& lTravelDBFilePath =
      lOPENTREP_ServiceContext.getTravelDBFilePath();
      
    // Retrieve the Unicode transliterator
    const OTransliterator& lTransliterator =
      lOPENTREP_ServiceContext.getTransliterator();
      
    // Delegate the index building to the dedicated command
    BasChronometer lBuildSearchIndexChronometer;
    lBuildSearchIndexChronometer.start();
    oNbOfEntries = IndexBuilder::buildSearchIndex (lPORFilePath,
                                                   lTravelDBFilePath,
                                                   lTransliterator);
    const double lBuildSearchIndexMeasure =
      lBuildSearchIndexChronometer.elapsed();
      
    // DEBUG
    OPENTREP_LOG_DEBUG ("Built Xapian database (index): "
                        << lBuildSearchIndexMeasure << " - "
                        << lOPENTREP_ServiceContext.display());

    return oNbOfEntries;
  }
  
  // //////////////////////////////////////////////////////////////////////
  NbOfMatches_T OPENTREP_Service::
  interpretTravelRequest (const std::string& iTravelQuery,
                          LocationList_T& ioLocationList,
                          WordList_T& ioWordList) {
    NbOfMatches_T nbOfMatches = 0;

    if (_opentrepServiceContext == NULL) {
      throw NonInitialisedServiceException ("The OpenTREP service has not been"
                                            " initialised");
    }
    assert (_opentrepServiceContext != NULL);
    OPENTREP_ServiceContext& lOPENTREP_ServiceContext= *_opentrepServiceContext;

    // Get the date-time for the present time
    boost::posix_time::ptime lNowDateTime =
      boost::posix_time::second_clock::local_time();
    // boost::gregorian::date lNowDate = lNowDateTime.date();

    // DEBUG
    OPENTREP_LOG_DEBUG (std::endl
                        << "==================================================="
                        << std::endl
                        << lNowDateTime << " - Match query '" << iTravelQuery
                        << "' on Xapian database (index)");
    
    // Check that the travel request is not empty
    if (iTravelQuery.empty() == true) {
      std::ostringstream errorStr;
      errorStr << "The travel request is empty.";
      OPENTREP_LOG_ERROR (errorStr.str());
      throw TravelRequestEmptyException (errorStr.str());
    }
    
    // Retrieve the Xapian database name (directorty of the index)
    const TravelDBFilePath_T& lTravelDBFilePath =
      lOPENTREP_ServiceContext.getTravelDBFilePath();
      
    // Delegate the query execution to the dedicated command
    BasChronometer lRequestInterpreterChronometer;
    lRequestInterpreterChronometer.start();
    nbOfMatches = RequestInterpreter::interpretTravelRequest (lTravelDBFilePath,
                                                              iTravelQuery,
                                                              ioLocationList,
                                                              ioWordList);
    const double lRequestInterpreterMeasure =
      lRequestInterpreterChronometer.elapsed();

    // DEBUG
    OPENTREP_LOG_DEBUG ("Match query on Xapian database (index): "
                        << lRequestInterpreterMeasure << " - "
                        << lOPENTREP_ServiceContext.display());
      
    return nbOfMatches;
  }
  
}
