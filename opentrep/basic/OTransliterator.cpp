// //////////////////////////////////////////////////////////////////////
// Import section
// //////////////////////////////////////////////////////////////////////
// STL
#include <cassert>
#include <sstream>
// OpenTrep
#include <opentrep/OPENTREP_exceptions.hpp>
#include <opentrep/basic/icu_util.hpp>
#include <opentrep/basic/BasConst_Unicode.hpp>
#include <opentrep/basic/OTransliterator.hpp>
#include <opentrep/service/Logger.hpp>

namespace OPENTREP {

  // //////////////////////////////////////////////////////////////////////
  OTransliterator::OTransliterator()
    : _punctuationRemover (NULL), _quoteRemover (NULL), _accentRemover (NULL),
      _tranlist (NULL) {
    init();
  }

  // //////////////////////////////////////////////////////////////////////
  OTransliterator::OTransliterator (const OTransliterator& iTransliterator)
    : _punctuationRemover (NULL), _quoteRemover (NULL), _accentRemover (NULL),
      _tranlist (NULL) {
    assert (iTransliterator._punctuationRemover != NULL);
    _punctuationRemover = iTransliterator._punctuationRemover->clone();

    assert (iTransliterator._quoteRemover != NULL);
    _quoteRemover = iTransliterator._quoteRemover->clone();

    assert (iTransliterator._accentRemover != NULL);
    _accentRemover = iTransliterator._accentRemover->clone();

    assert (iTransliterator._tranlist != NULL);
    _tranlist = iTransliterator._tranlist->clone();

  }

  // //////////////////////////////////////////////////////////////////////
  OTransliterator::~OTransliterator() {
    finalise();
  }

  // //////////////////////////////////////////////////////////////////////
  void OTransliterator::initPunctuationRemover() {
    // Create a remover of punctuation
    UErrorCode lStatus = U_ZERO_ERROR;
    _punctuationRemover =
      icu::Transliterator::createInstance (K_ICU_PUNCTUATION_REMOVAL_RULE,
                                      UTRANS_FORWARD, lStatus);

    if (_punctuationRemover == NULL || U_FAILURE (lStatus)) {
      std::ostringstream oStr;
      oStr << "Unicode error: no Transliterator can be created for the '"
           << K_ICU_PUNCTUATION_REMOVAL_RULE << "' rule.";
      OPENTREP_LOG_ERROR (oStr.str());
      throw UnicodeTransliteratorCreationException (oStr.str());
    }
    assert (_punctuationRemover != NULL);
               
    // Register the Unicode Transliterator
    icu::Transliterator::registerInstance (_punctuationRemover);
  }

  // //////////////////////////////////////////////////////////////////////
  void OTransliterator::initQuoteRemover() {
    // Create a remover of quotation
    UErrorCode lStatus = U_ZERO_ERROR;
    UParseError pError;
    icu::UnicodeString lUnquotedRules (K_ICU_QUOTATION_REMOVAL_RULE);
    _quoteRemover =
      icu::Transliterator::createFromRules ("RBTUnaccent", lUnquotedRules,
                                       UTRANS_FORWARD, pError, lStatus);

    if (_quoteRemover == NULL || U_FAILURE (lStatus)) {
      std::ostringstream oStr;
      oStr << "Unicode error: no Transliterator can be created for the '"
           << K_ICU_QUOTATION_REMOVAL_RULE << "' rule.";
      OPENTREP_LOG_ERROR (oStr.str());
      throw UnicodeTransliteratorCreationException (oStr.str());
    }
    assert (_quoteRemover != NULL);
               
    // Register the Unicode Transliterator
    icu::Transliterator::registerInstance (_quoteRemover);
  }

  // //////////////////////////////////////////////////////////////////////
  void OTransliterator::initAccentRemover() {
    // Create a remover of accents
    UErrorCode lStatus = U_ZERO_ERROR;
    _accentRemover =
      icu::Transliterator::createInstance (K_ICU_ACCENT_REMOVAL_RULE, UTRANS_FORWARD,
                                      lStatus);

    if (_accentRemover == NULL || U_FAILURE (lStatus)) {
      std::ostringstream oStr;
      oStr << "Unicode error: no Transliterator can be created for the '"
           << K_ICU_ACCENT_REMOVAL_RULE << "' rule.";
      OPENTREP_LOG_ERROR (oStr.str());
      throw UnicodeTransliteratorCreationException (oStr.str());
    }
    assert (_accentRemover != NULL);
               
    // Register the Unicode Transliterator
    icu::Transliterator::registerInstance (_accentRemover);
  }

