#ifndef __OPENTREP_BOM_GENERICBOM_HPP
#define __OPENTREP_BOM_GENERICBOM_HPP

// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// STL
#include <map>
// OpenTrep
#include <opentrep/bom/BomType.hpp>

namespace OPENTREP {

  // Forward declarations
  class BomAbstract;
  
  /** A generic BOM is a pair of a BOM type (enumeration) and a BOM
      object pointer. */
  typedef std::pair<BomType::EN_BomType, BomAbstract*> GenericBom_T;

  /** List of generic BOM objects. */
  typedef std::map<std::size_t, GenericBom_T> GenericBomList_T;
  
}
#endif // __OPENTREP_BOM_GENERICBOM_HPP
