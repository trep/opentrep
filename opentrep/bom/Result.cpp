// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// STL
#include <cassert>
#include <sstream>
// Boost
#include <boost/tokenizer.hpp>
#include <boost/lexical_cast.hpp>
// OpenTREP
#include <opentrep/LocationKey.hpp>
#include <opentrep/basic/BasConst_General.hpp>
#include <opentrep/bom/Filter.hpp>
#include <opentrep/bom/WordHolder.hpp>
#include <opentrep/bom/StringPartition.hpp>
#include <opentrep/bom/Levenshtein.hpp>
#include <opentrep/bom/Place.hpp>
#include <opentrep/bom/Result.hpp>
#include <opentrep/bom/PORParserHelper.hpp>
#include <opentrep/service/Logger.hpp>

namespace OPENTREP {

  // //////////////////////////////////////////////////////////////////////
  Result::Result (const TravelQuery_T& iQueryString,
                  const Xapian::Database& iDatabase)
    : _resultHolder (NULL), _database (iDatabase),
      _queryString (iQueryString), _hasFullTextMatched (false),
      _bestDocData (RawDataString_T ("")) {
    init();
  }
  
  // //////////////////////////////////////////////////////////////////////
  Result::~Result() {
  }

  // //////////////////////////////////////////////////////////////////////
  void Result::init() {
  }

  // //////////////////////////////////////////////////////////////////////
  std::string Result::describeShortKey() const {
    std::ostringstream oStr;
    oStr << _queryString;
    return oStr.str();
  }
  
  // //////////////////////////////////////////////////////////////////////
  std::string Result::describeKey() const {
    std::ostringstream oStr;
    oStr << "'" << describeShortKey() << "' ";
    if (_correctedQueryString.empty() == false
        && _correctedQueryString != _queryString) {
      oStr << "(corrected into '" << _correctedQueryString
           << "' with an edit distance/error of " << _editDistance
           << " over an allowable distance of " << _allowableEditDistance
           << ") - ";
    } else {
      oStr << "- ";
    }
    return oStr.str();
  }

