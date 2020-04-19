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
#include <opentrep/Location.hpp>
#include <opentrep/CityDetails.hpp>
#include <opentrep/basic/BasConst_OPENTREP_Service.hpp>
#include <opentrep/basic/BasChronometer.hpp>
#include <opentrep/factory/FacWorld.hpp>
#include <opentrep/command/DBManager.hpp>
#include <opentrep/command/FileManager.hpp>
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
                    const SQLDBConnectionString_T& iSQLDBConnStr,
                    const DeploymentNumber_T& iDeploymentNumber,
                    const shouldIndexNonIATAPOR_T& iShouldIndexNonIATAPOR,
                    const shouldIndexPORInXapian_T& iShouldIndexPORInXapian,
                    const shouldAddPORInSQLDB_T& iShouldAddPORInSQLDB)
    : _opentrepServiceContext (NULL) {
    init (ioLogStream, iPORFilepath, iTravelDBFilePath, iSQLDBType,
          iSQLDBConnStr, iDeploymentNumber, iShouldIndexNonIATAPOR,
          iShouldIndexPORInXapian, iShouldAddPORInSQLDB);
  }

  // //////////////////////////////////////////////////////////////////////
  OPENTREP_Service::
  OPENTREP_Service (std::ostream& ioLogStream,
                    const TravelDBFilePath_T& iTravelDBFilePath,
                    const DBType& iSQLDBType,
                    const SQLDBConnectionString_T& iSQLDBConnStr,
                    const DeploymentNumber_T& iDeploymentNumber)
    : _opentrepServiceContext (NULL) {
    init (ioLogStream, iTravelDBFilePath, iSQLDBType, iSQLDBConnStr,
          iDeploymentNumber);
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
  SQLDBConnectionString_T
  getSQLConnStr (const DBType& iSQLDBType,
                 const SQLDBConnectionString_T& iSQLDBConnStr) {
    // When the SQL database is MariaDB/MySQL and the connection string
    // is equal to the default SQLite one, override it
    std::string oSQLDBConnStr =
      static_cast<const std::string> (iSQLDBConnStr);
    if (iSQLDBType == DBType::MYSQL
        && oSQLDBConnStr == DEFAULT_OPENTREP_SQLITE_DB_FILEPATH) {
      oSQLDBConnStr = DEFAULT_OPENTREP_MYSQL_CONN_STRING;
    }
    return SQLDBConnectionString_T (oSQLDBConnStr);
  }

  // //////////////////////////////////////////////////////////////////////
  void OPENTREP_Service::init (std::ostream& ioLogStream,
                               const TravelDBFilePath_T& iTravelDBFilePath,
                               const DBType& iSQLDBType,
                               const SQLDBConnectionString_T& iSQLDBConnStr,
                               const DeploymentNumber_T& iDeploymentNumber) {
    // Set the log file
    logInit (LOG::DEBUG, ioLogStream);

    // Fix the SQL database connection string, if needed
    const SQLDBConnectionString_T& lSQLDBConnStr =
      getSQLConnStr (iSQLDBType, iSQLDBConnStr);

    // Initialise the context
    OPENTREP_ServiceContext& lOPENTREP_ServiceContext = 
      FacOpenTrepServiceContext::instance().create (iTravelDBFilePath,
                                                    iSQLDBType, lSQLDBConnStr,
                                                    iDeploymentNumber);
    _opentrepServiceContext = &lOPENTREP_ServiceContext;

    // Instanciate an empty World object
    World& lWorld = FacWorld::instance().create();
    lOPENTREP_ServiceContext.setWorld (lWorld);
  }
  
  // //////////////////////////////////////////////////////////////////////
  void OPENTREP_Service::
  init (std::ostream& ioLogStream,
        const PORFilePath_T& iPORFilepath,
        const TravelDBFilePath_T& iTravelDBFilePath,
        const DBType& iSQLDBType,
        const SQLDBConnectionString_T& iSQLDBConnStr,
        const DeploymentNumber_T& iDeploymentNumber,
        const shouldIndexNonIATAPOR_T& iShouldIndexNonIATAPOR,
        const shouldIndexPORInXapian_T& iShouldIndexPORInXapian,
        const shouldAddPORInSQLDB_T& iShouldAddPORInSQLDB) {
    // Set the log file
    logInit (LOG::DEBUG, ioLogStream);

    // Fix the SQL database connection string, if needed
    const SQLDBConnectionString_T& lSQLDBConnStr =
      getSQLConnStr (iSQLDBType, iSQLDBConnStr);

    // Initialise the context
    OPENTREP_ServiceContext& lOPENTREP_ServiceContext = 
      FacOpenTrepServiceContext::instance().create (iPORFilepath,
                                                    iTravelDBFilePath,
                                                    iSQLDBType, lSQLDBConnStr,
                                                    iDeploymentNumber,
                                                    iShouldIndexNonIATAPOR,
                                                    iShouldIndexPORInXapian,
                                                    iShouldAddPORInSQLDB);
    _opentrepServiceContext = &lOPENTREP_ServiceContext;

    // Instanciate an empty World object
    World& lWorld = FacWorld::instance().create();
    lOPENTREP_ServiceContext.setWorld (lWorld);
  }
  
  // //////////////////////////////////////////////////////////////////////
  void OPENTREP_Service::finalise() {
  }

  // //////////////////////////////////////////////////////////////////////
  const DeploymentNumber_T& OPENTREP_Service::getDeploymentNumber() const {
    if (_opentrepServiceContext == NULL) {
      throw NonInitialisedServiceException ("The OpenTREP service has not been"
                                            " initialised");
    }
    assert (_opentrepServiceContext != NULL);
    OPENTREP_ServiceContext& lOPENTREP_ServiceContext = *_opentrepServiceContext;

    // Retrieve the deployment number/version
    const DeploymentNumber_T& lDeploymentNumber =
      lOPENTREP_ServiceContext.getDeploymentNumber();
    
    return lDeploymentNumber;
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
  bool OPENTREP_Service::
  checkXapianDBOnFileSystem (const TravelDBFilePath_T& iTravelDBFilePath) const {
    bool oExistXapianDBDir =
      FileManager::checkXapianDBOnFileSystem (iTravelDBFilePath);
    return oExistXapianDBDir;
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
      
    // Sanity check: no need to perform any action when the current option
    // is to not use any SQL database
    if (lSQLDBType == DBType::NODB) {
      // DEBUG
      OPENTREP_LOG_DEBUG ("The current option is to not use any SQL database. "
                          << "Hence nothing is done at that stage. "
                          << " - " << lOPENTREP_ServiceContext.display());
      return oCreationSuccessful;
    }
      
    // Retrieve the SQL database connection string
    const SQLDBConnectionString_T& lSQLDBConnectionString =
      lOPENTREP_ServiceContext.getSQLDBConnectionString();
      
    // Retrieve the deployment number/version
    const DeploymentNumber_T& lDeploymentNumber =
      lOPENTREP_ServiceContext.getDeploymentNumber();

    // Delegate the database creation to the dedicated command
    BasChronometer lDBCreationChronometer;
    lDBCreationChronometer.start();

    // Create the SQL database user ('trep' on MySQL database)
    // and database ('trep_trep' on MySQL database)
    oCreationSuccessful =
      DBManager::createSQLDBUser (lSQLDBType, lSQLDBConnectionString,
                                  lDeploymentNumber);

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

    // Sanity check: no need to perform any action when the current option
    // is to not use any SQL database
    if (lSQLDBType == DBType::NODB) {
      // DEBUG
      OPENTREP_LOG_DEBUG ("The current option is to not use any SQL database. "
                          << "Hence nothing is done at that stage. "
                          << " - " << lOPENTREP_ServiceContext.display());
      return;
    }
      
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

    // Release the SQL database connection
    DBManager::terminateSQLDBSession (lSQLDBType, lSQLDBConnectionString,
                                      lSociSession);

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
      
    // Sanity check: no need to perform any action when the current option
    // is to not use any SQL database
    if (lSQLDBType == DBType::NODB) {
      // DEBUG
      OPENTREP_LOG_DEBUG ("The current option is to not use any SQL database. "
                          << "Hence nothing is done at that stage. "
                          << " - " << lOPENTREP_ServiceContext.display());
      return;
    }
      
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

    // Release the SQL database connection
    DBManager::terminateSQLDBSession (lSQLDBType, lSQLDBConnectionString,
                                      lSociSession);

    const double lDBCreationMeasure = lDBCreationChronometer.elapsed();
      
    // DEBUG
    OPENTREP_LOG_DEBUG ("Created the SQL database indexes: "
                        << lDBCreationMeasure << " - "
                        << lOPENTREP_ServiceContext.display());
  }

  // //////////////////////////////////////////////////////////////////////
  OPENTREP::DeploymentNumber_T OPENTREP_Service::toggleDeploymentNumber() {
    DeploymentNumber_T oDeploymentNumber = 0;
    
    if (_opentrepServiceContext == NULL) {
      throw NonInitialisedServiceException ("The OpenTREP service has not been"
                                            " initialised");
    }
    assert (_opentrepServiceContext != NULL);
    OPENTREP_ServiceContext& lOPENTREP_ServiceContext = *_opentrepServiceContext;

    // Retrieve the deployment number/version
    oDeploymentNumber = lOPENTREP_ServiceContext.getDeploymentNumber();

    // Toggle the number
    ++oDeploymentNumber;
    if (oDeploymentNumber >= DEFAULT_OPENTREP_DEPLOYMENT_NUMBER_SIZE) {
      oDeploymentNumber = DEFAULT_OPENTREP_DEPLOYMENT_NUMBER;
    }

    // Store back the toggled flag
    lOPENTREP_ServiceContext.setDeploymentNumber (oDeploymentNumber);

    // DEBUG
    OPENTREP_LOG_DEBUG ("The new deployment number/version is: "
                        << oDeploymentNumber << " - "
                        << lOPENTREP_ServiceContext.display());

    return oDeploymentNumber;
  }

  // //////////////////////////////////////////////////////////////////////
  OPENTREP::shouldIndexNonIATAPOR_T OPENTREP_Service::
  toggleShouldIncludeAllPORFlag() {
    shouldIndexNonIATAPOR_T oShouldIndexNonIATAPOR = false;
    
    if (_opentrepServiceContext == NULL) {
      throw NonInitialisedServiceException ("The OpenTREP service has not been"
                                            " initialised");
    }
    assert (_opentrepServiceContext != NULL);
    OPENTREP_ServiceContext& lOPENTREP_ServiceContext = *_opentrepServiceContext;

    // Retrieve the flag
    oShouldIndexNonIATAPOR =
      lOPENTREP_ServiceContext.getShouldIncludeAllPORFlag();

    // Toggle the flag
    oShouldIndexNonIATAPOR = !(oShouldIndexNonIATAPOR);

    // Store back the toggled flag
    lOPENTREP_ServiceContext.setShouldIncludeAllPORFlag (oShouldIndexNonIATAPOR);
      
    // DEBUG
    OPENTREP_LOG_DEBUG ("The new non-IATA-referenced POR flag is: "
                        << oShouldIndexNonIATAPOR << " - "
                        << lOPENTREP_ServiceContext.display());

    return oShouldIndexNonIATAPOR;
  }

  // //////////////////////////////////////////////////////////////////////
  OPENTREP::shouldIndexPORInXapian_T OPENTREP_Service::
  toggleShouldIndexPORInXapianFlag() {
    shouldIndexPORInXapian_T oShouldIndexPORInXapian = true;
    
    if (_opentrepServiceContext == NULL) {
      throw NonInitialisedServiceException ("The OpenTREP service has not been"
                                            " initialised");
    }
    assert (_opentrepServiceContext != NULL);
    OPENTREP_ServiceContext& lOPENTREP_ServiceContext = *_opentrepServiceContext;

    // Retrieve the flag
    oShouldIndexPORInXapian =
      lOPENTREP_ServiceContext.getShouldIndexPORInXapianFlag();

    // Toggle the flag
    oShouldIndexPORInXapian = !(oShouldIndexPORInXapian);

    // Store back the toggled flag
    lOPENTREP_ServiceContext.
      setShouldIndexPORInXapianFlag (oShouldIndexPORInXapian);
      
    // DEBUG
    OPENTREP_LOG_DEBUG ("The new index-in-Xapian-POR flag is: "
                        << oShouldIndexPORInXapian << " - "
                        << lOPENTREP_ServiceContext.display());

    return oShouldIndexPORInXapian;
  }

  // //////////////////////////////////////////////////////////////////////
  OPENTREP::shouldAddPORInSQLDB_T OPENTREP_Service::
  toggleShouldAddPORInSQLDBFlag() {
    shouldAddPORInSQLDB_T oShouldAddPORInSQLDB = false;
    
    if (_opentrepServiceContext == NULL) {
      throw NonInitialisedServiceException ("The OpenTREP service has not been"
                                            " initialised");
    }
    assert (_opentrepServiceContext != NULL);
    OPENTREP_ServiceContext& lOPENTREP_ServiceContext = *_opentrepServiceContext;

    // Retrieve the flag
    oShouldAddPORInSQLDB =
      lOPENTREP_ServiceContext.getShouldAddPORInSQLDB();

    // Toggle the flag
    oShouldAddPORInSQLDB = !(oShouldAddPORInSQLDB);

    // Store back the toggled flag
    lOPENTREP_ServiceContext.setShouldAddPORInSQLDB (oShouldAddPORInSQLDB);
      
    // DEBUG
    OPENTREP_LOG_DEBUG ("The new insert-in-DB-POR flag is: "
                        << oShouldAddPORInSQLDB << " - "
                        << lOPENTREP_ServiceContext.display());

    return oShouldAddPORInSQLDB;
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

    // Release the SQL database connection
    DBManager::terminateSQLDBSession (lSQLDBType, lSQLDBConnectionString,
                                      lSociSession);

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
    const bool lUniqueEntry = false;
    nbOfMatches = DBManager::getPORByIATACode (lSociSession, iIataCode,
                                               ioLocationList, lUniqueEntry);

    // Release the SQL database connection
    DBManager::terminateSQLDBSession (lSQLDBType, lSQLDBConnectionString,
                                      lSociSession);

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

    // Release the SQL database connection
    DBManager::terminateSQLDBSession (lSQLDBType, lSQLDBConnectionString,
                                      lSociSession);

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

    // Release the SQL database connection
    DBManager::terminateSQLDBSession (lSQLDBType, lSQLDBConnectionString,
                                      lSociSession);

    const double lDBListMeasure = lDBListChronometer.elapsed();
      
    // DEBUG
    OPENTREP_LOG_DEBUG ("Look up in the SQL database: " << lDBListMeasure
                        << " - " << lOPENTREP_ServiceContext.display());

    //
    return nbOfMatches;
  }

  // //////////////////////////////////////////////////////////////////////
  NbOfMatches_T OPENTREP_Service::
  listByUNLOCode (const UNLOCode_T& iUNLOCode,
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
      
    // Get the list of POR corresponding to the given UN/LOCODE code
    const bool lUniqueEntry = false;
    nbOfMatches =
      DBManager::getPORByUNLOCode (lSociSession, iUNLOCode, ioLocationList,
                                   lUniqueEntry);

    // Release the SQL database connection
    DBManager::terminateSQLDBSession (lSQLDBType, lSQLDBConnectionString,
                                      lSociSession);

    const double lDBListMeasure = lDBListChronometer.elapsed();
      
    // DEBUG
    OPENTREP_LOG_DEBUG ("Look up in the SQL database: " << lDBListMeasure
                        << " - " << lOPENTREP_ServiceContext.display());

    //
    return nbOfMatches;
  }

  // //////////////////////////////////////////////////////////////////////
  NbOfMatches_T OPENTREP_Service::
  listByUICCode (const UICCode_T& iUICCode, LocationList_T& ioLocationList) {
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
      
    // Get the list of POR corresponding to the given UIC code
    nbOfMatches =
      DBManager::getPORByUICCode (lSociSession, iUICCode, ioLocationList);

    // Release the SQL database connection
    DBManager::terminateSQLDBSession (lSQLDBType, lSQLDBConnectionString,
                                      lSociSession);

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

    // Release the SQL database connection
    DBManager::terminateSQLDBSession (lSQLDBType, lSQLDBConnectionString,
                                      lSociSession);

    const double lDBListMeasure = lDBListChronometer.elapsed();
      
    // DEBUG
    OPENTREP_LOG_DEBUG ("Look up in the SQL database: " << lDBListMeasure
                        << " - " << lOPENTREP_ServiceContext.display());

    //
    return nbOfMatches;
  }

  // //////////////////////////////////////////////////////////////////////
  NbOfDBEntries_T OPENTREP_Service::insertIntoDBAndXapian() {
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

    // Retrieve whether or not all the POR should be indexed
    const OPENTREP::shouldIndexNonIATAPOR_T& lIncludeNonIATAPOR =
      lOPENTREP_ServiceContext.getShouldIncludeAllPORFlag();

    // Retrieve whether or not the POR should be indexed in Xapian
    const OPENTREP::shouldIndexPORInXapian_T& lShouldIndexPORInXapian =
      lOPENTREP_ServiceContext.getShouldIndexPORInXapianFlag();

    // Retrieve whether or not the POR should be indexed in Xapian
    const OPENTREP::shouldAddPORInSQLDB_T& lShouldAddPORInSQLDB =
      lOPENTREP_ServiceContext.getShouldAddPORInSQLDB();

    // Retrieve the Unicode transliterator
    const OTransliterator& lTransliterator =
      lOPENTREP_ServiceContext.getTransliterator();
      
    // Delegate the index building to the dedicated command
    BasChronometer lInsertIntoXapianAndSQLDBChronometer;
    lInsertIntoXapianAndSQLDBChronometer.start();
    oNbOfEntries = IndexBuilder::buildSearchIndex (lPORFilePath,
                                                   lTravelDBFilePath,
                                                   lSQLDBType,
                                                   lSQLDBConnectionString,
                                                   lIncludeNonIATAPOR,
                                                   lShouldIndexPORInXapian,
                                                   lShouldAddPORInSQLDB,
                                                   lTransliterator);
    const double lInsertIntoXapianAndSQLDBMeasure =
      lInsertIntoXapianAndSQLDBChronometer.elapsed();
      
    // DEBUG
    OPENTREP_LOG_DEBUG ("Built Xapian database/index and filled SQL database: "
                        << lInsertIntoXapianAndSQLDBMeasure << " - "
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
    
    // Check that the travel query is not empty
    if (iTravelQuery.empty() == true) {
      std::ostringstream errorStr;
      errorStr << "The travel query is empty.";
      OPENTREP_LOG_ERROR (errorStr.str());
      throw TravelRequestEmptyException (errorStr.str());
    }
    
    // Retrieve the Xapian database name (directorty of the index)
    const TravelDBFilePath_T& lTravelDBFilePath =
      lOPENTREP_ServiceContext.getTravelDBFilePath();

    // Check whether the Xapian database/index is existing
    const bool lExistXapianDBDir = checkXapianDBOnFileSystem (lTravelDBFilePath);
    if (lExistXapianDBDir == false) {
      std::ostringstream errorStr;
      errorStr << "The file-path to the Xapian database/index ('"
               << lTravelDBFilePath << "') does not exist or is not a "
               << "directory." << std::endl;
      errorStr << "That usually means that the OpenTREP indexer "
               << "(opentrep-indexer) has not been launched yet, "
               << "or that it has operated on a different Xapian "
               << "database/index file-path, for instance with a different "
               << "deployment number";
      OPENTREP_LOG_ERROR (errorStr.str());
      throw XapianTravelDatabaseWrongPathnameException (errorStr.str());
      return nbOfMatches;
    }
      
    // Retrieve the SQL database type
    const DBType& lSQLDBType = lOPENTREP_ServiceContext.getSQLDBType();
      
    // Retrieve the SQL database connection string
    const SQLDBConnectionString_T& lSQLDBConnString =
      lOPENTREP_ServiceContext.getSQLDBConnectionString();
      
    // Delegate the query execution to the dedicated command
    BasChronometer lRequestInterpreterChronometer;
    lRequestInterpreterChronometer.start();
    nbOfMatches =
      RequestInterpreter::interpretTravelRequest (lTravelDBFilePath,
                                                  lSQLDBType, lSQLDBConnString,
                                                  iTravelQuery,
                                                  ioLocationList, ioWordList,
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
