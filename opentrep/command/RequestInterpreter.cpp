// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// STL
#include <cassert>
#include <sstream>
#include <string>
#include <vector>
#include <exception>
// OpenTrep
#include <opentrep/bom/Place.hpp>
#include <opentrep/bom/ResultHolder.hpp>
#include <opentrep/bom/Result.hpp>
#include <opentrep/bom/PlaceHolder.hpp>
#include <opentrep/bom/StringMatcher.hpp>
#include <opentrep/factory/FacPlaceHolder.hpp>
#include <opentrep/factory/FacPlace.hpp>
#include <opentrep/factory/FacResultHolder.hpp>
#include <opentrep/factory/FacResult.hpp>
#include <opentrep/command/DBManager.hpp>
#include <opentrep/command/RequestInterpreter.hpp>
#include <opentrep/service/Logger.hpp>
// Xapian
#include <xapian.h>

namespace OPENTREP {

  // //////////////////////////////////////////////////////////////////////
  void createResults (const DocumentList_T& iDocumentList,
                      const Xapian::Database& iXapianDatabase,
                      ResultHolder& ioResultHolder) {

    // Back-up the (retrieved) matching Xapian documents into still
    // to-be-created Result objects.
    for (DocumentList_T::const_iterator itDoc = iDocumentList.begin();
         itDoc != iDocumentList.end(); ++itDoc) {
      // Retrieve both the Xapian document object and the corresponding
      // matching percentage (most of the time, it is 100%)
      const Document& lMatchingDocument = *itDoc;
      
      // Create a Result object
      Result& lResult = FacResult::instance().create (iXapianDatabase);
      
      // Fill the Result object with both the corresponding Document object
      // and its associated query string
      lResult.setMatchingDocument (lMatchingDocument);
      
      // Add the Result object (holding the list of matching
      // documents) to the dedicated list.
      FacResultHolder::initLinkWithResult (ioResultHolder, lResult);
    }
    
    // DEBUG
    OPENTREP_LOG_DEBUG (std::endl
                        << "========================================="
                        << std::endl << "Matching list: "  << std::endl
                        << ioResultHolder.toString()
                        << "========================================="
                        << std::endl << std::endl);
  }

  /** Helper function. */
  // //////////////////////////////////////////////////////////////////////
  bool retrieveAndFillPlace (const std::string& iOriginalKeywords,
                             const std::string& iCorrectedKeywords,
                             const Xapian::Document& iDocument,
                             const Xapian::percent& iDocPercentage,
                             soci::session& ioSociSession, Place& ioPlace) {
    bool hasRetrievedPlace = false;

    // Set the original and corrected/suggested keywords
    ioPlace.setOriginalKeywords (iOriginalKeywords);
    ioPlace.setCorrectedKeywords (iCorrectedKeywords);
    
    // Set the matching percentage
    ioPlace.setPercentage (iDocPercentage);
    
    // Retrieve the parameters of the best matching document
    const PlaceKey& lKey = Document::getPrimaryKey (iDocument);

    // DEBUG
    const Xapian::docid& lDocID = iDocument.get_docid();
    const std::string& lDocData = iDocument.get_data();
    OPENTREP_LOG_DEBUG ("Place key: " << lKey << " - Xapian ID " << lDocID
                        << ", " << iDocPercentage << "% [" << lDocData << "]");

    // Fill the Place object with the row retrieved from the
    // (MySQL) database and corresponding to the given place code
    // (e.g., 'sfo' for the San Francisco Intl airport).
    hasRetrievedPlace = DBManager::retrievePlace (ioSociSession, lKey, ioPlace);

    if (hasRetrievedPlace == false) {
      /**
       * The Xapian database/index should contain only places
       * available within the SQL database, as the first is built from
       * the latter.  If that happens, it means that the user gave a
       * wrong Xapian database.
       */
      std::ostringstream errorStr;
      errorStr << "There is no document corresponding to " << lKey
               << " (Xapian document ID" << lDocID
               << " [" << lDocData << "]) in the SQL database. "
               << "It usually means that the Xapian index/database "
               << "is not synchronised with the SQL database. "
               << "[Hint] Rebuild the Xapian index/database "
               << "from the SQL database.";
      OPENTREP_LOG_ERROR (errorStr.str());
      throw XapianTravelDatabaseNotInSyncWithSQLDatabaseException (errorStr.str());
    }
    
    return hasRetrievedPlace;
  }
  
