#ifndef __OPENTREP_BOM_LOCATIONEXCHANGE_HPP
#define __OPENTREP_BOM_LOCATIONEXCHANGE_HPP

// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// STL
#include <string>
// OpenTrep
#include <opentrep/OPENTREP_Types.hpp>
#include <opentrep/LocationList.hpp>

// Forward declarations for the Protobuf structures
namespace treppb {
  class Place;
}

namespace OPENTREP {

  // Forward declarations
  struct Location;

  /**
   * @brief Utility class to export Opentrep structures in a Protobuf format.
   */
  class LocationExchange {
  public:

    /**
     * Export (dump in the underlying output log stream and in Protobuf format)
     * a list of Location objects.
     *
     * @return std::string& Output string in which the Location objects
                            are logged/dumped.
     * @param const LocationList_T& List of Location objects to be exported.
     * @param const WordList_T& The list of non-matching keywords.
     */
    static std::string exportLocationList(const LocationList_T&,
                                          const WordList_T& iNonMatchedWordList);

    /**
     * Export (dump in the underlying output log stream and in Protobuf format)
     * a Location object.
     *
     * @param treppb::Place& Protobuf holder in which the Location structure 
     *                       should be logged/dumped.
     * @param const Location& Location object to be exported.
     */
    static void exportLocation (treppb::Place&, const Location&);
  };
  
}
#endif // __OPENTREP_BOM_LOCATIONEXCHANGE_HPP
