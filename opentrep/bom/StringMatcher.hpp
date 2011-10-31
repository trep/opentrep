#ifndef __OPENTREP_BOM_STRINGMATCHER_HPP
#define __OPENTREP_BOM_STRINGMATCHER_HPP

// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// STL
#include <string>
// OpenTREP
#include <opentrep/OPENTREP_Types.hpp>
#include <opentrep/bom/BomAbstract.hpp>
#include <opentrep/bom/Document.hpp>

// Forward declarations
namespace Xapian {
  class MSet;
  class Database;
  class Document;
}

namespace OPENTREP {

  /** Class grouping a few utility methods based on the Xapian library.
      <br>See <a href="http://www.xapian.org">Xapian's Web site</a>
      for more information. */
  class StringMatcher : public BomAbstract {
  public:
    // ///////////////////////////////////////////////
    /** Search, within the Xapian database, for occurrences of the
        words of the search string.
        @param Xapian::MSet& The Xapian matching set. It can be empty.
        @param const TravelQuery_T& The query string.
        @param NbOfErrors_T& The Levenshtein edit distance/error.
        @param NbOfErrors_T& The effective allowable edit distance/error,
               as calculated as a function of the number of letters.
        @param NbOfErrors_T& The maximal allowable edit distance/error for
               that step/call. We always have:
               ioEditDistance <= ioAllowableEditDistance <= iMaxEditDistance
        @param bool& Whether or not the maximal allowable edit distance/error
               has become greater than the maximum of the edit distance/errors
               calculated on the phrase.
        @param const Xapian::Database& The Xapian index/database.
        @return std::string The query string, potentially corrected,
        which has yielded matches. */
    static std::string searchString (Xapian::MSet&,
                                     const TravelQuery_T& iQueryString,
                                     Document& ioMatchingDocument,
                                     NbOfErrors_T& ioEditDistance,
                                     NbOfErrors_T& ioAllowableEditDistance,
                                     const Xapian::Database&);
    
    /** Extract the best matching Xapian document.
        <br>If there are several such best matching documents (for
        instance, several at, say, 100%), one is taken randomly. Well,
        as we take the first one of the STL multimap, it is not exactly
        randomly, but the result is the same: it appears to be random.
        @param Xapian::MSet& The Xapian matching set. It can be empty. */
    static void
    extractBestMatchingDocumentFromMSet (const Xapian::MSet&, Document&);

    /** Remove the word furthest at right. */
    static void removeFurthestRightWord (std::string& ioQueryString);

    /** Remove the word furthest at left. */
    static Word_T removeFurthestLeftWord (std::string& ioQueryString);

    /** Remove, from a string, the part corresponding to the one given
        as parameter. */
    static void
    subtractParsedToRemaining (const std::string& iAlreadyParsedQueryString,
                               std::string& ioRemainingQueryString);

    
  public:
    // ///////////////////////////////////////////////
    /** Extract the place code from the document data.
        <br>The place code is the first 3-letter string of the Xapian
        document data/content. */
    static std::string getPlaceCode (const Xapian::Document&);
    
  };

}
#endif // __OPENTREP_BOM_STRINGMATCHER_HPP
