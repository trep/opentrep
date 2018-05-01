/********************************************************************
 * COPYRIGHT:
 * Copyright (c) 1999-2002, International Business Machines Corporation and
 * others. All Rights Reserved.
 ********************************************************************/
// STL
#include <cassert>
#include <iostream>
#include <sstream>
// ICU
#include <unicode/unistr.h> //  UnicodeString
#include <unicode/ucnv.h> // Converter
// OpenTrep
#include "icutranslit_util.hpp"

// Verify that a UErrorCode is successful; exit(1) if not
void check (UErrorCode& status, const char* msg) {
    if (U_FAILURE (status)) {
      std::cerr << "ERROR: " << u_errorName(status) << " (" << msg << ")"
                << std::endl;
    }
    // std::cout << "Ok: " << msg << std::endl;
}
                                                      
// Append a hex string to the target
static icu::UnicodeString& appendHex (uint32_t number, int8_t digits, 
                                      icu::UnicodeString& target) {
    static const icu::UnicodeString DIGIT_STRING ("0123456789ABCDEF");
    while (digits > 0) {
      target += DIGIT_STRING[(number >> ((--digits) * 4)) & 0xF];
    }
    
    return target;
}

// Replace nonprintable characters with unicode escapes
icu::UnicodeString escape (const icu::UnicodeString& source) {
  int32_t i;
  icu::UnicodeString target;
  target += "\"";
  for (i=0; i<source.length(); ++i) {
    UChar ch = source[i];

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

// Print the given string to stdout
std::string uprintf (const icu::UnicodeString& str) {
  std::ostringstream oStr;
  
  const int32_t len = str.length();
  // int32_t bufLen = str.extract(0, len, buf); // Preflight
  /* Preflighting seems to be broken now, so assume 1-1 conversion,
     plus some slop. */
  const int32_t bufLen = len + 16;
  
  // Default codepage conversion
  char* buf = new char[bufLen + 1];
  const int32_t actualLen = str.extract (0, len, buf/*, bufLen*/);
  buf[actualLen] = '\0';

  oStr << buf;
  delete[] buf; buf = NULL;

  return oStr.str();
}

// /////////////////////////////////////////////////////////////////////
std::string toUTF8String (const icu::UnicodeString& iString) {
  std::ostringstream oStr;

  // String length
  // const int32_t lLength = iString.length();
  
  // Default codepage conversion
  const int32_t lCapacity = 1000;
  UChar lUCharString[lCapacity];
  UErrorCode status = U_ZERO_ERROR;
  const int32_t actualLen = iString.extract (lUCharString, lCapacity, status);
  assert (U_SUCCESS (status));
  lUCharString[actualLen] = '\0';

  // UTF-8 converter
  UConverter* cnv = ucnv_open ("UTF-8", &status);
  assert (U_SUCCESS (status));

  char lCharString[1000];
  // const int32_t nbOfConvertedChars =
    ucnv_fromUChars (cnv, lCharString, 1000, lUCharString, -1, &status);
  assert (U_SUCCESS (status));

  // DEBUG
  /*
  std::cout << "toUTF8String(): converted " << nbOfConvertedChars
            << " for the icu::UnicodeString '" << uprintf(iString)
            << "' (of length " << lLength << std::endl;
  */
  
  oStr << lCharString;
  
  return oStr.str();
}
