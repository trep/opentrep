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
#include <soci/mysql/soci-mysql.h>
// OpenTrep
#include <opentrep/bom/World.hpp>
#include <opentrep/bom/Place.hpp>
#include <opentrep/factory/FacPlace.hpp>
#include <opentrep/dbadaptor/DbaPlace.hpp>
#include <opentrep/command/DBManager.hpp>
#include <opentrep/service/Logger.hpp>

namespace OPENTREP {

  // //////////////////////////////////////////////////////////////////////
  soci::session* DBManager::
  initSQLDBSession (const DBType& iDBType,
                    const SQLDBConnectionString_T& iSQLDBConnectionString) {
    soci::session* oSociSession_ptr = NULL;

    // DEBUG
    OPENTREP_LOG_DEBUG ("SQL database type: " << iDBType.describe());
    if (!(iDBType == DBType::NODB)) {
      OPENTREP_LOG_DEBUG ("The SQL database/file ('" << iSQLDBConnectionString
                          << "') will be cleared");
    }

    if (iDBType == DBType::SQLITE3) {

      try {

        // Re-create the SQLite3 directory, if needed
        boost::filesystem::path lSQLiteDBFullPath(iSQLDBConnectionString.begin(),
                                                  iSQLDBConnectionString.end());
        boost::filesystem::path lSQLiteDBParentPath =
          lSQLiteDBFullPath.parent_path();
        boost::filesystem::create_directories (lSQLiteDBParentPath);

        // Check whether the just created directory exists and is a directory.
        boost::filesystem::path lSQLiteDBFilename = lSQLiteDBFullPath.filename();
        if (!(boost::filesystem::exists (lSQLiteDBParentPath)
              && boost::filesystem::is_directory (lSQLiteDBParentPath))) {
          std::ostringstream oStr;
          oStr << "Error. The path to the SQLite3 database directory ('"
               << lSQLiteDBParentPath
               << "') does not exist or is not a directory.";
          OPENTREP_LOG_ERROR (oStr.str());
          throw FileNotFoundException (oStr.str());
        }

      } catch (std::exception const& lException) {
        std::ostringstream errorStr;
        errorStr << "Error when trying to create " << iSQLDBConnectionString
                 << " SQL database file: " << lException.what();
        errorStr << ". Check that the program has got write permission on the "
                 << "corresponding parent directories.";
        OPENTREP_LOG_ERROR (errorStr.str());
        throw SQLDatabaseFileCannotBeCreatedException (errorStr.str());
      }

      try {

        // Create the SQL database (file). As the directory has been fully
        // cleaned, deleted and re-created, that SQL database (file) is empty.
        oSociSession_ptr = new soci::session();
        assert (oSociSession_ptr != NULL);
        soci::session& lSociSession = *oSociSession_ptr;
        lSociSession.open (soci::sqlite3, iSQLDBConnectionString);

        // DEBUG
        OPENTREP_LOG_DEBUG ("The SQL database/file ('" << iSQLDBConnectionString
                            << "') has been checked and open");
        const std::string& lDBName = lSociSession.get_backend_name();
        OPENTREP_LOG_DEBUG ("SQL database type: " << lDBName);

      } catch (std::exception const& lException) {
        std::ostringstream errorStr;
        errorStr << "Error when trying to connect to the '"
                 << iSQLDBConnectionString << "' SQL database: "
                 << lException.what();
        OPENTREP_LOG_ERROR (errorStr.str());
        throw SQLDatabaseImpossibleConnectionException (errorStr.str());
      }

      // The SQLite3 connection is assumed to have been successful
      assert (oSociSession_ptr != NULL);

    } else if (iDBType == DBType::MYSQL) {

      try {

        // Create the SQL database tables.
        oSociSession_ptr = new soci::session();
        assert (oSociSession_ptr != NULL);
        soci::session& lSociSession = *oSociSession_ptr;
        lSociSession.open (soci::mysql, iSQLDBConnectionString);

        // DEBUG
        OPENTREP_LOG_DEBUG ("The SQL database/file ('" << iSQLDBConnectionString
                            << "') has been checked and open");
        const std::string& lDBName = lSociSession.get_backend_name();
        OPENTREP_LOG_DEBUG ("SQL database type: " << lDBName);

      } catch (std::exception const& lException) {
        std::ostringstream errorStr;
        errorStr << "Error when trying to connect to the '"
                 << iSQLDBConnectionString << "' SQL database: "
                 << lException.what();
        OPENTREP_LOG_ERROR (errorStr.str());
        throw SQLDatabaseImpossibleConnectionException (errorStr.str());
      }

      // The MySQL/MariaDB connection is assumed to have been successful
      assert (oSociSession_ptr != NULL);

    } else {
      // No connection will be made
    }

    return oSociSession_ptr;
  }