  // //////////////////////////////////////////////////////////////////////
  std::string Result::toString() const {
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
  void Result::toStream (std::ostream& ioOut) const {
    ioOut << toString();
  }
  
  // //////////////////////////////////////////////////////////////////////
  void Result::fromStream (std::istream& ioIn) {
  }
  
  // //////////////////////////////////////////////////////////////////////
  const Xapian::Document& Result::
  getDocument (const Xapian::docid& iDocID) const {
    // Retrieve the Xapian document corresponding to the doc ID of the
    // best matching document
    DocumentMap_T::const_iterator itDoc = _documentMap.find (iDocID);

    if (itDoc == _documentMap.end()) {
      OPENTREP_LOG_ERROR ("The Xapian document (ID = " << iDocID
                          << ") can not be found in the Result object "
                          << describeKey());
    }
    assert (itDoc != _documentMap.end());

    //
    const XapianDocumentPair_T& lDocumentPair = itDoc->second;
    const Xapian::Document& oXapianDocument = lDocumentPair.first;

    //
    return oXapianDocument;
  }

  // //////////////////////////////////////////////////////////////////////
  void Result::addDocument (const Xapian::Document& iDocument,
                            const Score_T& iScore) {
    /**
     * When the match has occurred with spelling correction, take that
     * into account by decreasing the matching percentage in proportion
     * to the Levenshtein edit distance.
     */
    Score_T lCorrectedScore = iScore;
    if (_editDistance > 0) {
      lCorrectedScore = iScore
        / (K_DEFAULT_SIZE_FOR_SPELLING_ERROR_UNIT * _editDistance);
    }

    // The document is created at the time of (Xapian-based) full-text matching
    const ScoreType lXapianScoreType (ScoreType::XAPIAN_PCT);

    // Create a ScoreBoard structure
    const ScoreBoard lScoreBoard (_queryString,
                                  lXapianScoreType, lCorrectedScore);

    // Retrieve the ID of the Xapian document
    const Xapian::docid& lDocID = iDocument.get_docid();

    /**
    // DEBUG
    const LocationKey& lLocationKey = getPrimaryKey (iDocument);
    OPENTREP_LOG_DEBUG ("        '" << describeShortKey()
                        << "', " << lLocationKey << " (doc ID = " << lDocID
                        << ") has the following weight: " << iScore
                        << "% (corrected into " << lCorrectedScore << "%)");
    */

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
  void Result::fillResult (const Xapian::MSet& iMatchingSet) {
    /**
     * Retrieve the best matching documents, each with its own
     * (Xapian-based) full-text score / weighting percentage.
     */
    for (Xapian::MSetIterator itDoc = iMatchingSet.begin();
         itDoc != iMatchingSet.end(); ++itDoc) {
      const Xapian::percent& lXapianPercentage = itDoc.get_percent();
      const Xapian::Document& lDocument = itDoc.get_document();
      addDocument (lDocument, lXapianPercentage);
    }
  }

  // //////////////////////////////////////////////////////////////////////
  void Result::fillPlace (Place& ioPlace) const {
    // Set the original and corrected/suggested keywords
    ioPlace.setOriginalKeywords (_queryString);
    ioPlace.setCorrectedKeywords (_correctedQueryString);

    // Set the effective (Levenshtein) edit distance/error, as
    // well as the allowable edit distance/error
    ioPlace.setEditDistance (_editDistance);
    ioPlace.setAllowableEditDistance (_allowableEditDistance);

    // Set the Xapian document ID
    ioPlace.setDocID (_bestDocID);
    
    // Set the matching percentage
    ioPlace.setPercentage (_bestCombinedWeight);
    
    // Retrieve the parameters of the best matching document
    const LocationKey& lKey = ioPlace.getKey();

    // DEBUG
    OPENTREP_LOG_DEBUG ("Place key: " << lKey << " - Xapian ID " << _bestDocID
                        << ", " << _bestCombinedWeight << "% [" << _bestDocData
                        << "]");
  }

  /**
   * @brief Helper function
   *
   * Given the size of the phrase, determine the allowed edit distance for
   * spelling purpose. For instance, if K_DEFAULT_SIZE_FOR_SPELLING_ERROR_UNIT
   * is equal to 4:
   * <ul>
   *   <li>An edit distance of 1 will be allowed on a 4-letter word</li>
   *   <li>While an edit distance of 3 will be allowed on an 11-letter word.</li>
   * </ul>
   */
  // //////////////////////////////////////////////////////////////////////
  static unsigned int calculateEditDistance (const TravelQuery_T& iPhrase) {
    NbOfErrors_T oEditDistance = 2;

    const NbOfErrors_T lQueryStringSize = iPhrase.size();
    
    oEditDistance = lQueryStringSize / K_DEFAULT_SIZE_FOR_SPELLING_ERROR_UNIT;
    return oEditDistance;
  }
  
  // //////////////////////////////////////////////////////////////////////
  Location Result::retrieveLocation (const RawDataString_T& iRawDataString) {
    // Initialise the POR (point of reference) parser
    PORStringParser lStringParser (iRawDataString);

    // Parse the raw data
    const Location& oLocation = lStringParser.generateLocation();

    // DEBUG
    //OPENTREP_LOG_DEBUG ("Location: " << oLocation);

    return oLocation;
  }

  // //////////////////////////////////////////////////////////////////////
  Location Result::retrieveLocation (const Xapian::Document& iDocument) {
    // Retrieve the Xapian document data
    const std::string& lDocumentDataStr = iDocument.get_data();
    const RawDataString_T& lDocumentData = RawDataString_T (lDocumentDataStr);

    // Parse the POR details and create the corresponding Location structure
    const Location& oLocation = retrieveLocation (lDocumentData);

    return oLocation;
  }

  // //////////////////////////////////////////////////////////////////////
  LocationKey Result::getPrimaryKey (const Xapian::Document& iDocument) {
    // Parse the POR (point of reference) details held by the Xapian document
    const Location& lLocation = retrieveLocation (iDocument);

    // Get the key (IATA and ICAO codes, GeonamesID)
    const LocationKey& oLocationKey = lLocation.getKey();

    return oLocationKey;
  }
  
  // //////////////////////////////////////////////////////////////////////
  PageRank_T Result::getPageRank (const Xapian::Document& iDocument) {
    // Parse the POR (point of reference) details held by the Xapian document
    const Location& lLocation = retrieveLocation (iDocument);

    // Get the PageRank value
    const PageRank_T& oPageRank = lLocation.getPageRank();

    return oPageRank;
  }
  
  // //////////////////////////////////////////////////////////////////////
  std::string Result::fullTextMatch (const Xapian::Database& iDatabase,
                                     const TravelQuery_T& iQueryString,
                                     Xapian::MSet& ioMatchingSet) {
    std::string oMatchedString;

    // Catch any Xapian::Error exceptions thrown
    try {
      
      // Build the query object
      Xapian::QueryParser lQueryParser;
      lQueryParser.set_database (iDatabase);

      /**
       * As explained in http://www.xapian.org/docs/queryparser.html,
       * Xapian::Query::OP_NEAR may be better than Xapian::Query::OP_PHRASE,
       * but only available from version 1.0.13 of Xapian.
       */
      // lQueryParser.set_default_op (Xapian::Query::OP_ADJ);
      lQueryParser.set_default_op (Xapian::Query::OP_PHRASE);

      // DEBUG
      /*
        OPENTREP_LOG_DEBUG ("Query parser `" << lQueryParser.get_description()
        << "'");
      */

      // DEBUG
      OPENTREP_LOG_DEBUG ("        --------");
        
      // Start an enquire session
      Xapian::Enquire enquire (iDatabase);

      /**
       * The Xapian::QueryParser::parse_query() method aggregates all
       * the words with operators inbetween them (here, the "PHRASE"
       * operator).  With the above example ('sna francicso'), it
       * yields "sna PHRASE 2 francicso".
       */
      const Xapian::Query& lXapianQuery =
        lQueryParser.parse_query (iQueryString,
                                  Xapian::QueryParser::FLAG_BOOLEAN
                                  | Xapian::QueryParser::FLAG_PHRASE
                                  | Xapian::QueryParser::FLAG_LOVEHATE);

      // Give the query object to the enquire session
      enquire.set_query (lXapianQuery);

      // Get the top 20 results of the query
      ioMatchingSet = enquire.get_mset (0, 20);

      // Display the results
      int nbMatches = ioMatchingSet.size();

      // DEBUG
      OPENTREP_LOG_DEBUG ("      Query string: `" << iQueryString
                          << "', i.e.: `" << lXapianQuery.get_description()
                          << "' => " << nbMatches << " result(s) found");

      if (nbMatches != 0) {
        // Store the effective (Levenshtein) edit distance/error
        const NbOfErrors_T lEditDistance = 0;
        setEditDistance (lEditDistance);

        // Store the allowable edit distance/error
        setAllowableEditDistance (lEditDistance);

        //
        oMatchedString = iQueryString;

        // Store the fact that there has been a full-text match
        setHasFullTextMatched (true);

        // Store the corrected string (the same as the given string, here,
        // as that latter directly gave full-text matches).
        setCorrectedQueryString (oMatchedString);

        // DEBUG
        OPENTREP_LOG_DEBUG ("        Query string: `" << iQueryString
                            << "' provides " << nbMatches << " exact matches.");

        return oMatchedString;
      }  
      assert (ioMatchingSet.empty() == true);

      /**
       * Since there is no match, we search for a spelling suggestion, if any.
       * With the above example, 'sna francisco' yields the suggestion
       * 'san francisco'.
       */
      const NbOfErrors_T& lAllowableEditDistance =
        calculateEditDistance (iQueryString);
      
      // Let Xapian find a spelling correction (if any)
      const std::string& lCorrectedString =
        iDatabase.get_spelling_suggestion (iQueryString, lAllowableEditDistance);

      // If the correction is no better than the original string, there is
      // no need to go further: there is no match.
      if (lCorrectedString.empty() == true || lCorrectedString == iQueryString) {
        // DEBUG
        OPENTREP_LOG_DEBUG ("        Query string: `"
                            << iQueryString << "' provides no match, "
                            << "and there is no spelling suggestion, "
                            << "even with an edit distance of "
                            << lAllowableEditDistance);

        // Store the fact that there has not been any full-text match
        setHasFullTextMatched (false);

        // Leave the string empty
        return oMatchedString;
      }
      assert (lCorrectedString.empty() == false
              && lCorrectedString != iQueryString);

      // Calculate the effective (Levenshtein) edit distance/error
      const NbOfErrors_T& lEditDistance =
        Levenshtein::getDistance (iQueryString, lCorrectedString);

      /**
       * Since there is no match, we search on the corrected string.       
       *
       * As, with the above example, the full corrected string is
       * 'san francisco', it yields the query "san PHRASE 2 francisco",
       * which should provide matches.
       */
      const Xapian::Query& lCorrectedXapianQuery = 
        lQueryParser.parse_query (lCorrectedString,
                                  Xapian::QueryParser::FLAG_BOOLEAN
                                  | Xapian::QueryParser::FLAG_PHRASE
                                  | Xapian::QueryParser::FLAG_LOVEHATE);

      enquire.set_query (lCorrectedXapianQuery);
      ioMatchingSet = enquire.get_mset (0, 20);

      // Display the results
      nbMatches = ioMatchingSet.size();

      // DEBUG
      OPENTREP_LOG_DEBUG ("      Corrected query string: `" << lCorrectedString
                          << "', i.e.: `"
                          << lCorrectedXapianQuery.get_description()
                          << "' => " << nbMatches << " result(s) found");

      if (nbMatches != 0) {
        // Store the effective (Levenshtein) edit distance/error
        setEditDistance (lEditDistance);

        // Store the allowable edit distance/error
        setAllowableEditDistance (lAllowableEditDistance);

        //
        oMatchedString = lCorrectedString;

        // Store the fact that there has been a full-text match
        setHasFullTextMatched (true);

        // Store the corrected string
        setCorrectedQueryString (oMatchedString);

        // DEBUG
        OPENTREP_LOG_DEBUG ("        Query string: `"
                            << iQueryString << "', spelling suggestion: `"
                            << lCorrectedString
                            << "', with a Levenshtein edit distance of "
                            << lEditDistance
                            << " over an allowable edit distance of "
                            << lAllowableEditDistance << ", provides "
                            << nbMatches << " matches.");

        //
        return oMatchedString;
      }

      // Error
      OPENTREP_LOG_ERROR ("        Query string: `"
                          << iQueryString << "', spelling suggestion: `"
                          << lCorrectedString
                          << "', with a Levenshtein edit distance of "
                          << lEditDistance
                          << " over an allowable edit distance of "
                          << lAllowableEditDistance << ", provides no match, "
                          << "which is not consistent with the existence of "
                          << "the spelling correction.");
      assert (false);
      
    } catch (const Xapian::Error& error) {
      OPENTREP_LOG_ERROR ("Exception: "  << error.get_msg());
      throw XapianException (error.get_msg());
    }

    // Store the fact that there has not been any full-text match
    setHasFullTextMatched (false);

    return oMatchedString;
  }

  // //////////////////////////////////////////////////////////////////////
  std::string Result::fullTextMatch (const Xapian::Database& iDatabase,
                                     const TravelQuery_T& iQueryString) {
    std::string oMatchedString;

    // Catch any Xapian::Error exceptions thrown
    try {

      // Copy the initial query string
      std::string lTrimmedQueryString (iQueryString);

      // Trim the left and right outer words from the query string
      Filter::trim (lTrimmedQueryString);

      // DEBUG
      OPENTREP_LOG_DEBUG ("      ----------------");
      OPENTREP_LOG_DEBUG ("      Current query string: '"<< iQueryString
                          << "' (trimmed: '" << lTrimmedQueryString << "')");

      // Check whether the string should be filtered out
      const bool isToBeAdded = Filter::shouldKeep ("", lTrimmedQueryString);

      Xapian::MSet lMatchingSet;
      if (isToBeAdded == true) {
        oMatchedString = fullTextMatch (iDatabase, lTrimmedQueryString,
                                        lMatchingSet);
      }

      // Create the corresponding documents (from the Xapian MSet object)
      fillResult (lMatchingSet);

      // DEBUG
      OPENTREP_LOG_DEBUG ("      ==> " << toString());
      OPENTREP_LOG_DEBUG ("      ----------------");

    } catch (const Xapian::Error& error) {
      OPENTREP_LOG_ERROR ("Xapian-related error: "  << error.get_msg());
      throw XapianException (error.get_msg());
    }

    return oMatchedString;
  }

  // //////////////////////////////////////////////////////////////////////
  void Result::calculatePageRanks() {
    // Browse the list of Xapian documents
    for (DocumentList_T::iterator itDoc = _documentList.begin();
         itDoc != _documentList.end(); ++itDoc) {
      XapianDocumentPair_T& lDocumentPair = *itDoc;

      // Retrieve the Xapian document
      const Xapian::Document& lXapianDoc = lDocumentPair.first;

      // Extract the PageRank from the document data.      
      const Score_T& lPageRank = getPageRank (lXapianDoc);

      // Retrieve the score board for that Xapian document
      ScoreBoard& lScoreBoard = lDocumentPair.second;

      // Store the PageRank weight
      lScoreBoard.setScore (ScoreType::PAGE_RANK, lPageRank);
    }
  }

  // //////////////////////////////////////////////////////////////////////
  void Result::calculateHeuristicWeights() {
    /**
     * Nothing for now. The heuristic weight, by construction,
     * should come from the caller of the API, not from the document.
     * So, parameters would be needed along the call to that method.
     */
  }

  // //////////////////////////////////////////////////////////////////////
  void Result::calculateCombinedWeights() {
    Percentage_T lMaxPercentage = 0.0;
    std::string lBestDocData; 

    // Browse the list of Xapian documents
    Xapian::docid lBestDocID = 0;
    for (DocumentList_T::iterator itDoc = _documentList.begin();
         itDoc != _documentList.end(); ++itDoc) {
      XapianDocumentPair_T& lDocumentPair = *itDoc;

      // Retrieve the Xapian document ID
      const Xapian::Document& lXapianDoc = lDocumentPair.first;
      const Xapian::docid& lDocID = lXapianDoc.get_docid();
      const std::string& lDocData = lXapianDoc.get_data();

      /**
       * Calculate the combined weight, resulting from all the rules
       * (e.g., full-text matching, PageRank, user input).
       */
      ScoreBoard& lScoreBoard = lDocumentPair.second;
      const Percentage_T& lPercentage = lScoreBoard.calculateCombinedWeight();

      /**
      // DEBUG
      const LocationKey& lLocationKey = getPrimaryKey (lXapianDoc);
      OPENTREP_LOG_DEBUG ("        [pct] '" << describeShortKey()
                          << "', " << lLocationKey << " (doc ID = " << lDocID
                          << ") having the following weight: " << lPercentage
                          << "%; whole score board: " << lScoreBoard);
      */

      // Register the document, if it is the best matching until now
      if (lPercentage > lMaxPercentage) {
        lMaxPercentage = lPercentage;
        lBestDocID = lDocID;
        lBestDocData = lDocData;
      }
    }

    //
    if (hasFullTextMatched() == true) {
      // DEBUG
      const Xapian::Document& lXapianDoc = getDocument (lBestDocID);
      const LocationKey& lLocationKey = getPrimaryKey (lXapianDoc);
      OPENTREP_LOG_DEBUG ("        [pct] '" << describeShortKey()
                          << "' matches at " << lMaxPercentage
                          << "% for " << lLocationKey << " (doc ID = "
                          << lBestDocID << ")");

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
        OPENTREP_LOG_DEBUG("        [pct] '" << describeShortKey()
                           << "' does not match, but it is a non black-listed "
                           << "single-word string; hence, the weight is "
                           << lMaxPercentage << "%");

      } else {
        /**
         * There is no full-text match for that query, which is made either
         * of several words or of a single black-listed word (e.g., 'airport').
         * The corresponding percentage is set to something low (0.1%),
         * in order to significantly decrease the overall matching
         * percentage. The corresponding string set will therefore have
         * almost no chance to being selected/chosen.
         */
        lMaxPercentage = 1e-6;

        // DEBUG
        OPENTREP_LOG_DEBUG("        [pct] '" << describeShortKey()
                           << "' does not match, and is either a multiple-word "
                           << "string or black-listed; hence, the weight is "
                           << lMaxPercentage << "%");
      }
    }

    // Store the doc ID of the best matching document
    setBestDocID (lBestDocID);

    // Store the best weight
    setBestCombinedWeight (lMaxPercentage);

    // Store all the details of the Xapian document
    setBestDocData (lBestDocData);
  }

}
