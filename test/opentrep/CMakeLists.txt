#
# OpenTREP Tests
#
module_set_name (opentrep)


##
# Specify each test suite configuration

# * OpenTREP Test Suite
module_test_add_suite (IndexBuildingTestSuite IndexBuildingTestSuite.cpp)
module_test_add_suite (SearchingTestSuite SearchingTestSuite.cpp)


##
# Register all the test suites to be built and performed
module_test_build_all ()

