// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// C
#include <assert.h>
// OpenTrep
#include <opentrep/bom/Language.hpp>

namespace OPENTREP {

  // //////////////////////////////////////////////////////////////////////
  const std::string Language::_longLabels[LAST_VALUE] = {
        "en_US", "en_GB", "fr_FR", "pt_PT", "pt_BR", "de_DE", "es_SP", "ru_RU"};

  // //////////////////////////////////////////////////////////////////////
  const std::string Language::_shortLabels[LAST_VALUE] = {
        "en", "en", "fr", "pt", "pt", "de", "es", "ru"};

  // //////////////////////////////////////////////////////////////////////
  const std::string& Language::
  getLongLabel (const Language::EN_Language& iLanguage) {
    return Language::_longLabels[iLanguage];
  }

  // //////////////////////////////////////////////////////////////////////
  const std::string& Language::
  getShortLabel (const Language::EN_Language& iLanguage) {
    return Language::_shortLabels[iLanguage];
  }

  // //////////////////////////////////////////////////////////////////////
  Language::EN_Language Language::getCode (const std::string& iLanguageString) {
    EN_Language oLanguageCode = en_US;
    unsigned int itLanguageCode = 0;
    for ( ; itLanguageCode != LAST_VALUE; ++itLanguageCode) {
      const EN_Language lLanguageCode =
        static_cast<EN_Language> (itLanguageCode);
      if (getShortLabel (lLanguageCode) == iLanguageString) {
        oLanguageCode = lLanguageCode;
        break;
      }
    }

    return oLanguageCode;
  }
  
}
