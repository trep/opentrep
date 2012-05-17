// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// STL
#include <cassert>
#include <sstream>
#include <limits>
// Xapian
#include <xapian.h>
// OpenTrep
#include <opentrep/basic/float_utils.hpp>
#include <opentrep/bom/Filter.hpp>
#include <opentrep/bom/StringPartition.hpp>
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
  std::string ResultHolder::searchString (const TravelQuery_T& iQueryString,
                                          Document& ioMatchingDocument) const {
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
        oMatchedString = StringMatcher::searchString (lMatchingSet,
                                                      iQueryString,
                                                      ioMatchingDocument,
                                                      _database);
      }

      if (oMatchedString.empty() == false) {
        // Create the corresponding document (from the Xapian MSet object)
        StringMatcher::
          extractBestMatchingDocumentFromMSet (lMatchingSet,
                                               ioMatchingDocument);

        // Note: the allowable edit distance/error, as well as the
        // effective (Levenshtein) edit distance/error, have been
        // set, in the Document object, by the above call to the
        // searchString() method.

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
  Percentage_T ResultHolder::searchString (DocumentList_T& ioDocumentList,
                                           const StringSet& iStringSet) const {
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
        Document lMatchingDocument;
        const std::string& lMatchedString = searchString (lQueryString,
                                                          lMatchingDocument);

        if (lMatchedString.empty() == false) {
          //
          lMatchingDocument.setQueryString (lQueryString);
          lMatchingDocument.setCorrectedQueryString (lMatchedString);

          // Retrieve the matching percentage for the corresponding string only
          const Xapian::percent& lXapianPercentage =
            lMatchingDocument.getXapianPercentage();
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
          ioDocumentList.push_back (lMatchingDocument);

          // DEBUG
          const NbOfMatches_T& lNbOfMatches =
            lMatchingDocument.notifyIfExtraMatch();
          const NbOfErrors_T& lEditDistance =
            lMatchingDocument.getEditDistance();
          const NbOfErrors_T& lAllowableEditDistance =
            lMatchingDocument.getAllowableEditDistance();
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
  void ResultHolder::searchString (DocumentList_T& ioDocumentList,
                                   WordList_T& ioWordList) {

    // Catch any thrown Xapian::Error exceptions
    try {
      
      StringPartition lStringPartition (_queryString);

      // DEBUG
      OPENTREP_LOG_DEBUG ("+++++++++++++++++++++");
      OPENTREP_LOG_DEBUG ("Query string: `" << _queryString << "'");
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
        const double lMatchingPercentage = searchString (lDocumentList,
                                                         lStringSet);

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

}
