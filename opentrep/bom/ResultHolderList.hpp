#ifndef __OPENTREP_BOM_RESULTHOLDERLIST_HPP
#define __OPENTREP_BOM_RESULTHOLDERLIST_HPP

// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// STL
#include <list>

namespace OPENTREP {

  // Forward declarations
  class ResultHolder;

  /**
   * List of ResultHolder objects.
   */
  typedef std::list<ResultHolder*> ResultHolderList_T;

}
#endif // __OPENTREP_BOM_RESULTHOLDERLIST_HPP
