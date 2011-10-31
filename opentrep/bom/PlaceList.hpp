#ifndef __OPENTREP_BOM_PLACELIST_HPP
#define __OPENTREP_BOM_PLACELIST_HPP

// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// STL
#include <map>
#include <list>

namespace OPENTREP {

  // Forward declarations
  class Place;

  // ///////////// Type definitions ////////////////////
  // typedef std::size_t PlaceID_T;
  // typedef std::map<PlaceID_T, Place*> PlaceDirectList_T;

  typedef std::multimap<std::string, Place*> PlaceList_T;
  typedef std::list<Place*> PlaceOrderedList_T;

}
#endif // __OPENTREP_BOM_PLACELIST_HPP
