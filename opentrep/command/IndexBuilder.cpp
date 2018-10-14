// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// STL
#include <cassert>
#include <string>
#include <vector>
#include <exception>
// Boost
#include <boost/filesystem.hpp>
#include <boost/filesystem/fstream.hpp>
#include <boost/tokenizer.hpp>
#include <boost/iostreams/device/file.hpp>
#include <boost/iostreams/filtering_stream.hpp>
#include <boost/iostreams/filter/gzip.hpp>
#include <boost/iostreams/filter/bzip2.hpp>
// SOCI
#include <soci/soci.h>
// Xapian
#include <xapian.h>
// OpenTrep
#include <opentrep/basic/OTransliterator.hpp>
#include <opentrep/basic/Utilities.hpp>
#include <opentrep/bom/StringPartition.hpp>
#include <opentrep/bom/WordCombinationHolder.hpp>
#include <opentrep/bom/World.hpp>
#include <opentrep/bom/Place.hpp>
#include <opentrep/bom/PORFileHelper.hpp>
#include <opentrep/bom/PORParserHelper.hpp>
#include <opentrep/factory/FacPlace.hpp>
#include <opentrep/factory/FacXapianDB.hpp>
#include <opentrep/command/FileManager.hpp>
#include <opentrep/command/DBManager.hpp>
#include <opentrep/command/IndexBuilder.hpp>
#include <opentrep/service/Logger.hpp>

namespace OPENTREP {

  // //////////////////////////////////////////////////////////////////////
  void addToXapian (const Place& iPlace, Xapian::Document& ioDocument,
                    Xapian::WritableDatabase& ioDatabase) {
    /**
     * Build a Xapian TermGenerator:
     * http://xapian.org/docs/apidoc/html/classXapian_1_1TermGenerator.html
     * It is an helper to insert terms into the Xapian index for the given
     * document.
     */
    Xapian::TermGenerator lTermGenerator;
    lTermGenerator.set_database (ioDatabase);
    lTermGenerator.set_document (ioDocument);

    // DEBUG
    // OPENTREP_LOG_DEBUG ("Indexing for " << iPlace.describeKey());

    const Place::TermSetMap_T& lTermSetMap = iPlace.getTermSetMap();
    for (Place::TermSetMap_T::const_iterator itStringSet = lTermSetMap.begin();
         itStringSet != lTermSetMap.end(); ++itStringSet) {
      // Retrieve the weight
      const Weight_T& lWeight = itStringSet->first;
      const Xapian::termcount lWDFInc =
        static_cast<const Xapian::termcount> (lWeight);

      // Retrieve the set of strings for that weight
      const Place::StringSet_T& lTermSet = itStringSet->second;
      for (Place::StringSet_T::const_iterator itString = lTermSet.begin();
           itString != lTermSet.end(); ++itString) {
        const std::string& lString = *itString;
        lTermGenerator.index_text (lString, lWDFInc);

        // DEBUG
        //OPENTREP_LOG_DEBUG("[" << lWeight << "/" << lWDFInc << "] "<< lString);
      }
    }

    // Spelling terms
    const Place::StringSet_T& lSpellingSet = iPlace.getSpellingSet();
    for (Place::StringSet_T::const_iterator itTerm = lSpellingSet.begin();
         itTerm != lSpellingSet.end(); ++itTerm) {
      const std::string& lTerm = *itTerm;
      ioDatabase.add_spelling (lTerm);
    }

    // DEBUG
    OPENTREP_LOG_DEBUG ("Added terms for '" << iPlace.describeKey()
                        << "': " << iPlace.describeSets()
                        << " into " << ioDocument.get_description());
  }

  // //////////////////////////////////////////////////////////////////////
  void IndexBuilder::addDocumentToIndex(Xapian::WritableDatabase& ioDatabase,
                                        Place& ioPlace,
                                        const OTransliterator& iTransliterator) {

    // Create an empty Xapian document
    Xapian::Document lDocument;
    
    // Retrieve the raw data string, to be stored as is within
    // the Xapian document
    const RawDataString_T& lRawDataString = ioPlace.getRawDataString();

    // The Xapian document data is indeed the same as the one of the
    // OPTD-maintained list of POR (points of reference), allowing the search
    // process to use exactly the same parser as the indexation process
    lDocument.set_data (lRawDataString);
      
    // Build the (STL) sets of terms to be added to the Xapian index and
    // spelling dictionary
    ioPlace.buildIndexSets (iTransliterator);

    // Add the (STL) sets of terms to the Xapian index and spelling dictionary
    addToXapian (ioPlace, lDocument, ioDatabase);

    // Add the document to the database
    const Xapian::docid& lDocID = ioDatabase.add_document (lDocument);
      
    // Assign back the newly generated Xapian document ID to the
    // Place object
    ioPlace.setDocID (lDocID);
  }

