#ifndef __OPENTREP_CMD_INDEXBUILDER_HPP
#define __OPENTREP_CMD_INDEXBUILDER_HPP

// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// OpenTrep
#include <opentrep/OPENTREP_Types.hpp>

/**
 * Forward declarations
 */
// Xapian
namespace Xapian {
  class WritableDatabase;
}

// SOCI (for SQL database)
namespace soci {
  class session;
  class statement;
}

namespace OPENTREP {

  // Forward declarations
  class Place;
  struct OTransliterator;

  /**
   * @brief Command wrapping the travel request process.
   */
  class IndexBuilder {
    friend class OPENTREP_Service;
  private:

    /**
     * Add a document, corresponding to a Place object, to the Xapian index.
     *
     * @param Xapian::WritableDatabase& Xapian database.
     * @param Place& Place object instance.
     * @param const OTransliterator& Unicode transliterator.
     */
    static void addDocumentToIndex (Xapian::WritableDatabase&,
                                    Place&, const OTransliterator&);

    /**
     * Build Xapian database.
     *
     * @param Xapian::WritableDatabase& Xapian database.
     * @param const DBType& SQL database type (can be no database at all).
     * @param soci::session* SOCI session handler (can be NULL; see above).
     * @param std::ifstream& File stream for the POR data file.
     * @param const OTransliterator& Unicode transliterator.
     */
    static NbOfDBEntries_T buildSearchIndex (Xapian::WritableDatabase&,
                                             const DBType&, soci::session*,
                                             std::istream& iPORFileStream,
                                             const OTransliterator&);

    /**
     * Build Xapian database.
     *
     * @param const PORFilePath_T& File-path of the POR file.
     * @param const TravelDBFilePath_T& File-path of the Xapian database.
     * @param const DBType& SQL database type (can be no database at all).
     * @param const SQLDBConnectionString_T& SQL DB connection string.
     * @param const OTransliterator& Unicode transliterator.
     */
    static NbOfDBEntries_T buildSearchIndex (const PORFilePath_T&,
                                             const TravelDBFilePath_T&,
                                             const DBType&,
                                             const SQLDBConnectionString_T&,
                                             const OTransliterator&);

  private:
    /**
     * Default constructor.
     */
    IndexBuilder() {}

    /**
     * Copy constructor.
     */
    IndexBuilder(const IndexBuilder&) {}

    /**
     * Destructor.
     */
    ~IndexBuilder() {}
  };

}
#endif // __OPENTREP_CMD_INDEXBUILDER_HPP
