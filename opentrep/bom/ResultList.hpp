#ifndef __OPENTREP_BOM_RESULTLIST_HPP
#define __OPENTREP_BOM_RESULTLIST_HPP

// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// STL
#include <list>

namespace OPENTREP {

  // Forward declarations
  class Result;

  /**
   * List of Result objects.
   */
  typedef std::list<Result*> ResultList_T;

}
#endif // __OPENTREP_BOM_RESULTLIST_HPP
