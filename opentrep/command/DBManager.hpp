#ifndef __OPENTREP_CMD_DBMANAGER_HPP
#define __OPENTREP_CMD_DBMANAGER_HPP

// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// OpenTrep
#include <opentrep/OPENTREP_Types.hpp>
#include <opentrep/LocationKey.hpp>
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
   * @brief Class building the Business Object Model (BOM) from data
   *        retrieved from the database.
   */
  class DBManager {
  public:
    /**
     * Create an empty SQL database.
     *
     * @param const SQLiteDBFilePath_T& File-path of the SQLite3 database.
     * @return soci::session* A pointer on the just created SQLite3 database
     *                        connection.
     */
    static soci::session* buildSQLDB (const SQLiteDBFilePath_T&);

    /**
     * Get all the rows of the SQLite3 database.
     *
     * @param const SQLiteDBFilePath_T& File-path of the SQLite3 database.
     * @return NbOfDBEntries_T Number of documents of the SQLite3 DB.
     */
    static NbOfDBEntries_T getAll (const SQLiteDBFilePath_T&);

    /**
     * Insert into the SQLite3 database the document
     * corresponding to the given Place object.
     *
     * @param soci::session& SOCI session handler.
     * @param const Place& The place to be inserted.
     * @param const std::string& The serialised version of that place.
     */
    static void insertPlaceInDB (soci::session&, const Place&,
                                 const std::string& iSerialisedPlaceStr);

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
     * @param std::string& The serialised place to be retrieved.
     *                     It has to be given empty, and is filled by the method.
     * @return bool Whether or not there are more rows to be fetched.
     */
    static bool iterateOnStatement (soci::statement&, std::string&);

    
  private:
    /**
     * Prepare (parse and put in cache) the SQL statement.
     *
     * @param soci::session& SOCI session handler.
     * @param soci::statement& SOCI SQL statement handler.
     * @param const IATACode_T& The IATA code of the place to be retrieved.
     * @return std::string The serialised place to be retrieved.
     */
    static std::string prepareSelectBlobOnPlaceCodeStatement (soci::session&,
                                                              soci::statement&,
                                                              const IATACode_T&);
    

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
