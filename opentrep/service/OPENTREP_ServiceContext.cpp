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

  /**
   * Helper function to check whether the Xapian travel database file-path
   * is valid.
   *
   * \todo Check with Boost that the file-path is valid.
   */
  // //////////////////////////////////////////////////////////////////////
  void checkXapian (const TravelDatabaseName_T& iTravelDatabaseName) {
    // Check that the Xapian travel database is not empty
    if (iTravelDatabaseName.empty() == true) {
      std::ostringstream errorStr;
      errorStr << "The file-path for the Xapian travel database is "
               << "not specified.";
      std::cerr << errorStr.str() << std::endl;
      throw XapianTravelDatabaseWrongPathnameException (errorStr.str());
    }
  }

  // //////////////////////////////////////////////////////////////////////
  OPENTREP_ServiceContext::OPENTREP_ServiceContext()
    : _world (NULL),
      _prFilePath (DEFAULT_OPENTREP_SERVICE_PR_FILEPATH),
      _porFilePath (DEFAULT_OPENTREP_SERVICE_POR_FILEPATH),
      _travelDatabaseName (DEFAULT_OPENTREP_SERVICE_DB_NAME) {
    assert (false);
  }

  // //////////////////////////////////////////////////////////////////////
  OPENTREP_ServiceContext::
  OPENTREP_ServiceContext (const TravelDatabaseName_T& iTravelDatabaseName)
    : _world (NULL),
      _prFilePath (DEFAULT_OPENTREP_SERVICE_PR_FILEPATH),
      _porFilePath (DEFAULT_OPENTREP_SERVICE_POR_FILEPATH),
      _travelDatabaseName (iTravelDatabaseName) {
    OPENTREP::checkXapian (iTravelDatabaseName);
  }

  // //////////////////////////////////////////////////////////////////////
  OPENTREP_ServiceContext::
  OPENTREP_ServiceContext (const PRFilePath_T& iPRFilePath,
                           const PORFilePath_T& iPORFilePath,
                           const TravelDatabaseName_T& iTravelDatabaseName)
    : _world (NULL), _prFilePath (iPRFilePath), _porFilePath (iPORFilePath),
      _travelDatabaseName (iTravelDatabaseName) {
    OPENTREP::checkXapian (iTravelDatabaseName);
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
         << "file-path of the PR file: " << _prFilePath << ", "
         << "file-path of the POR file: " << _porFilePath << ", "
         << "Xapian Database (directory of the index): " << _travelDatabaseName
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
