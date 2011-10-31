#ifndef __OPENTREP_BOM_BOMTYPE_HPP
#define __OPENTREP_BOM_BOMTYPE_HPP

// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// STL
#include <string>

namespace OPENTREP {

  /** Enumeration of BOM types. */
  struct BomType {
  public:
    typedef enum { 
      WORLD = 0,
      PLACE,
      AIRLINE,
      COUNTRY,
      DAY,
      MONTH,
      PASSENGER,
      LAST_VALUE
    } EN_BomType;

    /** Get the label (BOM type) as a string. */
    static const std::string& getLabel (const EN_BomType&);

  private:
    /** String version of the enumeration. */    
    static const std::string _labels[LAST_VALUE];
  };

}
#endif // __OPENTREP_BOM_BOMTYPE_HPP
