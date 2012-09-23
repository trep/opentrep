#ifndef __OPENTREP_BAS_BASCONST_UNICODE_HPP
#define __OPENTREP_BAS_BASCONST_UNICODE_HPP

// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////

namespace OPENTREP {

  /**
   * Default Unicode transliterator rule for the removal of
   * accents (e.g., "NFD; [:M:] Remove; NFC;").
   */
  extern const char* K_ICU_ACCENT_REMOVAL_RULE;

  /**
   * Default Unicode transliterator rule for the removal of
   * quotes (e.g., "[\\u02B9] > \\u0027; [\\u002D] > \\u0020;")
   */
  extern const char* K_ICU_QUOTATION_REMOVAL_RULE;

  /**
   * Default Unicode transliterator rule for the removal of
   * punctuation (e.g., "[:P:] Remove;")
   */
  extern const char* K_ICU_PUNCTUATION_REMOVAL_RULE;

  /**
   * Default Unicode transliterator rule for the removal of
   * punctuation (e.g., "Any-Latin;")
   */
  extern const char* K_ICU_GENERIC_TRANSLITERATOR_RULE;

}
#endif // __OPENTREP_BAS_BASCONST_UNICODE_HPP
