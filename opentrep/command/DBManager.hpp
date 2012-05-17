#ifndef __OPENTREP_CMD_DBMANAGER_HPP
#define __OPENTREP_CMD_DBMANAGER_HPP

// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// OpenTrep
#include <opentrep/OPENTREP_Types.hpp>
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
     * Update the Xapian document ID field of the database row
     * corresponding to the given Place object.
     *
     * @param soci::session& SOCI session handler.
     * @param const Place& The place to be updated.
     */
    static void updatePlaceInDB (soci::session&, const Place&);

    /**
     * Retrieve, from the (MySQL) database, the row corresponding to
     * the given place code (e.g., 'sfo' for San Francisco Intl
     * airport), and fill the given Place object with that retrieved
     * data.
     *
     * @param soci::session& SOCI session handler.
     * @param const PlaceKey& The primary key of the place to be retrieved.
     * @param Place& The object corresponding to the place to be retrieved.
     *               It has to be given empty, and is filled by the method.
     */
    static bool retrievePlace (soci::session&, const PlaceKey&, Place&);

    /**
     * Retrieve, from the (MySQL) database, the row corresponding to
     * the given place code (e.g., 'sfo' for San Francisco Intl
     * airport), and fill the given Place object with that retrieved
     * data.
     *
     * @param soci::session& SOCI session handler.
     * @param const std::double& The latitude of the place to be retrieved.
     * @param const std::double& The longitude of the place to be retrieved.
     * @param PlaceOrderedList_T& The list corresponding to the places to be
     *        retrieved. It has to be given empty, and is filled by the
     *        method.
     */
    static bool retrieveClosestPlaces (soci::session&, const double& iLatitude,
                                       const double& iLongitude,
                                       PlaceOrderedList_T&);

    
  public:
    /**
     * Prepare (parse and put in cache) the SQL statement.
     *
     * @param soci::session& SOCI session handler.
     * @param soci::statement& SOCI SQL statement handler.
     * @param Place& The object corresponding to the place to be retrieved.
     *        It has to be given empty, and is filled by the method.
     */
    static void prepareSelectFromCodeStatement (soci::session&,
                                                soci::statement&, Place&);

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
     * @param Place& The object corresponding to the place to be retrieved.
     *               It has to be given empty, and is filled by the method.
     * @param const bool Tells whether the Place object should be reset.
     */
    static bool iterateOnStatement (soci::statement&, Place&,
                                    const bool iShouldDoReset);

    
  private:
    /**
     * Prepare (parse and put in cache) the SQL statement.
     *
     * @param soci::session& SOCI session handler.
     * @param soci::statement& SOCI SQL statement handler.
     * @param const std::string& The IATA code of the place to be retrieved.
     * @param const std::string& The ICAO code of the place to be retrieved.
     * @param const std::string& The Geonames ID of the place to be retrieved.
     * @param Place& The object corresponding to the place to be retrieved.
     *               It has to be given empty, and is filled by the method.
     */
    static void prepareSelectOnPlaceCodeStatement(soci::session&,
                                                  soci::statement&,
                                                  const std::string& iPlaceCode,
                                                  const std::string& iIcaoCode,
                                                  const GeonamesID_T&, Place&);
    
    /**
     * Prepare (parse and put in cache) the SQL statement.
     *
     * @param soci::session& SOCI session handler.
     * @param soci::statement& SOCI SQL statement handler.
     * @param const XapianDocID_T& The code of the place to be retrieved.
     * @param Place& The object corresponding to the place to be retrieved.
     *               It has to be given empty, and is filled by the method.
     */
    static void prepareSelectOnDocIDStatement (soci::session&, soci::statement&,
                                               const XapianDocID_T&, Place&);

    
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
