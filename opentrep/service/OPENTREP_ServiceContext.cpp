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
  void checkXapian (const TravelDBFilePath_T& iTravelDBFilePath) {
    // Check that the Xapian travel database is not empty
    if (iTravelDBFilePath.empty() == true) {
      std::ostringstream errorStr;
      errorStr << "The file-path for the Xapian travel database/index is "
               << "not specified.";
      std::cerr << errorStr.str() << std::endl;
      throw XapianTravelDatabaseWrongPathnameException (errorStr.str());
    }
  }

  // //////////////////////////////////////////////////////////////////////
  OPENTREP_ServiceContext::OPENTREP_ServiceContext()
    : _world (NULL),
      _porFilePath (DEFAULT_OPENTREP_POR_FILEPATH),
      _travelDBFilePath (DEFAULT_OPENTREP_XAPIAN_DB_FILEPATH) {
    assert (false);
  }

  // //////////////////////////////////////////////////////////////////////
  OPENTREP_ServiceContext::
  OPENTREP_ServiceContext (const TravelDBFilePath_T& iTravelDBFilePath)
    : _world (NULL),
      _porFilePath (DEFAULT_OPENTREP_POR_FILEPATH),
      _travelDBFilePath (iTravelDBFilePath) {
    OPENTREP::checkXapian (iTravelDBFilePath);
  }

  // //////////////////////////////////////////////////////////////////////
  OPENTREP_ServiceContext::
  OPENTREP_ServiceContext (const PORFilePath_T& iPORFilePath,
                           const TravelDBFilePath_T& iTravelDBFilePath)
    : _world (NULL), _porFilePath (iPORFilePath),
      _travelDBFilePath (iTravelDBFilePath) {
    OPENTREP::checkXapian (iTravelDBFilePath);
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
         << "file-path of the POR file: " << _porFilePath << ", "
         << "Xapian Database (directory of the index): " << _travelDBFilePath
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
