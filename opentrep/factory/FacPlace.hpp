#ifndef __OPENTREP_FAC_FACPLACE_HPP
#define __OPENTREP_FAC_FACPLACE_HPP

// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// OpenTrep
#include <opentrep/factory/FacBomAbstract.hpp>

namespace OPENTREP {

  // Forward declarations.
  class Place;
  struct LocationKey;
  struct Location;

  /**
   * Factory for Place.
   */
  class FacPlace : public FacBomAbstract {
  public:

    /**
     * Provide the unique instance.
     *
     * The singleton is instantiated when first used
     *
     * @return FacPlace&
     */
    static FacPlace& instance();

    /**
     * Destructor.
     *
     * The Destruction put the _instance to NULL
     * in order to be clean for the next FacPlace::instance()
     */
    virtual ~FacPlace();

    /**
     * Create a new Place object.
     *
     * This new object is added to the list of instantiated objects.
     *
     * @return Place& The newly created object.
     */
    Place& create();
    
    /**
     * Create a new Place object.
     *
     * This new object is added to the list of instantiated objects.
     *
     * @param const LocationKey& The primary key (IATA and ICAO codes,
     *                           Geonames ID) of the place
     * @return Place& The newly created object.
     */
    Place& create (const LocationKey&);
    
    /**
     * Create a new Place object.
     *
     * This new object is added to the list of instantiated objects.
     *
     * @param const Location& The primary key (IATA and ICAO codes,
     *                        Geonames ID) of the place
     * @return Place& The newly created object.
     */
    Place& create (const Location&);
    
    /**
     * Create a copy of a Place object.
     *
     * This new object is added to the list of instantiated objects.
     * @return Place& The newly created object.
     */
    Place& clone (const Place&);
    
    /**
     * Initialise the link between a Place and an extra Place.
     *
     * @param Place& Main Place object.
     * @param Place& Extra Place object.
     * @exception FacExceptionNullPointer
     * @exception FacException.
     */
    static void initLinkWithExtraPlace (Place&, Place&);

    /**
     * Initialise the link between a Place and an alternate Place.
     * @param Place& Main Place object.
     * @param Place& Alternate Place object.
     * @exception FacExceptionNullPointer
     * @exception FacException.
     */
    static void initLinkWithAlternatePlace (Place&, Place&);

  private:
    /**
     * Default Constructor.
     *
     * This constructor is private in order to ensure the singleton
     * pattern.
     */
    FacPlace ();
    FacPlace (const FacPlace&);

  private:
    /**
     * The unique instance.
     */
    static FacPlace* _instance;
  };
}
#endif // __OPENTREP_FAC_FACPLACE_HPP
