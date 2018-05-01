/********************************************************************
 * COPYRIGHT:
 * Copyright (c) 1999-2002, International Business Machines Corporation and
 * others. All Rights Reserved.
 ********************************************************************/
// STL
#include <string>
// ICU
#include <unicode/unistr.h>

/** Verify that a UErrorCode is successful; exit(1) if not. */
void check (UErrorCode& status, const char* msg);

/** Replace nonprintable characters with unicode escapes. */
icu::UnicodeString escape (const icu::UnicodeString &source);

/** Print the given string to stdout. */
std::string uprintf (const icu::UnicodeString& str);

/** Print the given Unicode string as a UTF8-encoded string. */
std::string toUTF8String (const icu::UnicodeString& str);
