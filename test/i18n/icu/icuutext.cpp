// STL
#include <cassert>
#include <iostream>
// ICU
#include <unicode/utypes.h>
#include <unicode/uchar.h>
#include <unicode/locid.h>
#include <unicode/ustring.h>
#include <unicode/ucnv.h>
#include <unicode/unistr.h>
#include <unicode/utext.h>

// //////////////////// M A I N //////////////////////
int main (int argc, char* argv[]) {

  const char mucDECharString[] = "MÜNCHEN";
  const char mucRUCharString[] = "МЮНХЕН";

  // //////// UText ///////////
  UText uText = UTEXT_INITIALIZER;
  UErrorCode status = U_ZERO_ERROR;
  utext_openUTF8 (&uText, mucRUCharString, -1, &status);
  assert (U_SUCCESS(status));

  // ////////// Converter ////////////
  UConverter* cnv = ucnv_open ("UTF-8", &status);
  assert(U_SUCCESS(status));

  UChar mucDEUCharString[10];
  UChar mucRUUCharString[10];
  int32_t nbOfConvertedChars =
    ucnv_toUChars (cnv, mucDEUCharString, 10, mucDECharString, -1, &status);

  std::cout << "Converted " << nbOfConvertedChars
            << " chars from UTF-8 for German" << std::endl;

  nbOfConvertedChars =
    ucnv_toUChars (cnv, mucRUUCharString, 10, mucRUCharString, -1, &status);

  std::cout << "Converted " << nbOfConvertedChars
            << " chars from UTF-8 for Russian" << std::endl;
 
  if (U_FAILURE (status)) {
    std::cerr << "error " << u_errorName (status)
              << " setting the escape callback in the default converter"
              << std::endl;
  }

  UChar mucDELowerUCharString[15];
  UChar mucRULowerUCharString[15];
  nbOfConvertedChars =
    u_strToLower (mucDELowerUCharString, 15, mucDEUCharString, -1, "", &status);
  assert(U_SUCCESS(status));
  
  std::cout << "Converted to lower cases " << nbOfConvertedChars
            << " German chars" << std::endl;

  nbOfConvertedChars =
    u_strToLower (mucRULowerUCharString, 15, mucRUUCharString, -1, "", &status);
  assert(U_SUCCESS(status));
  
  std::cout << "Converted to lower cases " << nbOfConvertedChars
            << " Russian chars" << std::endl;
  
  char mucDELowerCharString[15];
  char mucRULowerCharString[15];
  nbOfConvertedChars =
    ucnv_fromUChars (cnv, mucDELowerCharString, 15, mucDELowerUCharString, -1,
                     &status);
  assert(U_SUCCESS(status));

  std::cout << "Converted " << nbOfConvertedChars
            << " chars to UTF-8 in German" << std::endl;
  
  nbOfConvertedChars =
    ucnv_fromUChars (cnv, mucRULowerCharString, 15, mucRULowerUCharString, -1,
                     &status);
  assert(U_SUCCESS(status));

  std::cout << "Converted " << nbOfConvertedChars
            << " chars to UTF-8 in Russian" << std::endl;

  std::cout << "German, converted from '" << mucDECharString << "': "
            << mucDELowerCharString << std::endl;
  std::cout << "Russian, converted from '" << mucRUCharString << "': "
            << mucRULowerCharString << std::endl;
  
  ucnv_close (cnv);
  
  return 0;
}
