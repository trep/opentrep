#ifndef __OPENTREP_SVC_OPENTREPSERVICECONTEXT_HPP
#define __OPENTREP_SVC_OPENTREPSERVICECONTEXT_HPP

// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// STL
#include <string>
// OpenTrep
#include <opentrep/OPENTREP_Types.hpp>
#include <opentrep/basic/OTransliterator.hpp>
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
     * Get the file-path of the file of POR (points of reference).
     */
    const PORFilePath_T& getPORFilePath() const {
      return _porFilePath;
    }

    /**
     * Get the Xapian database name.
     */
    const TravelDatabaseName_T& getTravelDatabaseName() const {
      return _travelDatabaseName;
    }
    
    /**
     * Get the Unicode transliterator.
     */
    const OTransliterator& getTransliterator() const {
      return _transliterator;
    }

  public:
    // ////////////////// Setters /////////////////////
    /**
     * Set the World object (BOM root).
     */
    void setWorld (World& ioWorld) {
      _world = &ioWorld;
    }

    /**
     * Set the file-path of the file of POR (points of reference).
     */
    void setPORFilePath (const PORFilePath_T& iPORFilePath) {
      _porFilePath = iPORFilePath;
    }

    /**
     * Set the Xapian database name.
     */
    void setTravelDatabaseName (const TravelDatabaseName_T& iTravelDBName) {
      _travelDatabaseName = iTravelDBName;
    }

    /**
     * Set the Unicode transliterator.
     */
    void setTransliterator (const OTransliterator& iTransliterator) {
      _transliterator = iTransliterator;
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
    OPENTREP_ServiceContext (const PORFilePath_T&, const TravelDatabaseName_T&);
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
     * File-path of the file of POR (points of reference).
     */
    PORFilePath_T _porFilePath;

    /**
     * Xapian Database (directory of the index).
     */
    TravelDatabaseName_T _travelDatabaseName;

    /**
     * Unicode transliterator.
     */
    OTransliterator _transliterator;
  };

}
#endif // __OPENTREP_SVC_OPENTREPSERVICECONTEXT_HPP
