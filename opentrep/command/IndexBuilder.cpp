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
#include <boost/tokenizer.hpp>
// OpenTrep
#include <opentrep/bom/Utilities.hpp>
#include <opentrep/bom/StringPartition.hpp>
#include <opentrep/bom/WordCombinationHolder.hpp>
#include <opentrep/bom/World.hpp>
#include <opentrep/bom/Place.hpp>
#include <opentrep/factory/FacPlace.hpp>
#include <opentrep/command/DBManager.hpp>
#include <opentrep/command/IndexBuilder.hpp>
#include <opentrep/service/Logger.hpp>
// SOCI
#if defined(SOCI_HEADERS_BURIED)
#include <soci/core/soci.h>
#else // SOCI_HEADERS_BURIED
#include <soci/soci.h>
#endif // SOCI_HEADERS_BURIED
// Xapian
#include <xapian.h>

namespace OPENTREP {

  // //////////////////////////////////////////////////////////////////////
  void tokeniseAndAddToDocument (const std::string& iPhrase,
                                 Xapian::Document& ioDocument,
                                 Xapian::WritableDatabase& ioDatabase) {

    // Boost Tokeniser
    typedef boost::tokenizer<boost::char_separator<char> > Tokeniser_T;

    // Define the separators
    const boost::char_separator<char> lSepatorList(" .,;:|+-*/_=!@#$%`~^&(){}[]?'<>\"");

    // Initialise the phrase to be tokenised
    Tokeniser_T lTokens (iPhrase, lSepatorList);
    for (Tokeniser_T::const_iterator tok_iter = lTokens.begin();
         tok_iter != lTokens.end(); ++tok_iter) {
      const std::string& lTerm = *tok_iter;
      
      ioDatabase.add_spelling (lTerm);
      ioDocument.add_term (lTerm);

      // OPENTREP_LOG_DEBUG ("Added term: " << lTerm);
    }
  }
  
  // //////////////////////////////////////////////////////////////////////
  void tokeniseAndAddToDocumentNew (const std::string& iPhrase,
                                    Xapian::Document& ioDocument,
                                    Xapian::WritableDatabase& ioDatabase) {

    // Create a list made of all the word combinations of the initial phrase
    WordCombinationHolder lWordCombinationHolder (iPhrase);

    // Browse the list of unique strings (word combinations)
    const WordCombinationHolder::StringList_T& lStringList =
      lWordCombinationHolder._list;
    for (WordCombinationHolder::StringList_T::const_iterator itString =
           lStringList.begin();
         itString != lStringList.end(); ++itString) {
      const std::string& lWordCombination = *itString;
      
      // Add that combination of words into the Xapian index
      ioDatabase.add_spelling (lWordCombination);
      ioDocument.add_term (lWordCombination);
    } 

    // DEBUG
    OPENTREP_LOG_DEBUG ("Added terms for '" << iPhrase
                        << "': " << lWordCombinationHolder.toShortString());
  }
  
