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
#include <opentrep/bom/PORFileHelper.hpp>
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
  buildSearchIndex (Xapian::WritableDatabase& ioDatabase,
                    const DBType& iSQLDBType, soci::session* ioSociSessionPtr,
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
        ++oNbOfEntries;

        // Progress status
        if (oNbOfEntries % 1000 == 0) {
          std::cout << "Number of parsed and Xapian-indexed records: "
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
  NbOfDBEntries_T IndexBuilder::
  buildSearchIndex (const PORFilePath_T& iPORFilePath,
                    const TravelDBFilePath_T& iTravelDBFilePath,
                    const DBType& iSQLDBType,
                    const SQLDBConnectionString_T& iSQLDBConnStr,
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
           << lTravelDBFilePath << "') does not exist or is not a directory.";
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
     *            2. Connection to the SQL Database
     */
    // Connect to the SQL database/file
    soci::session* lSociSession_ptr =
      DBManager::initSQLDBSession (iSQLDBType, iSQLDBConnStr);


    /**
     *            3. List of POR (points of reference)
     */
    // Check that the file exists and is regular.

    // DEBUG
    OPENTREP_LOG_DEBUG ("Parsing POR input file: " << iPORFilePath);

    // Get a reference on the file stream corresponding to the POR file.
    const PORFileHelper lPORFileHelper (iPORFilePath);
    std::istream& lPORFileStream = lPORFileHelper.getFileStreamRef();

    // Browse the input POR (point of reference) data file,
    // parse every of its rows, and put the result in the Xapian database/index
    // and, if needed, within the SQL database.
    oNbOfEntries = buildSearchIndex (lXapianDatabase, iSQLDBType,
                                     lSociSession_ptr,
                                     lPORFileStream, iTransliterator);

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


    return oNbOfEntries;
  }

}
