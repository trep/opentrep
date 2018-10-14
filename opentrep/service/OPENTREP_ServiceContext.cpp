// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// STL
#include <cassert>
#include <string>
#include <istream>
#include <ostream>
#include <sstream>
// OpenTrep
#include <opentrep/basic/BasConst_OPENTREP_Service.hpp>
#include <opentrep/basic/Utilities.hpp>
#include <opentrep/bom/World.hpp>
#include <opentrep/service/OPENTREP_ServiceContext.hpp>

namespace OPENTREP {

  /**
   * The Xapian index/database file-path and SQL database name are used
   * as prefix of the actual respective file-path and name. The deployment
   * number/version is added to the prefix, and that makes the actual
   * file-path and name.
   *
   * For instance, if the current deployment number/version is 0, and:
   * <ul>
   *  <li>Xapian file-path (prefix) is '/tmp/opentrep/xapian_traveldb',
   *      the actual Xapian file-path become
   *      '/tmp/opentrep/xapian_traveldb0';</li>
   *  <li>MySQL/MariaDB connection string is
   *      'db=trep_trep user=trep password=trep', then the actual connection
   *      string becomes 'db=trep_trep0 user=trep password=trep'</li>
   *  <li>SQLite connection string is '/tmp/opentrep/sqlite_travel.db',
   *      then the actual connection string becomes
   *      '/tmp/opentrep/sqlite_travel.db0'</li>
   * </ul>
   */
  // //////////////////////////////////////////////////////////////////////
  void OPENTREP_ServiceContext::
  updateXapianAndSQLDBConnectionWithDeploymentNumber() {
    /**
     * SQL database specification
     */
    if (_sqlDBType == DBType::NODB) {
      // Nothing more to be done here
      
    } else if (_sqlDBType == DBType::SQLITE3) {
      std::ostringstream oStr;
      oStr << _sqlDBConnectionStringWPfxDBName;
      oStr << _deploymentNumber;
      _sqlDBConnectionString = SQLDBConnectionString_T (oStr.str());
      
    } else if (_sqlDBType == DBType::MYSQL) {
      /**
       * Parse the connection string. Typically, it is
       * 'db=trep_trep user=trep password=trep'
       * First, split the connection string by the space character (' '),
       * which leads to key-value pairs. Then, split each of those key-value
       * pairs by the equal sign ('='), which leads to the key and the
       * corresponding value.
       */
      const StringMap_T& lStrMap =
        parseMySQLConnectionString (_sqlDBConnectionStringWPfxDBName);

      /**
       * Recompose the connection string
       * 'db=trep_trep0 user=trep password=trep'
       */
      const SQLDBConnectionString_T& lSQLDBConnStr =
        buildMySQLConnectionString (lStrMap, _deploymentNumber);

      // Store the newly formed SQL connection string
      _sqlDBConnectionString = lSQLDBConnStr;
    }

    /**
     * Xapian index/database specification
     */
    std::ostringstream oStr;
    oStr << _travelDBFilePathPrefix;
    oStr << _deploymentNumber;
    _travelDBFilePath = TravelDBFilePath_T (oStr.str());
  }
  
  // //////////////////////////////////////////////////////////////////////
  OPENTREP_ServiceContext::OPENTREP_ServiceContext()
    : _world (NULL),
      _porFilePath (DEFAULT_OPENTREP_POR_FILEPATH),
      _deploymentNumber (DEFAULT_OPENTREP_DEPLOYMENT_NUMBER),
      _travelDBFilePathPrefix (DEFAULT_OPENTREP_XAPIAN_DB_FILEPATH),
      _travelDBFilePath (DEFAULT_OPENTREP_XAPIAN_DB_FILEPATH),
      _sqlDBType (DEFAULT_OPENTREP_SQL_DB_TYPE),
      _sqlDBConnectionStringWPfxDBName (DEFAULT_OPENTREP_SQLITE_DB_FILEPATH),
      _sqlDBConnectionString (DEFAULT_OPENTREP_SQLITE_DB_FILEPATH),
      _shouldIndexNonIATAPOR (DEFAULT_OPENTREP_INCLUDE_NONIATA_POR),
      _shouldIndexPORInXapian (DEFAULT_OPENTREP_INDEX_IN_XAPIAN),
      _shouldAddPORInSQLDB (DEFAULT_OPENTREP_ADD_IN_DB) {
    assert (false);
  }

