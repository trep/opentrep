// STL
#include <cassert>
#include <iostream>
#include <sstream>
#include <string>
// ICU
#include <unicode/translit.h>
#include <unicode/unistr.h>
#include <unicode/ucnv.h>
//
#include "russian_latin_bgn.hpp"
#include "ukrainian_latin_bgn.hpp"

// /////////////////////////////////////////////////////////////////////
std::string toUTF8String (const UnicodeString& iString) {
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
            << " for the UnicodeString '" << uprintf(iString)
            << "' (of length " << lLength << std::endl;
  */
  
  oStr << lCharString;
  
  return oStr.str();
}

// /////////////////////////////////////////////////////////////////////
void initPunctuationTransliterators(Transliterator*& lNormaliser,
                                    Transliterator*& lPunctuationTransliterator,
                                    Transliterator*& lUnpunctuateTransliterator) {
  lPunctuationTransliterator = NULL;
  lUnpunctuateTransliterator = NULL;

  // Create a Normaliser
  UErrorCode status = U_ZERO_ERROR;
  const char* lNormaliserID = "NFD; [:M:] Remove; NFC;";
  lNormaliser =
    Transliterator::createInstance (lNormaliserID, UTRANS_FORWARD, status);
  
  if (lNormaliser == NULL || U_FAILURE (status)) {
    std::cerr << "ERROR: Transliterator::createInstance() failed for "
              << lNormaliserID << std::endl;
    return;
  }
  assert (lNormaliser != NULL);
  
  // Register the Transliterator object, so that the ICU library
  // manages the corresponding memory.
  Transliterator::registerInstance (lNormaliser);

  // RuleBasedTransliterator rules to transform alternate forms of
  // quotes, so that they can be removed by the transliterator removing
  // punctuation.
  // For instance, ʹ (\u02B9) is transformed into ' (\u0027)
  //           and - (\u002D) is transformed into space (\u0020)
  // (see 
  UnicodeString lUnquoteRules ("[\\u02B9] > \\u0027; [\\u002D] > \\u0020;");
   
  // Create a transformation of alternate forms of quotes into
  // standard quotes
  UParseError pError;
  lPunctuationTransliterator =
    Transliterator::createFromRules ("RBTUnaccent", lUnquoteRules,
                                     UTRANS_FORWARD, pError, status);
  if (lPunctuationTransliterator == NULL || U_FAILURE (status)) {
    std::cerr << "ERROR: Transliterator::createInstance() failed for "
              << toUTF8String (lUnquoteRules) << std::endl;
    return;
  }
  assert (lPunctuationTransliterator != NULL);
  
  // Register the Transliterator object, so that the ICU library
  // manages the corresponding memory.
  Transliterator::registerInstance (lPunctuationTransliterator);
  
  // Create a punctuation-remover Transliterator
  const char* lUnpunctuateTransliteratorID = "[:P:] Remove;";
  lUnpunctuateTransliterator =
    Transliterator::createInstance (lUnpunctuateTransliteratorID,
                                    UTRANS_FORWARD, status);
  
  if (lUnpunctuateTransliterator == NULL || U_FAILURE (status)) {
    std::cerr << "ERROR: Transliterator::createInstance() failed for "
              << lUnpunctuateTransliteratorID << std::endl;
    return;
  }
  assert (lUnpunctuateTransliterator != NULL);
  
  // Register the Transliterator object, so that the ICU library
  // manages the corresponding memory.
  Transliterator::registerInstance (lUnpunctuateTransliterator);
}

// /////////////////////////////////////////////////////////////////////
void removeQuotesAndPunctuation (UnicodeString& ioString,
                                 Transliterator& lNormaliser,
                                 Transliterator& lPunctuationTransliterator,
                                 Transliterator& lUnpunctuateTransliterator) {

  // Remove quotes and punctuations
  lNormaliser.transliterate (ioString);
  lPunctuationTransliterator.transliterate (ioString);
  lUnpunctuateTransliterator.transliterate (ioString);

  // Display the result
  std::cout << "After normalisation and without accents: ";
  std::cout << toUTF8String (ioString) << std::endl;
}