  // //////////////////////////////////////////////////////////////////////
  NbOfDBEntries_T IndexBuilder::
  buildSearchIndex (Xapian::WritableDatabase* ioXapianDB_ptr,
                    const DBType& iSQLDBType, soci::session* ioSociSessionPtr,
                    std::istream& iPORFileStream,
                    const shouldIndexNonIATAPOR_T& iIncludeNonIATAPOR,
                    const OTransliterator& iTransliterator) {
    NbOfDBEntries_T oNbOfEntries = 0;
    NbOfDBEntries_T oNbOfEntriesInPORFile = 0;

    // Open the file to be parsed
    Place& lPlace = FacPlace::instance().create();
    std::string itReadLine;
    while (std::getline (iPORFileStream, itReadLine)) {

      /* First, if only the IATA-refernced POR must be indexed
       * (ie, when iIncludeNonIATAPOR is set to false), the line
       * must start with a non empty IATA code of three letters;
       * in other words, the separator (the hat symbol) is first seen
       * at position 3 (remember that strings in C++ start at position 0).
       * Otherwise, the line is skipped.
       */
      if (!iIncludeNonIATAPOR) {
        const unsigned short lFirstSeparatorPos = itReadLine.find_first_of ("^");
        if (lFirstSeparatorPos != 3) {
          // DEBUG
          /*
            OPENTREP_LOG_ERROR ("[" << oNbOfEntries << "] pos of sep: "
                                << lFirstSeparatorPos << ", full line: "
                                << itReadLine);
          */

          //
          ++oNbOfEntriesInPORFile;

          //
          continue;
        }
      }
      
      // Initialise the parser
      PORStringParser lStringParser (itReadLine);

      // Parse the string
      const Location& lLocation = lStringParser.generateLocation();

      // DEBUG
      /*
        const LocationKey& lLocationKey = lLocation.getKey();
        OPENTREP_LOG_DEBUG ("[BEF-ADD] " << lLocationKey);
      */

      /* When the line/string is relevant, create a BOM instance from
       * the Location structure.
       * Otherwise, the line is skipped.
       */
      const std::string& lCommonName = lLocation.getCommonName();
      if (lCommonName == "NotAvailable") {
        continue;
      }
      
      // Fill the Place object with the Location structure.
      lPlace.setLocation (lLocation);

      // Add the document, associated to the Place object, to the Xapian index,
      // if required
      if (ioXapianDB_ptr != NULL) {
        IndexBuilder::addDocumentToIndex (*ioXapianDB_ptr, lPlace,
                                          iTransliterator);
      }

      // Add the document to the SQL database, if required
      if (ioSociSessionPtr != NULL) {
        DBManager::insertPlaceInDB (*ioSociSessionPtr, lPlace);
      }

      // DEBUG
      /*
        OPENTREP_LOG_DEBUG ("[AFT-ADD] " << lLocationKey
        << ", Place: " << lPlace);
      */

      // Iteration
      ++oNbOfEntries; ++oNbOfEntriesInPORFile;
      
      // Progress status
      if (oNbOfEntries % 1000 == 0) {
        std::cout.imbue( std::locale (std::locale::classic(), new NumSep));
        std::cout << "Number of actually parsed records: " << oNbOfEntries
                  << ", out of " << oNbOfEntriesInPORFile
                  << " records in the POR data file so far" << std::endl;
      }

      // DEBUG
      OPENTREP_LOG_DEBUG ("[" << oNbOfEntries << "] " << lPlace);

      // Reset for next turn
      lPlace.resetMatrix();
      lPlace.resetIndexSets();
    }

    return oNbOfEntries;
  }

