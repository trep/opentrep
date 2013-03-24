#ifndef __OPENTREP_PROTOBUF_TRAVEL_HPP
#define __OPENTREP_PROTOBUF_TRAVEL_HPP

// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// STL
#include <iosfwd>
#include <string>
#include <set>
// OpenTrep
#include <opentrep/OPENTREP_Types.hpp>
#include <opentrep/Location.hpp>

namespace OPENTREP {

  /**
   * Fill the Protobuf Place structure.
   */
  void fillLocation (Location&);
  
}
#endif // __OPENTREP_PROTOBUF_TRAVEL_HPP
