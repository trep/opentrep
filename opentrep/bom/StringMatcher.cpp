// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// C
#include <cassert>
// STL
#include <istream>
#include <ostream>
#include <sstream>
#include <limits>
#include <string>
#include <list>
#include <map>
// Boost
#include <boost/tokenizer.hpp>
// Xapian
#include <xapian.h>
// OpenTREP
#include <opentrep/bom/WordHolder.hpp>
#include <opentrep/bom/StringMatcher.hpp>
#include <opentrep/bom/Levenshtein.hpp>
#include <opentrep/service/Logger.hpp>

namespace OPENTREP {

  /** Given the size of the phrase, determine the allowed edit distance for
      spelling purpose. For instance, an edit distance of 1 will be allowed
      on a 4-letter word, while an edit distance of 3 will be allowed on an
      11-letter word. */
  // //////////////////////////////////////////////////////////////////////
  static unsigned int calculateEditDistance (const TravelQuery_T& iPhrase) {
    NbOfErrors_T oEditDistance = 2;

    const NbOfErrors_T lQueryStringSize = iPhrase.size();
    
    oEditDistance = lQueryStringSize / 4;
    return oEditDistance;
  }
  
  /** For each of the word in the given list, perform spelling
      corrections. If the word is correctly spelled, it is copied as
      is. Otherwise, a corrected version is stored. */
  // //////////////////////////////////////////////////////////////////////
  static void createCorrectedWordList (const WordList_T& iOriginalWordList,
                                       WordList_T& ioCorrectedWordList,
                                       NbOfErrors_T& ioEditDistance,
                                       NbOfErrors_T& ioAllowableEditDistance,
                                       const Xapian::Database& iDatabase) {
      
    // Empty the target list
    ioCorrectedWordList.clear();

    // Re-set the edit distances/errors
    ioEditDistance = 0;
    ioAllowableEditDistance = 0;

    // Catch any Xapian::Error exceptions thrown
    try {
      
      for (WordList_T::const_iterator itWord = iOriginalWordList.begin();
           itWord != iOriginalWordList.end(); ++itWord) {
        const std::string& lOriginalWord = *itWord;

        // Calculate the distance, depending on the length of the word
        const NbOfErrors_T lCalculatedEditDistance =
          calculateEditDistance (lOriginalWord);

        // The allowable edit distance/error is considered to be the
        // cumulated allowable edit distance/error over all the words
        ioAllowableEditDistance += lCalculatedEditDistance;

        // Get a spelling suggestion for that word
        const std::string& lSuggestedWord =
          iDatabase.get_spelling_suggestion (lOriginalWord,
                                             lCalculatedEditDistance);

        if (lSuggestedWord.empty() == true) {
          ioCorrectedWordList.push_back (lOriginalWord);
          // The edit distance is not modified (as the spelling was correct)
          
        } else {
          ioCorrectedWordList.push_back (lSuggestedWord);

          // The edit distance/error increases from the Levenshtein
          // edit distance/error
          const NbOfErrors_T& lLevenshteinDistance =
            Levenshtein::getDistance (lOriginalWord, lSuggestedWord);
          ioAllowableEditDistance += lLevenshteinDistance;
        }

        // DEBUG
        /*
        OPENTREP_LOG_DEBUG ("Original word: `" << lOriginalWord
                            << "' ==> corrected word: `"
                            << lSuggestedWord << "'");
        */
      }

    } catch (const Xapian::Error& error) {
      OPENTREP_LOG_ERROR ("Exception: "  << error.get_msg());
    }
  }

