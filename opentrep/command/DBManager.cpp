// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// STL
#include <cassert>
#include <sstream>
// Boost
#include <boost/lexical_cast.hpp>
#include <boost/date_time/gregorian/gregorian.hpp>
#include <boost/filesystem.hpp>
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
  std::string DBManager::
  prepareSelectAllBlobStatement (soci::session& ioSociSession,
                                 soci::statement& ioSelectStatement) {
    std::string oSerialisedPlaceStr;
  
    try {
    
      // Instanciate a SQL statement (no request is performed at that stage)
      /**
         select serialised_place from ori_por;
      */

      ioSelectStatement =
        (ioSociSession.prepare
         << "select serialised_place from ori_por",
         soci::into (oSerialisedPlaceStr));

      // Execute the SQL query
      ioSelectStatement.execute();

    } catch (std::exception const& lException) {
      std::ostringstream errorStr;
      errorStr << "Error in the 'select serialised_place from ori_por' SQL request: "
               << lException.what();
      OPENTREP_LOG_ERROR (errorStr.str());
      throw SQLDatabaseException (errorStr.str());
    }

    //
    return oSerialisedPlaceStr;
  }

  // //////////////////////////////////////////////////////////////////////
  std::string DBManager::
  prepareSelectBlobOnPlaceCodeStatement (soci::session& ioSociSession,
                                         soci::statement& ioSelectStatement,
                                         const IATACode_T& iIataCode) {
    std::string oSerialisedPlaceStr;
  
    try {
    
      // Instanciate a SQL statement (no request is performed at that stage)
      /**
         select serialised_place from ori_por where iata_code = iIataCode;
      */

      ioSelectStatement =
        (ioSociSession.prepare
         << "select serialised_place from ori_por "
         << "where iata_code = :place_iata_code",
         soci::into (oSerialisedPlaceStr),
         soci::use (static_cast<std::string>(iIataCode)));

      // Execute the SQL query
      ioSelectStatement.execute();

    } catch (std::exception const& lException) {
      std::ostringstream errorStr;
      errorStr << "Error in the 'select serialised_place from ori_por' SQL request: "
               << lException.what();
      OPENTREP_LOG_ERROR (errorStr.str());
      throw SQLDatabaseException (errorStr.str());
    }

    //
    return oSerialisedPlaceStr;
  }

  // //////////////////////////////////////////////////////////////////////
  bool DBManager::iterateOnStatement (soci::statement& ioStatement,
                                      std::string& ioPlace) {
    bool hasStillData = false;
  
    try {

      // Retrieve the next row of Place object
      hasStillData = ioStatement.fetch();
      
    } catch (std::exception const& lException) {
      std::ostringstream errorStr;
      errorStr << "Error when iterating on the SQL fetch: " << lException.what();
      errorStr << ". The current place is: " << ioPlace;
      OPENTREP_LOG_ERROR (errorStr.str());
      throw SQLDatabaseException (errorStr.str());
    }

    return hasStillData;
  }

  // //////////////////////////////////////////////////////////////////////
  void DBManager::insertPlaceInDB (soci::session& ioSociSession,
                                   const Place& iPlace,
                                   const std::string& iSerialisedPlaceStr) {
  
    try {
    
      // Begin a transaction on the database
      ioSociSession.begin();

      // Instanciate a SQL statement (no request is performed at that stage)
      const LocationKey& lLocationKey = iPlace.getKey();
      const std::string lPK (lLocationKey.toString());
      const IATAType& lIataType = iPlace.getIataType();
      const std::string lLocationType (lIataType.getTypeAsString());
      const std::string lIataCode (iPlace.getIataCode());
      const std::string lIcaoCode (iPlace.getIcaoCode());
      const std::string lFaaCode (iPlace.getFaaCode());
      const std::string lIsGeonames ((iPlace.isGeonames())?"Y":"N");
      const std::string lGeonameID =
        boost::lexical_cast<std::string> (iPlace.getGeonamesID());
      const std::string lEnvID =
        boost::lexical_cast<std::string> (iPlace.getEnvelopeID());
      const std::string lDateFrom =
        boost::gregorian::to_iso_extended_string (iPlace.getDateFrom());
      const std::string lDateEnd =
        boost::gregorian::to_iso_extended_string (iPlace.getDateEnd());
      // DEBUG
      std::ostringstream oStr;
      oStr << "insert into ori_por values (" << lPK << ", ";
      oStr << lLocationType << ", ";
      oStr << lIataCode << ", " << lIcaoCode << ", " << lFaaCode << ", ";
      oStr << lIsGeonames << ", " << lGeonameID << ", ";
      oStr << lEnvID << ", " << lDateFrom << ", " << lDateEnd << ", ";
      oStr << iSerialisedPlaceStr << ")";
      OPENTREP_LOG_DEBUG ("Full SQL statement: '" << oStr.str() << "'");

      ioSociSession << "insert into ori_por values (:pk, "
                    << ":location_type, :iata_code, :icao_code, :faa_code, "
                    << ":is_geonames, :geoname_id, "
                    << ":envelope_id, :date_from, :date_until, "
                    << ":serialised_place)",
        soci::use (lPK), soci::use (lLocationType), soci::use (lIataCode),
        soci::use (lIcaoCode), soci::use (lFaaCode),
        soci::use (lIsGeonames), soci::use (lGeonameID),
        soci::use (lEnvID), soci::use (lDateFrom), soci::use (lDateEnd),
        soci::use (iSerialisedPlaceStr);
      
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
  NbOfDBEntries_T DBManager::getAll(const SQLiteDBFilePath_T& iSQLiteDBFilePath){
    NbOfDBEntries_T oNbOfEntries = 0;

    try {

      // Open the connection to the database
      soci::session* lSociSession_ptr = new soci::session();
      assert (lSociSession_ptr != NULL);
      soci::session& lSociSession = *lSociSession_ptr;
      lSociSession.open (soci::sqlite3, iSQLiteDBFilePath);

      // Prepare the SQL request corresponding to the select statement
      soci::statement lSelectStatement (lSociSession);
      std::string lPlace =
        DBManager::prepareSelectAllBlobStatement(lSociSession, lSelectStatement);

      /**
       * Retrieve the details of the place, as well as the alternate
       * names, most often in other languages (e.g., "ru", "zh").
       */
      bool hasStillData = true;
      while (hasStillData == true) {
        hasStillData = iterateOnStatement (lSelectStatement, lPlace);

        // It is enough to have (at least) one database retrieved row
        if (hasStillData == true) {
          ++oNbOfEntries;

          // Debug
          OPENTREP_LOG_DEBUG ("[" << oNbOfEntries << "] " << lPlace);
        }
      }
      
    } catch (std::exception const& lException) {
      std::ostringstream errorStr;
      errorStr << "Error when trying to retrieve " << oNbOfEntries
               << "-th row from the SQLite3 database: " << lException.what();
      OPENTREP_LOG_ERROR (errorStr.str());
      throw SQLDatabaseException (errorStr.str());
    }

    return oNbOfEntries;
  }

  // //////////////////////////////////////////////////////////////////////
  soci::session* DBManager::
  buildSQLDB (const SQLiteDBFilePath_T& iSQLiteDBFilePath) {
    soci::session* oSociSession_ptr = NULL;

    // DEBUG
    OPENTREP_LOG_DEBUG ("The SQLite3 database/file ('" << iSQLiteDBFilePath
                        << "') will be cleared");

    try {

      // Re-create the SQLite3 directory
      boost::filesystem::path lSQLiteDBFullPath (iSQLiteDBFilePath.begin(),
                                                 iSQLiteDBFilePath.end());
      boost::filesystem::path lSQLiteDBParentPath =
        lSQLiteDBFullPath.parent_path();
      boost::filesystem::create_directories (lSQLiteDBParentPath);

      // Check whether the just created directory exists and is a directory.
      boost::filesystem::path lSQLiteDBFilename = lSQLiteDBFullPath.filename();
      if (!(boost::filesystem::exists (lSQLiteDBParentPath)
            && boost::filesystem::is_directory (lSQLiteDBParentPath))) {
        std::ostringstream oStr;
        oStr << "The path to the SQLite3 database ('"
             << lSQLiteDBParentPath<< "') does not exist or is not a directory.";
        OPENTREP_LOG_ERROR (oStr.str());
        throw FileNotFoundException (oStr.str());
      }

      // Create the SQLite3 database (file). As the directory has been fully
      // cleaned, deleted and re-created, that SQLite3 database (file) is empty.
      oSociSession_ptr = new soci::session();
      assert (oSociSession_ptr != NULL);
      soci::session& lSociSession = *oSociSession_ptr;
      lSociSession.open (soci::sqlite3, iSQLiteDBFilePath);

      // DEBUG
      OPENTREP_LOG_DEBUG ("The SQLite3 database/file ('" << iSQLiteDBFilePath
                          << "') has been checked and open");

      // Prepare the SQL request corresponding to the select statement
      lSociSession << "drop table if exists ori_por;";
      std::ostringstream lSqlCreateStringStr;
      lSqlCreateStringStr << "create table ori_por (";
      lSqlCreateStringStr << "pk varchar(20) NOT NULL, ";
      lSqlCreateStringStr << "location_type varchar(4) default NULL, ";
      lSqlCreateStringStr << "iata_code varchar(3) default NULL, ";
      lSqlCreateStringStr << "icao_code varchar(4) default NULL, ";
      lSqlCreateStringStr << "faa_code varchar(4) default NULL, ";
      lSqlCreateStringStr << "is_geonames varchar(1) default NULL, ";
      lSqlCreateStringStr << "geoname_id int(11) default NULL, ";
      lSqlCreateStringStr << "envelope_id int(11) default NULL, ";
      lSqlCreateStringStr << "date_from date default NULL, ";
      lSqlCreateStringStr << "date_until date default NULL, ";
      lSqlCreateStringStr << "serialised_place default NULL, ";
      lSqlCreateStringStr << "primary key (pk)); ";
      lSociSession << lSqlCreateStringStr.str();
      lSociSession << "create index ori_por_iata_code on ori_por (iata_code);";
      lSociSession << "create index ori_por_iata_date on ori_por (iata_code, date_from, date_until);";
      lSociSession << "create index ori_por_icao_code on ori_por (icao_code);";
      lSociSession << "create index ori_por_geonameid on ori_por (geoname_id);";

    } catch (std::exception const& lException) {
      std::ostringstream errorStr;
      errorStr << "Error when trying to create " << iSQLiteDBFilePath
               << " SQLite3 database: " << lException.what();
      OPENTREP_LOG_ERROR (errorStr.str());
      throw SQLDatabaseException (errorStr.str());
    }

    assert (oSociSession_ptr != NULL);
    return oSociSession_ptr;
  }

}
