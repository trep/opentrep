#ifndef __OPENTREP_CMD_INDEXBUILDER_HPP
#define __OPENTREP_CMD_INDEXBUILDER_HPP

// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// OpenTrep
#include <opentrep/OPENTREP_Types.hpp>

// Forward declarations
namespace soci {
  class session;
}
namespace Xapian {
  class WritableDatabase;
}

namespace OPENTREP {

  // Forward declarations
  class Place;

  /** Command wrapping the travel request process. */
  class IndexBuilder {
    friend class OPENTREP_Service;
  private:

    /** Add a document, corresponding to a Place object, to the Xapian index. */
    static void addDocumentToIndex (Xapian::WritableDatabase&, Place&);

    /** Build Xapian database. */
    static NbOfDBEntries_T buildSearchIndex (soci::session&,
                                             const TravelDatabaseName_T&);

  private:
    /** Constructors. */
    IndexBuilder() {}
    IndexBuilder(const IndexBuilder&) {}
    /** Destructor. */
    ~IndexBuilder() {}
  };

}
#endif // __OPENTREP_CMD_INDEXBUILDER_HPP
