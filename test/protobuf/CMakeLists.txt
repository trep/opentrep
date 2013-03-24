#
# OpenTREP tests related to Protobuf
#

##
# Specify each test suite configuration

##
# * Protobuf test suite

# With Protobuf
if (PROTOBUF_FOUND)
  module_test_add_suite (protobuf travel_protobuf "travel_protobuf.cpp" opentrep)
endif (PROTOBUF_FOUND)


##
# Register all the test suites to be built and performed
module_test_build_all ()