  /**
     That algorithm checks whether the first (resp. the last) word of
     the original string "belongs" to the spell-corrected suggestion
     (i.e., "recognised" by the Xapian database/index).
     
     <br>For instance, let assume that the original string is 'nce
     petropavlosk' and that the spell-corrected suggestion (for the
     full original string) is 'petropavlosk', meaning that an edit
     distance/error of 3 letters ('nce') has been allowed.

     <br>We check whether the left-reduced original string, namely
     'petropavlosk', returns the same suggestion as the non-reduced
     original string, namely 'nce petropavlosk'. In that case, both
     the reduced and non-reduced original strings yield the same
     spell-corrected suggestions, namely 'petropavlosk'. Hence, the
     spell-corrected suggestion (namely 'petropavlosk') should not be
     taken into account, i.e., the spell-corrected suggestion should
     be empty.

     <br>In other words, 'nce' does not belong to the spell-corrected
     suggestion 'petropavlosk'. So, 'petropavlosk' is not a good
     suggestion at that stage (because if accepted, it would enduce
     the removal of 'nce' from the remaining string).
   */
  // ///////////////////////////////////////////////////////////////////
  void checkAndAlterIfNeeded (TravelQuery_T& ioSuggestedString,
                              const TravelQuery_T& iOriginalString,
                              const NbOfErrors_T& iMaxEditDistance,
                              const Xapian::Database& iDatabase) {

    /**
       Store a copy of the suggested string, as it will be altered by
       the below method, i.e., removeFurthestLeftWord().
    */
    TravelQuery_T lOriginalStringCopy (iOriginalString);
    StringMatcher::removeFurthestLeftWord (lOriginalStringCopy);

    /**
       Get a spell-corrected suggestion for the reduced original string.
       <br>Limit the edit distance to the given maximal one.
    */
    NbOfErrors_T lEditDistance = calculateEditDistance (lOriginalStringCopy);
    
    lEditDistance = std::min (lEditDistance, iMaxEditDistance);
      
    std::string lSuggestionForReducedOriginalString =
      iDatabase.get_spelling_suggestion (lOriginalStringCopy, lEditDistance);

    /**
       Note that if the suggestion on the reduced-original string is
       empty, it normally means that the reduced-original string is
       correct (in spelling terms), as the non-reduced original string
       yielded a suggestion.
     */
    if (lSuggestionForReducedOriginalString.empty() == true) {
      lSuggestionForReducedOriginalString = lOriginalStringCopy;
    }
    
    if (lSuggestionForReducedOriginalString == ioSuggestedString) {

      // DEBUG
      OPENTREP_LOG_DEBUG ("The suggestion (`" << ioSuggestedString
                          << "') for `" << iOriginalString
                          << "', with an allowable edit distance/error of "
                          << lEditDistance
                          << " over a maximum of " << iMaxEditDistance
                          << ", is the same as the suggestion for the reduced "
                          << "original string (`" << lOriginalStringCopy
                          << "') -> discarded.");
      
      /**
         The suggestion on the reduced-original string is either empty
         or the same as the suggestion on the original string. Either
         way, the suggestion is not valid. It must be discarded
         (emptied).
         <br>There may be side effects with the edit distance/error,
         though, but it has been unobserved, as of now.
      */
      ioSuggestedString = "";

      return;
    }
    
    /**
       Store a copy of the suggested string, as it will be altered by
       the below method.
    */
    lOriginalStringCopy = iOriginalString;
    StringMatcher::removeFurthestRightWord (lOriginalStringCopy);

    /**
       Get a spell-corrected suggestion for the reduced original string.
       <br>Limit the edit distance to the given maximal one.
    */
    lEditDistance = calculateEditDistance (lOriginalStringCopy);
    
    lEditDistance = std::min (lEditDistance, iMaxEditDistance);
      
    lSuggestionForReducedOriginalString =
      iDatabase.get_spelling_suggestion (lOriginalStringCopy,
                                         lEditDistance);
    
    /**
       Note that if the suggestion on the reduced-original string is
       empty, it normally means that the reduced-original string is
       correct (in spelling terms), as the non-reduced original string
       yielded a suggestion.
     */
    if (lSuggestionForReducedOriginalString.empty() == true) {
      lSuggestionForReducedOriginalString = lOriginalStringCopy;
    }
    
    if (lSuggestionForReducedOriginalString == ioSuggestedString) {

      // DEBUG
      OPENTREP_LOG_DEBUG ("The suggestion (`" << ioSuggestedString
                          << "') for `" << iOriginalString
                          << "', with an allowable edit distance/error of "
                          << lEditDistance
                          << " over a maximum of " << iMaxEditDistance
                          << ", is the same as the suggestion for the reduced "
                          << "original string (`" << lOriginalStringCopy
                          << "') -> discarded.");
      
      /**
         See the remark above.
      */
      ioSuggestedString = "";

      return;
    }
  }

