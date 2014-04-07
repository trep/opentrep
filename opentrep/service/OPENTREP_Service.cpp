// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// STL
#include <cassert>
#include <ostream>
// Boost
#include <boost/date_time/gregorian/gregorian.hpp>
#include <boost/date_time/posix_time/ptime.hpp>
// SOCI
#include <soci/soci.h>
// OpenTrep
#include <opentrep/basic/BasConst_OPENTREP_Service.hpp>
#include <opentrep/basic/BasChronometer.hpp>
#include <opentrep/factory/FacWorld.hpp>
#include <opentrep/command/DBManager.hpp>
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
                    const TravelDBFilePath_T& iTravelDBFilePath,
                    const DBType& iSQLDBType,
                    const SQLDBConnectionString_T& iSQLDBConnStr)
    : _opentrepServiceContext (NULL) {
    init (ioLogStream, iPORFilepath, iTravelDBFilePath,
          iSQLDBType, iSQLDBConnStr);
  }

  // //////////////////////////////////////////////////////////////////////
  OPENTREP_Service::
  OPENTREP_Service (std::ostream& ioLogStream,
                    const TravelDBFilePath_T& iTravelDBFilePath,
                    const DBType& iSQLDBType,
                    const SQLDBConnectionString_T& iSQLDBConnStr)
    : _opentrepServiceContext (NULL) {
    init (ioLogStream, iTravelDBFilePath, iSQLDBType, iSQLDBConnStr);
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
                               const TravelDBFilePath_T& iTravelDBFilePath,
                               const DBType& iSQLDBType,
                               const SQLDBConnectionString_T& iSQLDBConnStr) {
    // Set the log file
    logInit (LOG::DEBUG, ioLogStream);

    // Initialise the context
    OPENTREP_ServiceContext& lOPENTREP_ServiceContext = 
      FacOpenTrepServiceContext::instance().create (iTravelDBFilePath,
                                                    iSQLDBType, iSQLDBConnStr);
    _opentrepServiceContext = &lOPENTREP_ServiceContext;

    // Instanciate an empty World object
    World& lWorld = FacWorld::instance().create();
    lOPENTREP_ServiceContext.setWorld (lWorld);
  }
  
  // //////////////////////////////////////////////////////////////////////
  void OPENTREP_Service::init (std::ostream& ioLogStream,
                               const PORFilePath_T& iPORFilepath,
                               const TravelDBFilePath_T& iTravelDBFilePath,
                               const DBType& iSQLDBType,
                               const SQLDBConnectionString_T& iSQLDBConnStr) {
    // Set the log file
    logInit (LOG::DEBUG, ioLogStream);

    // Initialise the context
    OPENTREP_ServiceContext& lOPENTREP_ServiceContext = 
      FacOpenTrepServiceContext::instance().create (iPORFilepath,
                                                    iTravelDBFilePath,
                                                    iSQLDBType, iSQLDBConnStr);
    _opentrepServiceContext = &lOPENTREP_ServiceContext;

    // Instanciate an empty World object
    World& lWorld = FacWorld::instance().create();
    lOPENTREP_ServiceContext.setWorld (lWorld);
  }
  
  // //////////////////////////////////////////////////////////////////////
  void OPENTREP_Service::finalise() {
  }

  // //////////////////////////////////////////////////////////////////////
  OPENTREP_Service::FilePathSet_T OPENTREP_Service::getFilePaths() const {
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
      
    // Retrieve the SQL database connection string
    const SQLDBConnectionString_T& lSQLDBConnectionString =
      lOPENTREP_ServiceContext.getSQLDBConnectionString();
      
    const DBFilePathPair_T lDBFilePathPair (lTravelDBFilePath,
                                            lSQLDBConnectionString);
    FilePathSet_T oFilePathSet (lPORFilePath, lDBFilePathPair);
    return oFilePathSet;
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
  bool OPENTREP_Service::createSQLDBUser() {
    bool oCreationSuccessful = true;

    if (_opentrepServiceContext == NULL) {
      throw NonInitialisedServiceException ("The OpenTREP service has not been"
                                            " initialised");
    }
    assert (_opentrepServiceContext != NULL);
    OPENTREP_ServiceContext& lOPENTREP_ServiceContext = *_opentrepServiceContext;

    // Retrieve the SQL database type
    const DBType& lSQLDBType = lOPENTREP_ServiceContext.getSQLDBType();
      
    // Retrieve the SQL database connection string
    const SQLDBConnectionString_T& lSQLDBConnectionString =
      lOPENTREP_ServiceContext.getSQLDBConnectionString();
      
    // Delegate the database creation to the dedicated command
    BasChronometer lDBCreationChronometer;
    lDBCreationChronometer.start();

    // Create the SQL database user ('trep' on MySQL database)
    // and database ('trep_trep' on MySQL database)
    oCreationSuccessful =
      DBManager::createSQLDBUser (lSQLDBType, lSQLDBConnectionString);

    const double lDBCreationMeasure = lDBCreationChronometer.elapsed();
      
    // DEBUG
    OPENTREP_LOG_DEBUG ("Created the SQL database: " << lDBCreationMeasure
                        << " - " << lOPENTREP_ServiceContext.display());

    return oCreationSuccessful;
  }

  // //////////////////////////////////////////////////////////////////////
  void OPENTREP_Service::
  setSQLDBConnectString (const SQLDBConnectionString_T& iSQLDBConnectionString) {
    if (_opentrepServiceContext == NULL) {
      throw NonInitialisedServiceException ("The OpenTREP service has not been"
                                            " initialised");
    }
    assert (_opentrepServiceContext != NULL);
    OPENTREP_ServiceContext& lOPENTREP_ServiceContext = *_opentrepServiceContext;

    // Set the SQL database connection string
    lOPENTREP_ServiceContext.setSQLDBConnectionString (iSQLDBConnectionString);
      
    // DEBUG
    OPENTREP_LOG_DEBUG ("Reset of the SQL database connection string: "
                        << lOPENTREP_ServiceContext.display());
  }

  // //////////////////////////////////////////////////////////////////////
  void OPENTREP_Service::createSQLDBTables() {
    if (_opentrepServiceContext == NULL) {
      throw NonInitialisedServiceException ("The OpenTREP service has not been"
                                            " initialised");
    }
    assert (_opentrepServiceContext != NULL);
    OPENTREP_ServiceContext& lOPENTREP_ServiceContext = *_opentrepServiceContext;

    // Retrieve the SQL database type
    const DBType& lSQLDBType = lOPENTREP_ServiceContext.getSQLDBType();
      
    // Retrieve the SQL database connection string
    const SQLDBConnectionString_T& lSQLDBConnectionString =
      lOPENTREP_ServiceContext.getSQLDBConnectionString();
      
    // Delegate the database creation to the dedicated command
    BasChronometer lDBCreationChronometer;
    lDBCreationChronometer.start();

    // Connect to the SQLite3/MySQL database
    soci::session* lSociSession_ptr =
      DBManager::initSQLDBSession (lSQLDBType, lSQLDBConnectionString);
    assert (lSociSession_ptr != NULL);
    soci::session& lSociSession = *lSociSession_ptr;

    // Create the SQL database tables
    DBManager::createSQLDBTables (lSociSession);

    const double lDBCreationMeasure = lDBCreationChronometer.elapsed();
      
    // DEBUG
    OPENTREP_LOG_DEBUG ("Created the SQL database tables: " << lDBCreationMeasure
                        << " - " << lOPENTREP_ServiceContext.display());
  }

  // //////////////////////////////////////////////////////////////////////
  void OPENTREP_Service::createSQLDBIndexes() {
    if (_opentrepServiceContext == NULL) {
      throw NonInitialisedServiceException ("The OpenTREP service has not been"
                                            " initialised");
    }
    assert (_opentrepServiceContext != NULL);
    OPENTREP_ServiceContext& lOPENTREP_ServiceContext = *_opentrepServiceContext;

    // Retrieve the SQL database type
    const DBType& lSQLDBType = lOPENTREP_ServiceContext.getSQLDBType();
      
    // Retrieve the SQL database connection string
    const SQLDBConnectionString_T& lSQLDBConnectionString =
      lOPENTREP_ServiceContext.getSQLDBConnectionString();
      
    // Delegate the database creation to the dedicated command
    BasChronometer lDBCreationChronometer;
    lDBCreationChronometer.start();

    // Connect to the SQLite3/MySQL database
    soci::session* lSociSession_ptr =
      DBManager::initSQLDBSession (lSQLDBType, lSQLDBConnectionString);
    assert (lSociSession_ptr != NULL);
    soci::session& lSociSession = *lSociSession_ptr;

    // Create the SQL database tables
    DBManager::createSQLDBIndexes (lSociSession);

    const double lDBCreationMeasure = lDBCreationChronometer.elapsed();
      
    // DEBUG
    OPENTREP_LOG_DEBUG ("Created the SQL database indexes: "
                        << lDBCreationMeasure << " - "
                        << lOPENTREP_ServiceContext.display());
  }

  // //////////////////////////////////////////////////////////////////////
  NbOfDBEntries_T OPENTREP_Service::getNbOfPORFromDB() {
    NbOfDBEntries_T nbOfMatches = 0;

    if (_opentrepServiceContext == NULL) {
      throw NonInitialisedServiceException ("The OpenTREP service has not been"
                                            " initialised");
    }
    assert (_opentrepServiceContext != NULL);
    OPENTREP_ServiceContext& lOPENTREP_ServiceContext = *_opentrepServiceContext;

    // Retrieve the SQL database type
    const DBType& lSQLDBType = lOPENTREP_ServiceContext.getSQLDBType();
      
    // Retrieve the SQL database connection string
    const SQLDBConnectionString_T& lSQLDBConnectionString =
      lOPENTREP_ServiceContext.getSQLDBConnectionString();
      
    // Delegate the database look up to the dedicated command
    BasChronometer lDBListChronometer;
    lDBListChronometer.start();

    // Connect to the SQLite3/MySQL database
    soci::session* lSociSession_ptr =
      DBManager::initSQLDBSession (lSQLDBType, lSQLDBConnectionString);
    assert (lSociSession_ptr != NULL);
    soci::session& lSociSession = *lSociSession_ptr;
      
    // Get the number of POR stored within the SQLite3/MySQL database
    nbOfMatches = DBManager::displayCount (lSociSession);

    const double lDBListMeasure = lDBListChronometer.elapsed();
      
    // DEBUG
    OPENTREP_LOG_DEBUG ("Look up in the SQL database: " << lDBListMeasure
                        << " - " << lOPENTREP_ServiceContext.display());

    //
    return nbOfMatches;
  }

  // //////////////////////////////////////////////////////////////////////
  NbOfMatches_T OPENTREP_Service::
  listByIataCode (const IATACode_T& iIataCode,
                  LocationList_T& ioLocationList) {
    NbOfMatches_T nbOfMatches = 0;

    if (_opentrepServiceContext == NULL) {
      throw NonInitialisedServiceException ("The OpenTREP service has not been"
                                            " initialised");
    }
    assert (_opentrepServiceContext != NULL);
    OPENTREP_ServiceContext& lOPENTREP_ServiceContext = *_opentrepServiceContext;

    // Retrieve the SQL database type
    const DBType& lSQLDBType = lOPENTREP_ServiceContext.getSQLDBType();
      
    // Retrieve the SQL database connection string
    const SQLDBConnectionString_T& lSQLDBConnectionString =
      lOPENTREP_ServiceContext.getSQLDBConnectionString();
      
    // Delegate the database look up to the dedicated command
    BasChronometer lDBListChronometer;
    lDBListChronometer.start();

    // Connect to the SQLite3/MySQL database
    soci::session* lSociSession_ptr =
      DBManager::initSQLDBSession (lSQLDBType, lSQLDBConnectionString);
    assert (lSociSession_ptr != NULL);
    soci::session& lSociSession = *lSociSession_ptr;
      
    // Get the list of POR corresponding to the given IATA code
    nbOfMatches =
      DBManager::getPORByIATACode (lSociSession, iIataCode, ioLocationList);

    const double lDBListMeasure = lDBListChronometer.elapsed();
      
    // DEBUG
    OPENTREP_LOG_DEBUG ("Look up in the SQL database: " << lDBListMeasure
                        << " - " << lOPENTREP_ServiceContext.display());

    //
    return nbOfMatches;
  }

  // //////////////////////////////////////////////////////////////////////
  NbOfMatches_T OPENTREP_Service::
  listByIcaoCode (const ICAOCode_T& iIcaoCode,
                  LocationList_T& ioLocationList) {
    NbOfMatches_T nbOfMatches = 0;

    if (_opentrepServiceContext == NULL) {
      throw NonInitialisedServiceException ("The OpenTREP service has not been"
                                            " initialised");
    }
    assert (_opentrepServiceContext != NULL);
    OPENTREP_ServiceContext& lOPENTREP_ServiceContext = *_opentrepServiceContext;

    // Retrieve the SQL database type
    const DBType& lSQLDBType = lOPENTREP_ServiceContext.getSQLDBType();
      
    // Retrieve the SQL database connection string
    const SQLDBConnectionString_T& lSQLDBConnectionString =
      lOPENTREP_ServiceContext.getSQLDBConnectionString();
      
    // Delegate the database look up to the dedicated command
    BasChronometer lDBListChronometer;
    lDBListChronometer.start();

    // Connect to the SQLite3/MySQL database
    soci::session* lSociSession_ptr =
      DBManager::initSQLDBSession (lSQLDBType, lSQLDBConnectionString);
    assert (lSociSession_ptr != NULL);
    soci::session& lSociSession = *lSociSession_ptr;
      
    // Get the list of POR corresponding to the given ICAO code
    nbOfMatches =
      DBManager::getPORByICAOCode (lSociSession, iIcaoCode, ioLocationList);

    const double lDBListMeasure = lDBListChronometer.elapsed();
      
    // DEBUG
    OPENTREP_LOG_DEBUG ("Look up in the SQL database: " << lDBListMeasure
                        << " - " << lOPENTREP_ServiceContext.display());

    //
    return nbOfMatches;
  }

  // //////////////////////////////////////////////////////////////////////
  NbOfMatches_T OPENTREP_Service::
  listByFaaCode (const FAACode_T& iFaaCode,
                 LocationList_T& ioLocationList) {
    NbOfMatches_T nbOfMatches = 0;

    if (_opentrepServiceContext == NULL) {
      throw NonInitialisedServiceException ("The OpenTREP service has not been"
                                            " initialised");
    }
    assert (_opentrepServiceContext != NULL);
    OPENTREP_ServiceContext& lOPENTREP_ServiceContext = *_opentrepServiceContext;

    // Retrieve the SQL database type
    const DBType& lSQLDBType = lOPENTREP_ServiceContext.getSQLDBType();
      
    // Retrieve the SQL database connection string
    const SQLDBConnectionString_T& lSQLDBConnectionString =
      lOPENTREP_ServiceContext.getSQLDBConnectionString();
      
    // Delegate the database look up to the dedicated command
    BasChronometer lDBListChronometer;
    lDBListChronometer.start();

    // Connect to the SQLite3/MySQL database
    soci::session* lSociSession_ptr =
      DBManager::initSQLDBSession (lSQLDBType, lSQLDBConnectionString);
    assert (lSociSession_ptr != NULL);
    soci::session& lSociSession = *lSociSession_ptr;
      
    // Get the list of POR corresponding to the given FAA code
    nbOfMatches =
      DBManager::getPORByFAACode (lSociSession, iFaaCode, ioLocationList);

    const double lDBListMeasure = lDBListChronometer.elapsed();
      
    // DEBUG
    OPENTREP_LOG_DEBUG ("Look up in the SQL database: " << lDBListMeasure
                        << " - " << lOPENTREP_ServiceContext.display());

    //
    return nbOfMatches;
  }

  // //////////////////////////////////////////////////////////////////////
  NbOfMatches_T OPENTREP_Service::
  listByGeonameID (const GeonamesID_T& iGeonameID,
                   LocationList_T& ioLocationList) {
    NbOfMatches_T nbOfMatches = 0;

    if (_opentrepServiceContext == NULL) {
      throw NonInitialisedServiceException ("The OpenTREP service has not been"
                                            " initialised");
    }
    assert (_opentrepServiceContext != NULL);
    OPENTREP_ServiceContext& lOPENTREP_ServiceContext = *_opentrepServiceContext;

    // Retrieve the SQL database type
    const DBType& lSQLDBType = lOPENTREP_ServiceContext.getSQLDBType();
      
    // Retrieve the SQL database connection string
    const SQLDBConnectionString_T& lSQLDBConnectionString =
      lOPENTREP_ServiceContext.getSQLDBConnectionString();
      
    // Delegate the database look up to the dedicated command
    BasChronometer lDBListChronometer;
    lDBListChronometer.start();

    // Connect to the SQLite3/MySQL database
    soci::session* lSociSession_ptr =
      DBManager::initSQLDBSession (lSQLDBType, lSQLDBConnectionString);
    assert (lSociSession_ptr != NULL);
    soci::session& lSociSession = *lSociSession_ptr;
      
    // Get the list of POR corresponding to the given Geoname ID
    nbOfMatches =
      DBManager::getPORByGeonameID (lSociSession, iGeonameID, ioLocationList);

    const double lDBListMeasure = lDBListChronometer.elapsed();
      
    // DEBUG
    OPENTREP_LOG_DEBUG ("Look up in the SQL database: " << lDBListMeasure
                        << " - " << lOPENTREP_ServiceContext.display());

    //
    return nbOfMatches;
  }

  // //////////////////////////////////////////////////////////////////////
  NbOfDBEntries_T OPENTREP_Service::fillInFromPORFile() {
    NbOfDBEntries_T oNbOfEntries = 0;
    
    if (_opentrepServiceContext == NULL) {
      throw NonInitialisedServiceException ("The OpenTREP service has not been"
                                            " initialised");
    }
    assert (_opentrepServiceContext != NULL);
    OPENTREP_ServiceContext& lOPENTREP_ServiceContext = *_opentrepServiceContext;

    // Retrieve the file-path of the POR (points of reference) file
    const PORFilePath_T& lPORFilePath= lOPENTREP_ServiceContext.getPORFilePath();
      
    // Retrieve the SQL database type
    const DBType& lSQLDBType = lOPENTREP_ServiceContext.getSQLDBType();
      
    // Retrieve the SQL database connection string
    const SQLDBConnectionString_T& lSQLDBConnectionString =
      lOPENTREP_ServiceContext.getSQLDBConnectionString();
      
    // Delegate the index building to the dedicated command
    BasChronometer lBuildSearchIndexChronometer;
    lBuildSearchIndexChronometer.start();
    oNbOfEntries =
      DBManager::fillInFromPORFile (lPORFilePath,
                                    lSQLDBType, lSQLDBConnectionString);
    const double lBuildSearchIndexMeasure =
      lBuildSearchIndexChronometer.elapsed();
      
    // DEBUG
    OPENTREP_LOG_DEBUG ("Filled in the SQL database: "
                        << lBuildSearchIndexMeasure << " - "
                        << lOPENTREP_ServiceContext.display());

    return oNbOfEntries;
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
      
    // Retrieve the SQL database type
    const DBType& lSQLDBType = lOPENTREP_ServiceContext.getSQLDBType();
      
    // Retrieve the SQL database connection string
    const SQLDBConnectionString_T& lSQLDBConnectionString =
      lOPENTREP_ServiceContext.getSQLDBConnectionString();
      
    // Retrieve the Unicode transliterator
    const OTransliterator& lTransliterator =
      lOPENTREP_ServiceContext.getTransliterator();
      
    // Delegate the index building to the dedicated command
    BasChronometer lBuildSearchIndexChronometer;
    lBuildSearchIndexChronometer.start();
    oNbOfEntries = IndexBuilder::buildSearchIndex (lPORFilePath,
                                                   lTravelDBFilePath,
                                                   lSQLDBType,
                                                   lSQLDBConnectionString,
                                                   lTransliterator);
    const double lBuildSearchIndexMeasure =
      lBuildSearchIndexChronometer.elapsed();
      
    // DEBUG
    OPENTREP_LOG_DEBUG ("Built Xapian database/index and SQL database: "
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

    // Retrieve the Unicode transliterator
    const OTransliterator& lTransliterator =
      lOPENTREP_ServiceContext.getTransliterator();
      
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
                                                              ioWordList,
                                                              lTransliterator);
    const double lRequestInterpreterMeasure =
      lRequestInterpreterChronometer.elapsed();

    // DEBUG
    OPENTREP_LOG_DEBUG ("Match query on Xapian database (index): "
                        << lRequestInterpreterMeasure << " - "
                        << lOPENTREP_ServiceContext.display());
      
    return nbOfMatches;
  }
  
}
