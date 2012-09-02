// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// C
#include <cassert>
#include <sstream>
// OpenTREP
#include <opentrep/bom/Place.hpp>
#include <opentrep/bom/PlaceHolder.hpp>
#include <opentrep/service/Logger.hpp>

namespace OPENTREP {

  // //////////////////////////////////////////////////////////////////////
  PlaceHolder::PlaceHolder () {
    init();
  }
  
  // //////////////////////////////////////////////////////////////////////
  PlaceHolder::~PlaceHolder () {
  }

  // //////////////////////////////////////////////////////////////////////
  void PlaceHolder::init () {
    _placeList.clear();
    _placeOrderedList.clear();
  }

  // //////////////////////////////////////////////////////////////////////
  std::string PlaceHolder::describeShortKey() const {
    std::ostringstream oStr;
    return oStr.str();
  }
  
  // //////////////////////////////////////////////////////////////////////
  std::string PlaceHolder::describeKey() const {
    return describeShortKey();
  }

  // //////////////////////////////////////////////////////////////////////
  std::string PlaceHolder::toString() const {
    std::ostringstream oStr;
    oStr << describeShortKey() << std::endl;
    
    for (PlaceOrderedList_T::const_iterator itPlace = _placeOrderedList.begin();
         itPlace != _placeOrderedList.end(); ++itPlace) {
      const Place* lPlace_ptr = *itPlace;
      assert (lPlace_ptr != NULL);
      
      oStr << lPlace_ptr->toString() << std::endl;
    }
    
    return oStr.str();
  }   

  // //////////////////////////////////////////////////////////////////////
  std::string PlaceHolder::toShortString() const {
    std::ostringstream oStr;
    oStr << describeShortKey() << std::endl;
    
    for (PlaceOrderedList_T::const_iterator itPlace = _placeOrderedList.begin();
         itPlace != _placeOrderedList.end(); ++itPlace) {
      const Place* lPlace_ptr = *itPlace;
      assert (lPlace_ptr != NULL);
      
      oStr << lPlace_ptr->toShortString() << std::endl;
    }
    
    return oStr.str();
  }   

  // //////////////////////////////////////////////////////////////////////
  void PlaceHolder::toStream (std::ostream& ioOut) const {
    ioOut << toString();
  }
  
  // //////////////////////////////////////////////////////////////////////
  void PlaceHolder::fromStream (std::istream& ioIn) {
  }

  // //////////////////////////////////////////////////////////////////////
  void PlaceHolder::createLocations (LocationList_T& ioLocationList) const {

    for (PlaceOrderedList_T::const_iterator itPlace = _placeOrderedList.begin();
         itPlace != _placeOrderedList.end(); ++itPlace) {
      Place* lPlace_ptr = *itPlace;
      assert (lPlace_ptr != NULL);

      const Location& lLocation = lPlace_ptr->completeLocation();
      ioLocationList.push_back (lLocation);
    }
  }
    

}
