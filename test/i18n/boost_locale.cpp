// STL
#include <iostream>
// Boost String
#include <boost/locale.hpp>

// ////////////// M A I N //////////////
int main (int argc, char* argv[]) {

  // Get the global localisation backend
  boost::locale::localization_backend_manager lLocBEMgr =
    boost::locale::localization_backend_manager::global(); 

  // Select ICU backend as default
  lLocBEMgr.select ("icu");

  // Set this backend globally
  boost::locale::localization_backend_manager::global (lLocBEMgr);

  // Create a generator that uses this backend.
  boost::locale::generator locGen (lLocBEMgr);

  // Create locale generator with the system default locale
  std::locale::global (locGen ("")); 

  // With STL locale feature
  std::string mucDE ("München");
  std::string mucRU ("Мюнхен");
  std::string pacaFR ("Provence Alpes Côtes d'Azur");
  std::string pvgZH ("上海浦东国际机场");

  // Test the Boost Locale string conversions
  std::cout << "Original: " << mucDE << std::endl
            <<"Upper " << boost::locale::to_upper (mucDE) << std::endl
            <<"Lower " << boost::locale::to_lower (mucDE) << std::endl
            <<"Title " << boost::locale::to_title (mucDE) << std::endl
            <<"Fold  " << boost::locale::fold_case (mucDE) << std::endl
            << "Normalise: "
            << boost::locale::normalize (mucDE, boost::locale::norm_nfkc)
            << std::endl;

  std::cout << "Original: " << mucRU << std::endl
            <<"Upper " << boost::locale::to_upper (mucRU) << std::endl
            <<"Lower " << boost::locale::to_lower (mucRU) << std::endl
            <<"Title " << boost::locale::to_title (mucRU) << std::endl
            <<"Fold  " << boost::locale::fold_case (mucRU) << std::endl
            << "Normalise: "
            << boost::locale::normalize (mucRU, boost::locale::norm_nfkc)
            << std::endl;

  std::cout << "Original: " << pacaFR << std::endl
            <<"Upper " << boost::locale::to_upper (pacaFR) << std::endl
            <<"Lower " << boost::locale::to_lower (pacaFR) << std::endl
            <<"Title " << boost::locale::to_title (pacaFR) << std::endl
            <<"Fold  " << boost::locale::fold_case (pacaFR) << std::endl
            << "Normalise: "
            << boost::locale::normalize (pacaFR, boost::locale::norm_nfkc)
            << std::endl;
  
  std::cout << "Original: " << pvgZH << std::endl
            <<"Upper " << boost::locale::to_upper (pvgZH) << std::endl
            <<"Lower " << boost::locale::to_lower (pvgZH) << std::endl
            <<"Title " << boost::locale::to_title (pvgZH) << std::endl
            <<"Fold  " << boost::locale::fold_case (pvgZH) << std::endl
            << "Normalise: "
            << boost::locale::normalize (pvgZH, boost::locale::norm_nfkc)
            << std::endl;
  
  return 0;
}
