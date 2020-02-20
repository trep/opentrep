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
#include <boost/algorithm/string.hpp>
// SOCI
#include <soci/soci.h>
#include <soci/sqlite3/soci-sqlite3.h>
#include <soci/mysql/soci-mysql.h>
// OpenTrep
#include <opentrep/Location.hpp>
#include <opentrep/basic/BasConst_OPENTREP_Service.hpp>
#include <opentrep/basic/Utilities.hpp>
#include <opentrep/bom/World.hpp>
#include <opentrep/bom/Place.hpp>
#include <opentrep/bom/Result.hpp>
#include <opentrep/bom/PORFileHelper.hpp>
#include <opentrep/bom/PORParserHelper.hpp>
#include <opentrep/factory/FacPlace.hpp>
#include <opentrep/dbadaptor/DbaPlace.hpp>
#include <opentrep/command/DBManager.hpp>
#include <opentrep/command/FileManager.hpp>
#include <opentrep/service/Logger.hpp>

namespace OPENTREP {

  // //////////////////////////////////////////////////////////////////////
  bool DBManager::
  createSQLDBUser (const DBType& iDBType,
                   const SQLDBConnectionString_T& iSQLDBConnStr,
                   const DeploymentNumber_T& iDeploymentNumber) {
    bool oCreationSuccessful = true;

    // DEBUG
    if (iDBType == DBType::MYSQL) {
      OPENTREP_LOG_DEBUG ("The MySQL/MariaDB database user will be created/reset");
    }

    if (iDBType == DBType::SQLITE3) {

      try {

        // Retrieve the full file-path of the SQLite3 directory
        boost::filesystem::path lSQLiteDBFullPath (iSQLDBConnStr.begin(),
                                                   iSQLDBConnStr.end());
        // Retrieve the directory hosting the SQLite3 database
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
        //boost::filesystem::path lSQLiteDBFilename=lSQLiteDBFullPath.filename();
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

      // DEBUG
      OPENTREP_LOG_DEBUG ("The SQLite database ('" << iSQLDBConnStr
                          << "') has been cleared and re-created");
      
    } else if (iDBType == DBType::MYSQL) {
      // DEBUG
      OPENTREP_LOG_DEBUG ("Create the '"
                          << DEFAULT_OPENTREP_MYSQL_DB_USER
                          << "' user and '" << DEFAULT_OPENTREP_MYSQL_DB_DBNAME
                          << iDeploymentNumber
                          << "' database in MySQL/MariaDB ('" << iSQLDBConnStr
                          << "')");

      // Connection to the MySQL/MariaDB database
      soci::session* lSociSession_ptr = NULL;      
      try {

        // Connect to the SQL database/file
        lSociSession_ptr = initSQLDBSession (iDBType, iSQLDBConnStr);
        if (lSociSession_ptr == NULL) {
          oCreationSuccessful = false;
          return oCreationSuccessful;
        }

      } catch (soci::mysql_soci_error const& lSociException) {
        std::ostringstream errorStr;
        errorStr << "SOCI-related error when trying to connect to the "
                 << "MySQL/MariaDB database ('" << iSQLDBConnStr
                 << "'). SOCI error message: " << lSociException.what();
        OPENTREP_LOG_ERROR (errorStr.str());
        std::cerr << errorStr.str() << std::endl;
        oCreationSuccessful = false;
        return oCreationSuccessful;
      }
      assert (lSociSession_ptr != NULL);
      soci::session& lSociSession = *lSociSession_ptr;

      /**
       * SQL DDL (Data Definition Language) queries:
       * -------------------------------------------
       -- Universal procedure for 'drop user if exists', as that feature
       -- has been available on MySQL from 5.6 only (and CentOS 6
       -- has MySQL 5.0): https://stackoverflow.com/a/12502107/798053

       drop user 'trep'@'localhost'; drop user 'trep'@'%';
         
       create user 'trep'@'localhost' identified by 'trep';
       grant SELECT, INSERT, UPDATE, DELETE, CREATE, DROP, FILE, INDEX, 
       ALTER, CREATE TEMPORARY TABLES, CREATE VIEW, EVENT, TRIGGER, SHOW VIEW, 
       CREATE ROUTINE, ALTER ROUTINE, EXECUTE ON *.* to 'trep'@'localhost';

       create user 'trep'@'%' identified by 'trep';
       grant SELECT, INSERT, UPDATE, DELETE, CREATE, DROP, FILE, INDEX, 
       ALTER, CREATE TEMPORARY TABLES, CREATE VIEW, EVENT, TRIGGER, SHOW VIEW, 
       CREATE ROUTINE, ALTER ROUTINE, EXECUTE ON *.* to 'trep'@'%';

       flush privileges;

       -- <N> is the deployment number
       drop database if exists trep_trep<N>;
       create database if not exists trep_trep<N>
       default character set utf8
       collate utf8_unicode_ci;
      */

      try {
        // Drop user 'trep'@'localhost'
        std::ostringstream lSQLDropTrepLocalStr;
        lSQLDropTrepLocalStr << "drop user '"
                             << DEFAULT_OPENTREP_MYSQL_DB_USER << "'@'"
                             << DEFAULT_OPENTREP_MYSQL_DB_HOST << "';";
        lSociSession << lSQLDropTrepLocalStr.str();
        
        // Drop user 'trep'@'%'
        std::ostringstream lSQLDropTrepAllStr;
        lSQLDropTrepAllStr << "drop user '"
                           << DEFAULT_OPENTREP_MYSQL_DB_USER << "'@'%';";
        lSociSession << lSQLDropTrepAllStr.str();

      } catch (soci::mysql_soci_error const& lSociException) {
        std::ostringstream issueStr;
        issueStr << "Issue when trying to drop MySQL/MariaDB '"
                 << DEFAULT_OPENTREP_MYSQL_DB_USER << "' user. "
                 << "Most probably the user did not exist before. " << std::endl
                 << "SOCI error message: " << lSociException.what() << std::endl
                 << "The database users should however be created without "
                 << "any issue ";
        OPENTREP_LOG_DEBUG (issueStr.str());
        std::cout << issueStr.str() << std::endl;
      }

      try {
        // Create user 'trep'@'localhost'
        std::ostringstream lSQLCreateTrepLocalStr;
        lSQLCreateTrepLocalStr << "create user '"
                               << DEFAULT_OPENTREP_MYSQL_DB_USER << "'@'"
                               << DEFAULT_OPENTREP_MYSQL_DB_HOST << "' ";
        lSQLCreateTrepLocalStr << "identified by '"
                               << DEFAULT_OPENTREP_MYSQL_DB_PASSWD << "';";
        lSociSession << lSQLCreateTrepLocalStr.str();

        // Grant privileges to 'trep'@'localhost'
        std::ostringstream lSQLGrantTrepLocalStr;
        lSQLGrantTrepLocalStr << "grant SELECT, INSERT, UPDATE, DELETE, ";
        lSQLGrantTrepLocalStr << "CREATE, DROP, FILE, INDEX, ALTER, ";
        lSQLGrantTrepLocalStr << "CREATE TEMPORARY TABLES, CREATE VIEW, EVENT, ";
        lSQLGrantTrepLocalStr << "TRIGGER, SHOW VIEW, CREATE ROUTINE, ";
        lSQLGrantTrepLocalStr << "ALTER ROUTINE, EXECUTE ON *.*";
        lSQLGrantTrepLocalStr << " to '" << DEFAULT_OPENTREP_MYSQL_DB_USER
                              << "'@'" << DEFAULT_OPENTREP_MYSQL_DB_HOST << "';";
        lSociSession << lSQLGrantTrepLocalStr.str();

        // Create user 'trep'@'%'
        std::ostringstream lSQLCreateTrepAllStr;
        lSQLCreateTrepAllStr << "create user '"
                             << DEFAULT_OPENTREP_MYSQL_DB_USER
                             << "'@'%' identified by '"
                             << DEFAULT_OPENTREP_MYSQL_DB_PASSWD << "';";
        lSociSession << lSQLCreateTrepAllStr.str();

        // Grant privileges to 'trep'@'%'
        std::ostringstream lSQLGrantTrepAllStr;
        lSQLGrantTrepAllStr << "grant SELECT, INSERT, UPDATE, DELETE, ";
        lSQLGrantTrepAllStr << "CREATE, DROP, FILE, INDEX, ALTER, ";
        lSQLGrantTrepAllStr << "CREATE TEMPORARY TABLES, CREATE VIEW, EVENT, ";
        lSQLGrantTrepAllStr << "TRIGGER, SHOW VIEW, CREATE ROUTINE, ";
        lSQLGrantTrepAllStr << "ALTER ROUTINE, EXECUTE ON *.*";
        lSQLGrantTrepAllStr << " to '" << DEFAULT_OPENTREP_MYSQL_DB_USER
                            << "'@'%';";
        lSociSession << lSQLGrantTrepAllStr.str();

        // Flush privileges
        std::ostringstream lSQLFlushPrivilegesStr;
        lSQLFlushPrivilegesStr << "flush privileges;";
        lSociSession << lSQLFlushPrivilegesStr.str();

      } catch (soci::mysql_soci_error const& lSociException) {
        oCreationSuccessful = false;
        std::ostringstream errorStr;
        errorStr << "SOCI-related error when trying to create MySQL/MariaDB "
                 << "'" << DEFAULT_OPENTREP_MYSQL_DB_USER
                 << "' user. Error message: " << lSociException.what();
        OPENTREP_LOG_ERROR (errorStr.str());
        std::cerr << errorStr.str() << std::endl;
        oCreationSuccessful = false;
        return oCreationSuccessful;
      }

      /**
       * On some MySQL server configurations (eg, MacOS), UTF8 just handles
       * 3-byte character. To support 4-byte character on those platforms,
       * 'utf8mb4' is needed, but is available only from MySQL 5.5. Moreover,
       * 'utf8' seems enough on MySQL servers on CentOS.
       * So, we first try with 'utf8mb4', and if it does not work, we try
       * 'utf8'.
       */
      try {
        // Drop the 'trep_trep' database, if existing
        std::ostringstream lSQLDropDBStr;
        lSQLDropDBStr << "drop database if exists "
                      << DEFAULT_OPENTREP_MYSQL_DB_DBNAME << iDeploymentNumber
                      << ";";
        lSociSession << lSQLDropDBStr.str();

        // Create the 'trep_trep' database
        std::ostringstream lSQLCreateDBStr;
        lSQLCreateDBStr << "create database if not exists "
                        << DEFAULT_OPENTREP_MYSQL_DB_DBNAME << iDeploymentNumber;
        lSQLCreateDBStr << " default character set utf8mb4";
        lSQLCreateDBStr << " collate utf8mb4_unicode_ci;";
        lSociSession << lSQLCreateDBStr.str();

      } catch (soci::mysql_soci_error const& lSociException) {
        oCreationSuccessful = false;
        std::ostringstream errorStr;
        errorStr << "SOCI-related error when trying to create MySQL/MariaDB "
                 << "'" << DEFAULT_OPENTREP_MYSQL_DB_DBNAME << iDeploymentNumber
                 << "' database with 'utf8mb4' as character set. "
                 << "Error message: " << lSociException.what();
        OPENTREP_LOG_ERROR (errorStr.str());
        std::cerr << errorStr.str() << std::endl;
      }
      if (oCreationSuccessful == false) {
        try {
          // Drop the 'trep_trep' database, if existing
          std::ostringstream lSQLDropDBStr;
          lSQLDropDBStr << "drop database if exists "
                        << DEFAULT_OPENTREP_MYSQL_DB_DBNAME << iDeploymentNumber
                        << ";";
          lSociSession << lSQLDropDBStr.str();

          // Create the 'trep_trep' database
          std::ostringstream lSQLCreateDBStr;
          lSQLCreateDBStr << "create database if not exists "
                          << DEFAULT_OPENTREP_MYSQL_DB_DBNAME
                          << iDeploymentNumber;
          lSQLCreateDBStr << " default character set utf8";
          lSQLCreateDBStr << " collate utf8_unicode_ci;";
          lSociSession << lSQLCreateDBStr.str();
          
        } catch (soci::mysql_soci_error const& lSociException) {
          oCreationSuccessful = false;
          std::ostringstream errorStr;
          errorStr << "SOCI-related error when trying to create MySQL/MariaDB "
                   << "'" << DEFAULT_OPENTREP_MYSQL_DB_DBNAME
                   << iDeploymentNumber
                   << "' database. Error message: " << lSociException.what();
          OPENTREP_LOG_ERROR (errorStr.str());
          std::cerr << errorStr.str() << std::endl;
          oCreationSuccessful = false;
          return oCreationSuccessful;
        }
      }

      // DEBUG
      OPENTREP_LOG_DEBUG ("The '" << DEFAULT_OPENTREP_MYSQL_DB_USER
                          << "' user and '" << DEFAULT_OPENTREP_MYSQL_DB_DBNAME
                          << iDeploymentNumber
                          << "' database have been created in MySQL/MariaDB ('"
                          << iSQLDBConnStr << "')");
      
    } else if (iDBType == DBType::NODB || iDBType == DBType::SQLITE3) {
      // Do nothing
    }

    return oCreationSuccessful;
  }

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

      // Check that the directory hosting the SQLite database exists
      const bool existSQLDBDir =
        FileManager::checkSQLiteDirectory (iSQLDBConnStr);
      if (existSQLDBDir == false) {
        std::ostringstream errorStr;
        errorStr << "Error when trying to connect to the '" << iSQLDBConnStr
                 << "' SQLite3 database; the directory hosting that "
                 << "database does not exist or is not readable";
        OPENTREP_LOG_ERROR (errorStr.str());
        throw SQLDatabaseImpossibleConnectionException (errorStr.str());
      }
      
      try {

        // Connect to the SQL database.
        oSociSession_ptr = new soci::session();
        assert (oSociSession_ptr != NULL);
        soci::session& lSociSession = *oSociSession_ptr;
        lSociSession.open (soci::sqlite3, iSQLDBConnStr);

        // DEBUG
        OPENTREP_LOG_DEBUG ("The SQLite3 database/file ('" << iSQLDBConnStr
                            << "') has been checked and opened");

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
        OPENTREP_LOG_DEBUG ("The " << iDBType.describe() << " database ("
                            << iSQLDBConnStr << ") is accessible");

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
  void DBManager::
  terminateSQLDBSession (const DBType& iDBType,
                         const SQLDBConnectionString_T& iSQLDBConnStr,
                         soci::session& ioSociSession) {
    // DEBUG
    if (!(iDBType == DBType::NODB)) {
      OPENTREP_LOG_DEBUG ("Connecting to the " << iDBType.describe()
                          << " SQL database/file ('" << iSQLDBConnStr << "')");
    }

    if (iDBType == DBType::SQLITE3) {
      //
      try {

        // Release the SQL database connection
        ioSociSession.close();

      } catch (std::exception const& lException) {
        std::ostringstream errorStr;
        errorStr << "Error when trying to release the connection ('"
                 << iSQLDBConnStr
                 << "') to the SQLite3 database: " << lException.what();
        OPENTREP_LOG_ERROR (errorStr.str());
        throw SQLDatabaseConnectionReleaseException (errorStr.str());
      }

    } else if (iDBType == DBType::MYSQL) {
      //
      try {

        // Release the SQL database connection
        ioSociSession.close();

      } catch (std::exception const& lException) {
        std::ostringstream errorStr;
        errorStr << "Error when trying to release the connection ('"
                 << iSQLDBConnStr
                 << "') to the MySQL/MariaDB database: " << lException.what();
        OPENTREP_LOG_ERROR (errorStr.str());
        throw SQLDatabaseConnectionReleaseException (errorStr.str());
      }

    } else if (iDBType == DBType::NODB) {
      // Do nothing

    } else {
      std::ostringstream errorStr;
      errorStr << "Error: the '" << iDBType.describe()
               << "' SQL database type is not supported";
      OPENTREP_LOG_ERROR (errorStr.str());
      throw SQLDatabaseTableCreationException (errorStr.str());
    }
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

      // DEBUG
      OPENTREP_LOG_DEBUG ("Create the optd_por table in the SQLite3 database");
        
      try {

        /**
         * SQL DDL (Data Definition Language) queries:
         * -------------------------------------------
           drop table if exists optd_por;
           create table optd_por (
           pk varchar(20) NOT NULL,
           location_type varchar(4) default NULL,
           iata_code varchar(3) default NULL,
           icao_code varchar(4) default NULL,
           faa_code varchar(4) default NULL,
           unlocode_code varchar(5) default NULL,
           uic_code int(11) default NULL,
           is_geonames varchar(1) default NULL,
           geoname_id int(11) default NULL,
           envelope_id int(11) default NULL,
           date_from date default NULL,
           date_until date default NULL,
           serialised_place varchar(12000) default NULL);
        */

        ioSociSession << "drop table if exists optd_por;";
        std::ostringstream lSQLTableCreationStr;
        lSQLTableCreationStr << "create table optd_por (";
        lSQLTableCreationStr << "pk varchar(20) NOT NULL, ";
        lSQLTableCreationStr << "location_type varchar(4) default NULL, ";
        lSQLTableCreationStr << "iata_code varchar(3) default NULL, ";
        lSQLTableCreationStr << "icao_code varchar(4) default NULL, ";
        lSQLTableCreationStr << "faa_code varchar(4) default NULL, ";
        lSQLTableCreationStr << "unlocode_code varchar(5) default NULL, ";
        lSQLTableCreationStr << "uic_code int(11) default NULL, ";
        lSQLTableCreationStr << "is_geonames varchar(1) default NULL, ";
        lSQLTableCreationStr << "geoname_id int(11) default NULL, ";
        lSQLTableCreationStr << "envelope_id int(11) default NULL, ";
        lSQLTableCreationStr << "date_from date default NULL, ";
        lSQLTableCreationStr << "date_until date default NULL, ";
        lSQLTableCreationStr << "serialised_place varchar(12000) default NULL);";
        ioSociSession << lSQLTableCreationStr.str();

      } catch (std::exception const& lException) {
        std::ostringstream errorStr;
        errorStr << "Error when trying to create SQLite3 tables: "
                 << lException.what();
        OPENTREP_LOG_ERROR (errorStr.str());
        throw SQLDatabaseTableCreationException (errorStr.str());
      }

      // DEBUG
      OPENTREP_LOG_DEBUG ("The optd_por table has been created in the SQLite3 database");
        
    } else if (lDBType == DBType::MYSQL) {

      // DEBUG
      OPENTREP_LOG_DEBUG ("Create the optd_por table in the MySQL database");
        
      try {

        /**
         * SQL DDL (Data Definition Language) queries:
         * -------------------------------------------
           drop table if exists optd_por;
           create table optd_por (
           pk varchar(20) NOT NULL,
           location_type varchar(4) default NULL,
           iata_code varchar(3) default NULL,
           icao_code varchar(4) default NULL,
           faa_code varchar(4) default NULL,
           unlocode_code varchar(5) default NULL,
           uic_code int(11) default NULL,
           is_geonames varchar(1) default NULL,
           geoname_id int(11) default NULL,
           envelope_id int(11) default NULL,
           date_from date default NULL,
           date_until date default NULL,
           serialised_place varchar(12000) default NULL);
        */

        ioSociSession << "drop table if exists optd_por;";
        std::ostringstream lSQLTableCreationStr;
        lSQLTableCreationStr << "create table optd_por (";
        lSQLTableCreationStr << "pk varchar(20) NOT NULL, ";
        lSQLTableCreationStr << "location_type varchar(4) default NULL, ";
        lSQLTableCreationStr << "iata_code varchar(3) default NULL, ";
        lSQLTableCreationStr << "icao_code varchar(4) default NULL, ";
        lSQLTableCreationStr << "faa_code varchar(4) default NULL, ";
        lSQLTableCreationStr << "unlocode_code varchar(5) default NULL, ";
        lSQLTableCreationStr << "uic_code int(11) default NULL, ";
        lSQLTableCreationStr << "is_geonames varchar(1) default NULL, ";
        lSQLTableCreationStr << "geoname_id int(11) default NULL, ";
        lSQLTableCreationStr << "envelope_id int(11) default NULL, ";
        lSQLTableCreationStr << "date_from date default NULL, ";
        lSQLTableCreationStr << "date_until date default NULL, ";
        lSQLTableCreationStr << "serialised_place varchar(12000) default NULL); ";
        ioSociSession << lSQLTableCreationStr.str();

      } catch (std::exception const& lException) {
        std::ostringstream errorStr;
        errorStr << "Error when trying to create MySQL/MariaDB tables: "
                 << lException.what();
        OPENTREP_LOG_ERROR (errorStr.str());
        throw SQLDatabaseTableCreationException (errorStr.str());
      }

      // DEBUG
      OPENTREP_LOG_DEBUG ("The optd_por table has been created in the MySQL database");
        
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

      // DEBUG
      OPENTREP_LOG_DEBUG ("Create the indices for the SQLite3 database");
        
      try {

        /**
         * SQL DDL (Data Definition Language) queries for SQLite3:
         * -------------------------------------------------------
         create index optd_por_iata_code on optd_por (iata_code);
         create index optd_por_iata_date on optd_por (iata_code, date_from, date_until);
         create index optd_por_icao_code on optd_por (icao_code);
         create index optd_por_geonameid on optd_por (geoname_id);
         create index optd_por_unlocode_code on optd_por (unlocode_code);
         create index optd_por_uic_code on optd_por (uic_code);
        */

        ioSociSession
          << "create index optd_por_iata_code on optd_por (iata_code);";
        ioSociSession
          << "create index optd_por_iata_date on optd_por (iata_code, date_from, date_until);";
        ioSociSession
          << "create index optd_por_icao_code on optd_por (icao_code);";
        ioSociSession
          << "create index optd_por_geonameid on optd_por (geoname_id);";
        ioSociSession
          << "create index optd_por_unlocode_code on optd_por (unlocode_code);";
        ioSociSession
          << "create index optd_por_uic_code on optd_por (uic_code);";

      } catch (std::exception const& lException) {
        std::ostringstream errorStr;
        errorStr << "Error when trying to create SQLite3 indexes: "
                 << lException.what();
        OPENTREP_LOG_ERROR (errorStr.str());
        throw SQLDatabaseIndexCreationException (errorStr.str());
      }

      // DEBUG
      OPENTREP_LOG_DEBUG ("The indices have been created "
                          "for the SQLite3 database");
        
    } else if (lDBType == DBType::MYSQL) {

      // DEBUG
      OPENTREP_LOG_DEBUG ("Create the indices for the MySQL database");
        
      try {

        /**
         * SQL DDL (Data Definition Language) queries for MySQL/MariaDB:
         * -------------------------------------------------------------
         -- alter table optd_por add primary key (pk);
         alter table optd_por add unique index optd_por_pk (pk asc);
         alter table optd_por add index optd_por_iata_code (iata_code asc);
         alter table optd_por add index optd_por_iata_date (iata_code asc, date_from asc, date_until asc);
         alter table optd_por add index optd_por_icao_code (icao_code asc);
         alter table optd_por add index optd_por_geonameid (geoname_id asc);
         alter table optd_por add index optd_por_unlocode_code (unlocode_code asc);
         alter table optd_por add index optd_por_uic_code (uic_code asc);
        */

        ioSociSession
          << "alter table optd_por add unique index optd_por_pk (pk asc);";
        ioSociSession
          << "alter table optd_por add index optd_por_iata_code (iata_code asc);";
        ioSociSession
          << "alter table optd_por add index optd_por_iata_date (iata_code asc, date_from asc, date_until asc);";
        ioSociSession
          << "alter table optd_por add index optd_por_icao_code (icao_code asc);";
        ioSociSession
          << "alter table optd_por add index optd_por_geonameid (geoname_id asc);";
        ioSociSession
          << "alter table optd_por add index optd_por_unlocode_code (unlocode_code asc);";
        ioSociSession
          << "alter table optd_por add index optd_por_uic_code (uic_code asc);";

      } catch (std::exception const& lException) {
        std::ostringstream errorStr;
        errorStr << "Error when trying to create MySQL/MariaDB indices: "
                 << lException.what();
        OPENTREP_LOG_ERROR (errorStr.str());
        throw SQLDatabaseIndexCreationException (errorStr.str());
      }

      // DEBUG
      OPENTREP_LOG_DEBUG ("The indices have been created "
                          "for the MySQL/MariaDB database");
      
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
         select serialised_place from optd_por;
      */

      ioSelectStatement = (ioSociSession.prepare
                           << "select serialised_place from optd_por",
                           soci::into (oSerialisedPlaceStr));

      // Execute the SQL query
      ioSelectStatement.execute();

    } catch (std::exception const& lException) {
      std::ostringstream errorStr;
      errorStr
        << "Error in the 'select serialised_place from optd_por' SQL request: "
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
                                        const std::string& iIataCode,
                                        std::string& ioSerialisedPlaceStr) {
  
    try {
    
      // Instanciate a SQL statement (no request is performed at that stage)
      /**
         select serialised_place from optd_por where iata_code = iIataCode;
      */
      ioSelectStatement = (ioSociSession.prepare
                           << "select serialised_place from optd_por "
                           << "where iata_code = :place_iata_code",
                           soci::use (iIataCode),
                           soci::into (ioSerialisedPlaceStr));

      // Execute the SQL query
      ioSelectStatement.execute();

    } catch (std::exception const& lException) {
      std::ostringstream errorStr;
      errorStr
        << "Error in the 'select serialised_place from optd_por' SQL request: "
        << lException.what();
      OPENTREP_LOG_ERROR (errorStr.str());
      throw SQLDatabaseException (errorStr.str());
    }
  }

  // //////////////////////////////////////////////////////////////////////
  void DBManager::
  prepareSelectBlobOnIcaoCodeStatement (soci::session& ioSociSession,
                                        soci::statement& ioSelectStatement,
                                        const std::string& iIcaoCode,
                                        std::string& ioSerialisedPlaceStr) {
  
    try {
    
      // Instanciate a SQL statement (no request is performed at that stage)
      /**
         select serialised_place from optd_por where icao_code = iIcaoCode;
      */
      ioSelectStatement = (ioSociSession.prepare
                           << "select serialised_place from optd_por "
                           << "where icao_code = :place_icao_code",
                           soci::use (iIcaoCode),
                           soci::into (ioSerialisedPlaceStr));

      // Execute the SQL query
      ioSelectStatement.execute();

    } catch (std::exception const& lException) {
      std::ostringstream errorStr;
      errorStr
        << "Error in the 'select serialised_place from optd_por' SQL request: "
        << lException.what();
      OPENTREP_LOG_ERROR (errorStr.str());
      throw SQLDatabaseException (errorStr.str());
    }
  }

  // //////////////////////////////////////////////////////////////////////
  void DBManager::
  prepareSelectBlobOnFaaCodeStatement (soci::session& ioSociSession,
                                       soci::statement& ioSelectStatement,
                                       const std::string& iFaaCode,
                                       std::string& ioSerialisedPlaceStr) {
  
    try {
    
      // Instanciate a SQL statement (no request is performed at that stage)
      /**
         select serialised_place from optd_por where faa_code = iFaaCode;
      */
      ioSelectStatement = (ioSociSession.prepare
                           << "select serialised_place from optd_por "
                           << "where faa_code = :place_faa_code",
                           soci::use (iFaaCode),
                           soci::into (ioSerialisedPlaceStr));

      // Execute the SQL query
      ioSelectStatement.execute();

    } catch (std::exception const& lException) {
      std::ostringstream errorStr;
      errorStr
        << "Error in the 'select serialised_place from optd_por' SQL request: "
        << lException.what();
      OPENTREP_LOG_ERROR (errorStr.str());
      throw SQLDatabaseException (errorStr.str());
    }
  }

  // //////////////////////////////////////////////////////////////////////
  void DBManager::
  prepareSelectBlobOnUNLOCodeStatement (soci::session& ioSociSession,
                                        soci::statement& ioSelectStatement,
                                        const std::string& iUNLOCode,
                                        std::string& ioSerialisedPlaceStr) {
  
    try {
    
      // Instanciate a SQL statement (no request is performed at that stage)
      /**
         select serialised_place from optd_por where unlocode_code = iUNLOCode;
      */
      ioSelectStatement = (ioSociSession.prepare
                           << "select serialised_place from optd_por "
                           << "where unlocode_code = :place_unlocode_code",
                           soci::use (iUNLOCode),
                           soci::into (ioSerialisedPlaceStr));

      // Execute the SQL query
      ioSelectStatement.execute();

    } catch (std::exception const& lException) {
      std::ostringstream errorStr;
      errorStr
        << "Error in the 'select serialised_place from optd_por' SQL request: "
        << lException.what();
      OPENTREP_LOG_ERROR (errorStr.str());
      throw SQLDatabaseException (errorStr.str());
    }
  }

  // //////////////////////////////////////////////////////////////////////
  void DBManager::
  prepareSelectBlobOnUICCodeStatement (soci::session& ioSociSession,
                                       soci::statement& ioSelectStatement,
                                       const UICCode_T& iUICCode,
                                       std::string& ioSerialisedPlaceStr) {
  
    try {
    
      // Instanciate a SQL statement (no request is performed at that stage)
      /**
         select serialised_place from optd_por where uic_code = iUICCode;
      */
      ioSelectStatement = (ioSociSession.prepare
                           << "select serialised_place from optd_por "
                           << "where uic_code = :place_uic_code",
                           soci::use (iUICCode),
                           soci::into (ioSerialisedPlaceStr));

      // Execute the SQL query
      ioSelectStatement.execute();

    } catch (std::exception const& lException) {
      std::ostringstream errorStr;
      errorStr
        << "Error in the 'select serialised_place from optd_por' SQL request: "
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
  
    try {
    
      // Instanciate a SQL statement (no request is performed at that stage)
      /**
         select serialised_place from optd_por where iata_code = iIataCode;
      */
      ioSelectStatement = (ioSociSession.prepare
                           << "select serialised_place from optd_por "
                           << "where geoname_id = :place_geoname_id",
                           soci::use (iGeonameID),
                           soci::into (ioSerialisedPlaceStr));

      // Execute the SQL query
      ioSelectStatement.execute();

    } catch (std::exception const& lException) {
      std::ostringstream errorStr;
      errorStr
        << "Error in the 'select serialised_place from optd_por' SQL request: "
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

      /**
       * Sometimes, there are several UN/LOCODE codes for a single POR,
       * for instance USACX/USAIY for
       * [Atlantic City](http://www.geonames.org/4500546),
       * New Jersey (NJ), United States (US).
       *
       * Take the first of those codes, when existing.
       *
       * That may not be optimal, but there is no easy way to solve this.
       * Indeed, we should either have a related table (with the IATA code
       * and Geonames ID as foreign key)
       * or de-normalize the tables, ie having one record per UN/LOCODE,
       * which would therefore inhibit the unicity in the current primary key.
       *
       * Nevertheless, adding that UN/LOCODE in the database allows a fast
       * retrieval from the database by searching on that code.
       * But that use case corresponds to a rare usage from the command-line.
       * Indeed, the UI (eg, http://search-travel.org) uses the full-text
       * search API, not the direct retrieval from the database.
       * The only cases when the UI uses the direct database retrieval
       * is for IATA codes. The condition to trigger that API usage is
       * to have only a sequence of 3-letter codes. As UN/LOCODE are
       * 5-letter, the normal full-text serach API is used for UN/LOCODE codes.
       */
      const UNLOCodeList_T& lUNLOCodeList = iPlace.getUNLOCodeList();
      std::string lUNLOCodeStr ("");
      if (lUNLOCodeList.empty() == false) {
        const UNLOCode_T& lUNLOCode = lUNLOCodeList.front();
        lUNLOCodeStr = static_cast<const std::string> (lUNLOCode);
      }

      /**
       * Same remark as for UN/LOCODE above. The only difference is that
       * UIC codes are integers rather than character strings.
       */
      const UICCodeList_T& lUICCodeList = iPlace.getUICCodeList();
      UICCode_T lUICCodeInt = 0;
      if (lUICCodeList.empty() == false) {
        const UICCode_T& lUICCode = lUICCodeList.front();
        lUICCodeInt = static_cast<const UICCode_T> (lUICCode);
      }

      
      // DEBUG
      /*
      std::ostringstream oStr;
      oStr << "insert into optd_por values (" << lPK << ", ";
      oStr << lLocationType << ", ";
      oStr << lIataCode << ", " << lIcaoCode << ", " << lFaaCode << ", ";
      oStr << lUNLOCode << ", ";
      oStr << lUICCode << ", ";
      oStr << lIsGeonames << ", " << lGeonameID << ", ";
      oStr << lEnvID << ", " << lDateFrom << ", " << lDateEnd << ", ";
      oStr << lRawDataString << ")";
      OPENTREP_LOG_DEBUG ("Full SQL statement: '" << oStr.str() << "'");
      */

      ioSociSession << "insert into optd_por values (:pk, "
                    << ":location_type, :iata_code, :icao_code, :faa_code, "
                    << ":unlocode_code, :uic_code, "
                    << ":is_geonames, :geoname_id, "
                    << ":envelope_id, :date_from, :date_until, "
                    << ":serialised_place)",
        soci::use (lPK), soci::use (lLocationType), soci::use (lIataCode),
        soci::use (lIcaoCode), soci::use (lFaaCode),
        soci::use (lUNLOCodeStr), soci::use (lUICCodeInt),
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
       select count(1) from optd_por;
      */

      ioSociSession << "select count(1) from optd_por;", soci::into(oNbOfEntries);

    } catch (std::exception const& lException) {
      std::ostringstream errorStr;
      errorStr
        << "Error when trying to count the number of rows in the optd_por table: "
        << lException.what();
      OPENTREP_LOG_ERROR (errorStr.str());
      throw SQLDatabaseIndexCreationException (errorStr.str());
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
                                               LocationList_T& ioLocationList,
                                               const bool iUniqueEntry) {
    NbOfDBEntries_T oNbOfEntries = 0;
    LocationList_T lLocationList;

    try {

      // Convert the code into uppercase. That way, one can search for codes
      // irrespective of the case (knwing that codes are stored uppercase
      // in the database)
      const std::string& lCode = static_cast<const std::string&> (iIataCode);
      const std::string lCodeUpper = boost::algorithm::to_upper_copy (lCode);
      
      // Prepare the SQL request corresponding to the select statement
      soci::statement lSelectStatement (ioSociSession);
      std::string lPlaceRawDataString;
      DBManager::prepareSelectBlobOnIataCodeStatement (ioSociSession,
                                                       lSelectStatement,
                                                       lCodeUpper,
                                                       lPlaceRawDataString);

      /**
       * Retrieve the details of the place, as well as the alternate
       * names, most often in other languages (e.g., "ru", "zh").
       */
      bool hasStillData = true;
      while (hasStillData == true) {
        hasStillData = iterateOnStatement (lSelectStatement,
                                           lPlaceRawDataString);

        // DEBUG
        const std::string lFoundStr = hasStillData?"more; see below":"no more";
        OPENTREP_LOG_DEBUG ("Checked whether there are more locations "
                            << "corresponding to '" << iIataCode
                            << "' IATA code. Result: " << lFoundStr);

        if (hasStillData == true) {
          //
          ++oNbOfEntries;

          // Parse the POR details and create the corresponding
          // Location structure
          const RawDataString_T lPlaceRawData (lPlaceRawDataString);
          Location lLocation = Result::retrieveLocation (lPlaceRawData);
          lLocation.setCorrectedKeywords (iIataCode);

          // Add the new found location to the list
          lLocationList.push_back (lLocation);

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

    // Add the just retrieved Location structure(s) to the list given
    // as parameter
    const Location* lHighestPRLocation_ptr = NULL;
    PageRank_T lHighestPRValue = 0.0;
    for (LocationList_T::const_iterator itLoc = lLocationList.begin();
         itLoc != lLocationList.end(); ++itLoc) {
      const Location& lLocation = *itLoc;
      const PageRank_T& lPRValue = lLocation.getPageRank();

      // Store (a pointer on) the Location structure with the highest Page Rank.
      // Normally, when there is no PageRank value, the field should be empty
      // (empty string). In some rare cases, actually when OPTD is buggy,
      // the PageRank value may be zero. While it is a bug from OPTD, there is
      // no reason it should trigger a bug from OpenTREP in turn. So, rather
      // then ignoring any POR with zero PageRank value, rather the first one
      // is (randomly) selected
      if (lPRValue >= lHighestPRValue) {
        lHighestPRLocation_ptr = &lLocation;
        lHighestPRValue = lPRValue;
      }

      // Add the Location structure now, only when a unique solution
      // is not expected
      if (iUniqueEntry == false) {
        ioLocationList.push_back (lLocation);
      }
    }

    // Add the Location structure with the highest Page Rank value
    if (iUniqueEntry == true && lHighestPRLocation_ptr != NULL) {
      assert (lHighestPRLocation_ptr != NULL);
      ioLocationList.push_back (*lHighestPRLocation_ptr);

      // DEBUG
      OPENTREP_LOG_DEBUG("Kept the location with the highest PageRank value ("
                         << lHighestPRValue << ") for '" << iIataCode
                         << "' IATA code: " << lHighestPRLocation_ptr->getKey());
    }

    // Make the number of retrieved locations consistent with the unicity
    // requirement (if set)
    if (oNbOfEntries > 0 && iUniqueEntry == true) {
      oNbOfEntries = 1;
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

      // Convert the code into uppercase. That way, one can search for codes
      // irrespective of the case (knwing that codes are stored uppercase
      // in the database)
      const std::string& lCode = static_cast<const std::string&> (iIcaoCode);
      const std::string lCodeUpper = boost::algorithm::to_upper_copy (lCode);
      
      // Prepare the SQL request corresponding to the select statement
      soci::statement lSelectStatement (ioSociSession);
      std::string lPlaceRawDataString;
      DBManager::prepareSelectBlobOnIcaoCodeStatement (ioSociSession,
                                                       lSelectStatement,
                                                       lCodeUpper,
                                                       lPlaceRawDataString);

      /**
       * Retrieve the details of the place, as well as the alternate
       * names, most often in other languages (e.g., "ru", "zh").
       */
      bool hasStillData = true;
      while (hasStillData == true) {
        hasStillData = iterateOnStatement (lSelectStatement,
                                           lPlaceRawDataString);

        // DEBUG
        const std::string lFoundStr = hasStillData?"Yes":"No";
        OPENTREP_LOG_DEBUG ("Checked locations corresponding to "
                            << iIcaoCode << " ICAO code. Found: " << lFoundStr);

        if (hasStillData == true) {
          //
          ++oNbOfEntries;

          // Parse the POR details and create the corresponding
          // Location structure
          const RawDataString_T lPlaceRawData (lPlaceRawDataString);
          Location lLocation = Result::retrieveLocation (lPlaceRawData);
          lLocation.setCorrectedKeywords (iIcaoCode);

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

      // Convert the code into uppercase. That way, one can search for codes
      // irrespective of the case (knwing that codes are stored uppercase
      // in the database)
      const std::string& lCode = static_cast<const std::string&> (iFaaCode);
      const std::string lCodeUpper = boost::algorithm::to_upper_copy (lCode);
      
      // Prepare the SQL request corresponding to the select statement
      soci::statement lSelectStatement (ioSociSession);
      std::string lPlaceRawDataString;
      DBManager::prepareSelectBlobOnFaaCodeStatement (ioSociSession,
                                                      lSelectStatement,
                                                      lCodeUpper,
                                                      lPlaceRawDataString);

      /**
       * Retrieve the details of the place, as well as the alternate
       * names, most often in other languages (e.g., "ru", "zh").
       */
      bool hasStillData = true;
      while (hasStillData == true) {
        hasStillData = iterateOnStatement (lSelectStatement,
                                           lPlaceRawDataString);

        // DEBUG
        const std::string lFoundStr = hasStillData?"Yes":"No";
        OPENTREP_LOG_DEBUG ("Checked locations corresponding to "
                            << iFaaCode << " FAA code. Found: " << lFoundStr);

        if (hasStillData == true) {
          //
          ++oNbOfEntries;

          // Parse the POR details and create the corresponding
          // Location structure
          const RawDataString_T lPlaceRawData (lPlaceRawDataString);
          Location lLocation = Result::retrieveLocation (lPlaceRawData);
          lLocation.setCorrectedKeywords (iFaaCode);

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
  NbOfDBEntries_T DBManager::getPORByUNLOCode (soci::session& ioSociSession,
                                               const UNLOCode_T& iUNLOCode,
                                               LocationList_T& ioLocationList,
                                               const bool iUniqueEntry) {
    NbOfDBEntries_T oNbOfEntries = 0;
    LocationList_T lLocationList;

    try {

      // Convert the code into uppercase. That way, one can search for codes
      // irrespective of the case (knwing that codes are stored uppercase
      // in the database)
      const std::string& lCode = static_cast<const std::string&> (iUNLOCode);
      const std::string lCodeUpper = boost::algorithm::to_upper_copy (lCode);
      
      // Prepare the SQL request corresponding to the select statement
      soci::statement lSelectStatement (ioSociSession);
      std::string lPlaceRawDataString;
      DBManager::prepareSelectBlobOnUNLOCodeStatement (ioSociSession,
                                                       lSelectStatement,
                                                       lCodeUpper,
                                                       lPlaceRawDataString);

      /**
       * Retrieve the details of the place, as well as the alternate
       * names, most often in other languages (e.g., "ru", "zh").
       */
      bool hasStillData = true;
      while (hasStillData == true) {
        hasStillData = iterateOnStatement (lSelectStatement,
                                           lPlaceRawDataString);

        // DEBUG
        const std::string lFoundStr = hasStillData?"Yes":"No";
        OPENTREP_LOG_DEBUG ("Checked locations corresponding to "
                            << iUNLOCode << " UN/LOCODE code. Found: "
                            << lFoundStr);

        if (hasStillData == true) {
          //
          ++oNbOfEntries;

          // Parse the POR details and create the corresponding
          // Location structure
          const RawDataString_T lPlaceRawData (lPlaceRawDataString);
          Location lLocation = Result::retrieveLocation (lPlaceRawData);
          lLocation.setCorrectedKeywords (iUNLOCode);

          // Add the new found location to the list
          lLocationList.push_back (lLocation);

          // Debug
          OPENTREP_LOG_DEBUG ("[" << oNbOfEntries << "] " << lLocation);
        }
      }
      
    } catch (std::exception const& lException) {
      std::ostringstream errorStr;
      errorStr << "Error when trying to retrieve a POR for " << iUNLOCode
               << " from the SQL database: " << lException.what();
      OPENTREP_LOG_ERROR (errorStr.str());
      throw SQLDatabaseException (errorStr.str());
    }

    // Add the just retrieved Location structure(s) to the list given
    // as parameter
    const Location* lHighestPRLocation_ptr = NULL;
    PageRank_T lHighestPRValue = 0.0;
    for (LocationList_T::const_iterator itLoc = lLocationList.begin();
         itLoc != lLocationList.end(); ++itLoc) {
      const Location& lLocation = *itLoc;
      const PageRank_T& lPRValue = lLocation.getPageRank();

      // Store (a pointer on) the Location structure with the highest Page Rank
      if (lPRValue > lHighestPRValue) {
        lHighestPRLocation_ptr = &lLocation;
        lHighestPRValue = lPRValue;
      }

      // Add the Location structure now, only when 
      if (iUniqueEntry == false) {
        ioLocationList.push_back (lLocation);
      }
    }

    // Add the Location structure with the highest Page Rank value
    if (iUniqueEntry == true && lHighestPRLocation_ptr != NULL) {
      assert (lHighestPRLocation_ptr != NULL);
      ioLocationList.push_back (*lHighestPRLocation_ptr);

      // DEBUG
      OPENTREP_LOG_DEBUG("Kept the location with the highest PageRank value ("
                         << lHighestPRValue << ") for '" << iUNLOCode
                         << "' IATA code: " << lHighestPRLocation_ptr->getKey());
    }

    // Make the number of retrieved locations consistent with the unicity
    // requirement (if set)
    if (oNbOfEntries > 0 && iUniqueEntry == true) {
      oNbOfEntries = 1;
    }
    
    //
    return oNbOfEntries;
  }

  // //////////////////////////////////////////////////////////////////////
  NbOfDBEntries_T DBManager::getPORByUICCode (soci::session& ioSociSession,
                                              const UICCode_T& iUICCode,
                                              LocationList_T& ioLocationList) {
    NbOfDBEntries_T oNbOfEntries = 0;

    try {

      // Prepare the SQL request corresponding to the select statement
      soci::statement lSelectStatement (ioSociSession);
      std::string lPlaceRawDataString;
      DBManager::prepareSelectBlobOnUICCodeStatement (ioSociSession,
                                                      lSelectStatement,
                                                      iUICCode,
                                                      lPlaceRawDataString);

      /**
       * Retrieve the details of the place, as well as the alternate
       * names, most often in other languages (e.g., "ru", "zh").
       */
      bool hasStillData = true;
      while (hasStillData == true) {
        hasStillData = iterateOnStatement (lSelectStatement,
                                           lPlaceRawDataString);

        // DEBUG
        const std::string lFoundStr = hasStillData?"Yes":"No";
        OPENTREP_LOG_DEBUG ("Checked locations corresponding to "
                            << iUICCode << " UIC code. Found: "
                            << lFoundStr);

        if (hasStillData == true) {
          //
          ++oNbOfEntries;

          // Parse the POR details and create the corresponding
          // Location structure
          const RawDataString_T lPlaceRawData (lPlaceRawDataString);
          Location lLocation = Result::retrieveLocation (lPlaceRawData);
          const std::string lUICCodeStr =
            boost::lexical_cast<std::string> (iUICCode);
          lLocation.setCorrectedKeywords (lUICCodeStr);

          // Add the new found location to the list
          ioLocationList.push_back (lLocation);

          // Debug
          OPENTREP_LOG_DEBUG ("[" << oNbOfEntries << "] " << lLocation);
        }
      }
      
    } catch (std::exception const& lException) {
      std::ostringstream errorStr;
      errorStr << "Error when trying to retrieve a POR for " << iUICCode
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

        // DEBUG
        const std::string lFoundStr = hasStillData?"Yes":"No";
        OPENTREP_LOG_DEBUG ("Checked locations corresponding to "
                            << iGeonameID<< " Geonames ID. Found: "<< lFoundStr);

        if (hasStillData == true) {
          //
          ++oNbOfEntries;

          // Parse the POR details and create the corresponding
          // Location structure
          const RawDataString_T lPlaceRawData (lPlaceRawDataString);
          Location lLocation = Result::retrieveLocation (lPlaceRawData);
          const std::string lGeonamesIDStr =
            boost::lexical_cast<std::string> (iGeonameID);
          lLocation.setCorrectedKeywords (lGeonamesIDStr);

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