  // //////////////////////////////////////////////////////////////////////
  void IndexBuilder::
  addDocumentToIndex (Xapian::WritableDatabase& ioDatabase,
                      Place& ioPlace) {

    // Build a Xapian document
    Xapian::Document lDocument;
    
    // The document data is indeed meta-data, allowing a human being
    // to read it
    lDocument.set_data (ioPlace.toString());
      
    const std::string& lIataCode = ioPlace.getIataCode();
    const std::string& lIcaoCode = ioPlace.getIcaoCode();
    const std::string& lCityCode = ioPlace.getCityCode();
    const std::string& lDBCityCode = (lCityCode.empty())?lIataCode:lCityCode;
    const std::string& lStateCode = ioPlace.getStateCode();
    const std::string lDBStateCode = (lStateCode.empty())?"NA":lStateCode;

    // Word index/position within the Xapian document
    unsigned short idx = 1;
      
    // Add indexing terms
    lDocument.add_term (lIataCode); ++idx;
    if (lIcaoCode.empty() == false) {
      lDocument.add_term (lIcaoCode); ++idx;
    }
    lDocument.add_term (lDBStateCode); ++idx;
    lDocument.add_term (lDBCityCode); ++idx;
    lDocument.add_term (ioPlace.getCountryCode()); ++idx;
    lDocument.add_term (ioPlace.getRegionCode()); ++idx;

    // Add terms to the spelling dictionary
    ioDatabase.add_spelling (lIataCode);
    if (lIataCode != lDBCityCode) {
      ioDatabase.add_spelling (lDBCityCode);
    }
    if (lIcaoCode.empty() == false) {
      ioDatabase.add_spelling (lIcaoCode);
    }
    if (lStateCode.empty() == false) {
      ioDatabase.add_spelling (lStateCode);
    }
      
    // Retrieve the place names in all the available languages
    const NameMatrix_T& lNameMatrix = ioPlace.getNameMatrix ();
    for (NameMatrix_T::const_iterator itNameList = lNameMatrix.begin();
         itNameList != lNameMatrix.end(); ++itNameList) {
      // Retrieve the language code and locale
      const Language::EN_Language& lLanguage = itNameList->first;
      const Names& lNames = itNameList->second;

      // Add that language code and locale to the Xapian document
      lDocument.add_term (Language::getLongLabel (lLanguage)); ++idx;

      // For a given language, retrieve the list of place names
      const NameList_T& lNameList = lNames.getNameList();
        
      for (NameList_T::const_iterator itName = lNameList.begin();
           itName != lNameList.end(); ++itName) {
        const std::string& lName = *itName;

        // Add the place name (it can be the classical one, or
        // extended, alternate, etc.)
        if (lName.empty() == false) {
          // Add the full name (potentially containing spaces, e.g.,
          // 'san francisco').
          lDocument.add_term (lName); ++idx;
          ioDatabase.add_spelling (lName);

          // Add, as well, all the strings of all the partitions.
          tokeniseAndAddToDocumentNew (lName, lDocument, ioDatabase);

          // OPENTREP_LOG_DEBUG ("Added name: " << lName);
        }
      }
    }
      
    // Add the document to the database
    const Xapian::docid& lDocID = ioDatabase.add_document (lDocument);
      
    // Assign back the newly generated Xapian document ID to the
    // Place object
    ioPlace.setDocID (lDocID);
  }
  
  // //////////////////////////////////////////////////////////////////////
  NbOfDBEntries_T IndexBuilder::
  buildSearchIndex (soci::session& ioSociSession,
                    const TravelDatabaseName_T& iTravelDatabaseName) {
    NbOfDBEntries_T oNbOfEntries = 0;

    // Instanciate an empty place object, which will be filled from the
    // rows retrieved from the database.
    Place& lPlace = FacPlace::instance().create();

    // Prepare and execute the select statement
    soci::statement lSelectStatement (ioSociSession);
    DBManager::prepareSelectFromCodeStatement (ioSociSession,
                                               lSelectStatement, lPlace);

    // Check that the directory for the Xapian database (index) exists and,
    // if not, create it.
    // DEBUG
    OPENTREP_LOG_DEBUG ("The Xapian database ('" << iTravelDatabaseName
                        << "') will be cleared");
    boost::filesystem::remove_all (iTravelDatabaseName);
    boost::filesystem::create_directory (iTravelDatabaseName);

    // Create the Xapian database (index). As the directory has been fully
    // cleaned, deleted and re-created, that Xapian database (index) is empty.
    Xapian::WritableDatabase lDatabase (iTravelDatabaseName, Xapian::DB_CREATE);

    // Iterate through the retrieved database rows
    const bool shouldDoReset = true;
    bool hasStillData = DBManager::iterateOnStatement (lSelectStatement,
                                                       lPlace,
                                                       shouldDoReset);

    /**
     * Begin a transation on the Xapian database (index).
     *
     * Normally, there are around 11,000 entries (rows/documents) to
     * be indexed. Not specifying the beginning of a transaction would
     * mean that every document addition would end up in a corresponding
     * independant transaction, which would be very much inefficient.
     */
    lDatabase.begin_transaction();

    while (hasStillData == true) {
      // Add the document, corresponding to the Place object, to the
      // Xapian index
      IndexBuilder::addDocumentToIndex (lDatabase, lPlace);

      //
      ++oNbOfEntries;
        
      // DEBUG
      OPENTREP_LOG_DEBUG ("[" << oNbOfEntries << "] " << lPlace);
        
      // Iterate on the MySQL database cursor
      hasStillData = DBManager::iterateOnStatement (lSelectStatement, lPlace,
                                                    shouldDoReset);
    }

    // Commit the pending modifications on the Xapian database (index)
    lDatabase.commit_transaction();

    // DEBUG
    OPENTREP_LOG_DEBUG ("Xapian has indexed " << oNbOfEntries << " entries.");

    /**
     * Close the Xapian database (index).
     *
     * \note For a yet unknown reason, when the current method is called within
     *       the Boost Unit Test framework, that latter kills the process.
     *       When called from within GDB, all is fine.
     */
    lDatabase.close();

    return oNbOfEntries;
  }

}
