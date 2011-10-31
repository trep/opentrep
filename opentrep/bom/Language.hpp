#ifndef __OPENTREP_BOM_LANGUAGE_HPP
#define __OPENTREP_BOM_LANGUAGE_HPP

// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// STL
#include <string>

namespace OPENTREP {

  /** Enumeration of languages. */
  struct Language {
  public:
    typedef enum { 
      en_US = 0,
      en_GB,
      fr_FR,
      pt_PT,
      pt_BR,
      de_DE,
      es_SP,
      ru_RU,
      LAST_VALUE
    } EN_Language;

    /** Get the long label (language and localisation) as a string. */
    static const std::string& getLongLabel (const EN_Language&);

    /** Get the short label (just the language code) as a string. */
    static const std::string& getShortLabel (const EN_Language&);

    /** Get the language code corresponding to the label (language). */
    static EN_Language getCode (const std::string&);
    
  private:
    /** Long string version (language and localisation) of the
        enumeration. */    
    static const std::string _longLabels[LAST_VALUE];

    /** Short string version (just the language code) of the enumeration. */    
    static const std::string _shortLabels[LAST_VALUE];
  };

}
#endif // __OPENTREP_BOM_LANGUAGE_HPP
