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
  class OTransliterator;

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
     * @param Xapian::WritableDatabase* Handle on the Xapian database/index
     *                                  It is NULL when no use of Xapian.
     * @param const DBType& SQL database type (can be no database at all).
     * @param soci::session* SOCI session handler. It can be NULL when there
     *                       is no use of SQL DB.
     * @param std::ifstream& File stream for the POR data file.
     * @param const shouldIndexNonIATAPOR_T& Whether all POR should be indexed.
     * @param const OTransliterator& Unicode transliterator.
     */
    static NbOfDBEntries_T buildSearchIndex (Xapian::WritableDatabase*,
                                             const DBType&, soci::session*,
                                             std::istream& iPORFileStream,
                                             const shouldIndexNonIATAPOR_T&,
                                             const OTransliterator&);

    /**
     * Build Xapian database.
     *
     * @param const PORFilePath_T& File-path of the POR file.
     * @param const TravelDBFilePath_T& File-path of the Xapian database.
     * @param const DBType& SQL database type (can be no database at all).
     * @param const SQLDBConnectionString_T& SQL DB connection string.
     * @param const shouldIndexNonIATAPOR_T& Whether all POR should be indexed.
     * @param const shouldIndexPORInXapian_T& Whether Xapian should be used.
     * @param const shouldAddPORInSQLDB_T& Whether the SQL DB should be used.
     * @param const OTransliterator& Unicode transliterator.
     */
    static NbOfDBEntries_T buildSearchIndex (const PORFilePath_T&,
                                             const TravelDBFilePath_T&,
                                             const DBType&,
                                             const SQLDBConnectionString_T&,
                                             const shouldIndexNonIATAPOR_T&,
                                             const shouldIndexPORInXapian_T&,
                                             const shouldAddPORInSQLDB_T&,
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