  // //////////////////////////////////////////////////////////////////////
  OPENTREP_ServiceContext::
  OPENTREP_ServiceContext (const TravelDBFilePath_T& iTravelDBFilePath,
                           const DBType& iSQLDBType,
                           const SQLDBConnectionString_T& iSQLDBConnStr,
                           const DeploymentNumber_T& iDeploymentNumber)
    : _world (NULL),
      _porFilePath (DEFAULT_OPENTREP_POR_FILEPATH),
      _deploymentNumber (iDeploymentNumber),
      _travelDBFilePathPrefix (iTravelDBFilePath),
      _travelDBFilePath (iTravelDBFilePath), _sqlDBType (iSQLDBType),
      _sqlDBConnectionStringWPfxDBName (iSQLDBConnStr),
      _sqlDBConnectionString (iSQLDBConnStr),
      _shouldIndexNonIATAPOR (DEFAULT_OPENTREP_INCLUDE_NONIATA_POR),
      _shouldIndexPORInXapian (DEFAULT_OPENTREP_INDEX_IN_XAPIAN),
      _shouldAddPORInSQLDB (DEFAULT_OPENTREP_ADD_IN_DB) {
    updateXapianAndSQLDBConnectionWithDeploymentNumber();
  }

  // //////////////////////////////////////////////////////////////////////
  OPENTREP_ServiceContext::
  OPENTREP_ServiceContext (const PORFilePath_T& iPORFilePath,
                           const TravelDBFilePath_T& iTravelDBFilePath,
                           const DBType& iSQLDBType,
                           const SQLDBConnectionString_T& iSQLDBConnStr,
                           const DeploymentNumber_T& iDeploymentNumber,
                           const shouldIndexNonIATAPOR_T& iShouldIndexNonIATAPOR,
                           const shouldIndexPORInXapian_T& iShouldIdxPORInXapian,
                           const shouldAddPORInSQLDB_T& iShouldAddPORInSQLDB)
    : _world (NULL), _porFilePath (iPORFilePath),
      _deploymentNumber (iDeploymentNumber),
      _travelDBFilePathPrefix (iTravelDBFilePath),
      _travelDBFilePath (iTravelDBFilePath), _sqlDBType (iSQLDBType),
      _sqlDBConnectionStringWPfxDBName (iSQLDBConnStr),
      _sqlDBConnectionString (iSQLDBConnStr),
      _shouldIndexNonIATAPOR (iShouldIndexNonIATAPOR),
      _shouldIndexPORInXapian (iShouldIdxPORInXapian),
      _shouldAddPORInSQLDB (iShouldAddPORInSQLDB) {
    updateXapianAndSQLDBConnectionWithDeploymentNumber();
  }

  // //////////////////////////////////////////////////////////////////////
  OPENTREP_ServiceContext::~OPENTREP_ServiceContext() {
  }
  
  // //////////////////////////////////////////////////////////////////////
  World& OPENTREP_ServiceContext::getWorldHandler() const {
    assert (_world != NULL);
    return *_world;
  }
  
  // //////////////////////////////////////////////////////////////////////
  const std::string OPENTREP_ServiceContext::shortDisplay() const {
    std::ostringstream oStr;
    oStr << "OPENTREP_ServiceContext: "
         << "file-path of the POR file: " << _porFilePath
         << "; deployment number/version: " << _deploymentNumber
         << "; Directory prefix of Xapian index/database: "
         << _travelDBFilePathPrefix
         << "; Actual directory of Xapian index/database: " << _travelDBFilePath
         << "; SQL database (" << _sqlDBType.describe()
         << ") connection string with DB name prefix: "
         << _sqlDBConnectionStringWPfxDBName
         << "); Connection string with actual DB name: "
         << _sqlDBConnectionString
         << "; should include non-IATA POR: " << _shouldIndexNonIATAPOR
         << "; should index POR in Xapian: " << _shouldIndexPORInXapian
         << "; should insert POR into the SQL DB: " << _shouldAddPORInSQLDB
         << std::endl;
    return oStr.str();
  }

  // //////////////////////////////////////////////////////////////////////
  const std::string OPENTREP_ServiceContext::display() const {
    std::ostringstream oStr;
    oStr << shortDisplay();
    if (_world != NULL) {
      oStr << _world->display();
    }
    return oStr.str();
  }

}
