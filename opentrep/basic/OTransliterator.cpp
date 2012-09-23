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
    : _normaliser (NULL), _unquoter (NULL), _unpunctuater (NULL),
      _tranlist (NULL) {
    init();
  }

  // //////////////////////////////////////////////////////////////////////
  OTransliterator::OTransliterator (const OTransliterator& iTransliterator)
    : _normaliser (NULL), _unquoter (NULL), _unpunctuater (NULL),
      _tranlist (NULL) {
    assert (iTransliterator._normaliser != NULL);
    _normaliser = iTransliterator._normaliser->clone();

    assert (iTransliterator._unquoter != NULL);
    _unquoter = iTransliterator._unquoter->clone();

    assert (iTransliterator._unpunctuater != NULL);
    _unpunctuater = iTransliterator._unpunctuater->clone();

    assert (iTransliterator._tranlist != NULL);
    _tranlist = iTransliterator._tranlist->clone();

  }

  // //////////////////////////////////////////////////////////////////////
  OTransliterator::~OTransliterator() {
    finalise();
  }

  // //////////////////////////////////////////////////////////////////////
  void OTransliterator::init() {
    // Create a normaliser
    UErrorCode lStatus = U_ZERO_ERROR;

    _normaliser =
      Transliterator::createInstance (K_ICU_ACCENT_REMOVAL_RULE, UTRANS_FORWARD,
                                      lStatus);

    if (_normaliser == NULL || U_FAILURE (lStatus)) {
      std::ostringstream oStr;
      oStr << "Unicode error: no Transliterator can be created for the '"
           << K_ICU_ACCENT_REMOVAL_RULE << "' rule.";
      OPENTREP_LOG_ERROR (oStr.str());
      throw UnicodeTransliteratorCreationException (oStr.str());
    }
    assert (_normaliser != NULL);
               
    // Register the Unicode Transliterator
    Transliterator::registerInstance (_normaliser);

    // Create a remover of quotation
    lStatus = U_ZERO_ERROR;
    UParseError pError;
    UnicodeString lUnquotedRules (K_ICU_QUOTATION_REMOVAL_RULE);
    _unquoter = Transliterator::createFromRules("RBTUnaccent", lUnquotedRules,
                                                UTRANS_FORWARD, pError, lStatus);

    if (_unquoter == NULL || U_FAILURE (lStatus)) {
      std::ostringstream oStr;
      oStr << "Unicode error: no Transliterator can be created for the '"
           << K_ICU_QUOTATION_REMOVAL_RULE << "' rule.";
      OPENTREP_LOG_ERROR (oStr.str());
      throw UnicodeTransliteratorCreationException (oStr.str());
    }
    assert (_unquoter != NULL);
               
    // Register the Unicode Transliterator
    Transliterator::registerInstance (_unquoter);

    // Create a remover of punctuation
    lStatus = U_ZERO_ERROR;

    _unpunctuater =
      Transliterator::createInstance (K_ICU_PUNCTUATION_REMOVAL_RULE,
                                      UTRANS_FORWARD, lStatus);

    if (_unpunctuater == NULL || U_FAILURE (lStatus)) {
      std::ostringstream oStr;
      oStr << "Unicode error: no Transliterator can be created for the '"
           << K_ICU_PUNCTUATION_REMOVAL_RULE << "' rule.";
      OPENTREP_LOG_ERROR (oStr.str());
      throw UnicodeTransliteratorCreationException (oStr.str());
    }
    assert (_unpunctuater != NULL);
               
    // Register the Unicode Transliterator
    Transliterator::registerInstance (_unpunctuater);

    // Create a generic transliterator
    lStatus = U_ZERO_ERROR;

    _tranlist =
      Transliterator::createInstance (K_ICU_GENERIC_TRANSLITERATOR_RULE,
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
    Transliterator::registerInstance (_tranlist);
  }

  // //////////////////////////////////////////////////////////////////////
  void OTransliterator::finalise() {
    delete _normaliser; _normaliser = NULL;
    delete _unquoter; _unquoter = NULL;
    delete _unpunctuater; _unpunctuater = NULL;
    delete _tranlist; _tranlist = NULL;
  }

  // //////////////////////////////////////////////////////////////////////
  std::string OTransliterator::normalise (const std::string& iString) const {
    assert (_normaliser != NULL);

    // Build a UnicodeString from the STL string
    UnicodeString lString (iString.c_str());

    // Apply the whole sery of transformators
    _normaliser->transliterate (lString);
    _unquoter->transliterate (lString);
    _unpunctuater->transliterate (lString);
    _tranlist->transliterate (lString);

    // Convert from UnicodeString to UTF8-encoded STL string
    const std::string& lNormalisedString = getUTF8 (lString);

    return lNormalisedString;
  }

}
