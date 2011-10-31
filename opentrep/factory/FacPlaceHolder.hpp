#ifndef __OPENTREP_FAC_FACPLACEHOLDER_HPP
#define __OPENTREP_FAC_FACPLACEHOLDER_HPP

// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// OPENTREP
#include <opentrep/factory/FacBomAbstract.hpp>

namespace OPENTREP {

  /** Forward declarations. */
  class PlaceHolder;
  class Place;

  /** Factory for Place. */
  class FacPlaceHolder : public FacBomAbstract {
  public:

    /** Provide the unique instance.
        <br> The singleton is instantiated when first used
        @return FacPlaceHolder& */
    static FacPlaceHolder& instance();

    /** Destructor.
        <br> The Destruction put the _instance to NULL
        in order to be clean for the next FacPlaceHolder::instance() */
    virtual ~FacPlaceHolder();

    /** Create a new PlaceHolder object.
        <br>This new object is added to the list of instantiated objects.
        @return PlaceHolder& The newly created object. */
    PlaceHolder& create ();

    /** Initialise the link between a PlaceHolder and a Place.
        @param PlaceHolder&
        @param Place&
        @exception FacExceptionNullPointer
        @exception FacException.*/
    static void initLinkWithPlace (PlaceHolder&, Place&);

    
  private:
    /** Default Constructor.
        <br>This constructor is private in order to ensure the singleton
        pattern.*/
    FacPlaceHolder ();
    FacPlaceHolder (const FacPlaceHolder&);

  private:
    /** The unique instance.*/
    static FacPlaceHolder* _instance;

  };
}
#endif // __OPENTREP_FAC_FACPLACEHOLDER_HPP
