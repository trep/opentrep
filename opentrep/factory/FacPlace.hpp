#ifndef __OPENTREP_FAC_FACPLACE_HPP
#define __OPENTREP_FAC_FACPLACE_HPP

// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// OPENTREP
#include <opentrep/factory/FacBomAbstract.hpp>

namespace OPENTREP {

  /** Forward declarations. */
  class Place;

  /** Factory for Place. */
  class FacPlace : public FacBomAbstract {
  public:

    /** Provide the unique instance.
        <br> The singleton is instantiated when first used
        @return FacPlace& */
    static FacPlace& instance();

    /** Destructor.
        <br> The Destruction put the _instance to NULL
        in order to be clean for the next FacPlace::instance() */
    virtual ~FacPlace();

    /** Create a new Place object.
        <br>This new object is added to the list of instantiated objects.
        @return Place& The newly created object. */
    Place& create ();
    
    /** Create a copy of a Place object.
        <br>This new object is added to the list of instantiated objects.
        @return Place& The newly created object. */
    Place& clone (const Place&);
    
    /** Initialise the link between a Place and an extra Place.
        @param Place& Main Place object.
        @param Place& Extra Place object.
        @exception FacExceptionNullPointer
        @exception FacException.*/
    static void initLinkWithExtraPlace (Place&, Place&);

    /** Initialise the link between a Place and an alternate Place.
        @param Place& Main Place object.
        @param Place& Alternate Place object.
        @exception FacExceptionNullPointer
        @exception FacException.*/
    static void initLinkWithAlternatePlace (Place&, Place&);

  private:
    /** Default Constructor.
        <br>This constructor is private in order to ensure the singleton
        pattern.*/
    FacPlace ();
    FacPlace (const FacPlace&);

  private:
    /** The unique instance.*/
    static FacPlace* _instance;

  };
}
#endif // __OPENTREP_FAC_FACPLACE_HPP
