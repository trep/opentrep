#ifndef __OPENTREP_SVC_OPENTREPSERVICECONTEXT_HPP
#define __OPENTREP_SVC_OPENTREPSERVICECONTEXT_HPP

// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// STL
#include <string>
// OpenTrep
#include <opentrep/OPENTREP_Types.hpp>
#include <opentrep/DBType.hpp>
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
     * Get the SQL database type.
     */
    const DBType& getSQLDBType() const {
      return _sqlDBType;
    }
    
    /**
     * Get the SQL database connection string.
     */
    const SQLDBConnectionString_T& getSQLDBConnectionString() const {
      return _sqlDBConnectionString;
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
     * Set the SQL database type.
     */
    void setSQLDBType (const DBType& iDBType) {
      _sqlDBType = iDBType;
    }
    
    /**
     * Set the SQL database connection string.
     */
    void setSQLDBConnectionString(const std::string& iSQLDBConnStr) {
      _sqlDBConnectionString = SQLDBConnectionString_T (iSQLDBConnStr);
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
     * Main constructor for the query-/search-related services.
     *
     * @param const TravelDBFilePath_T& File-path of the Xapian index/database.
     * @param const DBType& SQL database type (can be no database at all).
     * @param const SQLDBConnectionString_T& SQL DB connection string.
     */
    OPENTREP_ServiceContext (const TravelDBFilePath_T&,
                             const DBType&, const SQLDBConnectionString_T&);

    /**
     * Main constructor for the indexing-related services.
     *
     * @param const PORFilePath_T& File-path of the file of POR
     *                             (points of reference).
     * @param const TravelDBFilePath_T& File-path of the Xapian index/database.
     * @param const DBType& SQL database type (can be no database at all).
     * @param const SQLDBConnectionString_T& SQL DB connection string.
     * @param const shouldIndexNonIATAPOR_T& Whether to include non-IATA POR
     */
    OPENTREP_ServiceContext (const PORFilePath_T&, const TravelDBFilePath_T&,
                             const DBType&, const SQLDBConnectionString_T&,
                             const shouldIndexNonIATAPOR_T&);

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
    virtual ~OPENTREP_ServiceContext();
      

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
     * Type of the SQL database (NODB means that no SQL database should be used).
     */
    DBType _sqlDBType;

    /**
     * Connection string for the SQL database.
     *
     * For now, there is a choice among:
     * <ul>
     *  <li>No SQL database</li>
     *  <li>SQLite3 database. The connection string then consists
     *      of the corresponding file-path</li>
     *  <li>MySQL/MariaDB database. The connection string then consists
     *      of the database parameters (db name, db user, db password)</li>
     * </ul>
     */
    SQLDBConnectionString_T _sqlDBConnectionString;

    /**
     * Whether or not the non-IATA-referenced POR should be included
     * (and indexed).
     *
     * By default, and historically, only the POR, which are referenced
     * by IATA (ie, which have a specific IATA code) are indexed (and may
     * be searched for) in OpenTREP.
     *
     * POR are also referenced by other international organizations,
     * such as ICAO or UN/LOCODE, and may not be referenced by IATA
     * (in which case their IATA code is left empty).
     *
     * As of August 2018, there are around 110,000 POR in OpenTravelData (OPTD),
     * the reference data source for OpenTREP:
     * <ul>
     *  <li>Around 20,000 POR are referenced by IATA</li>
     *  <li>Around 90,000 POR are not referenced by IATA, but referenced
     *      by other international organizations (eg, ICAO, UN/LOCODE)</li>
     * </ul>
     *
     * Indexing 20,000 POR takes already a few minutes on standard hardware.
     * Indexing 110,000 POR would take 15 to 20 minutes.
     *
     * Once indexed, all those POR become searchable. That flag is therefore
     * only used at indexing time.
     */
    shouldIndexNonIATAPOR_T _shouldIndexNonIATAPOR;

    /**
     * Unicode transliterator.
     */
    OTransliterator _transliterator;
  };

}
#endif // __OPENTREP_SVC_OPENTREPSERVICECONTEXT_HPP
