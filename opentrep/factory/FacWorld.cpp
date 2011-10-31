// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// C
#include <assert.h>
// OPENTREP
#include <opentrep/bom/GenericBom.hpp>
#include <opentrep/bom/World.hpp>
#include <opentrep/bom/Place.hpp>
#include <opentrep/bom/PlaceList.hpp>
#include <opentrep/factory/FacSupervisor.hpp>
#include <opentrep/factory/FacBomAbstract.hpp>
#include <opentrep/factory/FacWorld.hpp>
#include <opentrep/service/Logger.hpp>

namespace OPENTREP {

  FacWorld* FacWorld::_instance = NULL;

  // //////////////////////////////////////////////////////////////////////
  FacWorld::~FacWorld () {
    _instance = NULL;
  }

  // //////////////////////////////////////////////////////////////////////
  FacWorld& FacWorld::instance () {

    if (_instance == NULL) {
      _instance = new FacWorld();
      assert (_instance != NULL);
      
      FacSupervisor::instance().registerBomFactory (_instance);
    }
    return *_instance;
  }

  // //////////////////////////////////////////////////////////////////////
  World& FacWorld::create () {
    World* oWorld_ptr = NULL;

    oWorld_ptr = new World ();
    assert (oWorld_ptr != NULL);

    // The new object is added to the Bom pool
    _pool.push_back (oWorld_ptr);

    return *oWorld_ptr;
  }

  // //////////////////////////////////////////////////////////////////////
  void FacWorld::linkPlaceToWorld (const bool iNeedsGeneric,
                                   World& ioWorld, Place& ioPlace) {
    // Link the World to the Place, and vice versa
    ioPlace._world = &ioWorld;
      
    // Retrieve the Xapian document ID of the Place object
    const XapianDocID_T& lDocID = ioPlace.getDocID();
    
    // Build a generic BOM object
    const GenericBom_T lGenericBom (BomType::PLACE, &ioPlace);
    
    if (iNeedsGeneric == true) {
      // Add the Place pointer to the dedicated list within the World object
      const bool insertSucceeded = ioWorld._genericBomList.
        insert (GenericBomList_T::value_type (lDocID, lGenericBom)).second;
      if (insertSucceeded == false) {
        OPENTREP_LOG_ERROR ("Insertion failed for " << ioWorld.describeKey()
                            << " and " << ioPlace.describeShortKey());
        assert (insertSucceeded == true);
      }

      // Add the Place pointer to the dedicated list within the World object
      /*
        const bool insertSucceeded2 = ioWorld._placeDirectList.
        insert (PlaceDirectList_T::value_type (lPlaceID, &ioPlace)).second;
        if (insertSucceeded2 == false) {
        OPENTREP_LOG_ERROR ("Insertion failed for " << ioWorld.describeKey()
        << " and " << ioPlace.describeShortKey());
        assert (insertSucceeded2 == true);
        }
      */
      
    }

    // Add the Place pointer to the dedicated list within the World object
    ioWorld._placeOrderedList.push_back (&ioPlace);
  }
  
  // //////////////////////////////////////////////////////////////////////
  World& FacWorld::clone (const World& iWorld) {
    World* oWorld_ptr = NULL;

    oWorld_ptr = new World (iWorld);
    assert (oWorld_ptr != NULL);

    // The new object is added to the Bom pool
    _pool.push_back (oWorld_ptr);

    return *oWorld_ptr;
  }
  
}
