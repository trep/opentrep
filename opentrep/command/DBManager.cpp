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
#include <opentrep/bom/Result.hpp>
#include <opentrep/bom/PORFileHelper.hpp>
#include <opentrep/bom/PORParserHelper.hpp>
#include <opentrep/factory/FacPlace.hpp>
#include <opentrep/dbadaptor/DbaPlace.hpp>
#include <opentrep/command/DBManager.hpp>
#include <opentrep/service/Logger.hpp>

namespace OPENTREP {

  // //////////////////////////////////////////////////////////////////////
  soci::session* DBManager::
  initSQLDBSession (const DBType& iDBType,
                    const SQLDBConnectionString_T& iSQLDBConnStr) {
    soci::session* oSociSession_ptr = NULL;

    // DEBUG
    if (!(iDBType == DBType::NODB)) {
      OPENTREP_LOG_DEBUG ("Connecting to the " << iDBType.describe()
                          << " SQL database/file ('" << iSQLDBConnStr << "')");
    }

    if (iDBType == DBType::SQLITE3) {

      // Re-create the SQLite3 directory, if needed
      boost::filesystem::path lSQLiteDBFullPath (iSQLDBConnStr.begin(),
                                                 iSQLDBConnStr.end());
      boost::filesystem::path lSQLiteDBParentPath =
        lSQLiteDBFullPath.parent_path();

      // Check whether the directory exists and is a directory.
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

      try {

        // Connect to the SQL database.
        oSociSession_ptr = new soci::session();
        assert (oSociSession_ptr != NULL);
        soci::session& lSociSession = *oSociSession_ptr;
        lSociSession.open (soci::sqlite3, iSQLDBConnStr);

        // DEBUG
        OPENTREP_LOG_DEBUG ("The SQLite3 database/file ('" << iSQLDBConnStr
                            << "') has been checked and open");

      } catch (std::exception const& lException) {
        std::ostringstream errorStr;
        errorStr << "Error when trying to connect to the '" << iSQLDBConnStr
                 << "' SQLite3 database: " << lException.what();
        OPENTREP_LOG_ERROR (errorStr.str());
        throw SQLDatabaseImpossibleConnectionException (errorStr.str());
      }

      // The SQLite3 connection is assumed to have been successful
      assert (oSociSession_ptr != NULL);

    } else if (iDBType == DBType::MYSQL) {

      try {

        // Connect to the SQL database.
        oSociSession_ptr = new soci::session();
        assert (oSociSession_ptr != NULL);
        soci::session& lSociSession = *oSociSession_ptr;
        lSociSession.open (soci::mysql, iSQLDBConnStr);

        // DEBUG
        OPENTREP_LOG_DEBUG ("The " << iDBType.describe() << " database "
                            << " is accessible");

      } catch (std::exception const& lException) {
        std::ostringstream errorStr;
        errorStr << "Error when trying to connect to the '" << iSQLDBConnStr
                 << "' MySQL/MariaDB database: " << lException.what();
        OPENTREP_LOG_ERROR (errorStr.str());
        throw SQLDatabaseImpossibleConnectionException (errorStr.str());
      }

      // The MySQL/MariaDB connection is assumed to have been successful
      assert (oSociSession_ptr != NULL);

    } else if (iDBType == DBType::NODB) {
      // Do nothing

    } else {
      std::ostringstream errorStr;
      errorStr << "Error: the '" << iDBType.describe()
               << "' SQL database type is not supported";
      OPENTREP_LOG_ERROR (errorStr.str());
      throw SQLDatabaseTableCreationException (errorStr.str());
    }

    return oSociSession_ptr;
  }

