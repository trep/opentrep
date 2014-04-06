#ifndef __OPENTREP_CMD_DBMANAGER_HPP
#define __OPENTREP_CMD_DBMANAGER_HPP

// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// OpenTrep
#include <opentrep/OPENTREP_Types.hpp>
#include <opentrep/Location.hpp>
#include <opentrep/DBType.hpp>
#include <opentrep/bom/PlaceList.hpp>

// Forward declarations
namespace soci {
  class session;
  class statement;
}

namespace OPENTREP {

  // Forward declarations
  struct PlaceKey;


  /**
   * @brief Class wrapping the access to an underlying SQL database.
   *
   * The SQL database may be one of SQLite3, MySQL, PostgreSQL.
   * That latter is not supported yet. However, patches are welcome on
   * http://github.com/trep/opentrep/issues.
   */
  class DBManager {
  public:
    /**
     * Create a SQL database.
     *
     * <ul>
     *  <li>With SQLite3, a database is given by just a file-path. If the SQLite3
     * database file was previously existing, it is deleted and re-created,
     * empty.</li>
     *  <li>With MySQL, a database server needs to run and being accessible
     *      thanks to details, including credentials, you know.
     *      On that database, the <tt>rfd</tt> user must have been created
     *      before hand. You can use the opentrep_create_db_user.sh helper
     *      script (in the <tt>bin</tt> of the OpenTREP installation directory)
     *      for that.</li>
     * </ul>
     *
     * @param const DBType& The SQL database type (e.g., SQLite3, MySQL).
     * @param const SQLDBConnectionString_T& Connection string for the SQL
     *                                       database.
     * @return soci::session* A pointer on the just created SQL database
     *                        connection.
     */
    static soci::session* initSQLDBSession (const DBType&,
                                            const SQLDBConnectionString_T&);

    /**
     * On MySQL, create the 'trep' database user and 'trep_trep' database.
     * On other database types (e.g., nosql, sqlite), that method has no effect.
     *
     * If tables were previously existing, they are deleted and re-created.
     *
     * @param soci::session& A reference on the SQL database session.
     */
    static void createSQLDBUser (soci::session&);

    /**
     * Create the database tables (e.g., 'ori_por' table).
     *
     * If tables were previously existing, they are deleted and re-created.
     *
     * @param soci::session& A reference on the SQL database session.
     */
    static void createSQLDBTables (soci::session&);

    /**
     * Create the database indexes.
     *
     * @param soci::session& A reference on the SQLite3 database session.
     */
    static void createSQLDBIndexes (soci::session&);

    /**
     * Dump all the POR (points of reference) of the SQL database.
     *
     * @param const SQLDBConnectionString_T& Connection string to the SQL DB.
     * @return NbOfDBEntries_T Number of documents of the SQL database.
     */
    static NbOfDBEntries_T displayAll (soci::session&);

    /**
     * Get the POR (point of reference), from the SQL database, corresponding
     * to the given IATA code.
     *
     * @param const SQLDBConnectionString_T& Connection string to the SQL DB.
     * @param const IATACode_T& The IATA code (key) of the POR to be retrieved.
     * @param LocationList_T& List of (geographical) locations, if any,
     *                        matching the given key.
     * @return NbOfDBEntries_T Number of documents of the SQL database.
     */
    static NbOfDBEntries_T getPORByIATACode (soci::session&, const IATACode_T&,
                                             LocationList_T&);

    /**
     * Get the POR (point of reference), from the SQL database, corresponding
     * to the given ICAO code.
     *
     * @param const SQLDBConnectionString_T& Connection string to the SQL DB.
     * @param const ICAOCode_T& The ICAO code (key) of the POR to be retrieved.
     * @param LocationList_T& List of (geographical) locations, if any,
     *                        matching the given key.
     * @return NbOfDBEntries_T Number of documents of the SQL database.
     */
    static NbOfDBEntries_T getPORByICAOCode (soci::session&, const ICAOCode_T&,
                                             LocationList_T&);

    /**
     * Get the POR (point of reference), from the SQL database, corresponding
     * to the given FAA code.
     *
     * @param const SQLDBConnectionString_T& Connection string to the SQL DB.
     * @param const FAACode_T& The FAA code (key) of the POR to be retrieved.
     * @param LocationList_T& List of (geographical) locations, if any,
     *                        matching the given key.
     * @return NbOfDBEntries_T Number of documents of the SQL database.
     */
    static NbOfDBEntries_T getPORByFAACode (soci::session&, const FAACode_T&,
                                            LocationList_T&);

