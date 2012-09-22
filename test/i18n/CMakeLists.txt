#
# OpenTREP Tests
#

##
# Specify each test suite configuration

# * Internationalisation (i18n) test suite
module_test_add_suite (i18n utf8string utf8/utf8string.cpp opentrep)
module_test_add_suite (i18n utf8 utf8/utf8.cpp opentrep)


##
# Register all the test suites to be built and performed
module_test_build_all ()
