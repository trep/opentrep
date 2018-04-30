#ifndef __OPENTREP_BAS_ICU_UTIL_HPP
#define __OPENTREP_BAS_ICU_UTIL_HPP

// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// STL
#include <string>
// ICU
#include <unicode/unistr.h> // UnicodeString
#include <unicode/translit.h> // Transliterator

namespace OPENTREP {
  
  /**
   * Verify that the given status corresponds to a successful operation.
   *
   * @param const UErrorCode& The status of the last Unicode operation
   * @param const char* Error message
   * @return bool Whether or not the operation was successful
   */
  bool check (const UErrorCode& iStatus, const char* iMsg);

  /**
   * Replace non-printable characters with Unicode escapes.
   *
   * @param const UnicodeString& ICU Unicode string
   * @return UnicodeString  ICU Unicode string with escaped characters
   */
  icu::UnicodeString escape (const icu::UnicodeString&);

  /**
   * Get a STL string from the given UnicodeString.
   *
   * @param const UnicodeString& ICU Unicode string
   * @return std::string STL string
   */
  std::string getUTF8 (const icu::UnicodeString&);

}
#endif // __OPENTREP_BAS_ICU_UTIL_HPP
