#
# OpenTREP tests related to Protobuf
#

##
# Specify each test suite configuration

##
# * Boost-based (Spirit) parser test suite

# With Protobuf
if (Boost_FOUND)
  module_test_add_suite (parsers schedule_parser "schedule_parser.cpp" opentrep)
  module_test_add_suite (parsers search_string_parser "search_string_parser.cpp")
endif (Boost_FOUND)


##
# Register all the test suites to be built and performed
module_test_build_all ()
