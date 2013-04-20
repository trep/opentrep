// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// C
#include <assert.h>
// STL
#include <sstream>
// OpenTrep
#include <opentrep/bom/Place.hpp>
#include <opentrep/bom/World.hpp>
#include <opentrep/service/Logger.hpp>

namespace OPENTREP {

  // //////////////////////////////////////////////////////////////////////
  World::World () {
  }
  
  // //////////////////////////////////////////////////////////////////////
  World::World (const World& iWorld) {
  }
  
  // //////////////////////////////////////////////////////////////////////
  World::~World () {
  }
  
  // //////////////////////////////////////////////////////////////////////
  std::string World::describeShortKey() const {
    std::ostringstream oStr;
    return oStr.str();
  }
  
  // //////////////////////////////////////////////////////////////////////
  std::string World::describeKey() const {
    return describeShortKey();
  }

  // //////////////////////////////////////////////////////////////////////
  std::string World::toString() const {
    return describeKey();
  }   

  // //////////////////////////////////////////////////////////////////////
  void World::toStream (std::ostream& ioOut) const {
    ioOut << toString();
  }
  
  // //////////////////////////////////////////////////////////////////////
  void World::fromStream (std::istream& ioIn) {
  }
    
  // //////////////////////////////////////////////////////////////////////
  std::string World::shortDisplay() const {
    std::ostringstream oStr;
    oStr << describeKey();
    return oStr.str();
  }

  // //////////////////////////////////////////////////////////////////////
  std::string World::display() const {
    std::ostringstream oStr;
    
    oStr << shortDisplay();
    
    for (PlaceOrderedList_T::const_iterator itPlace = _placeOrderedList.begin();
         itPlace != _placeOrderedList.end(); ++itPlace) {
      const Place* lPlace_ptr = *itPlace;
      assert (lPlace_ptr != NULL);
      
      oStr << lPlace_ptr->display ();
    }

    return oStr.str();
  }

  // //////////////////////////////////////////////////////////////////////
  GenericBom_T World::getGenericBom (const XapianDocID_T& iDocID) const {
    BomType::EN_BomType oBomType = BomType::LAST_VALUE;
    BomAbstract* oBom_ptr = NULL;

    GenericBomList_T::const_iterator itBom = _genericBomList.find (iDocID);
    if (itBom != _genericBomList.end()) {
      const GenericBom_T& lGenericBom = itBom->second;
      oBomType = lGenericBom.first;
      oBom_ptr = lGenericBom.second;
    }
    
    return GenericBom_T (oBomType, oBom_ptr);
  }

}
