#
# OpenTREP Tests
#

##
# Specify each test suite configuration

# * OpenTREP Test Suite
module_test_add_suite (opentrep IndexBuildingTestSuite IndexBuildingTestSuite.cpp)
module_test_add_suite (opentrep SearchingTestSuite SearchingTestSuite.cpp)


##
# Register all the test suites to be built and performed
module_test_build_all ()
