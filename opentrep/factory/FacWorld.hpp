#ifndef __OPENTREP_FAC_FACWORLD_HPP
#define __OPENTREP_FAC_FACWORLD_HPP

// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// OpenTrep
#include <opentrep/factory/FacBomAbstract.hpp>

namespace OPENTREP {

  /** Forward declarations. */
  class World;
  class Place;

  /** Factory for World. */
  class FacWorld : public FacBomAbstract {
  public:

    /** Provide the unique instance.
        <br> The singleton is instantiated when first used
        @return FacWorld& */
    static FacWorld& instance();

    /** Destructor.
        <br> The Destruction put the _instance to NULL
        in order to be clean for the next FacWorld::instance() */
    virtual ~FacWorld();

    /** Create a new World object.
        <br>This new object is added to the list of instantiated objects.
        @return World& The newly created object. */
    World& create ();
    
    /** Link a Place object to a World one.
        @return void. */
    static void linkPlaceToWorld (const bool iNeedsGeneric, World&, Place&);

    /** Create a copy of a World object.
        <br>This new object is added to the list of instantiated objects.
        @return World& The newly created object. */
    World& clone (const World&);
    
  private:
    /** Default Constructor.
        <br>This constructor is private in order to ensure the singleton
        pattern.*/
    FacWorld () {}
    FacWorld (const FacWorld&) {}

  private:
    /** The unique instance.*/
    static FacWorld* _instance;

  };
}
#endif // __OPENTREP_FAC_FACWORLD_HPP
