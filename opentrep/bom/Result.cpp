// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// STL
#include <cassert>
#include <string>
#include <sstream>
// OpenTREP
#include <opentrep/basic/float_utils.hpp>
#include <opentrep/bom/Filter.hpp>
#include <opentrep/bom/WordHolder.hpp>
#include <opentrep/bom/StringMatcher.hpp>
#include <opentrep/bom/Result.hpp>
#include <opentrep/service/Logger.hpp>

namespace OPENTREP {

  // //////////////////////////////////////////////////////////////////////
  Result::Result (const TravelQuery_T& iQueryString,
                  const Xapian::Database& iDatabase)
    : _resultHolder (NULL), _queryString (iQueryString), _database (iDatabase),
      _matchingDocuments (iQueryString) {
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
    return describeShortKey();
  }

  // //////////////////////////////////////////////////////////////////////
  std::string Result::toString() const {
    std::ostringstream oStr;
    if (hasFullTextMatched() == true) {
      oStr << _matchingDocuments.describe();
    } else {
      oStr << "No full-text match for '" << _queryString << "'";
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
  Percentage_T Result::calculateMatchingWeight() const {
    Percentage_T oPercentage = 0.0;

    if (hasFullTextMatched() == true) {
      // Retrieve the matching percentage for the corresponding string only
      const Xapian::percent& lXapianPercentage =
        _matchingDocuments.getXapianPercentage();
      oPercentage = static_cast<Percentage_T> (lXapianPercentage);

      /**
       * Trick to decrease the overall percentage of word combinations,
       * when compared to the whole string. For instance, {"san francisco"}
       * will have a percentage of 99.999, compared to {"san", "francisco"}
       * which will have a percentage of 99.998.
       */
      const FloatingPoint<Percentage_T> lComparablePct (oPercentage);
      const FloatingPoint<Percentage_T> lFullMatchingPct (100.0);
      if (lComparablePct.AlmostEquals (lFullMatchingPct) == true) {
        oPercentage = 99.999;
      }

      // DEBUG
      OPENTREP_LOG_DEBUG ("        [pct] '" << describeKey()
                          << "' matches at " << oPercentage << "%");

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
        oPercentage = 100.0;

        // DEBUG
        OPENTREP_LOG_DEBUG("        [pct] '" << describeKey()
                           << "' does not match, but non black-listed "
                           << "single-word string, i.e., "
                           << oPercentage << "%");

      } else {
        /**
         * There is no full-text match for that query, which is made either
         * of several words or of a single black-listed word (e.g., 'airport').
         * The corresponding percentage is set to something low (5%),
         * in order to significantly decrease the overall matching
         * percentage. The corresponding string set will therefore have
         * almost no chance to being selected/chosen.
         */
        oPercentage = 5.0;

        // DEBUG
        OPENTREP_LOG_DEBUG("        [pct] '" << describeKey()
                           << "' does not match, and either multiple-word "
                           << "string or black-listed i.e., "
                           << oPercentage << "%");
      }
    }

    //
    return oPercentage;
  }

}
