#
# String partition tests
#

##
# Specify each test suite configuration

# * Partition Test Suite
module_test_add_suite (partition PartitionTest partition.cpp)


##
# Register all the test suites to be built and performed
module_test_build_all ()

