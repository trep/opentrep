/********************************************************************
 * COPYRIGHT:
 * Copyright (c) 1999-2003, International Business Machines Corporation and
 * others. All Rights Reserved.
 ********************************************************************/
// STL
#include <iostream>
#include <cassert>
// ICU
#include <unicode/translit.h>
//#include <unicode/rbt.h>
#include <unicode/unistr.h>
#include <unicode/calendar.h>
#include <unicode/datefmt.h>
// 
#include "icutranslit_util.hpp"
#include "icutranslit_unaccent.hpp"

/** Display the available Transliterators. */
void displayTransliterators () {
  UErrorCode status = U_ZERO_ERROR;
  StringEnumeration* lStringEnumeration =
    Transliterator::getAvailableIDs (status);
  check (status, "Transliterator::getAvailableIDs()");
  
  const UnicodeString* lString = lStringEnumeration->snext (status);
  check (status, "StringEnumeration::snext()");

  if (lString != NULL) {
    std::cout << "Available transliterators:" << std::endl;
  }
  
  while (lString != NULL) {
    std::cout << toUTF8String (*lString) << std::endl;
    
    lString = lStringEnumeration->snext (status);
    check (status, "StringEnumeration::snext()");
  }
  
  delete lStringEnumeration; lStringEnumeration = NULL;
}

// ////////////////////////// M A I N //////////////////////////////
int main (int argc, char* argv[]) {

    // Create a calendar in the Russian locale
    UErrorCode status = U_ZERO_ERROR;

    // DEBUG
    // displayTransliterators();
    // return 0;
    
    Locale russia ("ru", "RU");
    Calendar* cal = Calendar::createInstance (russia, status);
    check (status, "Calendar::createInstance");

    // Create a formatter
    DateFormat* fmt = DateFormat::createDateInstance(DateFormat::kFull, russia);
    fmt->setCalendar (*cal);

    // Create a default formatter
    DateFormat* defFmt = DateFormat::createDateInstance (DateFormat::kFull);
    defFmt->setCalendar (*cal);

    // Create a Any-Latin Transliterator
    const char* lLatinTransliteratorID = "Any-Latin; NFD; [:M:] Remove; NFC;";
    Transliterator* lLatinTransliterator =
      Transliterator::createInstance (lLatinTransliteratorID, UTRANS_FORWARD,
                                      status);

    if (lLatinTransliterator == NULL || U_FAILURE (status)) {
      std::cerr << "ERROR: Transliterator::createInstance() failed for "
                << lLatinTransliteratorID << std::endl;
      return -1;
    }
    assert (lLatinTransliterator != NULL);

    // Register the Transliterator object, so that the ICU library
    // manages the corresponding memory.
    Transliterator::registerInstance (lLatinTransliterator);

    // RuleBasedTransliterator rules to transform alternate forms of
    // quotes, so that they can be removed by the transliterator removing
    // punctuation.
    // For instance, ʹ (\u02B9) is transformed into ' (\u0027)
    // (see 
    UnicodeString lUnquoteRules ("[\\u02B9] > \\u0027;");

    // Create a transformation of alternate forms of quotes into
    // standard quotes
    UParseError pError;
    Transliterator* lPunctuationTransliterator =
      Transliterator::createFromRules ("RBTUnaccent", lUnquoteRules,
                                       UTRANS_FORWARD, pError, status);
    if (lPunctuationTransliterator == NULL || U_FAILURE (status)) {
      std::cerr << "ERROR: Transliterator::createInstance() failed for "
                << toUTF8String (lUnquoteRules) << std::endl;
      return -1;
    }
    assert (lPunctuationTransliterator != NULL);

    // Register the Transliterator object, so that the ICU library
    // manages the corresponding memory.
    Transliterator::registerInstance (lPunctuationTransliterator);

    // Create a punctuation-remover Transliterator
    const char* lUnpunctuateTransliteratorID = "[:P:] Remove;";
    Transliterator* lUnpunctuateTransliterator =
      Transliterator::createInstance (lUnpunctuateTransliteratorID,
                                      UTRANS_FORWARD, status);

    if (lUnpunctuateTransliterator == NULL || U_FAILURE (status)) {
      std::cerr << "ERROR: Transliterator::createInstance() failed for "
                << lUnpunctuateTransliteratorID << std::endl;
      return -1;
    }
    assert (lUnpunctuateTransliterator != NULL);

    // Register the Transliterator object, so that the ICU library
    // manages the corresponding memory.
    Transliterator::registerInstance (lUnpunctuateTransliterator);

    // Loop over various months
    for (int32_t month = Calendar::JANUARY;
         month <= Calendar::DECEMBER;
         ++month) {
      //    const int32_t month = Calendar::JUNE;

      // Set the calendar to a date
      cal->clear();
      cal->set (2009, month, 21);
      
      // Format the date in default locale
      UnicodeString str;
      str.remove();
      defFmt->format (cal->getTime (status), str, status);
      check (status, "DateFormat::format");
      
      std::cout << std::endl << "-----------------" << std::endl
                << "Date in default format: ";
      std::cout << toUTF8String (str) << std::endl;
      
      // Format the date for Russia
      str.remove();
      fmt->format (cal->getTime(status), str, status);
      check (status, "DateFormat::format");
      
      std::cout << "Russian formatted date: ";
      std::cout << toUTF8String (str) << std::endl;
      
      // Transliterate result
      lLatinTransliterator->transliterate (str);
      
      std::cout << "Transliterated via " << lLatinTransliteratorID << ": ";
      std::cout << toUTF8String (str) << std::endl;
      
      // Transliterate result
      lPunctuationTransliterator->transliterate (str);
      lLatinTransliterator->transliterate (str);
      lUnpunctuateTransliterator->transliterate (str);
      std::cout << "Transliterated via RBT unaccent: ";
      std::cout << toUTF8String (str) << std::endl;
    }

    // Clean up
    delete fmt; fmt = NULL;
    delete cal; cal = NULL;
    // delete lLatinTransliterator; lLatinTransliterator = NULL;
    // delete lPunctuationTransliterator; lPunctuationTransliterator = NULL;

    std::cout << "Exiting successfully" << std::endl;
    
    return 0;
}
