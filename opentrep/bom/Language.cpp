// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// C
#include <cassert>
// OpenTrep
#include <opentrep/bom/Language.hpp>

namespace OPENTREP {

  // //////////////////////////////////////////////////////////////////////
  const std::string Language::_longLabels[LAST_VALUE] = {
    "ar", "ar_DZ", "ar_MA", "ar_SA", "ar_TN",
    "de", "de_DE", "en", "en_GB", "en_US", "es", "es_AR", "es_ES", "es_MX",
    "fa", "fa_IR",
    "fr", "fr_BE", "fr_CA", "fr_CH", "fr_FR",
    "he", "he_IL", "ko", "ko_KR", "pl", "pl_PL", "pt", "pt_BR", "pt_PT",
    "sv", "sv_SE",
    "ru", "ru_RU", "uk", "uk_UA", "zh", "zh_CN", "zh_HK", "zh_TW"};

  // //////////////////////////////////////////////////////////////////////
  const std::string Language::_shortLabels[LAST_VALUE] = {
    "ar", "ar", "ar", "ar", "ar",
    "de", "de", "en", "en", "en", "es", "es", "es", "es",
    "fa", "fa",
    "fr", "fr", "fr", "fr", "fr",
    "he", "he", "ko", "ko", "pl", "pl", "pt", "pt", "pt",
    "sv", "sv",
    "ru", "ru", "uk", "uk", "zh", "zh", "zh", "zh"};

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
