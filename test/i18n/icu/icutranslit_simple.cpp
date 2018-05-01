// STL
#include <cassert>
#include <iostream>
#include <string>
// ICU
#include <unicode/translit.h>
#include <unicode/unistr.h>
#include <unicode/ucnv.h>
// Local
#include "icutranslit_util.hpp"

// ///////////// M A I N ///////////////
int main() {

	// UTF8 string
	icu::UnicodeString lString ("À côté de chez vous");
	icu::UnicodeString lNormalisedString (lString);

	// Create a Normalizer
	UErrorCode status = U_ZERO_ERROR;
	const char* lNormaliserID = "NFD; [:M:] Remove; NFC;";
	icu::Transliterator* lNormaliser =
		icu::Transliterator::createInstance (lNormaliserID, UTRANS_FORWARD,
                                             status);

	if (lNormaliser == NULL || U_FAILURE (status)) {
		std::cerr << "ERROR: icu::Transliterator::createInstance() failed for "
                  << lNormaliserID << std::endl;
		return 0;
	}
	assert (lNormaliser != NULL);
               
	// Register the Transliterator
	icu::Transliterator::registerInstance (lNormaliser);

	// Normalise the string
	lNormaliser->transliterate (lNormalisedString);

	std::cout << "Original version (UTF8): '"
			<< toUTF8String (lString)
			 << "'; normalized version without accents: '"
			<< toUTF8String (lNormalisedString) << "'" << std::endl;
                                           
	return 0;
}