// ////////////////////////// M A I N //////////////////////////////
int main (int argc, char* argv[]) {

    // Default values
    UErrorCode status = U_ZERO_ERROR;

    //
    Transliterator* lNormaliser = NULL;
    Transliterator* lPunctuationTransliterator = NULL;
    Transliterator* lUnpunctuateTransliterator = NULL;
    initPunctuationTransliterators (lNormaliser, lPunctuationTransliterator,
                                    lUnpunctuateTransliterator);
    assert (lPunctuationTransliterator != NULL
            && lUnpunctuateTransliterator != NULL);
    
    // Create a Russian-Latin/BGN transliterator (which may be
    // integrated in the more recent versions of the ICU library)
    UParseError pError;
    Transliterator* lRussianLatinBGNTransliterator =
      Transliterator::createFromRules ("RBTRussianLatinBGN",
                                       RUSSIAN_LATIN_BGN_TRANSLITERATION_RULES,
                                       UTRANS_FORWARD, pError, status);
    if (lRussianLatinBGNTransliterator == NULL || U_FAILURE (status)) {
      std::cerr << "ERROR: Transliterator::createInstance() failed for "
                << "RBTRussianLatinBGN" << std::endl;
      return -1;
    }
    assert (lRussianLatinBGNTransliterator != NULL);

    // Create a Ukrainian-Latin/BGN transliterator (which may be
    // integrated in the more recent versions of the ICU library)
    Transliterator* lUkrainianLatinBGNTransliterator =
      Transliterator::createFromRules("RBTUkrainianLatinBGN",
                                      UKRAINIAN_LATIN_BGN_TRANSLITERATION_RULES,
                                      UTRANS_FORWARD, pError, status);
    if (lUkrainianLatinBGNTransliterator == NULL || U_FAILURE (status)) {
      std::cerr << "ERROR: Transliterator::createInstance() failed for "
                << "RBTUkrainianLatinBGN" << std::endl;
      return -1;
    }
    assert (lUkrainianLatinBGNTransliterator != NULL);

    // Create a Cyrillic-Latin Transliterator
    const char* lCyrillicLatinTransliteratorID = "Cyrillic-Latin; NFD; [:M:] Remove; NFC;";
    Transliterator* lCyrillicLatinTransliterator =
      Transliterator::createInstance (lCyrillicLatinTransliteratorID,
                                      UTRANS_FORWARD, status);

    if (lCyrillicLatinTransliterator == NULL || U_FAILURE (status)) {
      std::cerr << "ERROR: Transliterator::createInstance() failed for "
                << lCyrillicLatinTransliteratorID << std::endl;
      return -1;
    }
    assert (lCyrillicLatinTransliterator != NULL);

    // Register the Transliterator object, so that the ICU library
    // manages the corresponding memory.
    Transliterator::registerInstance (lCyrillicLatinTransliterator);

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

    //
    // Define an UTF-8-encoded query string, with a mix of Russian and
    // Ukrainian places.
    UnicodeString lQueryString ("München (de) - Мюнхен (ru), "
                                "Житомир (ru & uk), Ужгород (ru & uk), "
                                "Херсон (ru & uk), "
                                "Украина (ru) - Україна (uk), "
                                "Киев (ru) - Київ (uk), "
                                "Львов (ru) - Львів (uk), "
                                "Днепропетровск (ru) - Дніпропетровськ (uk), "
                                "Ивано-Франковск (ru) - Івано-Франківськ (uk), "
                                "Хортица (ru) - Хортиця (uk), "
                                "Запорожье (ru) - Запоріжжя (uk), "
                                "Хмельницкий (ru) - Хмельницький (uk), "
                                "Одесса (ru) - Одеса (uk), "
                                "Кировоград (ru) - Кіровоград (uk), "
                                "Луганск (ru) - Луганськ (uk), "
                                "Харьков (ru) - Харків (uk), "
                                "Чернигов (ru) - Чернігів (uk), "
                                "Донецк (ru) - Донецьк (uk), "
                                "Черкассы (ru) - Черкаси (uk), "
                                "Черновцы (ru) - Чернівці (uk), "
                                "Винница (ru) - Вінниця (uk)");
    UnicodeString lQueryString2 (lQueryString);
    UnicodeString lQueryString3 (lQueryString);
    UnicodeString lQueryString4 (lQueryString);

    //
    std::cout << std::endl << "-----------------" << std::endl
              << "Cities in Russian and Ukrainian languages: ";
    std::cout << toUTF8String (lQueryString) << std::endl;
      
    // Transliterate with the classical Any-Latin transliterator
    lLatinTransliterator->transliterate (lQueryString);
    
    std::cout << std::endl << "Transliterated via "
              << lLatinTransliteratorID << ": ";
    std::cout << toUTF8String (lQueryString) << std::endl;

    removeQuotesAndPunctuation (lQueryString, *lNormaliser,
                                *lPunctuationTransliterator,
                                *lUnpunctuateTransliterator);

    // Transliterate with the Cyrillic-Latin transliterator
    /** (it seems to be the same as the Any-Latin transliteration)
    lCyrillicLatinTransliterator->transliterate (lQueryString4);
    
    std::cout << std::endl << "Transliterated via "
              << lCyrillicLatinTransliteratorID << ": ";
    std::cout << toUTF8String (lQueryString4) << std::endl;
      
    removeQuotesAndPunctuation (lQueryString4, *lNormaliser,
                                *lPunctuationTransliterator,
                                *lUnpunctuateTransliterator);
    */
    
    // Transliterate with the rule-based Russian-Latin/BGN transliterator
    lRussianLatinBGNTransliterator->transliterate (lQueryString2);
    
    std::cout << std::endl << "Transliterated via RBTRussianLatinBGN: ";
    std::cout << toUTF8String (lQueryString2) << std::endl;
      
    removeQuotesAndPunctuation (lQueryString2, *lNormaliser,
                                *lPunctuationTransliterator,
                                *lUnpunctuateTransliterator);

    // Transliterate with the rule-based Ukrainian-Latin/BGN transliterator
    lUkrainianLatinBGNTransliterator->transliterate (lQueryString3);

    std::cout << std::endl << "Transliterated via RBTUkrainianLatinBGN: ";
    std::cout << toUTF8String (lQueryString3) << std::endl;

    removeQuotesAndPunctuation (lQueryString3, *lNormaliser,
                                *lPunctuationTransliterator,
                                *lUnpunctuateTransliterator);
      
    
    std::cout << "Exiting successfully" << std::endl;
    
    return 0;
}
