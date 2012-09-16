#ifndef __OPENTREP_LANGUAGE_HPP
#define __OPENTREP_LANGUAGE_HPP

// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// STL
#include <string>

namespace OPENTREP {

  /**
   * Enumeration of languages.
   *
   * On Unix, see also the /usr/share/locale/ directory.
   * The iso-codes (RPM, DEB) package provides all the ISO-related code information.
   */
  struct Language {
  public:
    /**
     * The standard language form, i.e., the one without the country code,
     * must be placed at the beginning of the list of the corresponding language
     * variations. For instance, "en" must be placed before "en_GB" or "en_US".
     */
    typedef enum { 
      lg_com = 0,
      ar, ar_DZ, ar_MA, ar_SA, ar_TN,
      de, de_DE,
      en, en_GB, en_US,
      es, es_AR, es_ES, es_MX,
      fa, fa_IR,
      fr, fr_BE, fr_CA, fr_CH, fr_FR,
      he, he_IL,
      ko, ko_KR,
      pl, pl_PL,
      pt, pt_BR, pt_PT,
      sv, sv_SE,
      ru, ru_RU,
      uk, uk_UA,
      zh, zh_CN, zh_HK, zh_TW,
      LAST_VALUE
    } EN_Language;

    /**
     * Get the long label (language and localisation) as a string.
     */
    static const std::string& getLongLabel (const EN_Language&);

    /**
     * Get the short label (just the language code) as a string.
     */
    static const std::string& getShortLabel (const EN_Language&);

    /**
     * Get the language code corresponding to the label (language).
     */
    static EN_Language getCode (const std::string&);
    
  private:
    /**
     * Long string version (language and localisation) of the
     * enumeration.
     */
    static const std::string _longLabels[LAST_VALUE+1];

    /**
     * Short string version (just the language code) of the enumeration.
     */
    static const std::string _shortLabels[LAST_VALUE+1];
  };

}
#endif // __OPENTREP_LANGUAGE_HPP