  // ///////////////////////////////////////////////////////////////////
  std::string searchStringIter (Xapian::MSet& ioMatchingSet,
                                const TravelQuery_T& iSearchString,
                                NbOfErrors_T& ioEditDistance,
                                NbOfErrors_T& ioAllowableEditDistance,
                                const NbOfErrors_T& iMaxEditDistance,
                                bool& ioHasReachedMaximalAllowableEditDistance,
                                const Xapian::Database& iDatabase) {
    NbOfErrors_T lMaxEditDistance = std::numeric_limits<NbOfErrors_T>::min();

    // Initialisation
    std::string oMatchedString;

    // Catch any Xapian::Error exceptions thrown
    try {
      
      /**
         Build another string, in addition to the original one. Overall,
         there are thus two strings:
         <br><ul>
         <li>One with the original words given by the user</li>
         <li>One with the orthographic-corrected words, wherever
         relevant (otherwise, the original word is taken)</li>
         </ul>
         <br>For instance, 'sna francisco' would give the following
         two strings:
         <br><ul>
         <li>'sna francicso' (original)</li>
         <li>'sna francisco' (corrected, where relevant, word by word)</li>
         </ul>
         <br>Note that, as 'sna' exists in the dictionary (Santa Ana, CA, USA),
         it is not replaced. We shall take care of the whole string in a
         further step below.
      */
      WordList_T lOriginalWordList;
      WordHolder::tokeniseStringIntoWordList (iSearchString, lOriginalWordList);

      /**
         A clean query string is rebuilt from the word list. Indeed,
         the original string may have contained a few separators
         (e.g., '/', ';', etc.), which have been removed by the
         tokeniseStringIntoWordList() method. All those separators are
         thus replaced by spaces.
         <br>For instance, the 'san francisco, ca, us' initial string
         would be replaced by 'san francisco ca us'.
      */
      const std::string lOriginalQueryString =
        WordHolder::createStringFromWordList (lOriginalWordList);

      // Build the query object
      Xapian::QueryParser lQueryParser;
      lQueryParser.set_database (iDatabase);
      /**
         As explained in http://www.xapian.org/docs/queryparser.html,
         Xapian::Query::OP_ADJ is better than Xapian::Query::OP_PHRASE,
         but only available from version 1.0.13 of Xapian.
      */
      // lQueryParser.set_default_op (Xapian::Query::OP_ADJ);
      lQueryParser.set_default_op (Xapian::Query::OP_PHRASE);

      // DEBUG
      /*
        OPENTREP_LOG_DEBUG ("Query parser `" << lQueryParser.get_description()
        << "'");
      */

      // By default, as there can be a match without changing
      // anything, the edit distance is null, and the allowable edit
      // distance could be anything. It makes sense, though, to set it
      // at the maximum.
      ioEditDistance = 0;
      ioAllowableEditDistance = iMaxEditDistance;
        
      // Start an enquire session
      Xapian::Enquire enquire (iDatabase);

      /**
         The Xapian::QueryParser::parse_query() method aggregates all
         the words with operators inbetween them (here, the "PHRASE"
         operator).  With the above example ('sna francicso'), it
         yields "sna PHRASE 2 francicso".
      */
      Xapian::Query lOriginalQuery =
        lQueryParser.parse_query (lOriginalQueryString,
                                  Xapian::QueryParser::FLAG_BOOLEAN
                                  | Xapian::QueryParser::FLAG_PHRASE
                                  | Xapian::QueryParser::FLAG_LOVEHATE);

      // Give the query object to the enquire session
      enquire.set_query (lOriginalQuery);

      // Get the top 10 results of the query
      ioMatchingSet = enquire.get_mset (0, 10);

      // Display the results
      int nbMatches = ioMatchingSet.size();

      // DEBUG
      /*
      OPENTREP_LOG_DEBUG ("Original query `" << lOriginalQueryString
                          << "', i.e., `" << lOriginalQuery.get_description()
                          << "' => " << nbMatches << " results found");
      */

      /**
         When no match is found, we search on the corrected phrase/string
         (where the words have been corrected one by one).
      */
      if (nbMatches != 0) {
        /**
           When the maximal allowable edit distance reaches the maximum
           of the calculated edit distance, it becomes useless to go on
           increasing the maximal allowable edit distance.
        */
        if (lMaxEditDistance <= iMaxEditDistance) {
          ioHasReachedMaximalAllowableEditDistance = true;
        }

        oMatchedString = lOriginalQueryString;
        return oMatchedString;
      }
      assert (nbMatches == 0 && oMatchedString.empty() == true);

      /**
         From the clean list of original words, another list of
         spell-corrected words is built, where every word is
         spell-corrected one by one, according to its own length.
         <br>For instance, "sna" can not be corrected into "san", as
         "sna" has a length of 3 letters only, but "francicso" will be
         corrected into "francisco". So, "sna francicso" will be
         corrected into "sna francisco".
      */
      WordList_T lCorrectedWordList;
      createCorrectedWordList (lOriginalWordList, lCorrectedWordList,
                               ioEditDistance, ioAllowableEditDistance,
                               iDatabase);
        
      const std::string lCorrectedQueryString =
        WordHolder::createStringFromWordList (lCorrectedWordList);
        
      /**
         Strangely enough (is it?), the corrected query given by the Xapian
         QueryParser corresponds to the full original string, where words
         have been corrected one by one, but considered as a single block.
         <br>With the above example, 'sna francicso' yields (wrongly)
         'sna francisco', instead of "sna PHRASE 2 francisco", as generated
         by the following (uncommented) code.
         <br>Just to be clear, the two lines of commented code below
         are wrong. Those two commented lines are kept, just to be
         sure that nobody gets the idea to use them.
      */
      // Xapian::Query lCorrectedQuery =
      //         lQueryParser.get_corrected_query_string();
      Xapian::Query lCorrectedQuery = 
        lQueryParser.parse_query (lCorrectedQueryString,
                                  Xapian::QueryParser::FLAG_BOOLEAN
                                  | Xapian::QueryParser::FLAG_PHRASE
                                  | Xapian::QueryParser::FLAG_LOVEHATE);
      
      enquire.set_query (lCorrectedQuery);
      ioMatchingSet = enquire.get_mset (0, 10);

      // Display the results
      nbMatches = ioMatchingSet.size();

      // DEBUG
      /*
      OPENTREP_LOG_DEBUG ("Corrected query `" << lCorrectedQueryString
                          << "', i.e., `" << lCorrectedQuery.get_description()
                          << "' => " << nbMatches
                          << " results found on corrected string");
      */
      
      if (nbMatches != 0) {
        /**
           When the maximal allowable edit distance reaches the maximum
           of the calculated edit distance, it becomes useless to go on
           increasing the maximal allowable edit distance.
        */
        if (lMaxEditDistance <= iMaxEditDistance) {
          ioHasReachedMaximalAllowableEditDistance = true;
        }
        
        // Store the effective (Levenshtein) edit distance/error
        ioEditDistance = Levenshtein::getDistance (lOriginalQueryString,
                                                   lCorrectedQueryString);
        
        oMatchedString = lCorrectedQueryString;
        return oMatchedString;
      }
      assert (nbMatches == 0 && oMatchedString.empty() == true);
      
      /**
         Try to find, if relevant, an orthographic suggestion for the whole
         phrase/string. With the above example, 'sna francisco' yields the
         suggestion 'san francisco'.
      */
      ioEditDistance = calculateEditDistance (lOriginalQueryString);
      
      // Store the greatest edit distance/error
      lMaxEditDistance = std::max (lMaxEditDistance, ioEditDistance);

      // Limit the edit distance to the given maximal one
      ioEditDistance = std::min (ioEditDistance, iMaxEditDistance);

      // Store the allowable edit distance/error
      ioAllowableEditDistance = ioEditDistance;
      
      // Let Xapian find a spelling correction (if any)
      std::string lFullWordCorrectedString =
        iDatabase.get_spelling_suggestion (lOriginalQueryString,
                                           ioEditDistance);

      /**
         Check that the suggestion does not encompass extra words, which
         will be otherwise recognised in another step.
         <br>See the comment of the checkAndAlterIfNeeded() function
         for more details.
      */
      checkAndAlterIfNeeded (lFullWordCorrectedString, lOriginalQueryString,
                             iMaxEditDistance, iDatabase);

      if (lFullWordCorrectedString.empty() == true) {
        ioEditDistance = 0;
        
      } else {
        // Store the effective (Levenshtein) edit distance/error
        ioEditDistance = Levenshtein::getDistance (lOriginalQueryString,
                                                   lFullWordCorrectedString);
      }

      /**
         Since there is still no match, we search on the string
         corrected as a whole.       
         <br>As, with the above example, the full corrected string is
         'san francisco', it yields the query "san PHRASE 2
         francisco", which is eventually right.
      */
      Xapian::Query lFullQueryCorrected = 
        lQueryParser.parse_query (lFullWordCorrectedString,
                                  Xapian::QueryParser::FLAG_BOOLEAN
                                  | Xapian::QueryParser::FLAG_PHRASE
                                  | Xapian::QueryParser::FLAG_LOVEHATE);
      
      enquire.set_query (lFullQueryCorrected);
      ioMatchingSet = enquire.get_mset (0, 10);
      
      // Display the results
      nbMatches = ioMatchingSet.size();
      
      // DEBUG
      /*
      OPENTREP_LOG_DEBUG ("Query corrected as a full sentence `"
                          << lFullWordCorrectedString
                          << "' with a Levenshtein edit distance of "
                          << ioEditDistance
                          << " over an allowable edit distance of "
                          << ioAllowableEditDistance
                          << " over a maximum of "
                          << iMaxEditDistance
                          << " on a potential of " << lMaxEditDistance
                          << ", i.e., `"<< lFullQueryCorrected.get_description()
                          << "' => " << nbMatches
                          << " results found on corrected full string");
      */
      
      if (nbMatches != 0) {
        oMatchedString = lFullWordCorrectedString;
      }
      
      // DEBUG
      /*
        const Xapian::Query& lActualQuery = enquire.get_query();
        OPENTREP_LOG_DEBUG ("Actual query `" << lActualQuery.get_description()
        << "'");
      */
      
    } catch (const Xapian::Error& error) {
      OPENTREP_LOG_ERROR ("Exception: "  << error.get_msg());
    }

    /**
       When the maximal allowable edit distance reaches the maximum
       of the calculated edit distance, it becomes useless to go on
       increasing the maximal allowable edit distance.
    */
    if (iMaxEditDistance >= lMaxEditDistance) {
      ioHasReachedMaximalAllowableEditDistance = true;
    }
        
    return oMatchedString;
  }