  // //////////////////////////////////////////////////////////////////////
  void DBManager::createSQLDBTables (soci::session& ioSociSession) {
    const std::string& lDBName = ioSociSession.get_backend_name();

    // DEBUG
    OPENTREP_LOG_DEBUG ("SQL database type: " << lDBName);

    if (lDBName == "sqlite3") {

      try {

        /**
         * SQL DDL (Data Definition Language) queries:
         * -------------------------------------------
           drop table if exists ori_por;
           create table ori_por (
           pk varchar(20) NOT NULL,
           location_type varchar(4) default NULL,
           iata_code varchar(3) default NULL,
           icao_code varchar(4) default NULL,
           faa_code varchar(4) default NULL,
           is_geonames varchar(1) default NULL,
           geoname_id int(11) default NULL,
           envelope_id int(11) default NULL,
           date_from date default NULL,
           date_until date default NULL,
           serialised_place varchar(8000) default NULL,
           primary key (pk));
        */

        ioSociSession << "drop table if exists ori_por;";
        std::ostringstream lSQLTableCreationStr;
        lSQLTableCreationStr << "create table ori_por (";
        lSQLTableCreationStr << "pk varchar(20) NOT NULL, ";
        lSQLTableCreationStr << "location_type varchar(4) default NULL, ";
        lSQLTableCreationStr << "iata_code varchar(3) default NULL, ";
        lSQLTableCreationStr << "icao_code varchar(4) default NULL, ";
        lSQLTableCreationStr << "faa_code varchar(4) default NULL, ";
        lSQLTableCreationStr << "is_geonames varchar(1) default NULL, ";
        lSQLTableCreationStr << "geoname_id int(11) default NULL, ";
        lSQLTableCreationStr << "envelope_id int(11) default NULL, ";
        lSQLTableCreationStr << "date_from date default NULL, ";
        lSQLTableCreationStr << "date_until date default NULL, ";
        lSQLTableCreationStr << "serialised_place varchar(8000) default NULL, ";
        lSQLTableCreationStr << "primary key (pk)); ";
        ioSociSession << lSQLTableCreationStr.str();

      } catch (std::exception const& lException) {
        std::ostringstream errorStr;
        errorStr << "Error when trying to create SQLite3 tables: "
                 << lException.what();
        OPENTREP_LOG_ERROR (errorStr.str());
        throw SQLDatabaseTableCreationException (errorStr.str());
      }

    } else if (lDBName == "mysql") {

      try {

        /**
         * SQL DDL (Data Definition Language) queries:
         * -------------------------------------------
           drop table if exists ori_por;
           create table ori_por (
           pk varchar(20) NOT NULL,
           location_type varchar(4) default NULL,
           iata_code varchar(3) default NULL,
           icao_code varchar(4) default NULL,
           faa_code varchar(4) default NULL,
           is_geonames varchar(1) default NULL,
           geoname_id int(11) default NULL,
           envelope_id int(11) default NULL,
           date_from date default NULL,
           date_until date default NULL,
           serialised_place varchar(8000) default NULL);
        */

        ioSociSession << "drop table if exists ori_por;";
        std::ostringstream lSQLTableCreationStr;
        lSQLTableCreationStr << "create table ori_por (";
        lSQLTableCreationStr << "pk varchar(20) NOT NULL, ";
        lSQLTableCreationStr << "location_type varchar(4) default NULL, ";
        lSQLTableCreationStr << "iata_code varchar(3) default NULL, ";
        lSQLTableCreationStr << "icao_code varchar(4) default NULL, ";
        lSQLTableCreationStr << "faa_code varchar(4) default NULL, ";
        lSQLTableCreationStr << "is_geonames varchar(1) default NULL, ";
        lSQLTableCreationStr << "geoname_id int(11) default NULL, ";
        lSQLTableCreationStr << "envelope_id int(11) default NULL, ";
        lSQLTableCreationStr << "date_from date default NULL, ";
        lSQLTableCreationStr << "date_until date default NULL, ";
        lSQLTableCreationStr << "serialised_place varchar(8000) default NULL); ";
        ioSociSession << lSQLTableCreationStr.str();

      } catch (std::exception const& lException) {
        std::ostringstream errorStr;
        errorStr << "Error when trying to create MySQL/MariaDB tables: "
                 << lException.what();
        OPENTREP_LOG_ERROR (errorStr.str());
        throw SQLDatabaseTableCreationException (errorStr.str());
      }

    } else {
      std::ostringstream errorStr;
      errorStr << "Error: the '" << lDBName
               << "' SQL database type is not supported";
      OPENTREP_LOG_ERROR (errorStr.str());
      throw SQLDatabaseTableCreationException (errorStr.str());
    }
  }

