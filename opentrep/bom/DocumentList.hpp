#ifndef __OPENTREP_BOM_DOCUMENTLIST_HPP
#define __OPENTREP_BOM_DOCUMENTLIST_HPP

// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// STL
#include <list>
// OpenTREP
#include <opentrep/OPENTREP_Types.hpp>
#include <opentrep/bom/MatchingDocuments.hpp>
// Xapian
#include <xapian.h>

namespace OPENTREP {

  /**
   * List of matching Xapian documents.
   */
  typedef std::list<MatchingDocuments> DocumentList_T;

}
#endif // __OPENTREP_BOM_DOCUMENTLIST_HPP
