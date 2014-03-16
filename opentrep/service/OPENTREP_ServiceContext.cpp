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
      _fillSQLDB (DEFAULT_OPENTREP_SQLITE_DB_FILLING_INDICATOR),
      _sqliteDBFilePath (DEFAULT_OPENTREP_SQLITE_DB_FILEPATH) {
    assert (false);
  }

  // //////////////////////////////////////////////////////////////////////
  OPENTREP_ServiceContext::
  OPENTREP_ServiceContext (const TravelDBFilePath_T& iTravelDBFilePath,
                           const FillSQLDB_T& iFillSQLDB,
                           const SQLiteDBFilePath_T& iSQLiteDBFilePath)
    : _world (NULL),
      _porFilePath (DEFAULT_OPENTREP_POR_FILEPATH),
      _travelDBFilePath (iTravelDBFilePath),
      _fillSQLDB (iFillSQLDB), _sqliteDBFilePath (iSQLiteDBFilePath) {
  }

  // //////////////////////////////////////////////////////////////////////
  OPENTREP_ServiceContext::
  OPENTREP_ServiceContext (const PORFilePath_T& iPORFilePath,
                           const TravelDBFilePath_T& iTravelDBFilePath,
                           const FillSQLDB_T& iFillSQLDB,
                           const SQLiteDBFilePath_T& iSQLiteDBFilePath)
    : _world (NULL), _porFilePath (iPORFilePath),
      _travelDBFilePath (iTravelDBFilePath),
      _fillSQLDB (iFillSQLDB), _sqliteDBFilePath (iSQLiteDBFilePath) {
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
    const std::string lFillDB (_fillSQLDB?"to be filled":"not to be filled");
    std::ostringstream oStr;
    oStr << "OPENTREP_ServiceContext: "
         << "file-path of the POR file: " << _porFilePath
         << "; Xapian database (directory of the index): " << _travelDBFilePath
         << "; SQLite3 database (" << lFillDB
         << ") file-path: " << _sqliteDBFilePath
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
