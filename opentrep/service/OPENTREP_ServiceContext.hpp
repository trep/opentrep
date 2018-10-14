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
     * Get the number/version of the current deployment.
     */
    const DeploymentNumber_T& getDeploymentNumber() const {
      return _deploymentNumber;
    }
    
    /**
     * Get the flag stating whether or not all the POR should be indexed.
     */
    const shouldIndexNonIATAPOR_T& getShouldIncludeAllPORFlag() const {
      return _shouldIndexNonIATAPOR;
    }
    
    /**
     * Get the flag stating whether or not to add the POR in the Xapian index.
     */
    const shouldIndexPORInXapian_T& getShouldIndexPORInXapianFlag() const {
      return _shouldIndexPORInXapian;
    }
    
    /**
     * Get the flag stating whether or not to add the POR in the SQL database.
     */
    const shouldAddPORInSQLDB_T& getShouldAddPORInSQLDB() const {
      return _shouldAddPORInSQLDB;
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
      _travelDBFilePathPrefix = TravelDBFilePath_T (iTravelDBFilePath);
      updateXapianAndSQLDBConnectionWithDeploymentNumber();
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
    void setSQLDBConnectionString (const std::string& iSQLDBConnStr) {
      _sqlDBConnectionStringWPfxDBName = SQLDBConnectionString_T (iSQLDBConnStr);
      updateXapianAndSQLDBConnectionWithDeploymentNumber();
    }
    
    /**
     * Set the number/version of the current deployment.
     */
    void setDeploymentNumber (const DeploymentNumber_T& iDeploymentNumber) {
      _deploymentNumber = iDeploymentNumber;
      updateXapianAndSQLDBConnectionWithDeploymentNumber();
    }
    
    /**
     * Set the flag stating whether or not all the POR should be indexed.
     */
    void setShouldIncludeAllPORFlag (const shouldIndexNonIATAPOR_T& iShouldIndexNonIATAPOR) {
      _shouldIndexNonIATAPOR = iShouldIndexNonIATAPOR;
    }
    
    /**
     * Set the flag stating whether or not to add the POR in the Xapian index.
     */
    void setShouldIndexPORInXapianFlag (const shouldIndexPORInXapian_T& iShouldIndexPORInXapian) {
      _shouldIndexPORInXapian = iShouldIndexPORInXapian;
    }
    
    /**
     * Set the flag stating whether or not to add the POR in the SQL database.
     */
    void setShouldAddPORInSQLDB (const shouldAddPORInSQLDB_T& iShouldAddPORInSQLDB) {
      _shouldAddPORInSQLDB = iShouldAddPORInSQLDB;
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
     * @param const DeploymentNumber_T& Deployment number.
     */
    OPENTREP_ServiceContext (const TravelDBFilePath_T&,
                             const DBType&, const SQLDBConnectionString_T&,
                             const DeploymentNumber_T&);

    /**
     * Main constructor for the indexing-related services.
     *
     * @param const PORFilePath_T& File-path of the file of POR
     *                             (points of reference).
     * @param const TravelDBFilePath_T& File-path of the Xapian index/database.
     * @param const DBType& SQL database type (can be no database at all).
     * @param const SQLDBConnectionString_T& SQL DB connection string.
     * @param const DeploymentNumber_T& Deployment number.
     * @param const shouldIndexNonIATAPOR_T& Whether to include non-IATA POR.
     * @param const shouldIndexPORInXapian_T& Whether to index POR in Xapian.
     * @param const shouldAddPORInSQLDB_T& Whether to add POR in SQL database.
     */
    OPENTREP_ServiceContext (const PORFilePath_T&, const TravelDBFilePath_T&,
                             const DBType&, const SQLDBConnectionString_T&,
                             const DeploymentNumber_T&,
                             const shouldIndexNonIATAPOR_T&,
                             const shouldIndexPORInXapian_T&,
                             const shouldAddPORInSQLDB_T&);

    /**
     * The Xapian index/database file-path and SQL database name are used
     * as prefix of the actual respective file-path and name. The deployment
     * number/version is added to the prefix, and that makes the actual
     * file-path and name.
     *
     * For instance, if the current deployment number/version is 0, and:
     * <ul>
     *  <li>Xapian file-path (prefix) is '/tmp/opentrep/xapian_traveldb',
     *      the actual Xapian file-path become
     *      '/tmp/opentrep/xapian_traveldb0';</li>
     *  <li>MySQL/MariaDB connection string is
     *      'db=trep_trep user=trep password=trep', then the actual connection
     *      string becomes 'db=trep_trep0 user=trep password=trep'</li>
     *  <li>SQLite connection string is '/tmp/opentrep/sqlite_travel.db',
     *      then the actual connection string becomes
     *      '/tmp/opentrep/sqlite_travel.db0'</li>
     * </ul>
     */
    void updateXapianAndSQLDBConnectionWithDeploymentNumber();
    
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
     * Number/version of the current deployment.
     *
     * The idea is to have at least two pieces of infrastructure (SQL
     * database, Xapian index) in parallel:
     * <ul>
     *   <li>one is used by the production;</li>
     *   <li>the other one used as a staging platform in order to test and
     * validate a new version.</li>
     *   <li>Once the new version has been validated, the two pieces
     *       of infrastructure can then be interverted, ie, the production
     *       becomes the new version, and the older version ends up in
     *       staging.</li>
     *   <li>It means that all programs have to choose which version they
     *       want to work on. That version may even be toggled in live.</li>
     *   <li>That method to deploy in production through a staging process
     *       is even more needed by the fact that indexing a new POR data file
     *       takes up to 30 minutes in the worst case. So, we cannot afford
     *       30-45 minutes of downtime everytime a new POR data file is
     *       released (potentially every day).</li>
     *   <li>With that staging process, it is even possible to fully automate
     *       the re-indexing after a new POR data file release:
     *       once the new release has been cleared by QA on staging,
     *       it becomes production.</li>
     * </ul>
     */
    DeploymentNumber_T _deploymentNumber;
  
    /**
     * Prefix (directory of) Xapian index/database.
     */
    TravelDBFilePath_T _travelDBFilePathPrefix;

    /**
     * Actual (directory of) Xapian index/database.
     */
    TravelDBFilePath_T _travelDBFilePath;

    /**
     * Type of the SQL database (NODB means that no SQL database should be used).
     */
    DBType _sqlDBType;

    /**
     * Connection string for the SQL database, with a prefix
     * for the database name.
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
    SQLDBConnectionString_T _sqlDBConnectionStringWPfxDBName;

    /**
     * Connection string for the SQL database, with the actual
     * database name.
     *
     * Examples of how the actual (SQLite) file-path or (MySQL/MariaDB) database
     * name is derived from the prefix file-path or database name.
     * <ul>
     *  <li>MySQL/MariaDB connection string is
     *      'db=trep_trep user=trep password=trep', then the actual connection
     *      string becomes 'db=trep_trep0 user=trep password=trep'</li>
     *  <li>SQLite connection string is '/tmp/opentrep/sqlite_travel.db',
     *      then the actual connection string becomes
     *      '/tmp/opentrep/sqlite_travel.db0'</li>
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
     * Whether or not to add the POR, or list of POR, in the Xapian index.
     */
    shouldIndexPORInXapian_T _shouldIndexPORInXapian;

    /**
     * Whether or not to add the POR, or list of POR, in the SQL database.
     */
    shouldAddPORInSQLDB_T _shouldAddPORInSQLDB;

    /**
     * Unicode transliterator.
     */
    OTransliterator _transliterator;
  };

}
#endif // __OPENTREP_SVC_OPENTREPSERVICECONTEXT_HPP