  // //////////////////////////////////////////////////////////////////////
  bool DBManager::
  createSQLDBUser (const DBType& iDBType,
                   const SQLDBConnectionString_T& iSQLDBConnStr) {
    bool oCreationSuccessful = true;

    // DEBUG
    if (iDBType == DBType::MYSQL) {
      OPENTREP_LOG_DEBUG ("The MySQL/MariaDB database user will be created/reset");
    }

    if (iDBType == DBType::SQLITE3) {

      try {

        // Re-create the SQLite3 directory, if needed
        boost::filesystem::path lSQLiteDBFullPath (iSQLDBConnStr.begin(),
                                                   iSQLDBConnStr.end());
        boost::filesystem::path lSQLiteDBParentPath =
          lSQLiteDBFullPath.parent_path();

        // DEBUG
        OPENTREP_LOG_DEBUG ("The SQLite database file ('" << lSQLiteDBFullPath
                            << "') will be cleared and re-created");

        // Delete the SQL database/file and its directory
        boost::filesystem::remove_all (lSQLiteDBFullPath);

        // Re-create the SQLite3 directory
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
        errorStr << "Error when trying to create " << iSQLDBConnStr
                 << " SQLite3 database file: " << lException.what();
        errorStr << ". Check that the program has got write permission on the "
                 << "corresponding parent directories.";
        OPENTREP_LOG_ERROR (errorStr.str());
        throw SQLDatabaseFileCannotBeCreatedException (errorStr.str());
      }
      
    } else if (iDBType == DBType::MYSQL) {

      try {

        // Connect to the SQL database/file
        soci::session* lSociSession_ptr = initSQLDBSession (iDBType,
                                                            iSQLDBConnStr);
        if (lSociSession_ptr == NULL) {
          oCreationSuccessful = false;
          return oCreationSuccessful;
        }
        assert (lSociSession_ptr != NULL);
        soci::session& lSociSession = *lSociSession_ptr;

        /**
         * SQL DDL (Data Definition Language) queries:
         * -------------------------------------------
         grant SELECT, INSERT, UPDATE, DELETE, CREATE, DROP, FILE, INDEX, 
         ALTER, CREATE TEMPORARY TABLES, CREATE VIEW, EVENT, TRIGGER, SHOW VIEW, 
         CREATE ROUTINE, ALTER ROUTINE, EXECUTE ON *.* 
         to 'trep'@'localhost' identified by 'trep';

         grant SELECT, INSERT, UPDATE, DELETE, CREATE, DROP, FILE, INDEX, 
         ALTER, CREATE TEMPORARY TABLES, CREATE VIEW, EVENT, TRIGGER, SHOW VIEW, 
         CREATE ROUTINE, ALTER ROUTINE, EXECUTE ON *.* 
         to 'trep'@'%' identified by 'trep';

         flush privileges;

         drop database if exists trep_trep;
         create database if not exists trep_trep
           default character set utf8
           collate utf8_unicode_ci;
        */

        //
        std::ostringstream lSQLGrantTrepLocalStr;
        lSQLGrantTrepLocalStr
          << "grant SELECT, INSERT, UPDATE, DELETE, CREATE, DROP, FILE, INDEX,";
        lSQLGrantTrepLocalStr
          << " ALTER, CREATE TEMPORARY TABLES, CREATE VIEW, EVENT, TRIGGER, SHOW VIEW,";
        lSQLGrantTrepLocalStr
          << " CREATE ROUTINE, ALTER ROUTINE, EXECUTE ON *.*";
        lSQLGrantTrepLocalStr << " to 'trep'@'localhost' identified by 'trep';";
        lSociSession << lSQLGrantTrepLocalStr.str();
        //
        std::ostringstream lSQLGrantTrepAllStr;
        lSQLGrantTrepAllStr
          << "grant SELECT, INSERT, UPDATE, DELETE, CREATE, DROP, FILE, INDEX,";
        lSQLGrantTrepAllStr
          << " ALTER, CREATE TEMPORARY TABLES, CREATE VIEW, EVENT, TRIGGER, SHOW VIEW,";
        lSQLGrantTrepAllStr << " CREATE ROUTINE, ALTER ROUTINE, EXECUTE ON *.*";
        lSQLGrantTrepAllStr << " to 'trep'@'%' identified by 'trep';";
        lSociSession << lSQLGrantTrepAllStr.str();
        //
        std::ostringstream lSQLFlushPrivilegesStr;
        lSQLFlushPrivilegesStr << "flush privileges;";
        lSociSession << lSQLFlushPrivilegesStr.str();
        //
        std::ostringstream lSQLDropDBStr;
        lSQLDropDBStr << "drop database if exists trep_trep;";
        lSociSession << lSQLDropDBStr.str();
        //
        std::ostringstream lSQLCreateDBStr;
        lSQLCreateDBStr << "create database if not exists trep_trep";
        lSQLCreateDBStr << " default character set utf8";
        lSQLCreateDBStr << " collate utf8_unicode_ci;";
        lSociSession << lSQLCreateDBStr.str();

      } catch (soci::mysql_soci_error const& lSociException) {
        oCreationSuccessful = false;
        std::ostringstream errorStr;
        errorStr << "Error when trying to create MySQL/MariaDB 'trep' user "
                 << "and 'trep_trep' database: " << lSociException.what();
        OPENTREP_LOG_ERROR (errorStr.str());
        std::cerr << errorStr.str() << std::endl;

      } catch (std::exception const& lException) {
        oCreationSuccessful = false;
        std::ostringstream errorStr;
        errorStr << "Error when trying to create MySQL/MariaDB 'trep' user "
                 << "and 'trep_trep' database: " << lException.what();
        OPENTREP_LOG_ERROR (errorStr.str());
        throw SQLDatabaseUserCreationException (errorStr.str());
      }

    } else if (iDBType == DBType::NODB || iDBType == DBType::SQLITE3) {
      // Do nothing
    }

    return oCreationSuccessful;
  }

