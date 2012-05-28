// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// STL
#include <cassert>
#include <sstream>
// Boost
#include <boost/lexical_cast.hpp>
// OpenTrep
#include <opentrep/basic/float_utils.hpp>
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
    oStr << "`" << _queryString << "'";
    if (_correctedQueryString.empty() == false) {
      oStr << " (corrected into `" << _correctedQueryString
           << "' with an edit distance/error of " << _editDistance << ")";
    }
    return oStr.str();
  }

  // //////////////////////////////////////////////////////////////////////
  std::string MatchingDocuments::describe() const {
    std::ostringstream oStr;
    oStr << describeKey();
    
    if (_hasFullTextMatched == false) {
      oStr << "{ no match }" << std::endl;
      return oStr.str();
    }
    assert (_hasFullTextMatched == true);

    const Xapian::docid& lDocID = _document.get_docid();
    oStr << " => Document ID " << lDocID << " matching at " << _percentage
         << "% (edit distance of " << _editDistance << " over "
         << _allowableEditDistance << ") [" << _document.get_data() << "]";

    if (_documentList.empty() == false) {
      oStr << "  along with " << _documentList.size()
           << " other equivalent matching document(s) { ";

      unsigned short idx = 0;
      for (XapianDocumentList_T::const_iterator itDoc = _documentList.begin();
           itDoc != _documentList.end(); ++itDoc, ++idx) {
        const Xapian::Document& lXapianDoc = *itDoc;
        const Xapian::docid& lDocID = lXapianDoc.get_docid();
        if (idx != 0) {
          oStr << ", ";
        }
        oStr << "Doc ID " << lDocID << " [" << lXapianDoc.get_data() << "]";
      }
      oStr << " }";
    }
    
    if (_alternateDocumentList.empty() == false) {
      oStr << "  and with still " << _alternateDocumentList.size()
           << " other less matching document(s) { ";

      unsigned short idx = 0;
      for (XapianAlternateDocumentList_T::const_iterator itDoc =
             _alternateDocumentList.begin();
           itDoc != _alternateDocumentList.end(); ++itDoc, ++idx) {
        const Xapian::percent& lPercentage = itDoc->first;
        const Xapian::Document& lXapianDoc = itDoc->second;
        const Xapian::docid& lDocID = lXapianDoc.get_docid();
        if (idx != 0) {
          oStr << ", ";
        }
        oStr << lDocID << " / " << lPercentage << "% ["
             << lXapianDoc.get_data() << "]";
      }
      oStr << " }." << std::endl;
      
    } else {
      oStr << std::endl;
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
  NbOfMatches_T MatchingDocuments::notifyIfExtraMatch () const {
    NbOfMatches_T oNbOfMatches = _documentList.size();
    
    // DEBUG
    if (oNbOfMatches != 0) {
      OPENTREP_LOG_NOTIFICATION ("NOTE: the following document gets several "
                                 << "matches with the same matching percentage."
                                 << " You may want to alter the SQL database "
                                 << "and re-index the Xapian database, so as "
                                 << "to allow a more specific match:"
                                 << std::endl << describe());
    }

    // Return the total number of matches (main plus extras)
    return (1 + oNbOfMatches);
  }

  // //////////////////////////////////////////////////////////////////////
  Percentage_T MatchingDocuments::calculateMatchingWeight() const {
    Percentage_T oPercentage = 0.0;

    if (hasFullTextMatched() == true) {
      // Retrieve the matching percentage for the corresponding string only
      oPercentage = static_cast<Percentage_T> (_percentage);

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