  // //////////////////////////////////////////////////////////////////////
  std::string StringMatcher::
  searchString (Xapian::MSet& ioMatchingSet,
                const TravelQuery_T& iPartialQueryString,
                Document& ioMatchingDocument,
                NbOfErrors_T& ioEditDistance,
                NbOfErrors_T& ioAllowableEditDistance,
                const Xapian::Database& iDatabase) {
    std::string oMatchedString;

    // Catch any Xapian::Error exceptions thrown
    try {
      
      /**
         The query string must first be checked, without allowing any
         spelling errors, but by removing the furthest right word at
         every step.
         <br>If no match is found, the maximal allowable edit
         distance/error becomes 1, and the process (trying to match
         the whole sentence, then by removing the furthest right word,
         etc.) is re-performed.
         <br>If no match is found, the maximal allowable edit
         distance/error becomes 2.
         <br>And so on until the maximum of the edit distance/error
         becomes greater than the maximal allowable distance/error.
         reached.
         
         <br>NOTE: that feature is de-activated, as it seems it does
         not bring any added value. To re-activate it, just initialise
         the lMaxEditDistance to 0, instead of to the positive infinite.
      */
      // NbOfErrors_T lMaxEditDistance = 0;
      NbOfErrors_T lMaxEditDistance = std::numeric_limits<NbOfErrors_T>::max();
      
      bool hasReachedMaximalAllowableEditDistance = false;
      bool shouldStop = false;
      while (shouldStop == false) {

        // DEBUG
        OPENTREP_LOG_DEBUG ("        --------");
        OPENTREP_LOG_DEBUG ("        Current query string: `"
                            << iPartialQueryString
                            << "', with a maximal edit distance of "
                            << lMaxEditDistance << ".");
        
        // Retrieve the list of Xapian documents matching the query string
        NbOfErrors_T lEditDistance;
        NbOfErrors_T lAllowableEditDistance;
        oMatchedString =
          searchStringIter (ioMatchingSet, iPartialQueryString, lEditDistance,
                            lAllowableEditDistance, lMaxEditDistance,
                            hasReachedMaximalAllowableEditDistance, iDatabase);
        
        // DEBUG
        OPENTREP_LOG_DEBUG ("        ---- Current query string: `"
                            << iPartialQueryString << "' --- Kept query: `"
                            << oMatchedString
                            << "', with a Levenshtein edit distance of "
                            << lEditDistance
                            << " over an allowable edit distance of "
                            << lAllowableEditDistance << " (over a maximum of "
                            << lMaxEditDistance << "), for "
                            << ioMatchingSet.size() << " matches.");
      
        if (ioMatchingSet.empty() == false) {
          // Store the effective (Levenshtein) edit distance/error
          ioMatchingDocument.setEditDistance (lEditDistance);

          // Store the allowable edit distance/error
          ioMatchingDocument.setAllowableEditDistance (lAllowableEditDistance);
          
          // Since a result has been found, the search can be stopped
          // for that part of the query.
          shouldStop = true;
          break;
        }
      
        // Allow for one more spelling error
        ++lMaxEditDistance;

        /**
           Stop when it is no longer necessary to increase the maximal
           allowable edit distance, as it is already greater than the
           maximum of the calculated edit distance.
        */
        if (hasReachedMaximalAllowableEditDistance == true) {
          shouldStop = true;
        }
      }

    } catch (const Xapian::Error& error) {
      OPENTREP_LOG_ERROR ("Exception: "  << error.get_msg());
      throw XapianException (error.get_msg());
    }

    return oMatchedString;
  }

