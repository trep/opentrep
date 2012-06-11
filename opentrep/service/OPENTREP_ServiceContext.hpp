#ifndef __OPENTREP_SVC_OPENTREPSERVICECONTEXT_HPP
#define __OPENTREP_SVC_OPENTREPSERVICECONTEXT_HPP

// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// STL
#include <string>
// OpenTrep
#include <opentrep/OPENTREP_Types.hpp>
#include <opentrep/command/DBSessionManager.hpp>
#include <opentrep/service/ServiceAbstract.hpp>

// Forward declarations
namespace soci {
  class session;
}

namespace OPENTREP {

  // Forward declarations
  class World;
  
  /**
   * @brief Class holding the context of the OpenTrep services.
   */
  class OPENTREP_ServiceContext : public ServiceAbstract {
    friend class FacOpenTrepServiceContext;
  public:
    // /////////////////// Getters //////////////////////
    /**
     * Get the World object (BOM root).
     */
    World* getWorld() const {
      return _world;
    }

    /**
     * Get the World object (BOM root).
     */
    World& getWorldHandler() const;

    /**
     * Get the Xapian database name.
     */
    const TravelDatabaseName_T& getTravelDatabaseName() const {
      return _travelDatabaseName;
    }
    
    /**
     * Get the SOCI Session.
     */
    soci::session* getDBSession() const {
      return _dbSessionManager.getDBSession();
    }      

    /**
     * Get the SOCI Session.
     */
    soci::session& getDBSessionRef() const;


  public:
    // ////////////////// Setters /////////////////////
    /**
     * Set the World object (BOM root).
     */
    void setWorld (World& ioWorld) {
      _world = &ioWorld;
    }

    /**
     * Set the Xapian database name.
     */
    void setTravelDatabaseName (const TravelDatabaseName_T& iTravelDatabaseName) {
      _travelDatabaseName = iTravelDatabaseName;
    }


  public:
    // ///////// Display Methods //////////
    /**
     * Display the short OPENTREP_ServiceContext content.
     */
    const std::string shortDisplay() const;
    
    /**
     * Display the full OPENTREP_ServiceContext content.
     */
    const std::string display() const;

    
  private:
    // /////// Construction / initialisation ////////
    /**
     * Main constructor without database usage.
     */
    OPENTREP_ServiceContext (const TravelDatabaseName_T&);
    /**
     * Main constructor with database usage.
     */
    OPENTREP_ServiceContext (const TravelDatabaseName_T&, const DBParams&);
    /**
     * Default constructor.
     */
    OPENTREP_ServiceContext();
    /**
     * Copy constructor.
     */
    OPENTREP_ServiceContext (const OPENTREP_ServiceContext&);

    /**
     * Destructor.
     */
    ~OPENTREP_ServiceContext();
      

  private:
    // ////////////// Attributes ///////////////
    /**
     * World object (BOM root).
     */
    World* _world;
    
    /**
     * Xapian Database (directory of the index).
     */
    TravelDatabaseName_T _travelDatabaseName;

    /**
     * Database session manager.
     */
    DBSessionManager _dbSessionManager;
  };

}
#endif // __OPENTREP_SVC_OPENTREPSERVICECONTEXT_HPP
