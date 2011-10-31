#ifndef __OPENTREP_BOM_WORLD_HPP
#define __OPENTREP_BOM_WORLD_HPP

// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// STL
#include <ostream>
#include <istream>
#include <string>
// OpenTrep
#include <opentrep/OPENTREP_Types.hpp>
#include <opentrep/bom/BomAbstract.hpp>
#include <opentrep/bom/GenericBom.hpp>
#include <opentrep/bom/PlaceList.hpp>

namespace OPENTREP {
  
  /** Structure modelling an world. */
  class World : public BomAbstract {
    friend class FacWorld;
    friend class DbaWorld;
  public:
    // //////////// Getters /////////////
    /** Get the list of Place objects.
    const PlaceDirectList_T& getPlaceList () const {
      return _placeList;
    }
    */
    
    /** Get the list of Place objects. */
    const PlaceOrderedList_T& getSimplePlaceList () const {
      return _placeOrderedList;
    }
    
    // //////////// Setters /////////////

    
    // ///////// Display methods ////////
    /** Dump a Business Object into an output stream.
        @param ostream& the output stream. */
    void toStream (std::ostream&) const;

    /** Read a Business Object from an input stream.
        @param istream& the input stream. */
    void fromStream (std::istream&);

    /** Get the serialised version of the World object. */
    std::string toString() const;

    /** Get a string describing the whole key (differentiating two objects
        at any level). */
    std::string describeKey() const;

    /** Get a string describing the short key (differentiating two objects
        at the same level). */
    std::string describeShortKey() const;
    
    /** Display the full World context. */
    std::string display() const;

    /** Display a short World context. */
    std::string shortDisplay() const;

    /** Retrieve a generic BOM object from the dedicated list. */
    GenericBom_T getGenericBom (const XapianDocID_T& iDocID) const;
    
  private:
    /** Constructor. */
    World ();
    World (const World&);
    
    /** Destructor. */
    virtual ~World();
    
  private:
    /** List of generic BOM objects. */
    GenericBomList_T _genericBomList;
    
    /** List of Place objects.
        <br>That list is actually a STL map, indexed on the
        Xapian document ID. */
    // PlaceDirectList_T _placeList;
    
    /** List of Place objects.
        <br>That list is actually a STL list, to store temporarily Place
        objects when indexing the Xapian database. */
    PlaceOrderedList_T _placeOrderedList;
  };

  // ///////////// Type definitions ////////////////////
  typedef std::size_t WorldID_T;
  
}
#endif // __OPENTREP_BOM_WORLD_HPP
