#ifndef __OPENTREP_LOCATIONLIST_HPP
#define __OPENTREP_LOCATIONLIST_HPP

// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// STL
#include <list>

namespace OPENTREP {

  // Forward declaration
  struct Location;
  
  /** 
   * List of (geographical) location structures. 
   */
  typedef std::list<Location> LocationList_T;

}
#endif // __OPENTREP_LOCATIONLIST_HPP

