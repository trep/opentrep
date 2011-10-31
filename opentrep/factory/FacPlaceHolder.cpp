// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// C
#include <cassert>
// OPENTREP
#include <opentrep/bom/PlaceHolder.hpp>
#include <opentrep/bom/Place.hpp>
#include <opentrep/factory/FacSupervisor.hpp>
#include <opentrep/factory/FacPlaceHolder.hpp>
#include <opentrep/service/Logger.hpp>

namespace OPENTREP {

  FacPlaceHolder* FacPlaceHolder::_instance = NULL;

  // //////////////////////////////////////////////////////////////////////
  FacPlaceHolder::FacPlaceHolder () {
  }

  // //////////////////////////////////////////////////////////////////////
  FacPlaceHolder::FacPlaceHolder (const FacPlaceHolder&) {
  }
  
  // //////////////////////////////////////////////////////////////////////
  FacPlaceHolder::~FacPlaceHolder () {
    _instance = NULL;
  }

  // //////////////////////////////////////////////////////////////////////
  FacPlaceHolder& FacPlaceHolder::instance () {

    if (_instance == NULL) {
      _instance = new FacPlaceHolder();
      assert (_instance != NULL);
      
      FacSupervisor::instance().registerBomFactory (_instance);
    }
    return *_instance;
  }

  // //////////////////////////////////////////////////////////////////////
  PlaceHolder& FacPlaceHolder::create () {
    PlaceHolder* oPlaceHolder_ptr = NULL;

    oPlaceHolder_ptr = new PlaceHolder ();
    assert (oPlaceHolder_ptr != NULL);

    // The new object is added to the Bom pool
    _pool.push_back (oPlaceHolder_ptr);

    return *oPlaceHolder_ptr;
  }

  // //////////////////////////////////////////////////////////////////////
  void FacPlaceHolder::initLinkWithPlace (PlaceHolder& ioPlaceHolder,
                                          Place& ioPlace) {
      // Link the PlaceHolder to the Place, and vice versa
      ioPlace._placeHolder = &ioPlaceHolder;
      
      // Add the Place to the PlaceHolder internal map (of Place objects)
      // const bool insertSucceeded =
      ioPlaceHolder._placeList.
        insert (PlaceList_T::value_type (ioPlace.describeShortKey(), &ioPlace));
      
//       if (insertSucceeded == false) {
//         OPENTREP_LOG_ERROR ("Insertion failed for "
//                             << ioPlaceHolder.describeKey()
//                             << " and " << ioPlace.describeShortKey());
//         assert (insertSucceeded == true);
//       }

      // Add the Place to the PlaceHolder internal list (of Place objects)
      ioPlaceHolder._placeOrderedList.push_back (&ioPlace);
    }

}
