// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// STL
#include <cassert>
#include <sstream>
// SOCI
#if defined(SOCI_HEADERS_BURIED)
#include <soci/core/soci.h>
#include <soci/backends/mysql/soci-mysql.h>
#else // SOCI_HEADERS_BURIED
#include <soci/soci.h>
#include <soci/mysql/soci-mysql.h>
#endif // SOCI_HEADERS_BURIED
// OpenTrep
#include <opentrep/bom/World.hpp>
#include <opentrep/bom/Place.hpp>
#include <opentrep/factory/FacPlace.hpp>
#include <opentrep/dbadaptor/DbaPlace.hpp>
#include <opentrep/command/DBManager.hpp>
#include <opentrep/service/Logger.hpp>

namespace OPENTREP {

  // //////////////////////////////////////////////////////////////////////
  void DBManager::
  prepareSelectFromCodeStatement (soci::session& ioSociSession,
                                  soci::statement& ioSelectStatement,
                                  Place& ioPlace) {
  
    try {
    
      // Instanciate a SQL statement (no request is performed at that stage)
      /**
         select rpd.code AS code, city_code, xapian_docid, is_airport, is_city,
         is_main, is_commercial, state_code, country_code, region_code,
         continent_code, time_zone_grp, longitude, latitude, language_code,
         classical_name, extended_name, alternate_name1, alternate_name2,
         alternate_name3, alternate_name4, alternate_name5, alternate_name6,
         alternate_name7, alternate_name8, alternate_name9, alternate_name10
         from ref_place_details rpd, ref_place_names rpn
         where rpd.code = rpn.code
      */

      ioSelectStatement =
        (ioSociSession.prepare
         << "select rpd.code AS code, city_code, xapian_docid, is_airport, "
         << "is_city, is_main, is_commercial, state_code, country_code, "
         << "region_code, continent_code, time_zone_grp, longitude, latitude, "
         << "language_code, classical_name, extended_name, "
         << "alternate_name1, alternate_name2, alternate_name3, "
         << "alternate_name4, alternate_name5, alternate_name6, "
         << "alternate_name7, alternate_name8, alternate_name9, "
         << "alternate_name10 "
         << "from ref_place_details rpd, ref_place_names rpn "
         << "where rpd.code = rpn.code", soci::into (ioPlace));

      // Execute the SQL query
      ioSelectStatement.execute();

    } catch (std::exception const& lException) {
      std::ostringstream errorStr;
      errorStr << "Error in the 'select * from ref_place_details' SQL request: "
               << lException.what();
      OPENTREP_LOG_ERROR (errorStr.str());
      throw SQLDatabaseException (errorStr.str());
    }
  }

  // //////////////////////////////////////////////////////////////////////
  void DBManager::
  prepareSelectFromCoordStatement (soci::session& ioSociSession,
                                   soci::statement& ioSelectStatement,
                                   const double& iLatitude,
                                   const double& iLongitude) {
  
    try {
      
      // Instanciate a SQL statement (no request is performed at that stage)
      /**
         select rpd.code AS code, city_code, xapian_docid, is_airport, is_city,
         is_main, is_commercial, state_code, country_code, region_code,
         continent_code, time_zone_grp, longitude, latitude, language_code,
         classical_name, extended_name, alternate_name1, alternate_name2,
         alternate_name3, alternate_name4, alternate_name5, alternate_name6,
         alternate_name7, alternate_name8, alternate_name9, alternate_name10
         from ref_place_details rpd, ref_place_names rpn
         where rpd.code = iPlaceCode
           and rpn.code = rpd.code;

        select (airpop.tpax)/1000 as 'popularity', 
		places.code as 'airport_code', places.code as 'city_code', 
		places.longitude as 'longitude', places.latitude  as 'latitude'
        from airport_popularity AS airpop, ref_place_details AS places
        WHERE places.longitude >= ${PL_LON_LOWER}
        AND places.longitude <= ${PL_LON_UPPER}
        AND places.latitude >= ${PL_LAT_LOWER}
        AND places.latitude <= ${PL_LAT_UPPER}
        AND airpop.airport_code = places.code
        AND places.is_city = 'y'
        AND names.code = places.code
        ORDER BY airpop.tpax DESC
      */
      Place& lPlace = FacPlace::instance().create();
      const double K_ERROR = 2.0;
      const double lLowerBoundLatitude = iLatitude - K_ERROR;
      const double lUpperBoundLatitude = iLatitude + K_ERROR;
      const double lLowerBoundLongitude = iLongitude - K_ERROR;
      const double lUpperBoundLongitude = iLongitude + K_ERROR;
      
      ioSelectStatement =
        (ioSociSession.prepare
         << "select rpd.code AS code, city_code, xapian_docid, is_airport, "
         << "is_city, is_main, is_commercial, state_code, country_code, "
         << "region_code, continent_code, time_zone_grp, longitude, latitude, "
         << "language_code, classical_name, extended_name, "
         << "alternate_name1, alternate_name2, alternate_name3, "
         << "alternate_name4, alternate_name5, alternate_name6, "
         << "alternate_name7, alternate_name8, alternate_name9, "
         << "alternate_name10 "
         << "from ref_place_details rpd, ref_place_names rpn, "
         << "     popularity pop "
         << "where latitude >= :lower_latitude "
         << "  and latitude <= :upper_latitude "
         << "  and longitude >= :lower_longitude "
         << "  and longitude <= :upper_longitude "
         << "  and rpn.code = rpd.code"
         << "  and pop.airport_code = rpd.code",
         soci::into (lPlace), soci::use (lLowerBoundLatitude),
         soci::use (lUpperBoundLatitude), soci::use (lLowerBoundLongitude),
         soci::use (lUpperBoundLongitude));

      // Execute the SQL query
      ioSelectStatement.execute();

    } catch (std::exception const& lException) {
      std::ostringstream errorStr;
      errorStr << "Error in the 'select * from ref_place_details' SQL request: "
               << lException.what();
      OPENTREP_LOG_ERROR (errorStr.str());
      throw SQLDatabaseException (errorStr.str());
    }
  }
  
