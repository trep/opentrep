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

namespace OPENTREP {

  /**
   * Wrapper around a Unicode transliterator.
   */
  class OTransliterator {
  public:
    // //////////////// Business support methods ///////////////
    /**
     * Remove the punctuation of the given string.
     *
     * @param const std::string& The string for which the punctuation
     *                           must be removed
     * @return std::string The unpunctuated string.
     */
    std::string unpunctuate (const std::string& iString) const;

    /**
     * Remove the quote characters of the given string.
     *
     * @param const std::string& The string for which the quote characters
     *                           must be removed
     * @return std::string The unquoted string.
     */
    std::string unquote (const std::string& iString) const;

    /**
     * Remove the accents of the given string.
     *
     * Note that this transformation implies to apply a normalisation process
     * (separation of the characters from their accents), typically forth (NFD)
     * and back (NFC).
     * See for instance http://www.unicode.org/faq/normalization.html
     *
     * @param const std::string& The string for which the accents must be removed
     * @return std::string The unaccentuated string.
     */
    std::string unaccent (const std::string& iString) const;

    /**
     * Transliterate (e.g., from Chinese or Russian scripts),
     * into the Latin alphabet, the given string.
     *
     * @param const std::string& The string to be transliterated
     * @return std::string The transliterated string.
     */
    std::string transliterate (const std::string& iString) const;

    /**
     * Perform all the above operations (unaccent, unquote, unpunctuate,
     * transliterate) the given string.
     *
     * @param const std::string& The string to be normalised.
     * @return std::string The normalised string.
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
    // //////////////// Business support methods ///////////////
    /**
     * Remove the punctuation of the given string.
     *
     * @param UnicodeString& The string for which the punctuation
     *                       must be removed
     */
    void unpunctuate (icu::UnicodeString&) const;

    /**
     * Remove the quote characters of the given string.
     *
     * @param UnicodeString& The string for which the quote characters
     *                       must be removed
     */
    void unquote (icu::UnicodeString&) const;

    /**
     * Remove the accents of the given string.
     *
     * Note that this transformation implies to apply a normalisation
     * process (separation of the characters from their accents),
     * typically forth (NFD) and back (NFC).
     * See for instance http://www.unicode.org/faq/normalization.html
     *
     * @param UnicodeString& The string for which the accents must be removed
     */
    void unaccent (icu::UnicodeString&) const;

    /**
     * Transliterate (e.g., from Chinese or Russian scripts),
     * into the Latin alphabet, the given string.
     *
     * @param UnicodeString& The string to be transliterated
     */
    void transliterate (icu::UnicodeString&) const;
    
  private:
    /**
     * Create a "Unicode normaliser" which removes punctuation characters.
     *
     * The Unicode transformation rule is equal to "[:P:] Remove;".
     *
     * The underlying specific Transliterator object is instantiated.
     */
    void initPunctuationRemover();

    /**
     * Create a "Unicode normaliser" which removes quotation characters.
     *
     * The Unicode transformation rule is equal to something like
     * "[\\u02B9] > \\u0027; [\\u002D] > \\u0020;".
     *
     * The underlying specific Transliterator object is instantiated.
     */
    void initQuoteRemover();

    /**
     * Create a "Unicode normaliser" which removes accentuated characters.
     *
     * The Unicode transformation rule is equal to "NFD; [:M:] Remove; NFC;".
     *
     * The underlying specific Transliterator object is instantiated.
     */
    void initAccentRemover();

    /**
     * Create a "Unicode normaliser" which operates transliteration.
     *
     * The Unicode transformation rule is equal to something like
     * "Any-Latin; NFD; [:M:] Remove; NFC; Lower;".
     *
     * The underlying specific Transliterator object is instantiated.
     */
    void initTranlisterator();

    /**
     * Perform all the above initialisation operations.
     */
    void init();

    /**
     * Close the resources for the "Unicode normalisation".
     */
    void finalise();


  private:
    // /////////////////////// Attributes //////////////////////
    /**
     * Pointer on the Unicode Transliterator for the removal of punctuation.
     */
    icu::Transliterator* _punctuationRemover;

    /**
     * Pointer on the Unicode Transliterator for the removal of quotation.
     */
    icu::Transliterator* _quoteRemover;

    /**
     * Pointer on the Unicode Transliterator for the removal of accents.
     */
    icu::Transliterator* _accentRemover;

    /**
     * Pointer on the Unicode Transliterator for the transliteration of any
     * language script (e.g., Arabic, Cyrillic, Greek, Han, Hangul, Hebrew,
     * Katakana, Thai) to Latin characters.
     */
    icu::Transliterator* _tranlist;
  };

}
#endif // __OPENTREP_BAS_OTRANSLITERATOR_HPP
