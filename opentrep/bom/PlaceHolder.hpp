#ifndef __OPENTREP_BOM_PLACEHOLDER_HPP
#define __OPENTREP_BOM_PLACEHOLDER_HPP

// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// OpenTREP
#include <opentrep/OPENTREP_Types.hpp>
#include <opentrep/Location.hpp>
#include <opentrep/bom/BomAbstract.hpp>
#include <opentrep/bom/PlaceList.hpp>

namespace OPENTREP {

  /** Class wrapping functions on a list of Place objects. */
  class PlaceHolder : public BomAbstract {
    friend class FacPlaceHolder;
  public:
    // ////////////// Getters /////////////
    /** Retrieve the list of place objects. */
    const PlaceList_T& getPlaceList() const {
      return _placeList;
    }

    
    // ////////////// Setters /////////////


  public:
    // /////////// Business methods /////////
    /** Create the list of Location structures, which are light copies
        of the Place objects. Those (Location) structures are passed
        back to the caller of the service. */
    void createLocations (LocationList_T&) const;
    
    
  public:
    // /////////// Display support methods /////////
    /** Dump a Business Object into an output stream.
        @param ostream& the output stream. */
    void toStream (std::ostream& ioOut) const;

    /** Read a Business Object from an input stream.
        @param istream& the input stream. */
    void fromStream (std::istream& ioIn);

    /** Get the serialised version of the Business Object. */
    std::string toString() const;
    
    /** Get a short display of the Business Object. */
    std::string toShortString() const;
    
    /** Get a string describing the whole key (differentiating two objects
        at any level). */
    std::string describeKey() const;

    /** Get a string describing the short key (differentiating two objects
        at the same level). */
    std::string describeShortKey() const;


  private:
    // ////////////// Constructors and Destructors /////////////
    /** Default constructor. */
    PlaceHolder ();
    /** Default copy constructor. */
    PlaceHolder (const PlaceHolder&);
    /** Destructor. */
    ~PlaceHolder ();
    /** Initialise (reset the list of documents). */
    void init ();

    
  private:
    // /////////////// Attributes ////////////////
    /** List of place objects, sorted by Place ID. */
    PlaceList_T _placeList;
    
    /** List of place objects, the sort order corresponding to their
        insertion order. */
    PlaceOrderedList_T _placeOrderedList;
  };

}
#endif // __OPENTREP_BOM_PLACEHOLDER_HPP