  // //////////////////////////////////////////////////////////////////////
  void OTransliterator::initTranlisterator() {
    // Create a generic transliterator
    UErrorCode lStatus = U_ZERO_ERROR;
    _tranlist =
      icu::Transliterator::createInstance (K_ICU_GENERIC_TRANSLITERATOR_RULE,
                                      UTRANS_FORWARD, lStatus);

    if (_tranlist == NULL || U_FAILURE (lStatus)) {
      std::ostringstream oStr;
      oStr << "Unicode error: no Transliterator can be created for the '"
           << K_ICU_GENERIC_TRANSLITERATOR_RULE << "' rule.";
      OPENTREP_LOG_ERROR (oStr.str());
      throw UnicodeTransliteratorCreationException (oStr.str());
    }
    assert (_tranlist != NULL);
               
    // Register the Unicode Transliterator
    icu::Transliterator::registerInstance (_tranlist);
  }

  // //////////////////////////////////////////////////////////////////////
  void OTransliterator::init() {
    initPunctuationRemover();
    initQuoteRemover();
    initAccentRemover();
    initTranlisterator();
  }

  // //////////////////////////////////////////////////////////////////////
  void OTransliterator::finalise() {
    delete _punctuationRemover; _punctuationRemover = NULL;
    delete _quoteRemover; _quoteRemover = NULL;
    delete _accentRemover; _accentRemover = NULL;
    delete _tranlist; _tranlist = NULL;
  }

  // //////////////////////////////////////////////////////////////////////
  void OTransliterator::unpunctuate (icu::UnicodeString& ioString) const {
    // Apply the punctuation removal scheme
    assert (_punctuationRemover != NULL);
    _punctuationRemover->transliterate (ioString);
  }

  // //////////////////////////////////////////////////////////////////////
  std::string OTransliterator::unpunctuate (const std::string& iString) const {
    // Build a UnicodeString from the STL string
    icu::UnicodeString lString (iString.c_str());

    // Apply the punctuation removal scheme
    unpunctuate (lString);

    // Convert back from UnicodeString to UTF8-encoded STL string
    const std::string& lPunctuatedString = getUTF8 (lString);

    return lPunctuatedString;
  }

  // //////////////////////////////////////////////////////////////////////
  void OTransliterator::unquote (icu::UnicodeString& ioString) const {
    // Apply the quotation removal scheme
    assert (_quoteRemover != NULL);
    _quoteRemover->transliterate (ioString);
  }

  // //////////////////////////////////////////////////////////////////////
  std::string OTransliterator::unquote (const std::string& iString) const {
    // Build a UnicodeString from the STL string
    icu::UnicodeString lString (iString.c_str());

    // Apply the quotation removal scheme
    unquote (lString);

    // Convert back from UnicodeString to UTF8-encoded STL string
    const std::string& lUnquotedString = getUTF8 (lString);

    return lUnquotedString;
  }

  // //////////////////////////////////////////////////////////////////////
  void OTransliterator::unaccent (icu::UnicodeString& ioString) const {
    // Apply the accent removal scheme
    assert (_accentRemover != NULL);
    _accentRemover->transliterate (ioString);
  }

  // //////////////////////////////////////////////////////////////////////
  std::string OTransliterator::unaccent (const std::string& iString) const {
    // Build a UnicodeString from the STL string
    icu::UnicodeString lString (iString.c_str());

    // Apply the accent removal scheme
    unaccent (lString);

    // Convert back from UnicodeString to UTF8-encoded STL string
    const std::string& lUnaccentuatedString = getUTF8 (lString);

    return lUnaccentuatedString;
  }

  // //////////////////////////////////////////////////////////////////////
  void OTransliterator::transliterate (icu::UnicodeString& ioString) const {
    // Apply the transliteration scheme
    assert (_tranlist != NULL);
    _tranlist->transliterate (ioString);
  }

  // //////////////////////////////////////////////////////////////////////
  std::string OTransliterator::transliterate (const std::string& iString) const {
    // Build a UnicodeString from the STL string
    icu::UnicodeString lString (iString.c_str());

    // Apply the transliteration scheme
    transliterate (lString);

    // Convert back from UnicodeString to UTF8-encoded STL string
    const std::string& lTransliteratedString = getUTF8 (lString);

    return lTransliteratedString;
  }

  // //////////////////////////////////////////////////////////////////////
  std::string OTransliterator::normalise (const std::string& iString) const {
    // Build a UnicodeString from the STL string
    icu::UnicodeString lString (iString.c_str());

    // Apply the whole sery of transformators
    unaccent (lString);
    unquote (lString);
    unpunctuate (lString);
    transliterate (lString);

    // Convert back from UnicodeString to UTF8-encoded STL string
    const std::string& lNormalisedString = getUTF8 (lString);

    return lNormalisedString;
  }

}