  // //////////////////////////////////////////////////////////////////////
  void StringMatcher::
  extractBestMatchingDocumentFromMSet (const Xapian::MSet& iMatchingSet,
                                       Document& ioMatchingDocument) {
    assert (iMatchingSet.empty() == false);

    /**
       Retrieve the best matching document. If there are several such
       best matching documents (for instance, several at, say, 100%),
       one is taken randomly (well, we take the first one of the STL
       multimap, so it is not exactly randomly, but the result is the
       same: it appears random).
     */
    Xapian::MSetIterator itDoc = iMatchingSet.begin();
    assert (itDoc != iMatchingSet.end());

    // Store the percentage
    const Xapian::percent& lBestPercentage = itDoc.get_percent();
    ioMatchingDocument.setXapianPercentage (lBestPercentage);

    // Store the (Xapian) document itself
    const Xapian::Document& lBestDocument = itDoc.get_document();
    ioMatchingDocument.setXapianDocument (lBestDocument);

    // Go on in the list of matches, if any
    ++itDoc;

    /*
    if (itDoc != iMatchingSet.end()) {
      // DEBUG
      OPENTREP_LOG_DEBUG ("There are extra matches for Document ID "
                          << lBestDocument.get_docid()
                          << ", itself matching at " << lBestPercentage
                          << "%.");
    }
    */

    /** Add all the Xapian documents having reached the same matching
        percentage. */
    NbOfMatches_T idx = 1;
    for ( ; itDoc != iMatchingSet.end(); ++itDoc, ++idx) {
      const Xapian::percent& lPercentage = itDoc.get_percent();
      const Xapian::Document& lDocument = itDoc.get_document();

      // DEBUG
      /*
      OPENTREP_LOG_DEBUG ("Extra #" << idx << ": Doc ID "
                          << lDocument.get_docid() << " matching at "
                          << lPercentage << "%.");
      */

      /** If the matching percentage is the same as for the main
          (chosen) Xapian document, then add it to the dedicated
          list. Otherwise, add it to the alternative choices. */
      if (lPercentage == lBestPercentage) {
        ioMatchingDocument.addExtraDocument (lDocument);
        
      } else {
        ioMatchingDocument.addAlternateDocument (lPercentage, lDocument);
      }
    }
  }
  