  // //////////////////////////////////////////////////////////////////////
  void DBManager::
  prepareSelectOnPlaceCodeStatement (soci::session& ioSociSession,
                                     soci::statement& ioSelectStatement,
                                     const std::string& iPlaceCode,
                                     Place& ioPlace) {
  
    try {
    
      // Instanciate a SQL statement (no request is performed at that stage)
      /**
         select rpd.code AS code, city_code, xapian_docid, is_airport, is_city,
         is_main, is_commercial, state_code, country_code, region_code,
         continent_code, time_zone_grp, longitude, latitude, language_code,
         classical_name, extended_name, alternate_name1, alternate_name2,
         alternate_name3, alternate_name4, alternate_name5, alternate_name6,
         alternate_name7, alternate_name8, alternate_name9, alternate_name10
         from ref_place_details rpd, ref_place_names rpn
         where rpd.code = iPlaceCode
           and rpn.code = rpd.code;
      */

      ioSelectStatement =
        (ioSociSession.prepare
         << "select rpd.code AS code, city_code, xapian_docid, is_airport, "
         << "is_city, is_main, is_commercial, state_code, country_code, "
         << "region_code, continent_code, time_zone_grp, longitude, latitude, "
         << "language_code, classical_name, extended_name, "
         << "alternate_name1, alternate_name2, alternate_name3, "
         << "alternate_name4, alternate_name5, alternate_name6, "
         << "alternate_name7, alternate_name8, alternate_name9, "
         << "alternate_name10 "
         << "from ref_place_details rpd, ref_place_names rpn "
         << "where rpd.code = :place_code "
         << "and rpn.code = rpd.code",
         soci::into (ioPlace), soci::use (iPlaceCode));

      // Execute the SQL query
      ioSelectStatement.execute();

    } catch (std::exception const& lException) {
      std::ostringstream errorStr;
      errorStr << "Error in the 'select * from ref_place_details' SQL request: "
               << lException.what();
      OPENTREP_LOG_ERROR (errorStr.str());
      throw SQLDatabaseException (errorStr.str());
    }
  }

  // //////////////////////////////////////////////////////////////////////
  void DBManager::
  prepareSelectOnDocIDStatement (soci::session& ioSociSession,
                                 soci::statement& ioSelectStatement,
                                 const XapianDocID_T& iDocID,
                                 Place& ioPlace) {
  
    try {
    
      // Instanciate a SQL statement (no request is performed at that stage)
      /**
         select rpd.code AS code, city_code, xapian_docid, is_airport, is_city,
         is_main, is_commercial, state_code, country_code, region_code,
         continent_code, time_zone_grp, longitude, latitude, language_code,
         classical_name, extended_name, alternate_name1, alternate_name2,
         alternate_name3, alternate_name4, alternate_name5, alternate_name6,
         alternate_name7, alternate_name8, alternate_name9, alternate_name10
         from ref_place_details rpd, ref_place_names rpn
         where rpd.xapian_docid = DocID
           and rpn.code = rpd.code;
      */

      ioSelectStatement =
        (ioSociSession.prepare
         << "select rpd.code AS code, city_code, xapian_docid, is_airport, "
         << "is_city, is_main, is_commercial, state_code, country_code, "
         << "region_code, continent_code, time_zone_grp, longitude, latitude, "
         << "language_code, classical_name, extended_name, "
         << "alternate_name1, alternate_name2, alternate_name3, "
         << "alternate_name4, alternate_name5, alternate_name6, "
         << "alternate_name7, alternate_name8, alternate_name9, "
         << "alternate_name10 "
         << "from ref_place_details rpd, ref_place_names rpn "
         << "where rpd.xapian_docid = :xapian_docid "
         << "and rpn.code = rpd.code",
         soci::into (ioPlace), soci::use (iDocID));

      // Execute the SQL query
      ioSelectStatement.execute();

    } catch (std::exception const& lException) {
      std::ostringstream errorStr;
      errorStr << "Error: " << lException.what();
      OPENTREP_LOG_ERROR (errorStr.str());
      throw SQLDatabaseException (errorStr.str());
    }
  }

