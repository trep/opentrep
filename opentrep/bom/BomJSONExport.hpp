#ifndef __OPENTREP_BOM_BOMJSONEXPORT_HPP
#define __OPENTREP_BOM_BOMJSONEXPORT_HPP

// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// STL
#include <iosfwd>
// Boost Property Tree (PT)
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
// OpenTrep
#include <opentrep/LocationList.hpp>

namespace bpt = boost::property_tree;

namespace OPENTREP {

  // Forward declarations
  struct Location;

  /**
   * @brief Utility class to export Opentrep structures in a JSON format.
   */
  class BomJSONExport {
  public:
    // //////////////// Export support methods /////////////////   

    /**
     * Export (dump in the underlying output log stream and in JSON format)
     * a list of Location objects.
     *
     * @param std::ostream& Output stream in which the Location objects
                            should be logged/dumped.
     * @param const LocationList_T& List of Location objects to be exported.
     */
    static void jsonExportLocationList (std::ostream&, const LocationList_T&);

    /**
     * Export (dump in the underlying output log stream and in JSON format)
     * a Location object.
     *
     * @param bpt::ptree& Property tree in which the Location structure 
     *                    should be logged/dumped.
     * @param const Location& Location object to be exported.
     */
    static void jsonExportLocation (bpt::ptree&, const Location&);
  };

}
#endif // __OPENTREP_BOM_BOMJSONEXPORT_HPP
