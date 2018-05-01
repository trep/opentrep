// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// STL
#include <cassert>
#include <iostream>
#include <sstream>
// ICU
#include <unicode/ucnv.h> // Converter
// OpenTrep
#include <opentrep/service/Logger.hpp>
// Local
#include "icu_util.hpp"

namespace OPENTREP {

  // //////////////////////////////////////////////////////////////////////
  bool check (const UErrorCode& iStatus, const char* iMsg) {
    bool oSuccessful = true;
    if (U_FAILURE (iStatus)) {
      oSuccessful = false;
      OPENTREP_LOG_ERROR ("Unicode error: " << u_errorName (iStatus)
                          << " (" << iMsg << ")");
    }

    // DEBUG
    // OPENTREP_LOG_DEBUG ("No Unicode operation error: " << iMsg);

    return oSuccessful;
  }
                                                      
  // Append a hex string to the target
  // //////////////////////////////////////////////////////////////////////
  icu::UnicodeString& appendHex (const uint32_t iNumber, int8_t ioDigits,
                                   icu::UnicodeString& ioTarget) {
    static const icu::UnicodeString DIGIT_STRING ("0123456789ABCDEF");
    while (ioDigits > 0) {
      ioTarget += DIGIT_STRING[(iNumber >> ((--ioDigits) * 4)) & 0xF];
    }
    
    return ioTarget;
  }

  // //////////////////////////////////////////////////////////////////////
  icu::UnicodeString escape (const icu::UnicodeString& iSource) {
    int32_t i;
    icu::UnicodeString target ("\"");
    for (i = 0; i < iSource.length(); ++i) {
      UChar ch = iSource[i];

      if (ch < 0x09 || (ch > 0x0A && ch < 0x20) || ch > 0x7E) {
        target += "\\u";
        appendHex (ch, 4, target);
      
      } else {
        target += ch;
      }
    }
    target += "\"";

    return target;
  }

  // //////////////////////////////////////////////////////////////////////
  std::string getUTF8 (const icu::UnicodeString& iString) {
    std::ostringstream oStr;
  
    const int32_t len = iString.length();
    // int32_t bufLen = iString.extract(0, len, buf); // Preflight
    /* Preflighting seems to be broken now, so assume 1-1 conversion,
       plus some slop. */
    const int32_t bufLen = len + 256;

    // Default codepage conversion
    char* buf = new char[bufLen + 1];
    const int32_t actualLen = iString.extract (0, len, buf/*, bufLen*/);
    buf[actualLen] = '\0';

    oStr << buf;
    delete[] buf; buf = NULL;

    return oStr.str();
  }

}
