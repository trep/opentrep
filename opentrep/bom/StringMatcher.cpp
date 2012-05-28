// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// STL
#include <cassert>
#include <sstream>
#include <limits>
// Boost
#include <boost/tokenizer.hpp>
// Xapian
#include <xapian.h>
// OpenTrep
#include <opentrep/basic/float_utils.hpp>
#include <opentrep/bom/Filter.hpp>
#include <opentrep/bom/StringPartition.hpp>
#include <opentrep/bom/WordHolder.hpp>
#include <opentrep/bom/StringMatcher.hpp>
#include <opentrep/bom/Levenshtein.hpp>
#include <opentrep/service/Logger.hpp>

namespace OPENTREP {

  /**
   * Given the size of the phrase, determine the allowed edit distance for
   * spelling purpose. For instance, an edit distance of 1 will be allowed
   * on a 4-letter word, while an edit distance of 3 will be allowed on an
   * 11-letter word.
   */
  // //////////////////////////////////////////////////////////////////////
  static unsigned int calculateEditDistance (const TravelQuery_T& iPhrase) {
    NbOfErrors_T oEditDistance = 2;

    const NbOfErrors_T lQueryStringSize = iPhrase.size();
    
    oEditDistance = lQueryStringSize / 4;
    return oEditDistance;
  }
  
