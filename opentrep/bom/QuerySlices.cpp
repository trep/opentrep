// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// STL
#include <cassert>
#include <sstream>
#include <set>
// OpenTrep
#include <opentrep/basic/BasConst_General.hpp>
#include <opentrep/basic/Utilities.hpp>
#include <opentrep/basic/OTransliterator.hpp>
#include <opentrep/bom/Levenshtein.hpp>
#include <opentrep/bom/Filter.hpp>
#include <opentrep/bom/QuerySlices.hpp>
#include <opentrep/service/Logger.hpp>

namespace OPENTREP {

  // //////////////////////////////////////////////////////////////////////
  QuerySlices::QuerySlices (const Xapian::Database& iDatabase,
                            const TravelQuery_T& iQueryString,
                            const OTransliterator& iTransliterator)
    : _database (iDatabase), _queryString (iQueryString) {
    init (iTransliterator);
  }

  // //////////////////////////////////////////////////////////////////////
  QuerySlices::~QuerySlices() {
  }

  // //////////////////////////////////////////////////////////////////////
  void QuerySlices::push_back (const StringPartition& iStringPartition) {
    if (iStringPartition.empty() == false) {
      _slices.push_back (iStringPartition);
    }
  }
  
  // //////////////////////////////////////////////////////////////////////
  size_t QuerySlices::size() const {
    return _slices.size();
  }
  
  // //////////////////////////////////////////////////////////////////////
  bool QuerySlices::empty() const {
    return _slices.empty();
  }
  
  // //////////////////////////////////////////////////////////////////////
  void QuerySlices::clear() {
    _slices.clear();
  }

  // //////////////////////////////////////////////////////////////////////
  std::string QuerySlices::describeKey() const {
    std::ostringstream oStr;
    oStr << "";
    return oStr.str();
  }

  // //////////////////////////////////////////////////////////////////////
  std::string QuerySlices::describe() const {
    std::ostringstream oStr;
    oStr << describeKey();

    //
    oStr << "[ ";

    short idx_sublist = 0;
    for (StringPartitionList_T::const_iterator itSlice = _slices.begin();
         itSlice != _slices.end(); ++itSlice, ++idx_sublist) {
      //
      if (idx_sublist != 0) {
        oStr << "; ";
      }
      
      //
      const StringPartition& lStringPartition = *itSlice;

      //
      oStr << idx_sublist << ". " << lStringPartition;
    }

    //
    oStr << " ]";

    return oStr.str();
  }

  // //////////////////////////////////////////////////////////////////////
  void QuerySlices::toStream (std::ostream& ioOut) const {
    ioOut << describe();
  }
  
  // //////////////////////////////////////////////////////////////////////
  void QuerySlices::fromStream (std::istream& ioIn) {
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
  
  /**
   * @brief Helper function to query for a Xapian-based full text match
   */
  // //////////////////////////////////////////////////////////////////////
  bool doesMatch (const Xapian::Database& iDatabase,
                  const std::string& iWord1, const std::string& iWord2) {
    bool oDoesMatch = false;

    //
    std::ostringstream oStr;
    oStr << iWord1 << " " << iWord2;
    const std::string lQueryString (oStr.str());

    // Catch any Xapian::Error exceptions thrown
    Xapian::MSet lMatchingSet;
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
      // OPENTREP_LOG_DEBUG ("        --------");
        
      // Start an enquire session
      Xapian::Enquire enquire (iDatabase);

      /**
       * The Xapian::QueryParser::parse_query() method aggregates all
       * the words with operators inbetween them (here, the "PHRASE"
       * operator).  With the above example ('sna francicso'), it
       * yields "sna PHRASE 2 francicso".
       */
      const Xapian::Query& lXapianQuery =
        lQueryParser.parse_query (lQueryString,
                                  Xapian::QueryParser::FLAG_BOOLEAN
                                  | Xapian::QueryParser::FLAG_PHRASE
                                  | Xapian::QueryParser::FLAG_LOVEHATE);

      // Give the query object to the enquire session
      enquire.set_query (lXapianQuery);

      // Get the top 20 results of the query
      lMatchingSet = enquire.get_mset (0, 20);

      // Display the results
      int nbMatches = lMatchingSet.size();

      // DEBUG
      /*
      OPENTREP_LOG_DEBUG ("      Query string: `" << lQueryString
                          << "', i.e.: `" << lXapianQuery.get_description()
                          << "' => " << nbMatches << " result(s) found");
      */

      if (nbMatches != 0) {
        // There has been a matching
        oDoesMatch = true;

        // DEBUG
        /*
        OPENTREP_LOG_DEBUG ("        Query string: `" << lQueryString
                            << "' provides " << nbMatches << " exact matches.");
        */

        return oDoesMatch;
      }  
      assert (lMatchingSet.empty() == true);

      /**
       * Since there is no match, we search for a spelling suggestion, if any.
       * With the above example, 'sna francisco' yields the suggestion
       * 'san francisco'.
       */
      const NbOfErrors_T& lAllowableEditDistance =
        calculateEditDistance (lQueryString);
      
      // Let Xapian find a spelling correction (if any)
      const std::string& lCorrectedString =
        iDatabase.get_spelling_suggestion (lQueryString, lAllowableEditDistance);

      // If the correction is no better than the original string, there is
      // no need to go further: there is no match.
      if (lCorrectedString.empty() == true || lCorrectedString == lQueryString) {
        // DEBUG
        /*
        OPENTREP_LOG_DEBUG ("        Query string: `"
                            << lQueryString << "' provides no match, "
                            << "and there is no spelling suggestion, "
                            << "even with an edit distance of "
                            << lAllowableEditDistance);
        */

        // No match
        return oDoesMatch;
      }
      assert (lCorrectedString.empty() == false
              && lCorrectedString != lQueryString);

      // Calculate the effective (Levenshtein) edit distance/error
      const NbOfErrors_T& lEditDistance =
        Levenshtein::getDistance (lQueryString, lCorrectedString);

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
      lMatchingSet = enquire.get_mset (0, 20);

      // Display the results
      nbMatches = lMatchingSet.size();

      // DEBUG
      /*
      OPENTREP_LOG_DEBUG ("      Corrected query string: `" << lCorrectedString
                          << "', i.e.: `"
                          << lCorrectedXapianQuery.get_description()
                          << "' => " << nbMatches << " result(s) found");
      */

      if (nbMatches != 0) {
        // DEBUG
        /*
        OPENTREP_LOG_DEBUG ("        Query string: `"
                            << lQueryString << "', spelling suggestion: `"
                            << lCorrectedString
                            << "', with a Levenshtein edit distance of "
                            << lEditDistance
                            << " over an allowable edit distance of "
                            << lAllowableEditDistance << ", provides "
                            << nbMatches << " matches.");
        */

        //
        oDoesMatch = true;
        return oDoesMatch;
      }

      // Error
      OPENTREP_LOG_ERROR ("        Query string: `"
                          << lQueryString << "', spelling suggestion: `"
                          << lCorrectedString
                          << "', with a Levenshtein edit distance of "
                          << lEditDistance
                          << " over an allowable edit distance of "
                          << lAllowableEditDistance << ", provides no match, "
                          << "which is not consistent with the existence of "
                          << "the spelling correction.");
      assert (false);
      
    } catch (const Xapian::Error& error) {
      // Error
      OPENTREP_LOG_ERROR ("Exception: "  << error.get_msg());
      throw XapianException (error.get_msg());
    }

    return oDoesMatch;
  }

