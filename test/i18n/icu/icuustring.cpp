// STL
#include <cstdio>
#include <iostream>
// ICU
#include <unicode/utypes.h>
#include <unicode/uchar.h>
#include <unicode/locid.h>
#include <unicode/ustring.h>
#include <unicode/ucnv.h>
#include <unicode/unistr.h>

#define LENGTHOF(array) (sizeof(array)/sizeof((array)[0]))

// helper functions -------------------------------------------------------- ***

// default converter for the platform encoding
static UConverter* cnv = NULL;

static void
printUnicodeString(const char *announce, const UnicodeString &s) {
    static char out[200];
    int32_t i, length;

    // output the string, converted to the platform encoding

    // Note for Windows: The "platform encoding" defaults to the "ANSI codepage",
    // which is different from the "OEM codepage" in the console window.
    // However, if you pipe the output into a file and look at it with Notepad
    // or similar, then "ANSI" characters will show correctly.
    // Production code should be aware of what encoding is required,
    // and use a UConverter or at least a charset name explicitly.
    out[s.extract(0, 99, out)]=0;
    printf("%s%s {", announce, out);

    // output the code units (not code points)
    length=s.length();
    for(i=0; i<length; ++i) {
        printf(" %04x", s.charAt(i));
    }
    printf(" }\n");
}

static void demoCaseMapInCPlusPlus() {
    /*
     * input=
     *   "<Cyrillic Capital Letter BE>"
     *   "<Cyrillic Capital Letter GHE>"
     */
    static const UChar input[]={
      0x411, 0x413, 0
    };

    std::cout << std::endl << "* demoCaseMapInCPlusPlus() --------- ***"
              << std::endl << std::endl;

    UnicodeString s(input), t;
    const Locale& en = Locale::getEnglish();
    Locale ru ("ru");

    /*
     * Full case mappings as in demoCaseMapInC(), using UnicodeString functions.
     * These functions modify the string object itself.
     * Since we want to keep the input string around, we copy it each time
     * and case-map the copy.
     */
    printUnicodeString("input string: ", s);

    /* lowercase/English */
    printUnicodeString("full-lowercased/en: ", (t=s).toLower(en));
    /* lowercase/Russian */
    printUnicodeString("full-lowercased/ru: ", (t=s).toLower(ru));
    /* uppercase/English */
    printUnicodeString("full-uppercased/en: ", (t=s).toUpper(en));
    /* uppercase/Russian */
    printUnicodeString("full-uppercased/ru: ", (t=s).toUpper(ru));
    /* titlecase/English */
    printUnicodeString("full-titlecased/en: ", (t=s).toTitle(NULL, en));
    /* titlecase/Russian */
    printUnicodeString("full-titlecased/ru: ", (t=s).toTitle(NULL, ru));
    /* case-folde/default */
    printUnicodeString("full-case-folded/default: ", (t=s).foldCase(U_FOLD_CASE_DEFAULT));
    /* case-folde/Russian */
    printUnicodeString("full-case-folded/Russian: ", (t=s).foldCase(U_FOLD_CASE_EXCLUDE_SPECIAL_I));
}

extern int
main(int argc, const char *argv[]) {
    UErrorCode errorCode=U_ZERO_ERROR;

    // Note: Using a global variable for any object is not exactly
    // thread-safe...
    // You can change this call to e.g. ucnv_open("UTF-8", &errorCode)
    // if you pipe the output to a file and look at it with a
    // Unicode-capable editor.  This will currently affect only the
    // printUString() function, see the code above.
    // printUnicodeString() could use this, too, by changing to an
    // extract() overload that takes a UConverter argument.
    // cnv = ucnv_open(NULL, &errorCode);
    cnv = ucnv_open("UTF-8", &errorCode);
    if (U_FAILURE(errorCode)) {
      fprintf (stderr, "error %s opening the default converter\n",
               u_errorName(errorCode));
      return errorCode;
    }

    ucnv_setFromUCallBack (cnv, UCNV_FROM_U_CALLBACK_ESCAPE, UCNV_ESCAPE_C,
                           NULL, NULL, &errorCode);
    if (U_FAILURE(errorCode)) {
      fprintf (stderr, "error %s setting the escape callback in the default converter\n",
               u_errorName(errorCode));
      ucnv_close(cnv);
      return errorCode;
    }

    demoCaseMapInCPlusPlus();

    ucnv_close(cnv);
    return 0;
}
