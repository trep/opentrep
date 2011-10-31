// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// C
#include <cassert>
#include <sstream>
#include <limits>
// Xapian
#include <xapian.h>
// OpenTREP
#include <opentrep/bom/StringMatcher.hpp>
#include <opentrep/bom/Result.hpp>
#include <opentrep/bom/ResultHolder.hpp>
#include <opentrep/service/Logger.hpp>

namespace OPENTREP {

  // //////////////////////////////////////////////////////////////////////
  ResultHolder::ResultHolder (const TravelQuery_T& iQueryString,
                              const Xapian::Database& iDatabase)
    : _queryString (iQueryString), _database (iDatabase) {
    init();
  }
  
  // //////////////////////////////////////////////////////////////////////
  ResultHolder::~ResultHolder () {
  }

  // //////////////////////////////////////////////////////////////////////
  void ResultHolder::init () {
    _resultList.clear();
  }

  // //////////////////////////////////////////////////////////////////////
  std::string ResultHolder::describeShortKey() const {
    std::ostringstream oStr;
    oStr << _queryString;
    return oStr.str();
  }
  
  // //////////////////////////////////////////////////////////////////////
  std::string ResultHolder::describeKey() const {
    return describeShortKey();
  }

  // //////////////////////////////////////////////////////////////////////
  std::string ResultHolder::toString() const {
    std::ostringstream oStr;
    oStr << describeShortKey() << std::endl;
    
    for (ResultList_T::const_iterator itResult = _resultList.begin();
         itResult != _resultList.end(); ++itResult) {
      const Result* lResult_ptr = *itResult;
      assert (lResult_ptr != NULL);
      
      oStr << " ==> " << std::endl << lResult_ptr->toString();
    }
    
    return oStr.str();
  }   

  // //////////////////////////////////////////////////////////////////////
  void ResultHolder::toStream (std::ostream& ioOut) const {
    ioOut << toString();
  }
  
  // //////////////////////////////////////////////////////////////////////
  void ResultHolder::fromStream (std::istream& ioIn) {
  }

  // //////////////////////////////////////////////////////////////////////
  std::string ResultHolder::searchString (TravelQuery_T& ioPartialQueryString,
                                          Document& ioMatchingDocument) {
    std::string oMatchedString;

    // Catch any Xapian::Error exceptions thrown
    try {
      
      /**
         A copy of the query is made, as that copy will be altered by
         the below process, whereas a clean copy needs to be reprocessed
         for each level of maximal edit distance/error.
         <br>However, in case of match, the modifications on the query
         string (lPartialQueryString) must be replicated on the
         original one (ioPartialQueryString).
         <br>And, in case of no match, that query string must be
         emptied, so that the caller knows about that match failure.
      */
      TravelQuery_T lPartialQueryString (ioPartialQueryString);

      bool shouldStop = false;
      while (shouldStop == false) {

        // DEBUG
        OPENTREP_LOG_DEBUG ("----------------");
        OPENTREP_LOG_DEBUG ("Current query string: `"
                            << lPartialQueryString << "'");
        
        Xapian::MSet lMatchingSet;
        NbOfErrors_T lEditDistance;
        NbOfErrors_T lAllowableEditDistance;
        oMatchedString =
          StringMatcher::searchString (lMatchingSet, lPartialQueryString,
                                       ioMatchingDocument, lEditDistance,
                                       lAllowableEditDistance, _database);

        if (oMatchedString.empty() == false) {
          // Create the corresponding document (from the Xapian MSet object)
          StringMatcher::
            extractBestMatchingDocumentFromMSet (lMatchingSet,
                                                 ioMatchingDocument);

          // Note: the allowable edit distance/error, as well as the
          // effective (Levenshtein) edit distance/error, have been
          // set, in the Document object, by the above call to the
          // searchString() method.

          // Since a result has been found, the search can be stopped
          // for that part of the query.
          ioPartialQueryString = lPartialQueryString;
          shouldStop = true;
          break;
        }
      
        // Since the query, as is, yields no match, the furthest right
        // word must be removed from the query string.
        StringMatcher::removeFurthestRightWord (lPartialQueryString);
        
        /**
           Stop when the resulting string gets empty.

           <br>Note that whether maximal allowable edit distance/error
           has been reached is not checked at that stage. That
           algorithm is performed independently for each level of
           maximal allowable edit distance/error. Only the caller
           (below) retriggers this process by changing the level of
           maximal allowable edit distance/error, until that latter be
           reached.
        */
        if (lPartialQueryString.empty() == true) {
          shouldStop = true;

          // DEBUG
          OPENTREP_LOG_DEBUG ("----------------");
          OPENTREP_LOG_DEBUG ("Still no match for current query string: `"
                              << ioPartialQueryString << "'");

          // Empty the query string, so that the caller knows about
          // the match failure
          ioPartialQueryString = "";
        }
      }

    } catch (const Xapian::Error& error) {
      OPENTREP_LOG_ERROR ("Xapian-related error: "  << error.get_msg());
      throw XapianException();
    }

    return oMatchedString;
  }

