// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// STL
#include <cassert>
// OpenTrep
#include <opentrep/bom/Place.hpp>
#include <opentrep/factory/FacSupervisor.hpp>
#include <opentrep/factory/FacPlace.hpp>

namespace OPENTREP {

  FacPlace* FacPlace::_instance = NULL;

  // //////////////////////////////////////////////////////////////////////
  FacPlace::FacPlace() {
  }

  // //////////////////////////////////////////////////////////////////////
  FacPlace::FacPlace (const FacPlace&) {
  }

  // //////////////////////////////////////////////////////////////////////
  FacPlace::~FacPlace() {
    _instance = NULL;
  }

  // //////////////////////////////////////////////////////////////////////
  FacPlace& FacPlace::instance() {

    if (_instance == NULL) {
      _instance = new FacPlace();
      assert (_instance != NULL);
      
      FacSupervisor::instance().registerBomFactory (_instance);
    }
    return *_instance;
  }

  // //////////////////////////////////////////////////////////////////////
  Place& FacPlace::create() {
    Place* oPlace_ptr = NULL;

    oPlace_ptr = new Place();
    assert (oPlace_ptr != NULL);

    // The new object is added to the Bom pool
    _pool.push_back (oPlace_ptr);

    return *oPlace_ptr;
  }

  // //////////////////////////////////////////////////////////////////////
  Place& FacPlace::create (const LocationKey& iLocationKey) {
    Place* oPlace_ptr = NULL;

    oPlace_ptr = new Place (iLocationKey);
    assert (oPlace_ptr != NULL);

    // The new object is added to the Bom pool
    _pool.push_back (oPlace_ptr);

    return *oPlace_ptr;
  }

  // //////////////////////////////////////////////////////////////////////
  Place& FacPlace::create (const Location& iLocation) {
    Place* oPlace_ptr = NULL;

    oPlace_ptr = new Place (iLocation);
    assert (oPlace_ptr != NULL);

    // The new object is added to the Bom pool
    _pool.push_back (oPlace_ptr);

    return *oPlace_ptr;
  }

  // //////////////////////////////////////////////////////////////////////
  Place& FacPlace::clone (const Place& iPlace) {
    Place* oPlace_ptr = NULL;

    oPlace_ptr = new Place (iPlace);
    assert (oPlace_ptr != NULL);

    // The new object is added to the Bom pool
    _pool.push_back (oPlace_ptr);

    return *oPlace_ptr;
  }
  
  // //////////////////////////////////////////////////////////////////////
  void FacPlace::initLinkWithExtraPlace (Place& ioMainPlace,
                                         Place& ioExtraPlace) {
      // Link the main Place to the extra Place, and vice versa
      ioExtraPlace._mainPlace = &ioMainPlace;
      
      // Add the extra Place to the main Place internal map (of extra
      // Place objects)
      ioMainPlace._extraPlaceList.push_back (&ioExtraPlace);
    }
  
  // //////////////////////////////////////////////////////////////////////
  void FacPlace::initLinkWithAlternatePlace (Place& ioMainPlace,
                                             Place& ioAlternatePlace) {
      // Link the main Place to the alternate Place, and vice versa
      ioAlternatePlace._mainPlace = &ioMainPlace;
      
      // Add the alternate Place to the main Place internal map (of
      // alternate Place objects)
      ioMainPlace._alternatePlaceList.push_back (&ioAlternatePlace);
    }
  
}
