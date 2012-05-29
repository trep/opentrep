// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// STL
#include <cassert>
#include <sstream>
// Boost
#include <boost/lexical_cast.hpp>
// OpenTrep
#include <opentrep/bom/Filter.hpp>
#include <opentrep/bom/WordHolder.hpp>
#include <opentrep/bom/PlaceKey.hpp>
#include <opentrep/bom/MatchingDocuments.hpp>
#include <opentrep/service/Logger.hpp>

namespace OPENTREP {

  // //////////////////////////////////////////////////////////////////////
  MatchingDocuments::MatchingDocuments (const TravelQuery_T& iQueryString)
    : _queryString (iQueryString), _hasFullTextMatched (false) {
  }

  // //////////////////////////////////////////////////////////////////////
  MatchingDocuments::~MatchingDocuments() {
  }

  // //////////////////////////////////////////////////////////////////////
  std::string MatchingDocuments::describeKey() const {
    std::ostringstream oStr;
    oStr << "'" << _queryString << "' ";
    if (_correctedQueryString.empty() == false) {
      oStr << "(corrected into '" << _correctedQueryString
           << "' with an edit distance/error of " << _editDistance
           << " over an allowable distance of " << _allowableEditDistance
           << ") ";
    }
    return oStr.str();
  }

  // //////////////////////////////////////////////////////////////////////
  std::string MatchingDocuments::describe() const {
    std::ostringstream oStr;
    oStr << describeKey();
    
    if (_documentList.empty() == true) {
      oStr << "No match" << std::endl;
      return oStr.str();
    }
    assert (_hasFullTextMatched == true);

    unsigned short idx = 0;
    for (DocumentList_T::const_iterator itDoc = _documentList.begin();
         itDoc != _documentList.end(); ++itDoc, ++idx) {
      const XapianDocumentPair_T& lDocumentPair = *itDoc;

      const Xapian::Document& lXapianDoc = lDocumentPair.first;
      const Xapian::docid& lDocID = lXapianDoc.get_docid();

      const ScoreBoard& lScoreBoard = lDocumentPair.second;

      if (idx != 0) {
        oStr << ", ";
      }
      oStr << "Doc ID: " << lDocID << ", matching with ("
           << lScoreBoard.describe() << "), containing: '"
           << lXapianDoc.get_data() << "'";
    }

    return oStr.str();
  }   

  // //////////////////////////////////////////////////////////////////////
  void MatchingDocuments::toStream (std::ostream& ioOut) const {
    ioOut << describe();
  }

  // //////////////////////////////////////////////////////////////////////
  void MatchingDocuments::fromStream (std::istream& ioIn) {
  }

  // //////////////////////////////////////////////////////////////////////
  void MatchingDocuments::addDocument (const Xapian::Document& iDocument,
                                       const Score_T& iScore) {
    // The document is created at the time of (Xapian-based) full-text matching
    const ScoreType lXapianScoreType (ScoreType::XAPIAN_PCT);

    // Create a ScoreBoard structure
    const ScoreBoard lScoreBoard (lXapianScoreType, iScore);

    // Retrieve the ID of the Xapian document
    const Xapian::docid& lDocID = iDocument.get_docid();

    // Create a (Xapian document, score board) pair, so as to store
    // the document along with its corresponding score board
    const XapianDocumentPair_T lDocumentPair (iDocument, lScoreBoard);

    // Insert the just created pair into the dedicated (STL) list
    _documentList.push_back (lDocumentPair);

    // Insert the just created pair into the dedicated (STL) map
    const bool hasInsertBeenSuccessful =
      _documentMap.insert (DocumentMap_T::value_type (lDocID,
                                                      lDocumentPair)).second;
    // Sanity check
    assert (hasInsertBeenSuccessful == true);
  }

  // //////////////////////////////////////////////////////////////////////
  PlaceKey MatchingDocuments::getPrimaryKey(const Xapian::Document& iDocument) {
    // Retrieve the Xapian document data
    const std::string& lDocumentData = iDocument.get_data();

    // Tokenise the string into words
    WordList_T lWordList;
    WordHolder::tokeniseStringIntoWordList (lDocumentData, lWordList);
    assert (lWordList.size() > 3);

    // By convention (within OpenTrep), the first three words of the Xapian
    // document data string constitute the primary key of the place
    WordList_T::const_iterator itWord = lWordList.begin();
    const std::string& lIataCode = *itWord;
    ++itWord; const std::string& lIcaoCode = *itWord;
    ++itWord; const std::string& lGeonamesIDStr = *itWord;
    const GeonamesID_T lGeonamesID =
      boost::lexical_cast<GeonamesID_T> (lGeonamesIDStr);

    return PlaceKey (lIataCode, lIcaoCode, lGeonamesID);
  }
  
  // //////////////////////////////////////////////////////////////////////
  const Xapian::Document& MatchingDocuments::getBestXapianDocument() const {
    // Retrieve the Xapian document corresponding to the doc ID of the
    // best matching document
    DocumentMap_T::const_iterator itDoc = _documentMap.find (_bestDocID);
    assert (itDoc != _documentMap.end());

    //
    const XapianDocumentPair_T& lDocumentPair = itDoc->second;
    const Xapian::Document& oXapianDocument = lDocumentPair.first;

    //
    return oXapianDocument;
  }

