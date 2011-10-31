// C
#include <iostream>
#include <locale>
#include <string>
#include <cstdlib>


// ////////////// M A I N //////////////
int main (int argc, char* argv[]) {

  // Create the default locale from the user's environment
  // std::locale langLocale ("ru_RU.UTF-8");
  std::locale langLocale ("");

  // and assign it to the standard output channel
  //std::cout.imbue (langLocale);

  // and assign it to the standard input channel
  //std::cin.imbue (langLocale);
  
  // Display with no processing
  std::cout << "Simple character literals without processing:" << std::endl;
  std::cout << "de: München" << std::endl;
  std::cout << "ru: Мюнхен" << std::endl;

  // With STL strings
  std::string mucDESTLString ("München");
  std::string mucRUSTLString ("Мюнхен");

  // Display the STL strings
  std::cout << "STL strings without processing:" << std::endl;
  std::cout << "de: " << mucDESTLString << std::endl;
  std::cout << "ru: " << mucRUSTLString << std::endl;

  // Single char strings
  char mucDECharString[] = "München";
  char mucRUCharString[] = "Мюнхен";

  // Display the altered single char strings
  std::cout << "Single character literals without processing:" << std::endl;
  std::cout << "de: " << mucDECharString << std::endl;
  std::cout << "ru: " << mucRUCharString << std::endl;
  
  // Wide char strings
  wchar_t mucDEWCharString[7];
  wchar_t mucRUWCharString[6];
  

  // Conversion from char* to wchar_t thanks to the STL locale
  std::use_facet<std::ctype<wchar_t> > (langLocale).widen (mucDECharString,
                                                           mucDECharString+7,
                                                           mucDEWCharString);
  std::use_facet<std::ctype<wchar_t> > (langLocale).widen (mucRUCharString,
                                                           mucRUCharString+6,
                                                           mucRUWCharString);
  
  // Display the wide char strings
  std::cout << "Wide character literals after widening:" << std::endl;
  std::cout << "de: " << mucDEWCharString << std::endl;
  std::cout << "ru: " << mucRUWCharString << std::endl;
  
  // STL ctypes on wchar_t
  std::use_facet<std::ctype<wchar_t> > (langLocale).toupper(mucDEWCharString,
                                                            mucDEWCharString+7);
  std::use_facet<std::ctype<wchar_t> > (langLocale).toupper(mucRUWCharString,
                                                            mucRUWCharString+6);
  
  // Conversion from wchar_t to char thanks to the STL locale
  std::use_facet<std::ctype<wchar_t> > (langLocale).narrow (mucDEWCharString,
                                                            mucDEWCharString+7,
                                                            ' ',
                                                            mucDECharString);
  std::use_facet<std::ctype<wchar_t> > (langLocale).narrow (mucRUWCharString,
                                                            mucRUWCharString+6,
                                                            ' ',
                                                            mucRUCharString);
  
  // Display the altered single char strings
  std::cout << "Character literals with STL locale processing:" << std::endl;
  std::cout << "de: " << mucDECharString << std::endl;
  std::cout << "ru: " << mucRUCharString << std::endl;

  return 0;
}
