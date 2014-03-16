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
// OpenTrep
#include <opentrep/basic/OTransliterator.hpp>
#include <opentrep/basic/Utilities.hpp>
#include <opentrep/bom/StringPartition.hpp>
#include <opentrep/bom/WordCombinationHolder.hpp>
#include <opentrep/bom/World.hpp>
#include <opentrep/bom/Place.hpp>
#include <opentrep/bom/PORParserHelper.hpp>
#include <opentrep/factory/FacPlace.hpp>
#include <opentrep/command/DBManager.hpp>
#include <opentrep/command/IndexBuilder.hpp>
#include <opentrep/service/Logger.hpp>
// Xapian
#include <xapian.h>

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
    // ORI-maintained list of POR (points of reference), allowing the search
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
  buildSearchIndex (Xapian::WritableDatabase& ioDatabase,
                    soci::session& ioSociSession,
                    std::istream& iPORFileStream,
                    const OTransliterator& iTransliterator) {
    NbOfDBEntries_T oNbOfEntries = 0;

    // Open the file to be parsed
    Place& lPlace = FacPlace::instance().create();
    std::string itReadLine;
    while (std::getline (iPORFileStream, itReadLine)) {
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

        // Add the document, associated to the Place object, to the Xapian index
        IndexBuilder::addDocumentToIndex (ioDatabase, lPlace, iTransliterator);

        // Add the document to the SQLite3 database
        DBManager::insertPlaceInDB (ioSociSession, lPlace);

        // DEBUG
        /*
        OPENTREP_LOG_DEBUG ("[AFT-ADD] " << lLocationKey
                            << ", Place: " << lPlace);
        */

        // Iteration
        ++oNbOfEntries;

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
  NbOfDBEntries_T IndexBuilder::
  buildSearchIndex (const PORFilePath_T& iPORFilePath,
                    const TravelDBFilePath_T& iTravelDBFilePath,
                    const SQLiteDBFilePath_T& iSQLiteDBFilePath,
                    const OTransliterator& iTransliterator) {
    NbOfDBEntries_T oNbOfEntries = 0;

    /**
     *            0. Remove any existing directory (for Xapian and SQLite)
     */
    // Xapian directory
    boost::filesystem::path lTravelDBFilePath (iTravelDBFilePath.begin(),
                                               iTravelDBFilePath.end());
    // DEBUG
    OPENTREP_LOG_DEBUG ("The Xapian database ('" << iTravelDBFilePath
                        << "') will be cleared");
    boost::filesystem::remove_all (lTravelDBFilePath);

    // SQLite3 file
    boost::filesystem::path lSQLiteDBFullPath (iSQLiteDBFilePath.begin(),
                                               iSQLiteDBFilePath.end());
    // DEBUG
    OPENTREP_LOG_DEBUG ("The SQLite database file ('" << lSQLiteDBFullPath
                        << "') will be cleared");
    boost::filesystem::remove_all (lSQLiteDBFullPath);


    /**
     *            1. Xapian Database Initialisation
     */
    // Re-create the Xapian directory
    boost::filesystem::create_directories (lTravelDBFilePath);

    // Check whether the just created directory exists and is a directory.
    if (!(boost::filesystem::exists (lTravelDBFilePath)
          && boost::filesystem::is_directory (lTravelDBFilePath))) {
      std::ostringstream oStr;
      oStr << "The file-path to the Xapian database/index ('"
           << iPORFilePath << "') does not exist or is not a directory.";
      OPENTREP_LOG_ERROR (oStr.str());
      throw FileNotFoundException (oStr.str());
    }

    // Create the Xapian database (index). As the directory has been fully
    // cleaned, deleted and re-created, that Xapian database (index) is empty.
    Xapian::WritableDatabase lXapianDatabase (iTravelDBFilePath,
                                              Xapian::DB_CREATE);

    // DEBUG
    OPENTREP_LOG_DEBUG ("The Xapian database ('" << iTravelDBFilePath
                        << "') has been checked and open");

    /**
     * Begin a transation on the Xapian database (index).
     *
     * Normally, there are around 11,000 entries (rows/documents) to
     * be indexed. Not specifying the beginning of a transaction would
     * mean that every document addition would end up in a corresponding
     * independant transaction, which would be very much inefficient.
     */
    lXapianDatabase.begin_transaction();

    // DEBUG
    OPENTREP_LOG_DEBUG ("A transaction has begun on the Xapian database ('"
                        << iTravelDBFilePath << "')");


    /**
     *            2. SQLite3 Database Initialisation
     */
    // Create the SQLite3 database file
    soci::session* lSociSession_ptr =
      DBManager::initSQLDBSession (iSQLiteDBFilePath);
    assert (lSociSession_ptr != NULL);
    soci::session& lSociSession = *lSociSession_ptr;

    // Create the SQLite3 database tables
    DBManager::createSQLDBTables (lSociSession);


    /**
     *            3. List of POR (points of reference)
     */
    // Check that the file exists and is regular.

    // DEBUG
    OPENTREP_LOG_DEBUG ("Parsing POR input file: " << iPORFilePath);

    // Check whether the file to be parsed exists and is readable.
    boost::filesystem::path lPORFilePath (iPORFilePath.begin(),
                                          iPORFilePath.end());
    if (!(boost::filesystem::exists (lPORFilePath)
          && boost::filesystem::is_regular_file (lPORFilePath))) {
      OPENTREP_LOG_ERROR ("The POR file " << iPORFilePath
                          << " does not exist or cannot be open." << std::endl);

      throw FileNotFoundException ("The POR file " + iPORFilePath
                                   + " does not exist or cannot be read");
    }

    /**
     * Check whether the data file is compressed.
     *
     * Note: that check is not very robust, as it is based on the
     * file extension. A more robust approach would be to extract
     * the magic number (first 4 bytes).
     */
    const boost::filesystem::path& lPORFileExtPath = lPORFilePath.extension();
    const std::string& lPORFileExt = lPORFileExtPath.string();
    if (lPORFileExt == ".bz2") {
      // Open the file
      boost::iostreams::file_source cprdFileToBeParsed (iPORFilePath,
                                                        std::ios_base::in
                                                        | std::ios_base::binary);

      // Uncompress the file with the BZ2 library and its Boost wrapper
      boost::iostreams::filtering_istream bunzip2Filter;
      bunzip2Filter.push (boost::iostreams::bzip2_decompressor());
      bunzip2Filter.push (cprdFileToBeParsed);

      // Browse the input POR (point of reference) data file,
      // and parse every of its rows
      oNbOfEntries = buildSearchIndex (lXapianDatabase, lSociSession,
                                       bunzip2Filter, iTransliterator);

    } else if (lPORFileExt == ".gz") {
      // Open the file
      boost::iostreams::file_source cprdFileToBeParsed (iPORFilePath,
                                                        std::ios_base::in
                                                        | std::ios_base::binary);

      // Uncompress the file with the BZ2 library and its Boost wrapper
      boost::iostreams::filtering_istream gunzipFilter;
      gunzipFilter.push (boost::iostreams::gzip_decompressor());
      gunzipFilter.push (cprdFileToBeParsed);

      // Browse the input POR (point of reference) data file,
      // and parse every of its rows
      oNbOfEntries = buildSearchIndex (lXapianDatabase, lSociSession,
                                       gunzipFilter, iTransliterator);

    } else if (lPORFileExt == ".csv") {
      // Open the file
      boost::filesystem::ifstream fileToBeParsed (iPORFilePath,
                                                  std::ios_base::in);

      // Browse the input POR (point of reference) data file,
      // and parse every of its rows
      oNbOfEntries = buildSearchIndex (lXapianDatabase, lSociSession,
                                       fileToBeParsed, iTransliterator);

    } else {
      //
      OPENTREP_LOG_ERROR ("The POR file " << iPORFilePath
                          << " has got an unknown extension ('" << lPORFileExt
                          << "'). Recognised extensions: .csv, .bz2, .gz"
                          << std::endl);

      throw FileExtensionUnknownException ("The POR file " + iPORFilePath
                                           + " has got an unknown extension ('"
                                           + lPORFileExt + "'). Recognised "
                                           + "extensions: .csv, .bz2, .gz");
    }

    // Commit the pending modifications on the Xapian database (index)
    lXapianDatabase.commit_transaction();

    // DEBUG
    OPENTREP_LOG_DEBUG ("Xapian has indexed " << oNbOfEntries << " entries.");

    /**
     * Close the Xapian database (index).
     *
     * \note For a yet unknown reason, when the current method is called within
     *       the Boost Unit Test framework, that latter kills the process.
     *       When called from within GDB, all is fine.
     */
    lXapianDatabase.close();


    // Build the SQLite3 database indexes
    DBManager::createSQLDBIndexes (lSociSession);


    return oNbOfEntries;
  }

}
