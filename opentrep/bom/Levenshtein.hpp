//
// Levenshtein Distance Algorithm: C++ Implementation by Anders Sewerin Johansen
//
#ifndef __OPENTREP_BOM_LEVENSHTEIN_HPP
#define __OPENTREP_BOM_LEVENSHTEIN_HPP

// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// STL
#include <string>
// OpenTREP
#include <opentrep/OPENTREP_Types.hpp>
#include <opentrep/bom/BomAbstract.hpp>

namespace OPENTREP {

  /** Class aggregating utilities around the Levenshtein edit
      distance/error. */
  class Levenshtein : public BomAbstract {
  public:
    /** Calculate the edit distance between two strings. */
    static int getDistance (const std::string& iSource,
                            const std::string& iTarget);
  };
  
}
#endif // __OPENTREP_BOM_LEVENSHTEIN_HPP
