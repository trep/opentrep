#ifndef __OPENTREP_BAS_UTF8HANDLER_HPP
#define __OPENTREP_BAS_UTF8HANDLER_HPP

// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// STL
#include <string>

namespace OPENTREP {

  /**
   * @brief Utility class for basic handling of UTF-8 encoded strings.
   *
   * Most of the methods have taken their inspiration from Jeff Bezanson's
   * work in the Wikix project (see http://meta.wikimedia.org/wiki/Wikix
   * for further details), and have been "C++-ified".
   */
  class UTF8Handler {
  public:    
    /*
     * Conversion from a UTF-8-encoded "simple character" (though
     * potentially multi-byte) STL string into a wide character STL
     * string.
     *
     * Note that as there is no checks of appropriate encoding, it
     * only works for valid UTF-8, i.e. no 5- or 6-byte sequences.
     *
     * Note that the "simple characters", within a STL string, may be
     * multi-byte (e.g., if they are UTF-8-encoded).
     *
     * @param std::string The "simple character" (though potentially
     *        multi-byte) STL string.
     * @return std::wstring The wide character STL string.
     */
    static std::wstring toWideString (const std::string& iSrc);

    /*
     * Conversion from a wide character STL string into a UTF-8-encoded
     * "simple character" (though potentially multi-byte) STL string.
     *
     * Note that as there is no checks of appropriate encoding, it
     * only works for valid UTF-8, i.e. no 5- or 6-byte sequences.
     *
     * Note that the "simple characters", within a STL string, may be
     * multi-byte (e.g., if they are UTF-8-encoded).
     *
     * @param std::wstring The wide character STL string.
     * @return std::string The "simple character" (though potentially
     *         multi-byte) STL string.
     */
    static std::string toSimpleString (const std::wstring& iStr);

    /**
     * Display the sequence of characters for the simple C-string.
     */
    static std::string displayCharString (const char* iString);

    /**
     * Display the sequence of characters (one by one) for the given
     * STL wide character string.
     */
    static std::string displaySTLWString (const std::wstring& iString);
  };

}
#endif // __OPENTREP_BAS_UTF8HANDLER_HPP
