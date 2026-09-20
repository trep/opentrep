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

##
# Regression/feature test for the opentrep-dbmgr command-line tool:
#  * The non-interactive '-c'/'--command' mode
#  * The 'list_by_iata' (and similar) crash when no SQL database is
#    configured
if (Boost_FOUND AND ENABLE_TEST)
  add_test (NAME opentrep-dbmgr_command_mode_test
    COMMAND bash "${CMAKE_CURRENT_SOURCE_DIR}/dbmgr-command-mode-test.sh"
            "$<TARGET_FILE:opentrep-dbmgrbin>")
  add_dependencies (check_${MODULE_NAME}tst opentrep-dbmgrbin)
endif (Boost_FOUND AND ENABLE_TEST)
