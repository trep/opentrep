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

##
# Regression/feature test for the opentrep-dbmgr 'reset_connection_string'
# command with the PostgreSQL ('-t pg') SQL database type:
#  * A malformed (MySQL-style 'db=') connection string used to crash the
#    application with an uncaught parsing exception; it is now reported as
#    a user-friendly error message.
#  * A well-formed (libpq-style 'dbname=') connection string is still
#    accepted. Neither scenario requires a running PostgreSQL server.
if (Boost_FOUND AND ENABLE_TEST)
  add_test (NAME opentrep-dbmgr_reset_connection_string_pg_test
    COMMAND bash "${CMAKE_CURRENT_SOURCE_DIR}/dbmgr-reset-connection-string-pg-test.sh"
            "$<TARGET_FILE:opentrep-dbmgrbin>")
  add_dependencies (check_${MODULE_NAME}tst opentrep-dbmgrbin)
endif (Boost_FOUND AND ENABLE_TEST)

##
# Regression/feature test for the opentrep-dbmgr 'create_user',
# 'create_tables' and 'create_indexes' commands with the PostgreSQL
# ('-t pg') SQL database type:
#  * Connecting to an unreachable/non-existent deployment-slot database
#    (e.g., 'trep0') used to crash the application with an uncaught
#    SQLDatabaseImpossibleConnectionException; it is now reported as a
#    user-friendly, actionable error message. This is reproduced
#    deterministically and does not require a running PostgreSQL server.
if (Boost_FOUND AND ENABLE_TEST)
  add_test (NAME opentrep-dbmgr_create_tables_pg_test
    COMMAND bash "${CMAKE_CURRENT_SOURCE_DIR}/dbmgr-create-tables-pg-test.sh"
            "$<TARGET_FILE:opentrep-dbmgrbin>")
  add_dependencies (check_${MODULE_NAME}tst opentrep-dbmgrbin)
endif (Boost_FOUND AND ENABLE_TEST)

##
# Regression/feature test for the opentrep-dbmgr 'reset_connection_string'
# command with the MySQL/MariaDB ('-t mysql') SQL database type:
#  * A connection string containing a 'host=' (and/or 'port=') key used to
#    crash the application with an assertion failure in
#    OPENTREP::parseMySQLConnectionString (a fragile, position-based
#    state-machine parser that only recognized 'db'/'user'/'password').
#    The parser has been rewritten as a generic key=value parser
#    (mirroring the PostgreSQL one), so 'host='/'port=' are now properly
#    recognized. Neither scenario requires a running MySQL/MariaDB
#    server.
if (Boost_FOUND AND ENABLE_TEST)
  add_test (NAME opentrep-dbmgr_reset_connection_string_mysql_host_test
    COMMAND bash "${CMAKE_CURRENT_SOURCE_DIR}/dbmgr-reset-connection-string-mysql-host-test.sh"
            "$<TARGET_FILE:opentrep-dbmgrbin>")
  add_dependencies (check_${MODULE_NAME}tst opentrep-dbmgrbin)
endif (Boost_FOUND AND ENABLE_TEST)
