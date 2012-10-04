#
# OpenTREP Tests
#

##
# Specify each test suite configuration

# * OpenTREP Test Suite
module_test_add_suite (opentrep IndexBuildingTestSuite IndexBuildingTestSuite.cpp)
module_test_add_suite (opentrep SearchingTestSuite SearchingTestSuite.cpp)
module_test_add_suite (opentrep PartitionTestSuite PartitionTestSuite.cpp)
module_test_add_suite (opentrep SliceTestSuite SliceTestSuite.cpp)
module_test_add_suite (opentrep UnicodeTestSuite UnicodeTestSuite.cpp)


##
# Register all the test suites to be built and performed
module_test_build_all ()