  // //////////////////////////////////////////////////////////////////////
  void QuerySlices::init (const OTransliterator& iTransliterator) {
    // 0. Initialisation
    // 0.1. Stripping of the punctuation and quotation characters
    _queryString = iTransliterator.unpunctuate (_queryString);
    _queryString = iTransliterator.unquote (_queryString);

    // 0.2. Initialisation of the tokenizer
    WordList_T lWordList;
    tokeniseStringIntoWordList (_queryString, lWordList);
    const unsigned short nbOfWords = lWordList.size();

    // When the query has a single word, stop here, as there is a single slice
    if (nbOfWords <= 1) {
      _slices.push_back (_queryString);
      return;
    }

    // 0.3. Re-create the initial phrase, without any (potential) seperator
    const std::string lPhrase = createStringFromWordList (lWordList);

    // 1. Browse the words, two by two, and check whether their association
    //    matches with the Xapian index
    WordList_T::const_iterator itWord = lWordList.begin();
    WordList_T::const_iterator itNextWord = lWordList.begin(); ++itNextWord;
    for (unsigned short idx = 1, idx_rel = 1; itNextWord != lWordList.end();
         ++itWord, ++itNextWord, ++idx, ++idx_rel) {
      const std::string& leftWord = *itWord;
      const std::string& rightWord = *itNextWord;

      // Store the left word in the staging string
      if (idx_rel >= 2) {
        _itLeftWords += " ";
      }
      _itLeftWords += leftWord;

      // Check whether the juxtaposition of the two contiguous words matches
      const bool lDoesMatch =
        OPENTREP::doesMatch (_database, leftWord, rightWord);

      if (lDoesMatch == true) {
        // When the two words give a match, do nothing now, as at the next turn,
        // the right word will become the left word and thus be added to the
        // staging string

        // DEBUG
        /*
        OPENTREP_LOG_DEBUG ("[" << idx << "][" << idx_rel
                            << "] Match - staging string: '"
                            << _itLeftWords << "'");
        */

      } else {
        // DEBUG
        /*
        OPENTREP_LOG_DEBUG ("[" << idx << "][" << idx_rel
                            << "] No match - staging string: '"
                            << _itLeftWords << "'");
        */

        // When the two words give no match, add the content of the staging
        // list to the list of slices. Then, empty the staging string.
        _slices.push_back (_itLeftWords);
        _itLeftWords = "";
        idx_rel = 0;
      }
    }
    
    // 2.
    const std::string& leftWord = *itWord;
    if (_itLeftWords.empty() == false) {
      _itLeftWords += " ";
    }
    _itLeftWords += leftWord;
    _slices.push_back (_itLeftWords);

    // DEBUG
    // OPENTREP_LOG_DEBUG ("Last staging string: '" << _itLeftWords << "'");
    // OPENTREP_LOG_DEBUG ("Slices: " << *this);
  }

}
