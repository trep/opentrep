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
  NbOfDBEntries_T DBManager::
  buildSQLDB (const PORFilePath_T& iPORFilepath,
              const SQLiteDBFilePath_T& iSQLiteDBFilePath) {
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

}