  // //////////////////////////////////////////////////////////////////////
  NbOfDBEntries_T IndexBuilder::
  buildSearchIndex (const PORFilePath_T& iPORFilePath,
                    const TravelDBFilePath_T& iTravelIndexFilePath,
                    const DBType& iSQLDBType,
                    const SQLDBConnectionString_T& iSQLDBConnStr,
                    const shouldIndexNonIATAPOR_T& iIncludeNonIATAPOR,
                    const shouldIndexPORInXapian_T& iShouldIndexPORInXapian,
                    const shouldAddPORInSQLDB_T& iShouldAddPORInSQLDB,
                    const OTransliterator& iTransliterator) {
    NbOfDBEntries_T oNbOfEntries = 0;
    soci::session* lSociSession_ptr = NULL;
    Xapian::WritableDatabase* lXapianDatabase_ptr = NULL;
    
    /**
     *            1. Xapian database (index) initialisation
     *
     * a. Remove any existing directory for Xapian
     * b. Create the Xapian database (index). As the directory has been fully
     * cleaned, deleted and re-created, that Xapian database (index) is empty
     * c. Start a transaction for Xapian
     */
    if (iShouldIndexPORInXapian) {
      // Delete and recreate the directory, and its full content,
      // hosting the Xapian index / database
      FileManager::recreateXapianDirectory (iTravelIndexFilePath);

      // Recreate the Xapian index / database
      lXapianDatabase_ptr =
        FacXapianDB::instance().create (iTravelIndexFilePath, Xapian::DB_CREATE);
      assert (lXapianDatabase_ptr != NULL);
      
      // DEBUG
      OPENTREP_LOG_DEBUG ("The Xapian index / database ('"
                          << iTravelIndexFilePath
                          << "') has been re-created, checked and opened");


      /**
       * Begin a transation on the Xapian database (index).
       *
       * There may be around 120,000 entries (rows/documents) to
       * be indexed. Not specifying the beginning of a transaction would
       * mean that every document addition would end up in a corresponding
       * independant transaction, which would be very much inefficient.
       */
      lXapianDatabase_ptr->begin_transaction();

      // DEBUG
      OPENTREP_LOG_DEBUG ("A transaction has begun on the Xapian database ('"
                          << iTravelIndexFilePath << "')");
    }

    /**
     *            2. Re-initialize the SQL database
     *
     * Drop and re-create, if necessary, the content of the SQL database
     */
    if (iShouldAddPORInSQLDB) {
      /*
      // Creation of the trep user and trep_trep database
      bool isSuccessful = DBManager::createSQLDBUser (iSQLDBType, iSQLDBConnStr);
      if (isSuccessful == false) {
        std::ostringstream errorStr;
        errorStr << "Error when trying to re-initialize the SQL database ('"
                 << iSQLDBConnStr << "')";
        OPENTREP_LOG_ERROR (errorStr.str());
        throw SQLDatabaseImpossibleConnectionException (errorStr.str());
      }
      */
    
      /**
       *            3. Connection to the SQL Database
       */
      if (!(iSQLDBType == DBType::NODB)) {
        // Connection to the database
        lSociSession_ptr =
          DBManager::initSQLDBSession (iSQLDBType, iSQLDBConnStr);
        
        if (lSociSession_ptr == NULL) {
          std::ostringstream errorStr;
          errorStr << "Error when trying to connect to the SQL database ('"
                   << iSQLDBConnStr << "')";
          OPENTREP_LOG_ERROR (errorStr.str());
          throw SQLDatabaseImpossibleConnectionException (errorStr.str());
        }
        assert (lSociSession_ptr != NULL);
        
        // Creation of the POR table
        DBManager::createSQLDBTables (*lSociSession_ptr);
      }
    }
    
    /**
     *            4. Parse and index the POR (points of reference)
     */
    // DEBUG
    OPENTREP_LOG_DEBUG ("Parsing POR input file: " << iPORFilePath);

    // Get a reference on the file stream corresponding to the POR file.
    const PORFileHelper lPORFileHelper (iPORFilePath);
    std::istream& lPORFileStream = lPORFileHelper.getFileStreamRef();

    // Browse the input POR (point of reference) data file,
    // parse every of its rows, and put the result in the Xapian database/index
    // and, if needed, within the SQL database.
    oNbOfEntries = buildSearchIndex (lXapianDatabase_ptr, iSQLDBType,
                                     lSociSession_ptr, lPORFileStream,
                                     iIncludeNonIATAPOR, iTransliterator);

    /**
     *            5. Commit the transactions of the Xapian database (index).
     *
     */
    if (iShouldIndexPORInXapian) {
      assert (lXapianDatabase_ptr != NULL);
      lXapianDatabase_ptr->commit_transaction();

      // DEBUG
      OPENTREP_LOG_DEBUG ("Xapian has indexed " << oNbOfEntries << " entries.");
    }

    /**
     *            6. Close the Xapian database (index).
     *
     * \note For a yet unknown reason, when the current method is called within
     *       the Boost Unit Test framework, that latter kills the process.
     *       When called from within GDB, all is fine.
     */
    if (iShouldIndexPORInXapian) {
      assert (lXapianDatabase_ptr != NULL);
      lXapianDatabase_ptr->close();
    }


    if (iShouldAddPORInSQLDB) {
      /**
       *            7. Index the SQL database
       */
      if (!(iSQLDBType == DBType::NODB)) {
        assert (lSociSession_ptr != NULL);
        DBManager::createSQLDBIndexes (*lSociSession_ptr);
      }
    
      /**
       *            8. Close the connection to the SQL database
       */
      if (!(iSQLDBType == DBType::NODB)) {
        assert (lSociSession_ptr != NULL);
        DBManager::terminateSQLDBSession (iSQLDBType, iSQLDBConnStr,
                                          *lSociSession_ptr);
      }
    }
    
    return oNbOfEntries;
  }

}