  // //////////////////////////////////////////////////////////////////////
  void DBManager::createSQLDBIndexes (soci::session& ioSociSession) {
    const std::string& lDBName = ioSociSession.get_backend_name();

    // DEBUG
    OPENTREP_LOG_DEBUG ("SQL database type: " << lDBName);

    if (lDBName == "sqlite3") {

      try {

        /**
         * SQL DDL (Data Definition Language) queries for SQLite3:
         * -------------------------------------------------------
         create index ori_por_iata_code on ori_por (iata_code);
         create index ori_por_iata_date on ori_por (iata_code, date_from, date_until);
         create index ori_por_icao_code on ori_por (icao_code);
         create index ori_por_geonameid on ori_por (geoname_id);
        */

        ioSociSession
          << "create index ori_por_iata_code on ori_por (iata_code);";
        ioSociSession
          << "create index ori_por_iata_date on ori_por (iata_code, date_from, date_until);";
        ioSociSession
          << "create index ori_por_icao_code on ori_por (icao_code);";
        ioSociSession
          << "create index ori_por_geonameid on ori_por (geoname_id);";

      } catch (std::exception const& lException) {
        std::ostringstream errorStr;
        errorStr << "Error when trying to create SQLite3 indexes: "
                 << lException.what();
        OPENTREP_LOG_ERROR (errorStr.str());
        throw SQLDatabaseIndexCreationException (errorStr.str());
      }

    } else if (lDBName == "mysql") {

      try {

        /**
         * SQL DDL (Data Definition Language) queries for MySQLMariaDB:
         * ------------------------------------------------------------
         alter table ori_por add primary key (pk);
         alter table ori_por add index ori_por_iata_code (iata_code asc);
         alter table ori_por add index ori_por_iata_date (iata_code asc, date_from asc, date_until asc);
         alter table ori_por add index ori_por_icao_code (icao_code asc);
         alter table ori_por add index ori_por_geonameid (geoname_id asc);
        */

        ioSociSession << "alter table ori_por add primary key (pk);";
        ioSociSession
          << "alter table ori_por add index ori_por_iata_code (iata_code asc);";
        ioSociSession
          << "alter table ori_por add index ori_por_iata_date (iata_code asc, date_from asc, date_until asc);";
        ioSociSession
          << "alter table ori_por add index ori_por_icao_code (icao_code asc);";
        ioSociSession
          << "alter table ori_por add index ori_por_geonameid (geoname_id asc);";

      } catch (std::exception const& lException) {
        std::ostringstream errorStr;
        errorStr << "Error when trying to create SQLite3 indexes: "
                 << lException.what();
        OPENTREP_LOG_ERROR (errorStr.str());
        throw SQLDatabaseIndexCreationException (errorStr.str());
      }

    } else {
      std::ostringstream errorStr;
      errorStr << "Error: the '" << lDBName
               << "' SQL database type is not supported";
      OPENTREP_LOG_ERROR (errorStr.str());
      throw SQLDatabaseTableCreationException (errorStr.str());
    }
  }

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
                                   const Place& iPlace) {
  
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
      const std::string lRawDataString (iPlace.getRawDataString());
      // DEBUG
      /*
      std::ostringstream oStr;
      oStr << "insert into ori_por values (" << lPK << ", ";
      oStr << lLocationType << ", ";
      oStr << lIataCode << ", " << lIcaoCode << ", " << lFaaCode << ", ";
      oStr << lIsGeonames << ", " << lGeonameID << ", ";
      oStr << lEnvID << ", " << lDateFrom << ", " << lDateEnd << ", ";
      oStr << lRawDataString << ")";
      OPENTREP_LOG_DEBUG ("Full SQL statement: '" << oStr.str() << "'");
      */

      ioSociSession << "insert into ori_por values (:pk, "
                    << ":location_type, :iata_code, :icao_code, :faa_code, "
                    << ":is_geonames, :geoname_id, "
                    << ":envelope_id, :date_from, :date_until, "
                    << ":serialised_place)",
        soci::use (lPK), soci::use (lLocationType), soci::use (lIataCode),
        soci::use (lIcaoCode), soci::use (lFaaCode),
        soci::use (lIsGeonames), soci::use (lGeonameID),
        soci::use (lEnvID), soci::use (lDateFrom), soci::use (lDateEnd),
        soci::use (lRawDataString);
      
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
  NbOfDBEntries_T DBManager::getAll (soci::session& ioSociSession) {
    NbOfDBEntries_T oNbOfEntries = 0;

    try {

      // Prepare the SQL request corresponding to the select statement
      soci::statement lSelectStatement (ioSociSession);
      std::string lPlace =
        DBManager::prepareSelectAllBlobStatement (ioSociSession,
                                                  lSelectStatement);

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
               << "-th row from the SQL database: " << lException.what();
      OPENTREP_LOG_ERROR (errorStr.str());
      throw SQLDatabaseException (errorStr.str());
    }

    return oNbOfEntries;
  }

}