  // //////////////////////////////////////////////////////////////////////
  Percentage_T MatchingDocuments::
  getPageRank (const Xapian::Document& iDocument) {
    // Retrieve the Xapian document data
    const std::string& lDocumentData = iDocument.get_data();

    // Tokenise the string into words
    WordList_T lWordList;
    WordHolder::tokeniseStringIntoWordList (lDocumentData, lWordList);
    assert (lWordList.size() >= 4);

    // By convention (within OpenTrep), the fourth word of the Xapian
    // document data string constitutes the PageRank percentage of the place
    WordList_T::const_iterator itWord = lWordList.begin();
    ++itWord; ++itWord; ++itWord; const std::string& lPercentageStr = *itWord;
    const Percentage_T oPercentage =
      boost::lexical_cast<Percentage_T> (lPercentageStr);

    return oPercentage;
  }
  
  // //////////////////////////////////////////////////////////////////////
  void MatchingDocuments::calculatePageRanks() {
    // Browse the list of Xapian documents
    for (DocumentList_T::iterator itDoc = _documentList.begin();
         itDoc != _documentList.end(); ++itDoc) {
      XapianDocumentPair_T& lDocumentPair = *itDoc;

      // Retrieve the Xapian document
      const Xapian::Document& lXapianDoc = lDocumentPair.first;

      // Extract the PageRank from the document data.      
      const Score_T& lPageRank = getPageRank (lXapianDoc);

      // Store the PageRank weight
      ScoreBoard& lScoreBoard = lDocumentPair.second;
      lScoreBoard.setScore (ScoreType::PAGE_RANK, lPageRank);
    }
  }

  // //////////////////////////////////////////////////////////////////////
  void MatchingDocuments::calculateUserInputWeights() {
    /**
     * Nothing for now. The user input weight, by construction,
     * should come from the user, not from the document. So,
     * parameters would be needed along the call to that method.
     */
  }

  // //////////////////////////////////////////////////////////////////////
  void MatchingDocuments::calculateCombinedWeights() {
    Percentage_T lMaxPercentage = 0.0;

    // Browse the list of Xapian documents
    Xapian::docid lBestDocID = 0;
    for (DocumentList_T::iterator itDoc = _documentList.begin();
         itDoc != _documentList.end(); ++itDoc) {
      XapianDocumentPair_T& lDocumentPair = *itDoc;

      // Retrieve the Xapian document ID
      const Xapian::Document& lXapianDoc = lDocumentPair.first;
      const Xapian::docid& lDocID = lXapianDoc.get_docid();

      /**
       * Calculate the combined weight, resulting from all the rules
       * (e.g., full-text matching, PageRank, user input).
       */
      ScoreBoard& lScoreBoard = lDocumentPair.second;
      const Percentage_T& lPercentage = lScoreBoard.calculateMatchingWeight();

      // Register the document, if it is the best matching until now
      if (lPercentage > lMaxPercentage) {
        lMaxPercentage = lPercentage;
        lBestDocID = lDocID;
      }
    }

    //
    if (hasFullTextMatched() == true) {
      // DEBUG
      OPENTREP_LOG_DEBUG ("        [pct] '" << describeKey()
                          << "' matches at " << lMaxPercentage
                          << "% for doc ID: " << lBestDocID);

    } else {
      // Check whether or not the query string is made of a single word
      WordList_T lWordList;
      WordHolder::tokeniseStringIntoWordList (_queryString, lWordList);
      NbOfWords_T nbOfWords = lWordList.size();

      /**
       * Check whether that word should be filtered out (e.g., less than
       * 3 characters, words like 'international', 'airport', etc).
       */
      const bool shouldBeKept = Filter::shouldKeep ("", _queryString);

      if (nbOfWords == 1 && shouldBeKept == true) {
        /**
         * There is no full-text match for that single-word query. That is
         * therefore an unknown word. The percentage is set to 100%, though,
         * to not disqualify the rest of the string set.
         */
        lMaxPercentage = 100.0;

        // DEBUG
        OPENTREP_LOG_DEBUG("        [pct] '" << describeKey()
                           << "' does not match, but non black-listed "
                           << "single-word string, i.e., "
                           << lMaxPercentage << "%");

      } else {
        /**
         * There is no full-text match for that query, which is made either
         * of several words or of a single black-listed word (e.g., 'airport').
         * The corresponding percentage is set to something low (5%),
         * in order to significantly decrease the overall matching
         * percentage. The corresponding string set will therefore have
         * almost no chance to being selected/chosen.
         */
        lMaxPercentage = 5.0;

        // DEBUG
        OPENTREP_LOG_DEBUG("        [pct] '" << describeKey()
                           << "' does not match, and either multiple-word "
                           << "string or black-listed i.e., "
                           << lMaxPercentage << "%");
      }
    }

    // Store the doc ID of the best matching document
    setBestDocID (lBestDocID);

    // Store the best weight
    setBestCombinedWeight (lMaxPercentage);
  }

}
