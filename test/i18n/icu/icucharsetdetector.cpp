// STL
#include <iostream>
#include <string>
// ICU
#include <unicode/utypes.h>
#include <unicode/ucsdet.h>

int main (int argc, char* argv[]) {
  
  UErrorCode status = U_ZERO_ERROR;
  UCharsetDetector* csd = ucsdet_open (&status);
  static char buffer[11] = "0123456789";
  int32_t inputLength = 10;
  ucsdet_setText (csd, buffer, inputLength, &status);
  const UCharsetMatch* ucm = ucsdet_detect (csd, &status);
  const std::string name = ucsdet_getName (ucm, &status);

  std::cout << "Character set encoding: " << name << std::endl;
  
  return 0;
}
