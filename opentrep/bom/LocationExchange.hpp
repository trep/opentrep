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
   * Place holder for Protobuf-related import/export helper functions.
   */
  class LocationExchange {
  public:
    /**
     * Fill the Protobuf Place structure.
     */
    static std::string exportLocation (const Location&);
  };
  
}
#endif // __OPENTREP_PROTOBUF_TRAVEL_HPP