  // //////////////////////////////////////////////////////////////////////
  bool DBManager::iterateOnStatement (soci::statement& ioStatement,
                                      Place& ioPlace,
                                      const bool iShouldDoReset) {
    bool hasStillData = false;
  
    try {

      // Reset the list of names of the given Place object
      if (iShouldDoReset == true) {
        ioPlace.resetMatrix();
      }

      // Retrieve the next row of Place object
      hasStillData = ioStatement.fetch();
      
    } catch (std::exception const& lException) {
      std::ostringstream errorStr;
      errorStr << "Error when iterating on the SQL fetch: "
               << lException.what();
      OPENTREP_LOG_ERROR (errorStr.str());
      throw SQLDatabaseException (errorStr.str());
    }

    return hasStillData;
  }

  // //////////////////////////////////////////////////////////////////////
  void DBManager::updatePlaceInDB (soci::session& ioSociSession,
                                   const Place& iPlace) {
  
    try {
    
      // Begin a transaction on the database
      ioSociSession.begin();
      
      // Instanciate a SQL statement (no request is performed at that stage)
      XapianDocID_T lDocID;
      std::string lPlaceCode;
      soci::statement lUpdateStatement =
        (ioSociSession.prepare
         << "update ref_place_details "
         << "set xapian_docid = :xapian_docid "
         << "where code = :code", soci::use (lDocID), soci::use (lPlaceCode));
      
      // Execute the SQL query
      lDocID = iPlace.getDocID();
      lPlaceCode = iPlace.getPlaceCode();
      lUpdateStatement.execute (true);
      
      // Commit the transaction on the database
      ioSociSession.commit();
        
      // Debug
      // OPENTREP_LOG_DEBUG ("[" << lDocID << "] " << iPlace);
      
    } catch (std::exception const& lException) {
      std::ostringstream errorStr;
      errorStr << "Error when updating " << iPlace.toString() << ": "
               << lException.what();
      OPENTREP_LOG_ERROR (errorStr.str());
      throw SQLDatabaseException (errorStr.str());
    }
  }

  // //////////////////////////////////////////////////////////////////////
  bool DBManager::retrievePlace (soci::session& ioSociSession,
                                 const std::string& iPlaceCode,
                                 Place& ioPlace) {
    bool oHasRetrievedPlace = false;
      
    try {

      // Prepare the SQL request corresponding to the select statement
      soci::statement lSelectStatement (ioSociSession);
      DBManager::prepareSelectOnPlaceCodeStatement (ioSociSession,
                                                    lSelectStatement,
                                                    iPlaceCode, ioPlace);
      const bool shouldDoReset = true;
      bool hasStillData = iterateOnStatement (lSelectStatement, ioPlace,
                                              shouldDoReset);
      if (hasStillData == true) {
        oHasRetrievedPlace = true;
      }

      // Sanity check
      const bool shouldNotDoReset = false;
      hasStillData = iterateOnStatement (lSelectStatement, ioPlace,
                                         shouldNotDoReset);
      if (hasStillData == true) {
        std::ostringstream errorStr;
        errorStr << "Error - There are multiple entries for the " << iPlaceCode
                 << " place code in the MySQL database.";
        OPENTREP_LOG_ERROR (errorStr.str());
        throw MultipleRowsForASingleDocIDException (errorStr.str());
      }

      // Debug
      // OPENTREP_LOG_DEBUG ("[" << iDocID << "] " << ioPlace);
      
    } catch (std::exception const& lException) {
      std::ostringstream errorStr;
      errorStr << "Error when trying to retrieve " << iPlaceCode
               << "from the MySQL database: " << lException.what();
      OPENTREP_LOG_ERROR (errorStr.str());
      throw SQLDatabaseException (errorStr.str());
    }

    return oHasRetrievedPlace;
  }

  // //////////////////////////////////////////////////////////////////////
  bool DBManager::retrieveClosestPlaces (soci::session&  ioSociSession,
                                         const double& iLatitude,
                                         const double& iLongitude,
                                         PlaceOrderedList_T& ioPlaceList) {
    bool oHasRetrievedPlace = false;
      
    try {

      
    } catch (std::exception const& lException) {
      std::ostringstream errorStr;
      errorStr << "Error when trying to retrieve the closest places for "
               << iLatitude << " / " << iLongitude << ": " << lException.what();
      OPENTREP_LOG_ERROR (errorStr.str());
      throw SQLDatabaseException (errorStr.str());
    }

    return oHasRetrievedPlace;
  }
  
}