  // //////////////////////////////////////////////////////////////////////
  void ResultHolder::searchString (DocumentList_T& ioDocumentList,
                                   WordList_T& ioWordList) {

    // Catch any Xapian::Error exceptions thrown
    try {
      
      std::string lRemainingQueryString (_queryString);
      bool shouldStop = false;
      while (shouldStop == false) {
        // DEBUG
        OPENTREP_LOG_DEBUG ("+++++++++++++++++++++")
        OPENTREP_LOG_DEBUG ("Remaining part of the query string: `"
                            << lRemainingQueryString << "'");
        
        /**
           Search with the initial full string, then by removing a word if
           there was no result, then by removing another word if there was
           again no result, until either a result is found or the
           resulting string gets empty.
        */
        std::string lQueryString (lRemainingQueryString);

        /**
           Main algorithm, altering the query string (suppressing the
           furthest right words, so that the remaining left part be matched
           against the Xapian database).
        */
        Document lMatchingDocument;
        const std::string lMatchedString = searchString (lQueryString,
                                                         lMatchingDocument);

        if (lMatchedString.empty() == false) {
          lMatchingDocument.setQueryString (lQueryString);
          lMatchingDocument.setCorrectedQueryString (lMatchedString);
          ioDocumentList.push_back (lMatchingDocument);

          // DEBUG
          const NbOfMatches_T& lNbOfMatches =
            lMatchingDocument.notifyIfExtraMatch();
          const NbOfErrors_T& lEditDistance =
            lMatchingDocument.getEditDistance();
          const NbOfErrors_T& lAllowableEditDistance =
            lMatchingDocument.getAllowableEditDistance();
          OPENTREP_LOG_DEBUG ("==> " << lNbOfMatches
                              << " main matches for the query string: `"
                              << lMatchedString << "' (from `" << lQueryString
                              << "' -> Levenshtein edit distance of "
                              << lEditDistance << " over allowable "
                              << lAllowableEditDistance << ")");
        
          /**
             Remove, from the lRemainingQueryString string, the part
             which has been already successfully parsed.
             <br>For instance, when 'sna francisco rio de janeiro' is
             the initial full clean query string, the searchString()
             method first reduce the query string to 'sna francisco',
             which successfully matches against SFO (San Francisco
             airport).
             <br>Then, the remaining part of the query string to be
             parsed is 'rio de janeiro'. So, the already parsed part,
             namely 'sna francisco', must be subtracted from the
             initial query string.
             <br>To do that, it is not an exact subtraction. Rather,
             the words within the already parsed part ('sna
             francisco') are counted, and the corresponding number of
             words are subtracted from the furthest left hand of the
             remaining part of the string.
          */
          StringMatcher::subtractParsedToRemaining (lQueryString,
                                                    lRemainingQueryString);

        } else {
          // DEBUG
          OPENTREP_LOG_DEBUG ("==> No matching of the query string: `"
                              << lRemainingQueryString
                              << "'. Skip the beginning word.");
          OPENTREP_LOG_DEBUG ("Query string: " << lQueryString);
          assert (lQueryString.empty() == true);

          /**
             We must suppress (at least) the furthest left word, as it
             hinders the remaining of the query string to be
             matched. If that furthest left word is the only word of
             the query string, the remaining query string will
             therefore be empty, and the loop will therefore be exited
             in the next step below.
          */
          // Remove the furthest right word from the query string...
          const Word_T& lNonMatchedWord = 
            StringMatcher::removeFurthestLeftWord (lRemainingQueryString);

          // ... and add it to the list of unmatched words.
          ioWordList.push_back (lNonMatchedWord);
        }
        
        // If there is nothing left to be parsed, we have then finished
        // to parse the initial string.
        if (lRemainingQueryString.empty() == true) {
          shouldStop = true;
          break;
        }
      }
      
    } catch (const Xapian::Error& error) {
      OPENTREP_LOG_ERROR ("Exception: "  << error.get_msg());
      throw XapianException();
    }
  }

}
