// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// STL
#include <cassert>
#include <string>
#include <sstream>
// SOCI
#if defined(SOCI_HEADERS_BURIED)
#include <soci/core/soci.h>
#include <soci/backends/mysql/soci-mysql.h>
#else // SOCI_HEADERS_BURIED
#include <soci/soci.h>
#include <soci/mysql/soci-mysql.h>
#endif // SOCI_HEADERS_BURIED
// OpenTrep
#include <opentrep/DBParams.hpp>
#include <opentrep/command/DBSessionManager.hpp>
#include <opentrep/service/Logger.hpp>

namespace OPENTREP {

  // //////////////////////////////////////////////////////////////////////
  DBSessionManager::DBSessionManager() : _dbSession (NULL) {
  }

  // //////////////////////////////////////////////////////////////////////
  DBSessionManager::DBSessionManager (const DBParams& iDBParams)
    : _dbSession (NULL) {
    init (iDBParams);
  }

  // //////////////////////////////////////////////////////////////////////
  DBSessionManager::~DBSessionManager() {
    delete _dbSession; _dbSession = NULL;
  }

  // //////////////////////////////////////////////////////////////////////
  soci::session& DBSessionManager::getDBSessionRef() const {
    assert (_dbSession != NULL);
    return *_dbSession;
  }

  // //////////////////////////////////////////////////////////////////////
  void DBSessionManager::init (const DBParams& iDBParams) {
    
    // Check that the parameters for the SQL database are not empty
    if (iDBParams.check() == false) {
      std::ostringstream errorStr;
      errorStr << "At least one of the parameters for the SQL "
               << "database is empty: " << iDBParams;
      OPENTREP_LOG_ERROR (errorStr.str());
      throw XapianTravelDatabaseEmptyException (errorStr.str());
    }

    // Instanciate a (SOCI) database session: nothing is performed at
    // that stage
    _dbSession = new soci::session();
    
    try {

      // Open the connection to the database
      _dbSession->open (soci::mysql, iDBParams.toConnectionString());
      
    } catch (std::exception const& lException) {
      std::ostringstream errorStr;
      errorStr << "Error while opening a connection to database: "
               << lException.what() << std::endl;
      errorStr << "Database parameters used: " << iDBParams.toString();
      OPENTREP_LOG_ERROR (errorStr.str());
      throw SQLDatabaseConnectionImpossibleException (errorStr.str());
    }
  }
  
}
