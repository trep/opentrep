#ifndef __OPENTREP_DBA_DBAPLACE_HPP
#define __OPENTREP_DBA_DBAPLACE_HPP

// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// SOCI
#if defined(SOCI_HEADERS_BURIED)
#include <soci/core/soci.h>
#else // SOCI_HEADERS_BURIED
#include <soci/soci.h>
#endif // SOCI_HEADERS_BURIED

// Forward declarations
namespace OPENTREP {
  class Place;
}

namespace soci {

  /**
   * @brief Specify how the Place class can be converted to (resp. from)
   *        values stored into (resp. retrieved from) database, using
   *        the SOCI framework.
   */
  template <>
  struct type_conversion<OPENTREP::Place> {

    typedef values base_type;

    /**
     * Fill an Place object from the database values.
     */
    static void from_base (values const& iPlaceValues, indicator,
                           OPENTREP::Place&);


    /**
     * Fill the database values from an Place object.
     */
    static void to_base (const OPENTREP::Place&, values& ioPlaceValues,
                         indicator&);
  };
}
#endif // __OPENTREP_DBA_DBAPLACE_HPP
