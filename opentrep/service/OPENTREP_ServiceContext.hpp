#ifndef __OPENTREP_SVC_OPENTREPSERVICECONTEXT_HPP
#define __OPENTREP_SVC_OPENTREPSERVICECONTEXT_HPP

// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// STL
#include <string>
// OpenTrep
#include <opentrep/OPENTREP_Types.hpp>
#include <opentrep/service/ServiceAbstract.hpp>

// Forward declarations
namespace soci {
  class session;
}

namespace OPENTREP {

  // Forward declarations
  class World;
  
  /** Class holding the context of the OpenTrep services. */
  class OPENTREP_ServiceContext : public ServiceAbstract {
    friend class FacOpenTrepServiceContext;
  public:
    // ///////// Getters //////////
    /** Get the SOCI Session. */
    soci::session* getSociSession () const {
      return _sociSession;
    }

    /** Get the World object (BOM root). */
    World* getWorld () const {
      return _world;
    }

    /** Get the Xapian database name. */
    const TravelDatabaseName_T& getTravelDatabaseName () const {
      return _travelDatabaseName;
    }
    
    /** Get the SOCI Session. */
    soci::session& getSociSessionHandler () const;

    /** Get the World object (BOM root). */
    World& getWorldHandler () const;

    
    // ///////// Setters //////////
    /** Set the SOCI Session. */
    void setSociSession (soci::session& ioSociSession) {
      _sociSession = &ioSociSession;
    }

    /** Set the World object (BOM root). */
    void setWorld (World& ioWorld) {
      _world = &ioWorld;
    }

    /** Set the Xapian database name. */
    void setTravelDatabaseName (const TravelDatabaseName_T& iTravelDatabaseName) {
      _travelDatabaseName = iTravelDatabaseName;
    }


    // ///////// Display Methods //////////
    /** Display the short OPENTREP_ServiceContext content. */
    const std::string shortDisplay() const;
    
    /** Display the full OPENTREP_ServiceContext content. */
    const std::string display() const;

    
  private:
    // /////// Construction / initialisation ////////
    /** Constructors. */
    OPENTREP_ServiceContext ();
    OPENTREP_ServiceContext (const TravelDatabaseName_T& iTravelDatabaseName);
    OPENTREP_ServiceContext (const OPENTREP_ServiceContext&);

    /** Destructor. */
    ~OPENTREP_ServiceContext();
      
  private:
    /** SOCI session. */
    soci::session* _sociSession;

    /** World object (BOM root). */
    World* _world;
    
    /** Xapian Database (directory of the index). */
    TravelDatabaseName_T _travelDatabaseName;
  };

}
#endif // __OPENTREP_SVC_OPENTREPSERVICECONTEXT_HPP
