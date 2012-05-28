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

// Forward declarations
namespace Xapian {
  class Database;
}

namespace OPENTREP {

  // Forward declarations
  struct MatchingDocuments;

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
     * For all the elements (strings) of the partial travel query (string
     * set), perform a Xapian-based full-text match.
     * That Xapian-based full-text match gives (potentially) a full set of
     * matches, some with the highest matching percentage and some with a
     * lower percentage.
     *
     * @param TravelQuery_T& The query string.
     * @param MatchingDocuments& The set of full-trext matching documents.
     * @param const Xapian::Database& The Xapian index/database.
     */
    static std::string fullTextMatch (const TravelQuery_T&, MatchingDocuments&,
                                      const Xapian::Database&);
  };

}
#endif // __OPENTREP_BOM_STRINGMATCHER_HPP
