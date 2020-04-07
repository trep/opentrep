#ifndef __OPENTREP_CITYDETAILSLIST_HPP
#define __OPENTREP_CITYDETAILSLIST_HPP

// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// STL
#include <list>

namespace OPENTREP {

  // Forward declaration
  struct CityDetails;
  
  /** 
   * @brief A list of cities, for instance the list of cities served
   * by a travel-/transport-related POR (point of reference),
   * such as an airport, a railway/bus station or a ferry/maritime port.
   *
   * For instance,
   * [Bradley International Airport](http://www.geonames.org/5282636) serves
   * HFD/Hartford, BDL/Windsor Locks and SFY/Springfield in Connecticut (CT),
   * United States (US).
   */
  typedef std::list<CityDetails> CityDetailsList_T;

}
#endif // __OPENTREP_CITYDETAILSLIST_HPP

