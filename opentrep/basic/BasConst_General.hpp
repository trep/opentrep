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
   * Default size of matching set for Xapian (e.g., 30)
   */
  extern const NbOfMatches_T K_DEFAULT_XAPIAN_MATCHING_SET_SIZE;

  /**
   * Default indexing weight for standard terms (e.g., 1)
   */
  extern const Weight_T K_DEFAULT_INDEXING_STD_WEIGHT;

  /**
   * Default indexing weight for some terms like IATA/ICAO/FAA codes (e.g., 2)
   */
  extern const Weight_T K_DEFAULT_INDEXING_EXTRA_WEIGHT;

  /**
   * Default modified matching percentage (e.g., 99.999% == 0.99999)
   */
  extern const Percentage_T K_DEFAULT_MODIFIED_MATCHING_PCT;

  /**
   * Default PageRank (e.g., 0.10% == 0.001)
   */
  extern const Percentage_T K_DEFAULT_PAGE_RANK;

  /**
   * Default envelope percentage (e.g., 0.10% == 0.001).
   * Usually equal to K_DEFAULT_PAGE_RANK.
   */
  extern const Percentage_T K_DEFAULT_ENVELOPE_PCT;

  /**
   * Default full match percentage for a IATA/ICAO code (e.g.,
   * 11,000,000% == 110,000 == 110 / 0.001).
   * Usually equal to 100 * 1.1 / (K_DEFAULT_PAGE_RANK / 100).
   */
  extern const Percentage_T K_DEFAULT_FULL_CODE_MATCH_PCT;

  /**
   * Default attenuation factor (e.g., 850.0). It weighs down a set
   * of strings when compared to the single string.
   * For instance, the {"paris", "texas"} combination is weighed down,
   * so that it is ranked less than "paris texas".
   */
  extern const Percentage_T K_DEFAULT_ATTENUATION_FCTR;

  /**
   * Default number of characters by allowable spelling error unit
   */
  extern const NbOfErrors_T K_DEFAULT_SIZE_FOR_SPELLING_ERROR_UNIT;

  /**
   * Maximum number of words a string may have (e.g., 14).
   * The issue typically arises with Bangkok (BKK):
   * http://www.geonames.org/1609350
   */
  extern const NbOfWords_T K_DEFAULT_MAXIMUM_NUMBER_OF_WORDS_IN_STRING;

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
