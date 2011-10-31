// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// STL
#include <cassert>
#include <sstream>
// OpenTrep
#include <opentrep/basic/BasConst_OPENTREP_Service.hpp>
#include <opentrep/bom/World.hpp>
#include <opentrep/service/OPENTREP_ServiceContext.hpp>

namespace OPENTREP {

  // //////////////////////////////////////////////////////////////////////
  OPENTREP_ServiceContext::OPENTREP_ServiceContext ()
    : _sociSession (NULL), _world (NULL),
      _travelDatabaseName (DEFAULT_OPENTREP_SERVICE_DB_NAME) {
  }

  // //////////////////////////////////////////////////////////////////////
  OPENTREP_ServiceContext::
  OPENTREP_ServiceContext (const TravelDatabaseName_T& iTravelDatabaseName)
    : _sociSession (NULL), _world (NULL),
      _travelDatabaseName (iTravelDatabaseName) {
  }

  // //////////////////////////////////////////////////////////////////////
  OPENTREP_ServiceContext::~OPENTREP_ServiceContext() {
  }
  
  // //////////////////////////////////////////////////////////////////////
  soci::session& OPENTREP_ServiceContext::getSociSessionHandler() const {
    assert (_sociSession != NULL);
    return *_sociSession;
  }

  // //////////////////////////////////////////////////////////////////////
  World& OPENTREP_ServiceContext::getWorldHandler () const {
    assert (_world != NULL);
    return *_world;
  }
  
  // //////////////////////////////////////////////////////////////////////
  const std::string OPENTREP_ServiceContext::shortDisplay() const {
    std::ostringstream oStr;
    oStr << "OPENTREP_ServiceContext: " << std::endl
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
