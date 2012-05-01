#ifndef __OPENTREP_BAS_BASCONST_GENERAL_HPP
#define __OPENTREP_BAS_BASCONST_GENERAL_HPP

// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// OpenTrep
#include <opentrep/OPENTREP_Types.hpp>

namespace OPENTREP {

  /** Default "black list". */
  struct ListGenerator {
    static BlackList_T createBlackList();
  };

  /**
   * Black list, i.e., a list of words which should not be indexed
   * and/or searched for (e.g., "airport", "international").
   */
  extern const BlackList_T K_BLACK_LIST;

}
#endif // __OPENTREP_BAS_BASCONST_GENERAL_HPP
