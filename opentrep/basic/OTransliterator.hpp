#ifndef __OPENTREP_BAS_OTRANSLITERATOR_HPP
#define __OPENTREP_BAS_OTRANSLITERATOR_HPP

// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// STL
#include <string>
// ICU
#include <unicode/unistr.h> // UnicodeString
#include <unicode/translit.h> // Transliterator
// OpenTrep
#include <opentrep/basic/StructAbstract.hpp>

namespace OPENTREP {

  /**
   * Wrapper around a Unicode transliterator.
   */
  class OTransliterator {
  public:
    // //////////////// Business support methods ///////////////
    /**
     * Remove the accents of the given string.
     *
     * @param const std::string& The string for which the strings must be removed
     * @return std::string The unaccentuated string.
     */
    std::string normalise (const std::string& iString) const;


  public:
    // //////////////// Construction and destruction ///////////////
    /**
     * Default Constructor.
     */
    OTransliterator();

    /**
     * Copy Constructor.
     */
    OTransliterator (const OTransliterator&);

    /**
     * Destructor.
     */
    ~OTransliterator();


  private:
    /**
     * Create a "Unicode normaliser" which removes accentuated characters.
     *
     * The Unicode transformation rule is equal to "NFD; [:M:] Remove; NFC;".
     *
     * @return Transliterator* The just created "Unicode normaliser".
     */
    void init();

    /**
     * Close the resources for the "Unicode normalisation".
     */
    void finalise();


  private:
    // /////////////////////// Attributes //////////////////////
    /**
     * Pointer on the Unicode Transliterator for the standard normalisation.
     */
    Transliterator* _normaliser;

    /**
     * Pointer on the Unicode Transliterator for the removal of quotation.
     */
    Transliterator* _unquoter;

    /**
     * Pointer on the Unicode Transliterator for the removal of punctuation.
     */
    Transliterator* _unpunctuater;

    /**
     * Pointer on the Unicode Transliterator for the transliteration of any
     * language script (e.g., Arabic, Cyrillic, Greek, Han, Hangul, Hebrew,
     * Katakana, Thai) to Latin characters.
     */
    Transliterator* _tranlist;
  };

}
#endif // __OPENTREP_BAS_OTRANSLITERATOR_HPP