  // //////////////////////////////////////////////////////////////////////
  void StringMatcher::removeFurthestRightWord (std::string& ioQueryString) {
    assert (ioQueryString.empty() == false);
  
    WordList_T lWordList;
    WordHolder::tokeniseStringIntoWordList (ioQueryString, lWordList);
    assert (lWordList.empty() == false);

    // Remove the furthest right word
    lWordList.pop_back();
  
    const std::string& lReducedString =
      WordHolder::createStringFromWordList (lWordList);
    ioQueryString = lReducedString;
  }

  // //////////////////////////////////////////////////////////////////////
  Word_T StringMatcher::removeFurthestLeftWord (std::string& ioQueryString) {
    assert (ioQueryString.empty() == false);
  
    WordList_T lWordList;
    WordHolder::tokeniseStringIntoWordList (ioQueryString, lWordList);
    assert (lWordList.empty() == false);

    // Store (a copy of) the furthest left word
    const Word_T lFurthestLeftWord = lWordList.front();
    
    // Remove the furthest left word
    lWordList.pop_front();
  
    const std::string& lReducedString =
      WordHolder::createStringFromWordList (lWordList);
    ioQueryString = lReducedString;

    return lFurthestLeftWord;
  }

  // //////////////////////////////////////////////////////////////////////
  void StringMatcher::
  subtractParsedToRemaining (const std::string& iAlreadyParsedQueryString,
                             std::string& ioRemainingQueryString) {
    /**
       Remove, from the lRemainingQueryString string, the part which
       has been already successfully parsed.
       <br>For instance, when 'sna francisco rio de janeiro' is the
       initial full clean query string, the searchString() method
       first reduce the query string to 'sna francisco', which
       successfully matches against SFO (San Francisco airport).
       <br>Then, the remaining part of the query string to be parsed is
       'rio de janeiro'. So, the already parsed part must be subtracted
       from the initial query string.
    */
    WordList_T lRemainingWordList;
    WordHolder::tokeniseStringIntoWordList (ioRemainingQueryString,
                                            lRemainingWordList);

    WordList_T lParsedWordList;
    WordHolder::tokeniseStringIntoWordList (iAlreadyParsedQueryString,
                                            lParsedWordList);
  
    unsigned int idx = lParsedWordList.size();
    for ( ; idx != 0 && lRemainingWordList.empty() == false; --idx) {
      lRemainingWordList.pop_front();
    }
  
    // Build the remaining part of the string still to be parsed.
    // <br>Note that that part may be empty.
    ioRemainingQueryString =
      WordHolder::createStringFromWordList (lRemainingWordList);
  }

  // //////////////////////////////////////////////////////////////////////
  std::string StringMatcher::getPlaceCode (const Xapian::Document& iDocument) {
    // Retrieve the Xapian document data
    const std::string& lDocumentData = iDocument.get_data();

    // Tokenise the string into words
    WordList_T lWordList;
    WordHolder::tokeniseStringIntoWordList (lDocumentData, lWordList);
    assert (lWordList.empty() == false);

    // By convention (within OpenTrep), the first word of the Xapian
    // document data string is the place code
    const std::string& lPlaceCode = lWordList.front();

    return lPlaceCode;
  }
  
}