  // //////////////////////////////////////////////////////////////////////
  std::string fullTextMatch (Xapian::MSet& ioMatchingSet,
                             const TravelQuery_T& iQueryString,
                             MatchingDocuments& ioMatchingDocuments,
                             const Xapian::Database& iDatabase) {
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

      // Get the top 10 results of the query
      ioMatchingSet = enquire.get_mset (0, 10);

      // Display the results
      int nbMatches = ioMatchingSet.size();

      // DEBUG
      OPENTREP_LOG_DEBUG ("      Query string: `" << iQueryString
                          << "', i.e.: `" << lXapianQuery.get_description()
                          << "' => " << nbMatches << " result(s) found");

      if (nbMatches != 0) {
        // Store the effective (Levenshtein) edit distance/error
        const NbOfErrors_T lEditDistance = 0;
        ioMatchingDocuments.setEditDistance (lEditDistance);

        // Store the allowable edit distance/error
        ioMatchingDocuments.setAllowableEditDistance (lEditDistance);

        //
        oMatchedString = iQueryString;

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
        iDatabase.get_spelling_suggestion(iQueryString, lAllowableEditDistance);

      // If the correction is no better than the original string, there is
      // no need to go further: there is no match.
      if (lCorrectedString.empty() == true
          || lCorrectedString == iQueryString) {
        // DEBUG
        OPENTREP_LOG_DEBUG ("        Query string: `"
                            << iQueryString << "' provides no match, "
                            << "and there is no spelling suggestion, "
                            << "even with an edit distance of "
                            << lAllowableEditDistance);

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
      ioMatchingSet = enquire.get_mset (0, 10);

      // Display the results
      nbMatches = ioMatchingSet.size();

      // DEBUG
      OPENTREP_LOG_DEBUG ("      Corrected query string: `" << lCorrectedString
                          << "', i.e.: `"
                          << lCorrectedXapianQuery.get_description()
                          << "' => " << nbMatches << " result(s) found");

      if (nbMatches != 0) {
        // Store the effective (Levenshtein) edit distance/error
        ioMatchingDocuments.setEditDistance (lEditDistance);

        // Store the allowable edit distance/error
        ioMatchingDocuments.setAllowableEditDistance (lAllowableEditDistance);

        //
        oMatchedString = lCorrectedString;

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

    return oMatchedString;
  }

  // //////////////////////////////////////////////////////////////////////
  std::string fullTextMatch (const TravelQuery_T& iQueryString,
                             MatchingDocuments& ioMatchingDocuments,
                             const Xapian::Database& iDatabase) {
    std::string oMatchedString;

    // Catch any Xapian::Error exceptions thrown
    try {
      
      // DEBUG
      OPENTREP_LOG_DEBUG ("      ----------------");
      OPENTREP_LOG_DEBUG ("      Current query string: `" << iQueryString
                          << "'");

      // Check whether the string should be filtered out
      const bool isToBeAdded = Filter::shouldKeep ("", iQueryString);

      Xapian::MSet lMatchingSet;
      if (isToBeAdded == true) {
        oMatchedString = fullTextMatch (lMatchingSet, iQueryString,
                                        ioMatchingDocuments, iDatabase);
      }

      if (oMatchedString.empty() == false) {
        // Create the corresponding document (from the Xapian MSet object)
        StringMatcher::
          extractBestMatchingDocumentFromMSet (lMatchingSet,
                                               ioMatchingDocuments);

        // Note: the allowable edit distance/error, as well as the
        // effective (Levenshtein) edit distance/error, have been
        // set, in the Document object, by the above call to the
        // fullTextMatch() method.

        // DEBUG
        OPENTREP_LOG_DEBUG ("      Match for query string: `" << iQueryString
                            << "': `" << oMatchedString << "'");

      } else {

        // DEBUG
        OPENTREP_LOG_DEBUG ("      No match for query string: `"
                            << iQueryString << "'");
      }

      // DEBUG
      OPENTREP_LOG_DEBUG ("      ----------------");

    } catch (const Xapian::Error& error) {
      OPENTREP_LOG_ERROR ("Xapian-related error: "  << error.get_msg());
      throw XapianException (error.get_msg());
    }

    return oMatchedString;
  }

  // //////////////////////////////////////////////////////////////////////
  Percentage_T fullTextMatch (DocumentList_T& ioDocumentList,
                              const StringSet& iStringSet,
                              const Xapian::Database& iDatabase) {
    Percentage_T oTotalMatchingPercentage = 100.0;
    Percentage_T lPercentage = 0.0;

    // Catch any thrown Xapian::Error exceptions
    try {
      
      // Browse through all the word combinations of the partition
      for (StringSet::StringSet_T::const_iterator itString =
             iStringSet._set.begin();
           itString != iStringSet._set.end(); ++itString) {
        //
        const std::string lQueryString (*itString);

        // DEBUG
        OPENTREP_LOG_DEBUG ("    Query string: " << lQueryString);

        //
        MatchingDocuments lMatchingDocuments;
        const std::string& lMatchedString = fullTextMatch (lQueryString,
                                                           lMatchingDocuments,
                                                           iDatabase);

        if (lMatchedString.empty() == false) {
          //
          lMatchingDocuments.setQueryString (lQueryString);
          lMatchingDocuments.setCorrectedQueryString (lMatchedString);

          // Retrieve the matching percentage for the corresponding string only
          const Xapian::percent& lXapianPercentage =
            lMatchingDocuments.getXapianPercentage();
          lPercentage = static_cast<Percentage_T> (lXapianPercentage);

          // Trick to decrease the overall percentage of word combinations,
          // when compared to the whole string. For instance, {"san francisco"}
          // will have a percentage of 99.999, compared to {"san", "francisco"}
          // which will have a percentage of 99.998.
          const FloatingPoint<Percentage_T> lComparablePct (lPercentage);
          const FloatingPoint<Percentage_T> lFullMatchingPct (100.0);
          if (lComparablePct.AlmostEquals (lFullMatchingPct) == true) {
            lPercentage = 99.999;
          }

          // "Add" the contribution to the total
          oTotalMatchingPercentage *= lPercentage / 100.0;

          // Store the Document into the output list
          ioDocumentList.push_back (lMatchingDocuments);

          // DEBUG
          const NbOfMatches_T& lNbOfMatches =
            lMatchingDocuments.notifyIfExtraMatch();
          const NbOfErrors_T& lEditDistance =
            lMatchingDocuments.getEditDistance();
          const NbOfErrors_T& lAllowableEditDistance =
            lMatchingDocuments.getAllowableEditDistance();
          OPENTREP_LOG_DEBUG ("      ==> " << lNbOfMatches
                              << " main matches for the query string: `"
                              << lMatchedString << "' (from `" << lQueryString
                              << "' -> Levenshtein edit distance of "
                              << lEditDistance << " over allowable "
                              << lAllowableEditDistance << ")");
          OPENTREP_LOG_DEBUG ("      matching at " << lPercentage
                              << "%, giving a cumulative of "
                              << oTotalMatchingPercentage << "%");

        } else {
          // "Add" the contribution to the total
          lPercentage = 0.05;
          oTotalMatchingPercentage *= lPercentage;

          // DEBUG
          OPENTREP_LOG_DEBUG ("      ==> No match, i.e., " << lPercentage
                              << "%, giving a cumulative of "
                              << oTotalMatchingPercentage << "%");

          // As there may be unmatched terms in the query string,
          // those will turn down the matching percentage. At the end,
          // the string set with the greatest percentage will be selected.
        }
      }

    } catch (const Xapian::Error& error) {
      OPENTREP_LOG_ERROR ("Exception: "  << error.get_msg());
      throw XapianException (error.get_msg());
    }

    //
    return oTotalMatchingPercentage;
  }

  // //////////////////////////////////////////////////////////////////////
  void StringMatcher::searchString (const TravelQuery_T& iQueryString,
                                    DocumentList_T& ioDocumentList,
                                    WordList_T& ioWordList,
                                    const Xapian::Database& iDatabase) {

    // Catch any thrown Xapian::Error exceptions
    try {
      
      StringPartition lStringPartition (iQueryString);

      // DEBUG
      OPENTREP_LOG_DEBUG ("+++++++++++++++++++++");
      OPENTREP_LOG_DEBUG ("Query string: `" << iQueryString << "'");
      OPENTREP_LOG_DEBUG ("Partitions: " << lStringPartition);

      // Calculate the matching percentage of all the partitions
      double lMaxMatchingPercentage = 0.0;
      for (StringPartition::StringPartition_T::const_iterator itSet =
             lStringPartition._partition.begin();
           itSet != lStringPartition._partition.end(); ++itSet) {
        const StringSet& lStringSet = *itSet;

        // DEBUG
        OPENTREP_LOG_DEBUG ("  ==========");
        OPENTREP_LOG_DEBUG ("  String set: " << lStringSet);

        // Calculate the matching sets for the string set
        DocumentList_T lDocumentList;
        const double lMatchingPercentage =
          OPENTREP::fullTextMatch (lDocumentList, lStringSet, iDatabase);

        // Keep track of the maximum percentage, if needed
        if (lMatchingPercentage > lMaxMatchingPercentage) {
          lMaxMatchingPercentage = lMatchingPercentage;
          ioDocumentList = lDocumentList;
        }
      }

    } catch (const Xapian::Error& error) {
      OPENTREP_LOG_ERROR ("Exception: "  << error.get_msg());
      throw XapianException (error.get_msg());
    }
  }

  // //////////////////////////////////////////////////////////////////////
  void StringMatcher::
  extractBestMatchingDocumentFromMSet (const Xapian::MSet& iMatchingSet,
                                       MatchingDocuments& ioMatchingDocuments) {
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
    ioMatchingDocuments.setXapianPercentage (lBestPercentage);

    // Store the (Xapian) document itself
    const Xapian::Document& lBestDocument = itDoc.get_document();
    ioMatchingDocuments.setXapianDocument (lBestDocument);

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

    /**
       Add all the Xapian documents having reached the same matching
       percentage.
    */
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
        ioMatchingDocuments.addExtraDocument (lDocument);
        
      } else {
        ioMatchingDocuments.addAlternateDocument (lPercentage, lDocument);
      }
    }
  }
  
}
