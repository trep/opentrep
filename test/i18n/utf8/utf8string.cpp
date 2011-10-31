// STL
#include <iostream>
// OpenTrep
#include <opentrep/basic/UTF8Handler.hpp>

// //////////////////////// M A I N /////////////////////////
int main (int argc, char* argv[]) {

  // STL strings
  std::string mucDESTLString ("München");
  std::string mucRUSTLString ("Мюнхен");

  std::cout << "--------" << std::endl
            << "STL strings without processing" << std::endl;
  std::cout << "Deutsch: '" << mucDESTLString << "'" << std::endl;
  std::cout << "Russian: '" << mucRUSTLString << "'" << std::endl;
  
  //
  std::wstring mucDESTLWString =
    OPENTREP::UTF8Handler::toWideString (mucDESTLString);
  std::wstring mucRUSTLWString =
    OPENTREP::UTF8Handler::toWideString (mucRUSTLString);
  
  std::cout << "--------" << std::endl
            << "UTF-8 decoded wide char strings" << std::endl;
  std::cout << "Deutsch: " << std::endl;
  // std::cout << "Deutsch: '" << mucDESTLWString << "'" << std::endl;
  std::cout << OPENTREP::UTF8Handler::displaySTLWString (mucDESTLWString);
  
  std::cout << "Russian: " << std::endl;
  // std::cout << "Russian: '" << mucRUSTLWString << "'" << std::endl;
  std::cout << OPENTREP::UTF8Handler::displaySTLWString (mucRUSTLWString);

  mucDESTLString = OPENTREP::UTF8Handler::toSimpleString (mucDESTLWString);
  mucRUSTLString = OPENTREP::UTF8Handler::toSimpleString (mucRUSTLWString);
  
  std::cout << "--------" << std::endl
            << "STL strings after processing" << std::endl;
  std::cout << "Deutsch: '" << mucDESTLString << "'" << std::endl;
  std::cout << "Russian: '" << mucRUSTLString << "'" << std::endl;
  
  return 0;
}
