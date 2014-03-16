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
    const TravelDBFilePath_T& getTravelDBFilePath() const {
      return _travelDBFilePath;
    }
    
    /**
     * Get the SQLite3 database filling indicator.
     */
    const FillSQLDB_T& getFillSQLDB() const {
      return _fillSQLDB;
    }
    
    /**
     * Get the SQLite3 database file-path.
     */
    const SQLiteDBFilePath_T& getSQLiteDBFilePath() const {
      return _sqliteDBFilePath;
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
    void setPORFilePath (const std::string& iPORFilePath) {
      _porFilePath = PORFilePath_T (iPORFilePath);
    }

    /**
     * Set the Xapian database name.
     */
    void setTravelDBFilePath (const std::string& iTravelDBFilePath) {
      _travelDBFilePath = TravelDBFilePath_T (iTravelDBFilePath);
    }

    /**
     * Set the SQLite3 database filling indicator.
     */
    void setFillSQLDB (const FillSQLDB_T& iFillSQLDB) {
      _fillSQLDB = iFillSQLDB;
    }
    
    /**
     * Set the SQLite3 database file-path.
     */
    void setSQLiteDBFilePath (const std::string& iSQLiteDBFilePath) {
      _sqliteDBFilePath = SQLiteDBFilePath_T (iSQLiteDBFilePath);
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
     * Main constructor for the search-related services.
     *
     * @param const TravelDBFilePath_T& File-path of the Xapian index/database.
     * @param const FillSQLDB_T& Whether the SQLite3 database should be filled.
     * @param const SQLiteDBFilePath_T& File-path of the SQLite3 database.
     */
    OPENTREP_ServiceContext (const TravelDBFilePath_T&,
                             const FillSQLDB_T&, const SQLiteDBFilePath_T&);

    /**
     * Main constructor for the indexation-related services.
     *
     * @param const PORFilePath_T& File-path of the file of POR
     *                             (points of reference).
     * @param const TravelDBFilePath_T& File-path of the Xapian index/database.
     * @param const FillSQLDB_T& Whether the SQLite3 database should be filled.
     * @param const SQLiteDBFilePath_T& File-path of the SQLite3 database.
     */
    OPENTREP_ServiceContext (const PORFilePath_T&, const TravelDBFilePath_T&,
                             const FillSQLDB_T&, const SQLiteDBFilePath_T&);

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
     * Xapian database (directory of the index).
     */
    TravelDBFilePath_T _travelDBFilePath;

    /**
     * Whether the SQLite3 database should be filled
     * at the same time as the Xapian database/index.
     */
    FillSQLDB_T _fillSQLDB;

    /**
     * Read/write SQLite database file-path.
     *
     * OpenTREP is delivered with a SQLite3 database. That latter is read-only,
     * as it is potentially delivered in a read-only directory on standard
     * platforms (e.g., thanks to RPM on RedHat-derived Linux platforms).
     * A read-write copy is then needed to for local changes (addition,
     * deletion and updates of the POR).
     */
    SQLiteDBFilePath_T _sqliteDBFilePath;

    /**
     * Unicode transliterator.
     */
    OTransliterator _transliterator;
  };

}
#endif // __OPENTREP_SVC_OPENTREPSERVICECONTEXT_HPP