  /** Helper function. */
  // //////////////////////////////////////////////////////////////////////
  bool retrieveAndFillPlace (const Document& iDocument,
                             soci::session& ioSociSession, Place& ioPlace) {
    // Note that Document::getTravelQuery() returns a TravelQuery_T, which
    // is actually a std::string
    const std::string& lOriginalKeywords = iDocument.getTravelQuery();
    const std::string& lCorrectedKeywords = iDocument.getCorrectedTravelQuery();
    
    // Delegate
    const Xapian::Document& lXapianDocument = iDocument.getXapianDocument();
    const Xapian::percent& lDocPercentage = iDocument.getXapianPercentage();
    return retrieveAndFillPlace (lOriginalKeywords, lCorrectedKeywords,
                                 lXapianDocument, lDocPercentage,
                                 ioSociSession, ioPlace);
  }
  
  // //////////////////////////////////////////////////////////////////////
  void createPlaces (const ResultHolder& iResultHolder,
                     soci::session& ioSociSession, PlaceHolder& ioPlaceHolder) {
    
    // Browse the list of result objects
    const ResultList_T& lResultList = iResultHolder.getResultList();
    for (ResultList_T::const_iterator itResult = lResultList.begin();
         itResult != lResultList.end(); ++itResult) {
      // Retrieve the result object
      const Result* lResult_ptr = *itResult;
      assert (lResult_ptr != NULL);

      // Retrieve the matching document
      const Document& lDocument = lResult_ptr->getMatchingDocument();
      
      // Instanciate an empty place object, which will be filled from the
      // rows retrieved from the database.
      Place& lPlace = FacPlace::instance().create();
      
      // Retrieve, in the MySQL database, the place corresponding to
      // the place code located as the first word of the Xapian
      // document data.
      bool hasRetrievedPlace = retrieveAndFillPlace (lDocument, ioSociSession,
                                                     lPlace);

      // Retrieve the effective (Levenshtein) edit distance/error, as
      // well as the allowable edit distance/error, and store them in
      // the Place object.
      const NbOfErrors_T& lEditDistance = lDocument.getEditDistance();
      const NbOfErrors_T& lAllowableEditDistance =
        lDocument.getAllowableEditDistance();
      lPlace.setEditDistance (lEditDistance);
      lPlace.setAllowableEditDistance (lAllowableEditDistance);

      // If there was no place corresponding to the place code with
      // the SQL database, an exception is thrown. Hence, here, by
      // construction, the place has been retrieved from the SQL
      // database.
      assert (hasRetrievedPlace == true);

      // Insert the Place object within the PlaceHolder object
      FacPlaceHolder::initLinkWithPlace (ioPlaceHolder, lPlace);
      
      // DEBUG
      OPENTREP_LOG_DEBUG ("Retrieved Document: " << lPlace.toString());

      // Retrieve the list of extra matching documents (documents
      // matching with the same weight/percentage)
      const std::string& lOriginalKeywords = lDocument.getTravelQuery();
      const std::string& lCorrectedKeywords =
        lDocument.getCorrectedTravelQuery();
      const Xapian::percent& lExtraDocPercentage =
        lDocument.getXapianPercentage();
      const XapianDocumentList_T& lExtraDocumentList =
        lDocument.getExtraDocumentList();
      for (XapianDocumentList_T::const_iterator itExtraDoc =
             lExtraDocumentList.begin();
           itExtraDoc != lExtraDocumentList.end(); ++itExtraDoc) {
        // Retrieve the extra matching Xapian document
        const Xapian::Document& lExtraDocument = *itExtraDoc;
        
        // Instanciate an empty place object, which will be filled from the
        // rows retrieved from the database.
        Place& lExtraPlace = FacPlace::instance().create();
      
        // Retrieve, in the MySQL database, the place corresponding to
        // the place code located as the first word of the Xapian
        // document data.
        hasRetrievedPlace =
          retrieveAndFillPlace (lOriginalKeywords, lCorrectedKeywords,
                                lExtraDocument, lExtraDocPercentage,
                                ioSociSession, lExtraPlace);

        // Same remark as above
        assert (hasRetrievedPlace == true);

        // The extra matching Place object has the very same effective
        // (Levenshtein) and allowable edit distances/errors as the
        // main Place object.
        lExtraPlace.setEditDistance (lEditDistance);
        lExtraPlace.setAllowableEditDistance (lAllowableEditDistance);
      
        // Insert the extra matching Place object within the dedicated
        // list within the main Place object
        FacPlace::initLinkWithExtraPlace (lPlace, lExtraPlace);

        // DEBUG
        OPENTREP_LOG_DEBUG ("Retrieved Document: " << lExtraPlace.toString());
      }

      // Retrieve the list of alternate matching documents (documents
      // matching with the a lower weight)
      const XapianAlternateDocumentList_T& lAlternateDocumentList =
        lDocument.getAlternateDocumentList();
      for (XapianAlternateDocumentList_T::const_iterator itAlterDoc =
             lAlternateDocumentList.begin();
           itAlterDoc != lAlternateDocumentList.end(); ++itAlterDoc) {
        // Retrieve the alternate matching Xapian document
        const Xapian::percent& lAlterDocPercentage = itAlterDoc->first;
        const Xapian::Document& lAlterDocument = itAlterDoc->second;
        
        // Instanciate an empty place object, which will be filled from the
        // rows retrieved from the database.
        Place& lAlterPlace = FacPlace::instance().create();
      
        // Retrieve, in the MySQL database, the place corresponding to
        // the place code located as the first word of the Xapian
        // document data.
        hasRetrievedPlace =
          retrieveAndFillPlace (lOriginalKeywords, lCorrectedKeywords,
                                lAlterDocument, lAlterDocPercentage,
                                ioSociSession, lAlterPlace);

        // Same remark as above
        assert (hasRetrievedPlace == true);

        // The extra matching Place object has the very same effective
        // (Levenshtein) and allowable edit distances/errors as the
        // main Place object.
        lAlterPlace.setEditDistance (lEditDistance);
        lAlterPlace.setAllowableEditDistance (lAllowableEditDistance);
        
        // Insert the alternate matching Place object within the dedicated
        // list within the main Place object
        FacPlace::initLinkWithAlternatePlace (lPlace, lAlterPlace);

        // DEBUG
        OPENTREP_LOG_DEBUG ("Retrieved Document: " << lAlterPlace.toString());
      }

      // DEBUG
      OPENTREP_LOG_DEBUG ("Fully retrieved Document: " << lPlace.toString());
    }
  }
  
