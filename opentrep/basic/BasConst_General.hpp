#ifndef __OPENTREP_BAS_BASCONST_GENERAL_HPP
#define __OPENTREP_BAS_BASCONST_GENERAL_HPP

// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// STL
#include <ctime>
// OpenTrep
#include <opentrep/OPENTREP_Types.hpp>

namespace OPENTREP {


  /**
   * Default envelope percentage (e.g., 0.10% == 0.001)
   */
  extern const Percentage_T K_DEFAULT_ENVELOPE_PCT;

  /**
   * Default modified matching percentage (e.g., 99.999% == 0.99999)
   */
  extern const Percentage_T K_DEFAULT_MODIFIED_MATCHING_PCT;

  /**
   * Default PageRank (e.g., 0.10% == 0.001)
   */
  extern const Percentage_T K_DEFAULT_PAGE_RANK;

  /**
   * Default number of characters by allowable spelling error unit
   */
  extern const NbOfErrors_T K_DEFAULT_SIZE_FOR_SPELLING_ERROR_UNIT;

  /**
   * Default "black list".
   */
  struct ListGenerator {
    static BlackList_T createBlackList();
  };

  /**
   * Black list, i.e., a list of words which should not be indexed
   * and/or searched for (e.g., "airport", "international").
   */
  extern const BlackList_T K_BLACK_LIST;

  /**
   * Default std::tm (date-time) structure (e.g., 1970-JAN-01).
   */
  extern const std::tm DEFAULT_DATE_TM;

  /**
   * Default date-time with std::string as type (e.g., "1970-01-01").
   */
  extern const std::string DEFAULT_DATE_STR;
}
#endif // __OPENTREP_BAS_BASCONST_GENERAL_HPP
