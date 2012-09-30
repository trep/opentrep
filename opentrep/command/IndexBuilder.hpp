#ifndef __OPENTREP_CMD_INDEXBUILDER_HPP
#define __OPENTREP_CMD_INDEXBUILDER_HPP

// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// OpenTrep
#include <opentrep/OPENTREP_Types.hpp>

// Forward declarations
namespace Xapian {
  class WritableDatabase;
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
    static void addDocumentToIndex (Xapian::WritableDatabase&, Place&,
                                    const OTransliterator&);

    /**
     * Parse the file of PageRank values.
     *
     * @param const PRFilePath_T& File-path of the PageRank file.
     */
    static NbOfDBEntries_T retrievePR (const PRFilePath_T&);

    /**
     * Build Xapian database.
     *
     * @param const PRFilePath_T&  File-path of the PageRank file.
     * @param const PORFilePath_T& File-path of the POR file.
     * @param const TravelDatabaseName_T& File-path of the Xapian database.
     * @param const OTransliterator& Unicode transliterator.
     */
    static NbOfDBEntries_T buildSearchIndex (const PRFilePath_T&,
                                             const PORFilePath_T&,
                                             const TravelDatabaseName_T&,
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
