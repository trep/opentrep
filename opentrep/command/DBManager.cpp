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
         select rpd.iata_code, xapian_docid, icao_code, 
         is_geonames, geonameid, 
         latitude, longitude, fclass, fcode, 
         country_code, cc2, admin1, admin2, admin3, admin4, 
         population, elevation, gtopo30, 
         timezone, gmt_offset, dst_offset, raw_offset, moddate, 
         is_airport, is_commercial, 
         city_code, state_code, region_code, location_type, 
         language_code, ascii_name, utf_name, 
         alternate_name1, alternate_name2, alternate_name3, 
         alternate_name4, alternate_name5, alternate_name6, 
         alternate_name7, alternate_name8, alternate_name9, 
         alternate_name10,
         page_rank
         from place_names as pn, place_details as rpd 
         left join airport_pageranked pr on pr.iata_code = rpd.iata_code 
         where rpd.iata_code = pn.iata_code
      */

      ioSelectStatement =
        (ioSociSession.prepare
         << "select rpd.iata_code, xapian_docid, icao_code, "
         << "is_geonames, geonameid, "
         << "latitude, longitude, fclass, fcode, "
         << "country_code, cc2, admin1, admin2, admin3, admin4, "
         << "population, elevation, gtopo30, "
         << "timezone, gmt_offset, dst_offset, raw_offset, moddate, "
         << "is_airport, is_commercial, "
         << "city_code, state_code, region_code, location_type, "
         << "language_code, ascii_name, utf_name, "
         << "alternate_name1, alternate_name2, alternate_name3, "
         << "alternate_name4, alternate_name5, alternate_name6, "
         << "alternate_name7, alternate_name8, alternate_name9, "
         << "alternate_name10, "
         << "page_rank "
         << "from place_names as pn, place_details as rpd "
         << "left join airport_pageranked pr on pr.iata_code = rpd.iata_code "
         << "where rpd.iata_code = pn.iata_code", soci::into (ioPlace));

      // Execute the SQL query
      ioSelectStatement.execute();

    } catch (std::exception const& lException) {
      std::ostringstream errorStr;
      errorStr << "Error in the 'select * from place_details' SQL request: "
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
         select rpd.iata_code, xapian_docid, icao_code, 
         is_geonames, geonameid, 
         latitude, longitude, fclass, fcode, 
         country_code, cc2, admin1, admin2, admin3, admin4, 
         population, elevation, gtopo30, 
         timezone, gmt_offset, dst_offset, raw_offset, moddate, 
         is_airport, is_commercial, 
         city_code, state_code, region_code, location_type, 
         language_code, ascii_name, utf_name, 
         alternate_name1, alternate_name2, alternate_name3,
         alternate_name4, alternate_name5, alternate_name6,
         alternate_name7, alternate_name8, alternate_name9,
         alternate_name10,
         page_rank
         from place_names as pn, place_details as rpd 
         left join airport_pageranked pr on pr.iata_code = rpd.iata_code 
         where latitude >= :lower_latitude
           and latitude <= :upper_latitude
           and longitude >= :lower_longitude
           and longitude <= :upper_longitude
           and rpd.iata_code = pn.iata_code
      */
      Place& lPlace = FacPlace::instance().create();
      const double K_ERROR = 2.0;
      const double lLowerBoundLatitude = iLatitude - K_ERROR;
      const double lUpperBoundLatitude = iLatitude + K_ERROR;
      const double lLowerBoundLongitude = iLongitude - K_ERROR;
      const double lUpperBoundLongitude = iLongitude + K_ERROR;
      
      ioSelectStatement =
        (ioSociSession.prepare
         << "select rpd.iata_code, xapian_docid, icao_code, "
         << "is_geonames, geonameid, "
         << "latitude, longitude, fclass, fcode, "
         << "country_code, cc2, admin1, admin2, admin3, admin4, "
         << "population, elevation, gtopo30, "
         << "timezone, gmt_offset, dst_offset, raw_offset, moddate, "
         << "is_airport, is_commercial, "
         << "city_code, state_code, region_code, location_type, "
         << "language_code, ascii_name, utf_name, "
         << "alternate_name1, alternate_name2, alternate_name3, "
         << "alternate_name4, alternate_name5, alternate_name6, "
         << "alternate_name7, alternate_name8, alternate_name9, "
         << "alternate_name10, "
         << "page_rank "
         << "from place_names as pn, place_details as rpd "
         << "left join airport_pageranked pr on pr.iata_code = rpd.iata_code "
         << "where latitude >= :lower_latitude "
         << "  and latitude <= :upper_latitude "
         << "  and longitude >= :lower_longitude "
         << "  and longitude <= :upper_longitude "
         << "  and rpd.iata_code = pn.iata_code",
         soci::into (lPlace), soci::use (lLowerBoundLatitude),
         soci::use (lUpperBoundLatitude), soci::use (lLowerBoundLongitude),
         soci::use (lUpperBoundLongitude));

      // Execute the SQL query
      ioSelectStatement.execute();

    } catch (std::exception const& lException) {
      std::ostringstream errorStr;
      errorStr << "Error in the 'select * from place_details' SQL request: "
               << lException.what();
      OPENTREP_LOG_ERROR (errorStr.str());
      throw SQLDatabaseException (errorStr.str());
    }
  }
  
  // //////////////////////////////////////////////////////////////////////
  void DBManager::
  prepareSelectOnPlaceCodeStatement (soci::session& ioSociSession,
                                     soci::statement& ioSelectStatement,
                                     const std::string& iIataCode,
                                     const std::string& iIcaoCode,
                                     const GeonamesID_T& iGeonamesID,
                                     Place& ioPlace) {
  
    try {
    
      // Instanciate a SQL statement (no request is performed at that stage)
      /**
         select rpd.iata_code, xapian_docid, icao_code, 
         is_geonames, geonameid, 
         latitude, longitude, fclass, fcode, 
         country_code, cc2, admin1, admin2, admin3, admin4, 
         population, elevation, gtopo30, 
         timezone, gmt_offset, dst_offset, raw_offset, moddate, 
         is_airport, is_commercial, 
         city_code, state_code, region_code, location_type, 
         language_code, ascii_name, utf_name, 
         alternate_name1, alternate_name2, alternate_name3,
         alternate_name4, alternate_name5, alternate_name6,
         alternate_name7, alternate_name8, alternate_name9,
         alternate_name10,
         page_rank
         from place_names as pn, place_details as rpd 
         left join airport_pageranked pr on pr.iata_code = rpd.iata_code 
         where rpd.iata_code = iIataCode
           and rpd.icao_code = iIcaoCode
           and rpd.geonameid = iGeonamesID
           and rpd.iata_code = pn.iata_code 
      */

      ioSelectStatement =
        (ioSociSession.prepare
         << "select rpd.iata_code, xapian_docid, icao_code, "
         << "is_geonames, geonameid, "
         << "latitude, longitude, fclass, fcode, "
         << "country_code, cc2, admin1, admin2, admin3, admin4, "
         << "population, elevation, gtopo30, "
         << "timezone, gmt_offset, dst_offset, raw_offset, moddate, "
         << "is_airport, is_commercial, "
         << "city_code, state_code, region_code, location_type, "
         << "language_code, ascii_name, utf_name, "
         << "alternate_name1, alternate_name2, alternate_name3, "
         << "alternate_name4, alternate_name5, alternate_name6, "
         << "alternate_name7, alternate_name8, alternate_name9, "
         << "alternate_name10, "
         << "page_rank "
         << "from place_names as pn, place_details as rpd "
         << "left join airport_pageranked pr on pr.iata_code = rpd.iata_code "
         << "where rpd.iata_code = :place_iata_code "
         << "  and rpd.icao_code = :place_icao_code "
         << "  and rpd.geonameid = :place_geonameid "
         << "  and rpd.iata_code = pn.iata_code",
         soci::into (ioPlace), soci::use (iIataCode), soci::use (iIcaoCode),
         soci::use (iGeonamesID));

      // Execute the SQL query
      ioSelectStatement.execute();

    } catch (std::exception const& lException) {
      std::ostringstream errorStr;
      errorStr << "Error in the 'select * from place_details' SQL request: "
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
         select rpd.iata_code, xapian_docid, icao_code, 
         is_geonames, geonameid, 
         latitude, longitude, fclass, fcode, 
         country_code, cc2, admin1, admin2, admin3, admin4, 
         population, elevation, gtopo30, 
         timezone, gmt_offset, dst_offset, raw_offset, moddate, 
         is_airport, is_commercial, 
         city_code, state_code, region_code, location_type, 
         language_code, ascii_name, utf_name, 
         alternate_name1, alternate_name2, alternate_name3,
         alternate_name4, alternate_name5, alternate_name6,
         alternate_name7, alternate_name8, alternate_name9,
         alternate_name10,
         page_rank
         from place_names as pn, place_details as rpd 
         left join airport_pageranked pr on pr.iata_code = rpd.iata_code 
         where rpd.xapian_docid = DocID
           and rpd.iata_code = pn.iata_code
           and rpd.iata_code = pr.iata_code
      */

      ioSelectStatement =
        (ioSociSession.prepare
         << "select rpd.iata_code, xapian_docid, icao_code, "
         << "is_geonames, geonameid, "
         << "latitude, longitude, fclass, fcode, "
         << "country_code, cc2, admin1, admin2, admin3, admin4, "
         << "population, elevation, gtopo30, "
         << "timezone, gmt_offset, dst_offset, raw_offset, moddate, "
         << "is_airport, is_commercial, "
         << "city_code, state_code, region_code, location_type, "
         << "language_code, ascii_name, utf_name, "
         << "alternate_name1, alternate_name2, alternate_name3, "
         << "alternate_name4, alternate_name5, alternate_name6, "
         << "alternate_name7, alternate_name8, alternate_name9, "
         << "alternate_name10, "
         << "page_rank "
         << "from place_names as pn, place_details as rpd "
         << "left join airport_pageranked pr on pr.iata_code = rpd.iata_code "
         << "where rpd.xapian_docid = :xapian_docid "
         << "  and rpd.iata_code = pn.iata_code "
         << "  and rpd.iata_code = pr.iata_code",
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
      std::string lIataCode;
      soci::statement lUpdateStatement =
        (ioSociSession.prepare
         << "update place_details "
         << "set xapian_docid = :xapian_docid "
         << "where iata_code = :iata_code",
         soci::use (lDocID), soci::use (lIataCode));
      
      // Execute the SQL query
      lDocID = iPlace.getDocID();
      lIataCode = iPlace.getIataCode();
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
                                 const PlaceKey& iPlaceKey, Place& ioPlace) {
    bool oHasRetrievedPlace = false;
      
    try {

      // Prepare the SQL request corresponding to the select statement
      soci::statement lSelectStatement (ioSociSession);
      const std::string& lIataCode = iPlaceKey.getIataCode();
      const std::string& lIcaoCode = iPlaceKey.getIcaoCode();
      const GeonamesID_T& lGeonamesID = iPlaceKey.getGeonamesID();
      DBManager::prepareSelectOnPlaceCodeStatement (ioSociSession,
                                                    lSelectStatement,
                                                    lIataCode, lIcaoCode,
                                                    lGeonamesID, ioPlace);
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
        errorStr << "Error - There are multiple entries for the " << iPlaceKey
                 << " place code in the MySQL database.";
        OPENTREP_LOG_ERROR (errorStr.str());
        throw MultipleRowsForASingleDocIDException (errorStr.str());
      }

      // Debug
      // OPENTREP_LOG_DEBUG ("[" << iDocID << "] " << ioPlace);
      
    } catch (std::exception const& lException) {
      std::ostringstream errorStr;
      errorStr << "Error when trying to retrieve " << iPlaceKey
               << " from the MySQL database: " << lException.what();
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
