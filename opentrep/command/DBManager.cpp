// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// STL
#include <cassert>
#include <sstream>
// SOCI
#include <soci/soci.h>
#include <soci/sqlite3/soci-sqlite3.h>
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
         city_code, state_code, region_code, location_type, wiki_link,
         language_code, ascii_name, utf_name, 
         alternate_name1, alternate_name2, alternate_name3, 
         alternate_name4, alternate_name5, alternate_name6, 
         alternate_name7, alternate_name8, alternate_name9, 
         alternate_name10,
         page_rank, por_type
         from place_names as pn, place_details as rpd 
         left join airport_pageranked pr on pr.iata_code = rpd.iata_code 
         where rpd.iata_code = pn.iata_code
         order by rpd.iata_code, icao_code, geonameid
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
         << "city_code, state_code, region_code, location_type, wiki_link, "
         << "language_code, ascii_name, utf_name, "
         << "alternate_name1, alternate_name2, alternate_name3, "
         << "alternate_name4, alternate_name5, alternate_name6, "
         << "alternate_name7, alternate_name8, alternate_name9, "
         << "alternate_name10, "
         << "page_rank, por_type "
         << "from place_names as pn, place_details as rpd "
         << "left join airport_pageranked pr on pr.iata_code = rpd.iata_code "
         << "where rpd.iata_code = pn.iata_code "
         << "order by rpd.iata_code, icao_code, geonameid", soci::into (ioPlace));

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
         city_code, state_code, region_code, location_type, wiki_link, 
         language_code, ascii_name, utf_name, 
         alternate_name1, alternate_name2, alternate_name3,
         alternate_name4, alternate_name5, alternate_name6,
         alternate_name7, alternate_name8, alternate_name9,
         alternate_name10,
         page_rank, por_type
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
         << "city_code, state_code, region_code, location_type, wiki_link, "
         << "language_code, ascii_name, utf_name, "
         << "alternate_name1, alternate_name2, alternate_name3, "
         << "alternate_name4, alternate_name5, alternate_name6, "
         << "alternate_name7, alternate_name8, alternate_name9, "
         << "alternate_name10, "
         << "page_rank, por_type "
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
         city_code, state_code, region_code, location_type, wiki_link, 
         language_code, ascii_name, utf_name, 
         alternate_name1, alternate_name2, alternate_name3,
         alternate_name4, alternate_name5, alternate_name6,
         alternate_name7, alternate_name8, alternate_name9,
         alternate_name10,
         page_rank, por_type
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
         << "city_code, state_code, region_code, location_type, wiki_link, "
         << "language_code, ascii_name, utf_name, "
         << "alternate_name1, alternate_name2, alternate_name3, "
         << "alternate_name4, alternate_name5, alternate_name6, "
         << "alternate_name7, alternate_name8, alternate_name9, "
         << "alternate_name10, "
         << "page_rank, por_type "
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
         city_code, state_code, region_code, location_type, wiki_link, 
         language_code, ascii_name, utf_name, 
         alternate_name1, alternate_name2, alternate_name3,
         alternate_name4, alternate_name5, alternate_name6,
         alternate_name7, alternate_name8, alternate_name9,
         alternate_name10,
         page_rank, por_type
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
         << "city_code, state_code, region_code, location_type, wiki_link, "
         << "language_code, ascii_name, utf_name, "
         << "alternate_name1, alternate_name2, alternate_name3, "
         << "alternate_name4, alternate_name5, alternate_name6, "
         << "alternate_name7, alternate_name8, alternate_name9, "
         << "alternate_name10, "
         << "page_rank, por_type "
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
  bool DBManager::iterateOnStatement (soci::statement& ioStatement, Place& ioPlace) {
    bool hasStillData = false;
  
    try {

      // Retrieve the next row of Place object
      hasStillData = ioStatement.fetch();
      
    } catch (std::exception const& lException) {
      std::ostringstream errorStr;
      errorStr << "Error when iterating on the SQL fetch: " << lException.what();
      errorStr << ". The current place key is: " << ioPlace.describeKey()
               << " (it may be mis-leading, though, if the key could not be retrieved).";
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
                                 const LocationKey& iPlaceKey, Place& ioPlace) {
    bool oHasRetrievedPlace = false;
      
    try {

      // Prepare the SQL request corresponding to the select statement
      soci::statement lSelectStatement (ioSociSession);
      const std::string& lIataCode = iPlaceKey.getIataCode();
      const IATAType& lIataType = iPlaceKey.getIataType();
      const std::string& lIataTypeStr = lIataType.getTypeAsString();
      const GeonamesID_T& lGeonamesID = iPlaceKey.getGeonamesID();
      DBManager::prepareSelectOnPlaceCodeStatement (ioSociSession,
                                                    lSelectStatement,
                                                    lIataCode, lIataTypeStr,
                                                    lGeonamesID, ioPlace);
      /**
       * Retrieve the details of the place, as well as the alternate
       * names, most often in other languages (e.g., "ru", "zh").
       */
      bool hasStillData = true;
      while (hasStillData == true) {
        hasStillData = iterateOnStatement (lSelectStatement, ioPlace);

        // It is enough to have (at least) one database retrieved row
        if (hasStillData == true) {
          oHasRetrievedPlace = true;
        }

        // Debug
        OPENTREP_LOG_DEBUG ("[" << iPlaceKey << "] " << ioPlace);
      }
      
    } catch (std::exception const& lException) {
      std::ostringstream errorStr;
      errorStr << "Error when trying to retrieve " << iPlaceKey
               << " from the SQLite3 database: " << lException.what();
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
