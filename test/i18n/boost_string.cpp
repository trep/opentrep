// C
#include <iostream>
// Boost String
#include <boost/algorithm/string.hpp>

// ////////////// M A I N //////////////
int main (int argc, char* argv[]) {

  std::string str1(" hello world! ");
  std::cout << "Before: " << str1 << std::endl;

  boost::to_upper (str1);
  std::cout << "After: " << str1 << std::endl;

  // With STL locale feature
  std::string mucDE ("München");
  std::string mucRU ("Мюнхен");
  std::string pacaFR ("Provence Alpes Côtes d'Azur");

  // Test the Boost String conversions
  boost::to_upper (mucDE);
  boost::to_upper (mucRU);
  boost::to_upper (pacaFR);
  
  // Display the strings
  std::cout << "de: " << mucDE << std::endl;
  std::cout << "ru: " << mucRU << std::endl;
  std::cout << "fr: " << pacaFR << std::endl;

  return 0;
}
