// C
#include <iostream>
#include <locale>
#include <string>
#include <cstdlib>


// ////////////// M A I N //////////////
int main (int argc, char* argv[]) {

  // Create the default locale from the user's environment
  std::locale langLocale ("");

  // and assign it to the standard output channel
  std::cout.imbue (langLocale);

  //
  std::cout << "User's environment locale: " << langLocale.name() << std::endl;
  
  bool isFrench = false;
  if (langLocale.name() == "fr_FR"
      || langLocale.name() == "fr"
      || langLocale.name() == "french") {
    isFrench = true;
  }

  // Read locale for the input
  if (isFrench) {
    std::cout << "Locale pour l'entrée: " << std::endl;

  } else {
    std::cout << "Locale for input: " << std::endl;
  }

  std::string tmpString;
  std::cin >> tmpString;

  if (!std::cin) {
    if (isFrench) {
      std::cerr << "Error while reading the locale" << std::endl;

    } else {
      std::cerr << "Error while reading the locale" << std::endl;
    }
    return EXIT_FAILURE;
  }
  
  std::locale cinLocale (tmpString.c_str());

  // and assign it to the standard input channel
  std::cin.imbue (cinLocale);

  // Read and output floating-point values in a loop
  double value;
  while (std::cin >> value) {
    std::cout << value << std::endl;
  }

  return 0;
}
