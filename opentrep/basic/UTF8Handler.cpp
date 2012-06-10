// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// STL
#include <cassert>
#include <sstream>
#include <string>
// OpenTrep
#include <opentrep/basic/UTF8Handler.hpp>

namespace OPENTREP {

  /**
   * 32-bit unsigned integer
   */
  typedef long unsigned int u_int32_t;

  // //////////////////////////////////////////////////////////////////////
  static const u_int32_t offsetsFromUTF8[6] = {
    0x00000000UL, 0x00003080UL, 0x000E2080UL,
    0x03C82080UL, 0xFA082080UL, 0x82082080UL
  };

  // //////////////////////////////////////////////////////////////////////
  static const char trailingBytesForUTF8[256] = {
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2, 3,3,3,3,3,3,3,3,4,4,4,4,5,5,5,5
  };

  // //////////////////////////////////////////////////////////////////////
  std::wstring UTF8Handler::toWideString (const std::string& iSrc) {
    std::basic_ostringstream<wchar_t> oStr;

    // Length of the source string
    const size_t lStringSize = iSrc.size();

    // Transform the source string in a regular C-string (char*)
    const char* src = iSrc.c_str();

    //
    typedef unsigned char uchar_t;
  
    size_t idx = 0;
    while (idx != lStringSize) {

      uchar_t lCurrentChar = static_cast<uchar_t> (src[idx]);

      /**
       * When there are multi-byte characters (e.g., for UTF-8 encoded
       * STL strings), the size of the STL string corresponds to the
       * total number of bytes. For instance, "München" has a size of 8
       * bytes (and not 7 characters). However, the iteration is made on
       * the number of characters (idx); when the end of the string is
       * reached, the loop must therefore be exited.
       */
      if (lCurrentChar == '\0') {
        break;
      }

      const int nb = trailingBytesForUTF8[lCurrentChar];
    
      wchar_t tmpChar = 0;
      switch (nb) {
        // These fall through deliberately
      case 3: {
        lCurrentChar = static_cast<uchar_t> (src[idx]); ++idx;
        tmpChar += lCurrentChar; tmpChar <<= 6;
      }
      case 2: {
        lCurrentChar = static_cast<uchar_t> (src[idx]); ++idx;
        tmpChar += lCurrentChar; tmpChar <<= 6;
      }
      case 1: {
        lCurrentChar = static_cast<uchar_t> (src[idx]); ++idx;
        tmpChar += lCurrentChar; tmpChar <<= 6;
      }
      case 0: {
        lCurrentChar = static_cast<uchar_t> (src[idx]); ++idx;
        tmpChar += lCurrentChar;
      }
      }
    
      tmpChar -= offsetsFromUTF8[nb];
      oStr << tmpChar;
    }

    oStr << '\0';
    return oStr.str();
  }

  // //////////////////////////////////////////////////////////////////////
  std::string UTF8Handler::toSimpleString (const std::wstring& iStr) {
    std::ostringstream oStr;
  
    const wchar_t* src = iStr.c_str();
    size_t idx = 0;
    size_t i = 0;
  
    while (src[i] != 0) {
      wchar_t ch = src[i];
    
      if (ch < 0x80) {
        const char tmpChar = static_cast<const char> (ch);
        oStr << tmpChar; ++idx;
      
      } else if (ch < 0x800) {
        char tmpChar = static_cast<const char> ((ch >> 6) | 0xC0);
        oStr << tmpChar; ++idx;

        tmpChar = static_cast<const char> ((ch & 0x3F) | 0x80);
        oStr << tmpChar; ++idx;
      
      } else if (ch < 0x10000) {
        char tmpChar = static_cast<const char> ((ch>>12) | 0xE0);
        oStr << tmpChar; ++idx;

        tmpChar = static_cast<const char> (((ch>>6) & 0x3F) | 0x80);
        oStr << tmpChar; ++idx;

        tmpChar = static_cast<const char> ((ch & 0x3F) | 0x80);
        oStr << tmpChar; ++idx;
      
      } else if (ch < 0x110000) {
        char tmpChar = static_cast<const char> ((ch>>18) | 0xF0);
        oStr << tmpChar; ++idx;

        tmpChar = static_cast<const char> (((ch>>12) & 0x3F) | 0x80);
        oStr << tmpChar; ++idx;
      
        tmpChar = static_cast<const char> (((ch>>6) & 0x3F) | 0x80);
        oStr << tmpChar; ++idx;
      
        tmpChar = static_cast<const char> ((ch & 0x3F) | 0x80);
        oStr << tmpChar; ++idx;
      }
      i++;
    }

    oStr << '\0';
  
    return oStr.str();
  }

  // //////////////////////////////////////////////////////////////////////
  std::string UTF8Handler::displayCharString (const char* iString) {
    std::ostringstream oStr;
    
    bool hasReachedEnd = false;
    for (size_t idx = 0; hasReachedEnd == false; ++idx) {
      if (idx != 0) {
        oStr << "; ";
      }
      const unsigned char lChar = iString[idx];
      // const wchar_t lChar = iString[idx];
      if (lChar == '\0') {
        hasReachedEnd = true;
      }
      oStr << "[" << idx << "]: " << std::hex << lChar;
    }
    oStr << std::endl;
    
    return oStr.str();
  }

  // //////////////////////////////////////////////////////////////////////
  std::string UTF8Handler::displaySTLWString (const std::wstring& iString) {
    std::ostringstream oStr;
    
    size_t idx = 0;
    for (std::wstring::const_iterator itChar = iString.begin();
         itChar != iString.end(); ++itChar, ++idx) {
      if (idx != 0) {
        oStr << "; ";
      }
      const wchar_t lChar = *itChar;
      oStr << "[" << idx << "]: " << std::hex << lChar;
    }
    oStr << std::endl;
    
    return oStr.str();
  }

}

