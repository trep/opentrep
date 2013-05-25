#ifndef __OPENTREP_CMD_DBSESSIONMANAGER_HPP
#define __OPENTREP_CMD_DBSESSIONMANAGER_HPP

// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////

// Forward declarations
namespace soci {
  class session;
}

namespace OPENTREP {
  
  // Forward declarations
  struct DBParams;

  /**
   * @brief Class handling the SOCI session.
   */
  class DBSessionManager {
    friend class OPENTREP_ServiceContext;
  private:
    // ////////////////// Getters ////////////////////
    /**
     * Get the SOCI Session.
     */
    soci::session* getDBSession() const {
      return _dbSession;
    }

    /**
     * Get the SOCI Session.
     */
    soci::session& getDBSessionRef() const;


  private:
    // ////////////////// Constructors and Destructors ////////////////////
    /**
     * Main constructor.
     */
    DBSessionManager (const DBParams&);

    /**
     * Default constructor.
     */
    DBSessionManager();

    /**
     * Copy constructor.
     */
    DBSessionManager (const DBSessionManager&);

    /**
     * Destructor.
     */
    ~DBSessionManager();

  private:
    /**
     * Initialise (SQLite3) database connection.
     */
    void init (const DBParams&);


  private:
    // /////////////////////// Attributes //////////////////////
    /**
     * Database session.
     */
    soci::session* _dbSession;
  };

}
#endif // __OPENTREP_CMD_DBSESSIONMANAGER_HPP
