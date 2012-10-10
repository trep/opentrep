#
# OpenTREP Tests
#

##
# Specify each test suite configuration

##
# * Internationalisation (i18n) test suite

# With standard libraries
module_test_add_suite (i18n utf8string utf8/utf8string.cpp opentrep)
module_test_add_suite (i18n utf8test utf8/utf8.cpp opentrep)

# With ICU
if (ICU_FOUND)
  module_test_add_suite (i18n icutranslit_simple "icu/icutranslit_simple.cpp;icu/icutranslit_util.hpp;icu/icutranslit_util.cpp" opentrep)
endif (ICU_FOUND)

# With Boost.Algorithm
if (Boost_FOUND)
  module_test_add_suite (i18n boost_string boost_string.cpp)
endif (Boost_FOUND)

# With Boost.Locale
if (Boost_LOCALE_FOUND)
  module_test_add_suite (i18n boost_locale boost_locale.cpp opentrep)
endif (Boost_LOCALE_FOUND)


##
# Register all the test suites to be built and performed
module_test_build_all ()