  // //////////////////////////////////////////////////////////////////////
  void DBManager::createSQLDBTables (soci::session& ioSociSession) {
    const std::string& lDBName = ioSociSession.get_backend_name();
    const DBType lDBType (lDBName);

    // DEBUG
    if (!(lDBType == DBType::NODB)) {
      OPENTREP_LOG_DEBUG ("The tables of the " << lDBType.describe()
                          << " SQL database/file will be created/reset");
    }

    if (lDBType == DBType::SQLITE3) {

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

    } else if (lDBType == DBType::MYSQL) {

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

    } else if (lDBType == DBType::NODB) {
      // Do nothing

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
    const DBType lDBType (lDBName);

    // DEBUG
    if (!(lDBType == DBType::NODB)) {
      OPENTREP_LOG_DEBUG ("The indexes of the " << lDBType.describe()
                          << " SQL database/file will be created/reset");
    }

    if (lDBType == DBType::SQLITE3) {

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

    } else if (lDBType == DBType::MYSQL) {

      try {

        /**
         * SQL DDL (Data Definition Language) queries for MySQLMariaDB:
         * ------------------------------------------------------------
         -- alter table ori_por add primary key (pk);
         alter table ori_por add unique index ori_por_pk (pk asc);
         alter table ori_por add index ori_por_iata_code (iata_code asc);
         alter table ori_por add index ori_por_iata_date (iata_code asc, date_from asc, date_until asc);
         alter table ori_por add index ori_por_icao_code (icao_code asc);
         alter table ori_por add index ori_por_geonameid (geoname_id asc);
        */

        ioSociSession
          << "alter table ori_por add unique index ori_por_pk (pk asc);";
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
        errorStr << "Error when trying to create MySQL/MariaDB indexes: "
                 << lException.what();
        OPENTREP_LOG_ERROR (errorStr.str());
        throw SQLDatabaseIndexCreationException (errorStr.str());
      }

    } else if (lDBType == DBType::NODB) {
      // Do nothing

    } else {
      std::ostringstream errorStr;
      errorStr << "Error: the '" << lDBName
               << "' SQL database type is not supported";
      OPENTREP_LOG_ERROR (errorStr.str());
      throw SQLDatabaseIndexCreationException (errorStr.str());
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
      errorStr
        << "Error in the 'select serialised_place from ori_por' SQL request: "
        << lException.what();
      OPENTREP_LOG_ERROR (errorStr.str());
      throw SQLDatabaseException (errorStr.str());
    }

    //
    return oSerialisedPlaceStr;
  }

  // //////////////////////////////////////////////////////////////////////
  void DBManager::
  prepareSelectBlobOnIataCodeStatement (soci::session& ioSociSession,
                                        soci::statement& ioSelectStatement,
                                        const IATACode_T& iIataCode,
                                        std::string& ioSerialisedPlaceStr) {
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
         soci::into (ioSerialisedPlaceStr),
         soci::use (static_cast<std::string>(iIataCode)));

      // Execute the SQL query
      ioSelectStatement.execute();

    } catch (std::exception const& lException) {
      std::ostringstream errorStr;
      errorStr
        << "Error in the 'select serialised_place from ori_por' SQL request: "
        << lException.what();
      OPENTREP_LOG_ERROR (errorStr.str());
      throw SQLDatabaseException (errorStr.str());
    }
  }

  // //////////////////////////////////////////////////////////////////////
  void DBManager::
  prepareSelectBlobOnIcaoCodeStatement (soci::session& ioSociSession,
                                        soci::statement& ioSelectStatement,
                                        const ICAOCode_T& iIcaoCode,
                                        std::string& ioSerialisedPlaceStr) {
    std::string oSerialisedPlaceStr;
  
    try {
    
      // Instanciate a SQL statement (no request is performed at that stage)
      /**
         select serialised_place from ori_por where iata_code = iIataCode;
      */

      ioSelectStatement =
        (ioSociSession.prepare
         << "select serialised_place from ori_por "
         << "where icao_code = :place_icao_code",
         soci::into (ioSerialisedPlaceStr),
         soci::use (static_cast<std::string>(iIcaoCode)));

      // Execute the SQL query
      ioSelectStatement.execute();

    } catch (std::exception const& lException) {
      std::ostringstream errorStr;
      errorStr
        << "Error in the 'select serialised_place from ori_por' SQL request: "
        << lException.what();
      OPENTREP_LOG_ERROR (errorStr.str());
      throw SQLDatabaseException (errorStr.str());
    }
  }

  // //////////////////////////////////////////////////////////////////////
  void DBManager::
  prepareSelectBlobOnFaaCodeStatement (soci::session& ioSociSession,
                                       soci::statement& ioSelectStatement,
                                       const FAACode_T& iFaaCode,
                                       std::string& ioSerialisedPlaceStr) {
    std::string oSerialisedPlaceStr;
  
    try {
    
      // Instanciate a SQL statement (no request is performed at that stage)
      /**
         select serialised_place from ori_por where iata_code = iIataCode;
      */

      ioSelectStatement =
        (ioSociSession.prepare
         << "select serialised_place from ori_por "
         << "where faa_code = :place_faa_code",
         soci::into (ioSerialisedPlaceStr),
         soci::use (static_cast<std::string>(iFaaCode)));

      // Execute the SQL query
      ioSelectStatement.execute();

    } catch (std::exception const& lException) {
      std::ostringstream errorStr;
      errorStr
        << "Error in the 'select serialised_place from ori_por' SQL request: "
        << lException.what();
      OPENTREP_LOG_ERROR (errorStr.str());
      throw SQLDatabaseException (errorStr.str());
    }
  }

  // //////////////////////////////////////////////////////////////////////
  void DBManager::
  prepareSelectBlobOnPlaceGeoIDStatement (soci::session& ioSociSession,
                                          soci::statement& ioSelectStatement,
                                          const GeonamesID_T& iGeonameID,
                                          std::string& ioSerialisedPlaceStr) {
    std::string oSerialisedPlaceStr;
  
    try {
    
      // Instanciate a SQL statement (no request is performed at that stage)
      /**
         select serialised_place from ori_por where iata_code = iIataCode;
      */

      ioSelectStatement =
        (ioSociSession.prepare
         << "select serialised_place from ori_por "
         << "where geoname_id = :place_geoname_id",
         soci::into (ioSerialisedPlaceStr),
         soci::use (iGeonameID));

      // Execute the SQL query
      ioSelectStatement.execute();

    } catch (std::exception const& lException) {
      std::ostringstream errorStr;
      errorStr
        << "Error in the 'select serialised_place from ori_por' SQL request: "
        << lException.what();
      OPENTREP_LOG_ERROR (errorStr.str());
      throw SQLDatabaseException (errorStr.str());
    }
  }

  // //////////////////////////////////////////////////////////////////////
  bool DBManager::iterateOnStatement (soci::statement& ioStatement,
                                      const std::string& iSerialisedPlaceStr) {
    bool hasStillData = false;
  
    try {

      // Retrieve the next row of Place object
      hasStillData = ioStatement.fetch();
      
    } catch (std::exception const& lException) {
      std::ostringstream errorStr;
      errorStr << "Error when iterating on the SQL fetch: " << lException.what();
      errorStr << ". The current place is: " << iSerialisedPlaceStr;
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
  NbOfDBEntries_T DBManager::displayCount (soci::session& ioSociSession) {
    NbOfDBEntries_T oNbOfEntries = 0;

    try {

      /**
       * SQL DDL (Data Definition Language) queries for SQLite3:
       * -------------------------------------------------------
       select count(1) from ori_por;
      */

      ioSociSession << "select count(1) from ori_por;", soci::into(oNbOfEntries);

    } catch (std::exception const& lException) {
      std::ostringstream errorStr;
      errorStr
        << "Error when trying to count the number of rows in the ori_por table: "
        << lException.what();
      OPENTREP_LOG_ERROR (errorStr.str());
      throw SQLDatabaseIndexCreationException (errorStr.str());
    }

    return oNbOfEntries;
  }

  // //////////////////////////////////////////////////////////////////////
  NbOfDBEntries_T DBManager::
  fillInFromPORFile (const PORFilePath_T& iPORFilePath, const DBType& iDBType,
                     const SQLDBConnectionString_T& iSQLDBConnStr) {
    NbOfDBEntries_T oNbOfEntries = 0;

    // DEBUG
    if (!(iDBType == DBType::NODB)) {
      OPENTREP_LOG_DEBUG ("The " << iDBType.describe() << " SQL database/file "
                          << "will be filled thanks to the POR "
                          << "(points of reference) file");
    }

    // Connect to the SQL database/file
    soci::session* lSociSession_ptr = initSQLDBSession (iDBType, iSQLDBConnStr);
    if (lSociSession_ptr == NULL) {
      return oNbOfEntries;
    }
    assert (lSociSession_ptr != NULL);
    soci::session& lSociSession = *lSociSession_ptr;

    // Get a reference on the file stream corresponding to the POR file.
    const PORFileHelper lPORFileHelper (iPORFilePath);
    std::istream& lPORFileStream = lPORFileHelper.getFileStreamRef();

    // Open the file to be parsed
    Place& lPlace = FacPlace::instance().create();
    std::string itReadLine;
    while (std::getline (lPORFileStream, itReadLine)) {
      // Initialise the parser
      PORStringParser lStringParser (itReadLine);

      // Parse the string
      const Location& lLocation = lStringParser.generateLocation();
      //const LocationKey& lLocationKey = lLocation.getKey();

      // DEBUG
      //OPENTREP_LOG_DEBUG ("[BEF-ADD] " << lLocationKey);

      // When the line/string was relevant, create a BOM instance from
      // the Location structure.
      if (!(lLocation.getCommonName() == "NotAvailable")) {
        // Fill the Place object with the Location structure.
        lPlace.setLocation (lLocation);

        // Add the document to the SQL database
        insertPlaceInDB (lSociSession, lPlace);

        // DEBUG
        /*
        OPENTREP_LOG_DEBUG ("[AFT-ADD] " << lLocationKey
                            << ", Place: " << lPlace);
        */

        // Iteration
        ++oNbOfEntries;

        // Progress status
        if (oNbOfEntries % 1000 == 0) {
          std::cout << "Number of records inserted into the DB: "
                    << oNbOfEntries << std::endl;
        }

        // DEBUG
        OPENTREP_LOG_DEBUG ("[" << oNbOfEntries << "] " << lPlace);

        // Reset for next turn
        lPlace.resetMatrix();
        lPlace.resetIndexSets();
      }
    }

    return oNbOfEntries;
  }

  // //////////////////////////////////////////////////////////////////////
  NbOfDBEntries_T DBManager::displayAll (soci::session& ioSociSession) {
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

  // //////////////////////////////////////////////////////////////////////
  NbOfDBEntries_T DBManager::getPORByIATACode (soci::session& ioSociSession,
                                               const IATACode_T& iIataCode,
                                               LocationList_T& ioLocationList) {
    NbOfDBEntries_T oNbOfEntries = 0;

    try {

      // Prepare the SQL request corresponding to the select statement
      soci::statement lSelectStatement (ioSociSession);
      std::string lPlaceRawDataString;
      DBManager::prepareSelectBlobOnIataCodeStatement (ioSociSession,
                                                       lSelectStatement,
                                                       iIataCode,
                                                       lPlaceRawDataString);

      /**
       * Retrieve the details of the place, as well as the alternate
       * names, most often in other languages (e.g., "ru", "zh").
       */
      bool hasStillData = true;
      while (hasStillData == true) {
        hasStillData = iterateOnStatement (lSelectStatement,
                                           lPlaceRawDataString);

        if (hasStillData == true) {
          //
          ++oNbOfEntries;

          // Parse the POR details and create the corresponding
          // Location structure
          const RawDataString_T lPlaceRawData (lPlaceRawDataString);
          const Location& lLocation = Result::retrieveLocation (lPlaceRawData);

          // Add the new found location to the list
          ioLocationList.push_back (lLocation);

          // Debug
          OPENTREP_LOG_DEBUG ("[" << oNbOfEntries << "] " << lLocation);
        }
      }
      
    } catch (std::exception const& lException) {
      std::ostringstream errorStr;
      errorStr << "Error when trying to retrieve a POR for " << iIataCode
               << " from the SQL database: " << lException.what();
      OPENTREP_LOG_ERROR (errorStr.str());
      throw SQLDatabaseException (errorStr.str());
    }

    //
    return oNbOfEntries;
  }

  // //////////////////////////////////////////////////////////////////////
  NbOfDBEntries_T DBManager::getPORByICAOCode (soci::session& ioSociSession,
                                               const ICAOCode_T& iIcaoCode,
                                               LocationList_T& ioLocationList) {
    NbOfDBEntries_T oNbOfEntries = 0;

    try {

      // Prepare the SQL request corresponding to the select statement
      soci::statement lSelectStatement (ioSociSession);
      std::string lPlaceRawDataString;
      DBManager::prepareSelectBlobOnIcaoCodeStatement (ioSociSession,
                                                       lSelectStatement,
                                                       iIcaoCode,
                                                       lPlaceRawDataString);

      /**
       * Retrieve the details of the place, as well as the alternate
       * names, most often in other languages (e.g., "ru", "zh").
       */
      bool hasStillData = true;
      while (hasStillData == true) {
        hasStillData = iterateOnStatement (lSelectStatement,
                                           lPlaceRawDataString);

        if (hasStillData == true) {
          //
          ++oNbOfEntries;

          // Parse the POR details and create the corresponding
          // Location structure
          const RawDataString_T lPlaceRawData (lPlaceRawDataString);
          const Location& lLocation = Result::retrieveLocation (lPlaceRawData);

          // Add the new found location to the list
          ioLocationList.push_back (lLocation);

          // Debug
          OPENTREP_LOG_DEBUG ("[" << oNbOfEntries << "] " << lLocation);
        }
      }
      
    } catch (std::exception const& lException) {
      std::ostringstream errorStr;
      errorStr << "Error when trying to retrieve a POR for " << iIcaoCode
               << " from the SQL database: " << lException.what();
      OPENTREP_LOG_ERROR (errorStr.str());
      throw SQLDatabaseException (errorStr.str());
    }

    //
    return oNbOfEntries;
  }

  // //////////////////////////////////////////////////////////////////////
  NbOfDBEntries_T DBManager::getPORByFAACode (soci::session& ioSociSession,
                                              const FAACode_T& iFaaCode,
                                              LocationList_T& ioLocationList) {
    NbOfDBEntries_T oNbOfEntries = 0;

    try {

      // Prepare the SQL request corresponding to the select statement
      soci::statement lSelectStatement (ioSociSession);
      std::string lPlaceRawDataString;
      DBManager::prepareSelectBlobOnFaaCodeStatement (ioSociSession,
                                                      lSelectStatement,
                                                      iFaaCode,
                                                      lPlaceRawDataString);

      /**
       * Retrieve the details of the place, as well as the alternate
       * names, most often in other languages (e.g., "ru", "zh").
       */
      bool hasStillData = true;
      while (hasStillData == true) {
        hasStillData = iterateOnStatement (lSelectStatement,
                                           lPlaceRawDataString);

        if (hasStillData == true) {
          //
          ++oNbOfEntries;

          // Parse the POR details and create the corresponding
          // Location structure
          const RawDataString_T lPlaceRawData (lPlaceRawDataString);
          const Location& lLocation = Result::retrieveLocation (lPlaceRawData);

          // Add the new found location to the list
          ioLocationList.push_back (lLocation);

          // Debug
          OPENTREP_LOG_DEBUG ("[" << oNbOfEntries << "] " << lLocation);
        }
      }
      
    } catch (std::exception const& lException) {
      std::ostringstream errorStr;
      errorStr << "Error when trying to retrieve a POR for " << iFaaCode
               << " from the SQL database: " << lException.what();
      OPENTREP_LOG_ERROR (errorStr.str());
      throw SQLDatabaseException (errorStr.str());
    }

    //
    return oNbOfEntries;
  }

  // //////////////////////////////////////////////////////////////////////
  NbOfDBEntries_T DBManager::getPORByGeonameID (soci::session& ioSociSession,
                                                const GeonamesID_T& iGeonameID,
                                                LocationList_T& ioLocationList) {
    NbOfDBEntries_T oNbOfEntries = 0;

    try {

      // Prepare the SQL request corresponding to the select statement
      soci::statement lSelectStatement (ioSociSession);
      std::string lPlaceRawDataString;
      DBManager::prepareSelectBlobOnPlaceGeoIDStatement (ioSociSession,
                                                         lSelectStatement,
                                                         iGeonameID,
                                                         lPlaceRawDataString);

      /**
       * Retrieve the details of the place, as well as the alternate
       * names, most often in other languages (e.g., "ru", "zh").
       */
      bool hasStillData = true;
      while (hasStillData == true) {
        hasStillData = iterateOnStatement (lSelectStatement,
                                           lPlaceRawDataString);

        if (hasStillData == true) {
          //
          ++oNbOfEntries;

          // Parse the POR details and create the corresponding
          // Location structure
          const RawDataString_T lPlaceRawData (lPlaceRawDataString);
          const Location& lLocation = Result::retrieveLocation (lPlaceRawData);

          // Add the new found location to the list
          ioLocationList.push_back (lLocation);

          // Debug
          OPENTREP_LOG_DEBUG ("[" << oNbOfEntries << "] " << lLocation);
        }
      }
      
    } catch (std::exception const& lException) {
      std::ostringstream errorStr;
      errorStr << "Error when trying to retrieve a POR for " << iGeonameID
               << " from the SQL database: " << lException.what();
      OPENTREP_LOG_ERROR (errorStr.str());
      throw SQLDatabaseException (errorStr.str());
    }

    //
    return oNbOfEntries;
  }

}