    /**
     * Get the POR (point of reference), from the SQL database, corresponding
     * to the given IATA code.
     *
     * @param const SQLDBConnectionString_T& Connection string to the SQL DB.
     * @param const GeonamesID_T& The GeonameID (key) of the POR to be retrieved.
     * @param LocationList_T& List of (geographical) locations, if any,
     *                        matching the given key.
     * @return NbOfDBEntries_T Number of documents of the SQL database.
     */
    static NbOfDBEntries_T getPORByGeonameID (soci::session&,
                                              const GeonamesID_T&,
                                              LocationList_T&);

    /**
     * Insert into the SQL database the document
     * corresponding to the given Place object.
     *
     * @param soci::session& SOCI session handler.
     * @param const Place& The place to be inserted.
     */
    static void insertPlaceInDB (soci::session&, const Place&);

    /**
     * Update the Xapian document ID field of the database row
     * corresponding to the given Place object.
     *
     * @param soci::session& SOCI session handler.
     * @param const Place& The place to be updated.
     */
    static void updatePlaceInDB (soci::session&, const Place&);

    
  public:
    /**
     * Prepare (parse and put in cache) the SQL statement.
     *
     * @param soci::session& SOCI session handler.
     * @param soci::statement& SOCI SQL statement handler.
     * @return std::string The serialised place to be retrieved.
     */
    static std::string prepareSelectAllBlobStatement (soci::session&,
                                                      soci::statement&);

    /**
     * Prepare (parse and put in cache) the SQL statement.
     *
     * @param soci::session& SOCI session handler.
     * @param soci::statement& SOCI SQL statement handler.
     * @param Place& The object corresponding to the place to be retrieved.
     *        It has to be given empty, and is filled by the method.
     */
    static void prepareSelectFromCoordStatement (soci::session&,
                                                 soci::statement&,
                                                 const double& iLatitude,
                                                 const double& iLongitude);

    /**
     * Iterate on the SQL statement.
     *
     * The SQL has to be already prepared.
     *
     * @param soci::statement& SOCI SQL statement handler.
     * @param const std::string& The current serialised place,
     *                           for debugging purpose.
     * @return bool Whether or not there are more rows to be fetched.
     */
    static bool iterateOnStatement (soci::statement&, const std::string&);

    
  private:
    /**
     * Prepare (parse and put in cache) the SQL statement.
     *
     * @param soci::session& SOCI session handler.
     * @param soci::statement& SOCI SQL statement handler.
     * @param const IATACode_T& The IATA code of the place to be retrieved.
     * @param std::string& The serialised place to be retrieved.
     */
    static void
    prepareSelectBlobOnIataCodeStatement (soci::session&, soci::statement&,
                                          const IATACode_T&,
                                          std::string& ioSerialisedPlaceStr);
    /**
     * Prepare (parse and put in cache) the SQL statement.
     *
     * @param soci::session& SOCI session handler.
     * @param soci::statement& SOCI SQL statement handler.
     * @param const ICAOCode_T& The ICAO code of the place to be retrieved.
     * @param std::string& The serialised place to be retrieved.
     */
    static void
    prepareSelectBlobOnIcaoCodeStatement (soci::session&, soci::statement&,
                                          const ICAOCode_T&,
                                          std::string& ioSerialisedPlaceStr);
    /**
     * Prepare (parse and put in cache) the SQL statement.
     *
     * @param soci::session& SOCI session handler.
     * @param soci::statement& SOCI SQL statement handler.
     * @param const FAACode_T& The FAA code of the place to be retrieved.
     * @param std::string& The serialised place to be retrieved.
     */
    static void
    prepareSelectBlobOnFaaCodeStatement (soci::session&, soci::statement&,
                                         const FAACode_T&,
                                         std::string& ioSerialisedPlaceStr);
    /**
     * Prepare (parse and put in cache) the SQL statement.
     *
     * @param soci::session& SOCI session handler.
     * @param soci::statement& SOCI SQL statement handler.
     * @param const GeonamesID_T& The Geoname ID of the place to be retrieved.
     * @param std::string& The serialised place to be retrieved.
     */
    static void
    prepareSelectBlobOnPlaceGeoIDStatement (soci::session&, soci::statement&,
                                            const GeonamesID_T&,
                                            std::string& ioSerialisedPlaceStr);


  private:
    /**
     * Default constructor.
     */
    DBManager() {}
    /**
     * Default copy constructor.
     */
    DBManager (const DBManager&) {}
    /**
     * Destructor.
     */
    ~DBManager() {}
  };
  
}
#endif // __OPENTREP_CMD_DBMANAGER_HPP
