// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// STL
#include <cassert>
#include <ostream>
#include <sstream>
// OpenTrep
#include <opentrep/basic/BasConst_OPENTREP_Service.hpp>
#include <opentrep/bom/World.hpp>
#include <opentrep/service/OPENTREP_ServiceContext.hpp>

namespace OPENTREP {

  // //////////////////////////////////////////////////////////////////////
  OPENTREP_ServiceContext::OPENTREP_ServiceContext()
    : _world (NULL),
      _porFilePath (DEFAULT_OPENTREP_POR_FILEPATH),
      _travelDBFilePath (DEFAULT_OPENTREP_XAPIAN_DB_FILEPATH),
      _sqlDBType (DEFAULT_OPENTREP_SQL_DB_TYPE),
      _sqlDBConnectionString (DEFAULT_OPENTREP_SQLITE_DB_FILEPATH),
      _deploymentNumber (DEFAULT_OPENTREP_DEPLOYMENT_NUMBER),
      _shouldIndexNonIATAPOR (DEFAULT_OPENTREP_INCLUDE_NONIATA_POR),
      _shouldIndexPORInXapian (DEFAULT_OPENTREP_INDEX_IN_XAPIAN),
      _shouldAddPORInSQLDB (DEFAULT_OPENTREP_ADD_IN_DB) {
    assert (false);
  }

  // //////////////////////////////////////////////////////////////////////
  OPENTREP_ServiceContext::
  OPENTREP_ServiceContext (const TravelDBFilePath_T& iTravelDBFilePath,
                           const DBType& iSQLDBType,
                           const SQLDBConnectionString_T& iSQLDBConnStr)
    : _world (NULL),
      _porFilePath (DEFAULT_OPENTREP_POR_FILEPATH),
      _travelDBFilePath (iTravelDBFilePath),
      _sqlDBType (iSQLDBType), _sqlDBConnectionString (iSQLDBConnStr),
      _deploymentNumber (DEFAULT_OPENTREP_DEPLOYMENT_NUMBER),
      _shouldIndexNonIATAPOR (DEFAULT_OPENTREP_INCLUDE_NONIATA_POR),
      _shouldIndexPORInXapian (DEFAULT_OPENTREP_INDEX_IN_XAPIAN),
      _shouldAddPORInSQLDB (DEFAULT_OPENTREP_ADD_IN_DB) {
  }

  // //////////////////////////////////////////////////////////////////////
  OPENTREP_ServiceContext::
  OPENTREP_ServiceContext (const PORFilePath_T& iPORFilePath,
                           const TravelDBFilePath_T& iTravelDBFilePath,
                           const DBType& iSQLDBType,
                           const SQLDBConnectionString_T& iSQLDBConnStr,
                           const shouldIndexNonIATAPOR_T& iShouldIndexNonIATAPOR)
    : _world (NULL), _porFilePath (iPORFilePath),
      _travelDBFilePath (iTravelDBFilePath),
      _sqlDBType (iSQLDBType), _sqlDBConnectionString (iSQLDBConnStr),
      _deploymentNumber (DEFAULT_OPENTREP_DEPLOYMENT_NUMBER),
      _shouldIndexNonIATAPOR (iShouldIndexNonIATAPOR),
      _shouldIndexPORInXapian (DEFAULT_OPENTREP_INDEX_IN_XAPIAN),
      _shouldAddPORInSQLDB (DEFAULT_OPENTREP_ADD_IN_DB) {
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
         << "; Xapian database (directory of the index): " << _travelDBFilePath
         << "; SQL database (" << _sqlDBType.describe()
         << ") connection string: " << _sqlDBConnectionString
         << "; deployment number/version: " << _deploymentNumber
         << "; should include non-IATA POR: " << _shouldIndexNonIATAPOR
         << "; should index POR in Xapian: " << _shouldIndexNonIATAPOR
         << "; should insert POR into the SQL DB: " << _shouldIndexNonIATAPOR
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
