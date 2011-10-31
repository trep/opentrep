// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// STL
#include <istream>
#include <ostream>
#include <string>
#include <vector>
#include <exception>
// Boost
#include <boost/tokenizer.hpp>
// OpenTrep
#include <opentrep/bom/World.hpp>
#include <opentrep/bom/Place.hpp>
#include <opentrep/factory/FacPlace.hpp>
#include <opentrep/command/DBManager.hpp>
#include <opentrep/command/IndexBuilder.hpp>
#include <opentrep/service/Logger.hpp>
// SOCI
#include <soci/core/soci.h>
//#include <soci/backends/mysql/soci-mysql.h>
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
  void IndexBuilder::
  addDocumentToIndex (Xapian::WritableDatabase& ioDatabase,
                      Place& ioPlace) {

    try {
    
      // Build a Xapian document
      Xapian::Document lDocument;
    
      // The document data is indeed meta-data, allowing a human being
      // to read it
      lDocument.set_data (ioPlace.toString());
      
      /* When the city code is empty, it means that the place is a city and
         not an airport. The city code is thus the same as the place code
         itself. It is added twice to the indexing terms, so that when the
         user searches for that term, the corresponding city get more weight
         compared to the airports of the same city.
      */
      const std::string& lPlaceCode = ioPlace.getPlaceCode();
      const std::string& lCityCode = ioPlace.getCityCode();
      const std::string lDBCityCode =
        (lCityCode.empty())?lPlaceCode:lCityCode;
      const std::string& lStateCode = ioPlace.getStateCode();
      const std::string lDBStateCode = (lStateCode.empty())?"NA":lStateCode;

      // Word index/position within the Xapian document
      unsigned short idx = 1;
      
      // Add indexing terms
      lDocument.add_term (lPlaceCode); ++idx;
      lDocument.add_term (lDBCityCode); ++idx;
      lDocument.add_term (lDBStateCode); ++idx;
      lDocument.add_term (ioPlace.getCountryCode()); ++idx;
      lDocument.add_term (ioPlace.getRegionCode()); ++idx;
      lDocument.add_term (ioPlace.getContinentCode()); ++idx;
      lDocument.add_term (ioPlace.getTimeZoneGroup()); ++idx;

      // Add terms to the spelling dictionnary
      ioDatabase.add_spelling (lPlaceCode);
      ioDatabase.add_spelling (lDBCityCode);
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
            // 'san francisco'), as well as each word
            // within it (with the example above, 'san' and 'francisco').
            lDocument.add_term (lName); ++idx;
            ioDatabase.add_spelling (lName);
            tokeniseAndAddToDocument (lName, lDocument, ioDatabase);

            // OPENTREP_LOG_DEBUG ("Added name: " << lName);
          }
        }
      }
      
      // Add the document to the database
      const Xapian::docid& lDocID = ioDatabase.add_document (lDocument);
      
      // Assign back the newly generated Xapian document ID to the
      // Place object
      ioPlace.setDocID (lDocID);
      
    } catch (const Xapian::Error& lXapianError) {
      OPENTREP_LOG_ERROR ("Xapian error: "  << lXapianError.get_msg());
      throw XapianException();
    }
  }
  
  // //////////////////////////////////////////////////////////////////////
  NbOfDBEntries_T IndexBuilder::
  buildSearchIndex (soci::session& ioSociSession,
                    const TravelDatabaseName_T& iTravelDatabaseName) {
    NbOfDBEntries_T oNbOfEntries = 0;

    try {
    
      // Instanciate an empty place object, which will be filled from the
      // rows retrieved from the database.
      Place& lPlace = FacPlace::instance().create();

      // Prepare and execute the select statement
      soci::statement lSelectStatement (ioSociSession);
      DBManager::prepareSelectFromCodeStatement (ioSociSession,
                                                 lSelectStatement, lPlace);

      // Create the Xapian database (index)
      Xapian::WritableDatabase lDatabase (iTravelDatabaseName,
                                          Xapian::DB_CREATE_OR_OVERWRITE);

      // Start a transaction on the Xapian database (index)
      lDatabase.begin_transaction();

      // Iterate through the retrieved database rows
      const bool shouldDoReset = true;
      bool hasStillData = DBManager::iterateOnStatement (lSelectStatement,
                                                         lPlace,
                                                         shouldDoReset);
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

      // Commit the transaction on the Xapian database (index)
      lDatabase.commit_transaction();
    
    } catch (const Xapian::Error& lXapianError) {
      OPENTREP_LOG_ERROR ("Xapian error: "  << lXapianError.get_msg());
      throw XapianException();
      
    } catch (const RootException& lTrepException) {
      OPENTREP_LOG_ERROR ("General OpenTrep error: " << lTrepException.what());
      throw RootException();
      
    } catch (const std::exception& lStdError) {
      OPENTREP_LOG_ERROR ("Error: " << lStdError.what());
      throw RootException();
    }

    return oNbOfEntries;
  }

}