  // //////////////////////////////////////////////////////////////////////
  NbOfMatches_T RequestInterpreter::
  interpretTravelRequest (soci::session& ioSociSession,
                          const TravelDatabaseName_T& iTravelDatabaseName,
                          const TravelQuery_T& iTravelQuery,
                          LocationList_T& ioLocationList,
                          WordList_T& ioWordList) {
    NbOfMatches_T oNbOfMatches = 0;

    // Sanity checks
    assert (iTravelDatabaseName.empty() == false);
    assert (iTravelQuery.empty() == false);

    // Create a PlaceHolder object, to collect the matching Place objects
    PlaceHolder& lPlaceHolder = FacPlaceHolder::instance().create();

    // Make the database
    Xapian::Database lXapianDatabase (iTravelDatabaseName);

    // Create a ResultHolder object
    ResultHolder& lResultHolder =
      FacResultHolder::instance().create (iTravelQuery, lXapianDatabase);

    // DEBUG
    OPENTREP_LOG_DEBUG (std::endl
                        << "=========================================");
      
    // Main algorithm
    DocumentList_T lDocumentList;
    lResultHolder.searchStringNew (lDocumentList, ioWordList);

    /** Create the list of Result objects corresponding to the list
        of documents. */
    createResults (lDocumentList, lXapianDatabase, lResultHolder);

    /** Create the list of Place objects, for each of which a
        look-up is made in the SQL database (e.g., MySQL or Oracle)
        to retrieve complementary data. */
    createPlaces (lResultHolder, ioSociSession, lPlaceHolder);
      
    // DEBUG
    OPENTREP_LOG_DEBUG (std::endl
                        << "========================================="
                        << std::endl << "Summary:" << std::endl
                        << lPlaceHolder.toShortString() << std::endl
                        << "========================================="
                        << std::endl);

    /** Create the list of Location structures, which are light copies
        of the Place objects. Those (Location) structures are passed
        back to the caller of the service. */
    lPlaceHolder.createLocations (ioLocationList);
    oNbOfMatches = ioLocationList.size();
    
    return oNbOfMatches;
  }
  
}
