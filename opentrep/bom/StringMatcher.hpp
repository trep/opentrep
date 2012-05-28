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
#include <opentrep/bom/DocumentList.hpp>

// Forward declarations
namespace Xapian {
  class MSet;
  class Database;
  class Document;
}

namespace OPENTREP {

  // Forward declarations
  struct StringSet;

  /**
   * @brief Class grouping a few utility methods.
   *
   * The utility methods heavily use the Xapian library.
   *
   * \see <a href="http://www.xapian.org">Xapian's Web site</a>
   * for more information.
   */
  class StringMatcher : public BomAbstract {
  public:
    // /////////// Business methods /////////
    /**
     * For all the elements (StringSet) of the string partitions, derived
     * from the given travel query, perform a Xapian-based full-text match.
     * Each Xapian-based full-text match gives (potentially) a full set of
     * matches, some with the highest matching percentage and some with a
     * lower percentage.
     *
     * @param TravelQuery_T& The query string.
     * @param DocumentList_T& List of matched documents for the query string.
     * @param WordList_T& List of non-matched words of the query string.
     * @param const Xapian::Database& The Xapian index/database.
     */
    static void searchString (const TravelQuery_T&, DocumentList_T&,
                              WordList_T&, const Xapian::Database&);


  public:
    // ///////////////////////////////////////////////
    /**
     * Extract the best matching Xapian document.
     *
     * If there are several such best matching documents (for
     * instance, several at, say, 100%), one is taken randomly. Well,
     * as we take the first one of the STL multimap, it is not exactly
     * randomly, but the result is the same: it appears to be random.
     *
     * @param Xapian::MSet& The Xapian matching set. It can be empty.
     */
    static void extractBestMatchingDocumentFromMSet (const Xapian::MSet&,
                                                     MatchingDocuments&);

    /**
     * Remove the word furthest at right.
     */
    static void removeFurthestRightWord (TravelQuery_T&);

    /**
     * Remove the word furthest at left.
     */
    static Word_T removeFurthestLeftWord (TravelQuery_T&);

    /**
     * Remove, from a string, the part corresponding to the one given
     * as parameter.
     */
    static void
    subtractParsedToRemaining (const TravelQuery_T& iAlreadyParsedQueryString,
                               TravelQuery_T& ioRemainingQueryString);
  };

}
#endif // __OPENTREP_BOM_STRINGMATCHER_HPP
