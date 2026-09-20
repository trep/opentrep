#!/usr/bin/env bash
#
# Regression/feature test for opentrep-dbmgr's 'reset_connection_string'
# command, specifically for the PostgreSQL ('-t pg') SQL database type:
#  1. A malformed connection string (e.g., using the MySQL-style 'db='
#     keyword instead of the PostgreSQL/libpq 'dbname=' keyword) used to
#     make the whole application abort with an uncaught
#     OPENTREP::SQLDatabaseConnectionStringParsingException; it now
#     reports a user-friendly error message instead, and the command-mode
#     session terminates cleanly (exit code 0).
#  2. A well-formed, libpq-style connection string (using 'dbname=') is
#     still accepted and applied without error.
#
# Neither scenario requires a running PostgreSQL server: the failure (and
# its fix) both occur at the connection-string parsing/dispatch stage,
# before any attempt is made to actually connect to a database.
#
# Usage: dbmgr-reset-connection-string-pg-test.sh <path-to-opentrep-dbmgr-binary>
#
set -u

DBMGR_BIN="${1:-}"

if [ -z "${DBMGR_BIN}" ] || [ ! -x "${DBMGR_BIN}" ]; then
  echo "FAIL: opentrep-dbmgr binary not found or not executable: '${DBMGR_BIN}'" >&2
  exit 1
fi

# Run in an isolated, throw-away working directory, so that the test does
# not depend on (nor pollute) any pre-existing Xapian index, SQL database
# or log/history files.
WORKDIR=$(mktemp -d)
trap 'rm -rf "${WORKDIR}"' EXIT
cd "${WORKDIR}" || exit 1

# //////////////////////////////////////////////////////////////////
# Scenario 1: malformed (MySQL-style 'db=') connection string for '-t pg'
# //////////////////////////////////////////////////////////////////
OUTPUT_BAD=$("${DBMGR_BIN}" -t pg \
  -c "reset_connection_string db=trep user=trep password=trep" 2>&1)
STATUS_BAD=$?

echo "${OUTPUT_BAD}"

if [ ${STATUS_BAD} -ne 0 ]; then
  echo "FAIL: opentrep-dbmgr exited with status ${STATUS_BAD} (expected 0) for a malformed PG connection string" >&2
  exit 1
fi

if echo "${OUTPUT_BAD}" | grep -qi "Assertion failed\|terminating due to uncaught exception"; then
  echo "FAIL: opentrep-dbmgr crashed on a malformed PG connection string (regression)" >&2
  exit 1
fi

if ! echo "${OUTPUT_BAD}" | grep -qi "cannot be understood"; then
  echo "FAIL: 'reset_connection_string' did not report the expected parsing-error message" >&2
  exit 1
fi

if ! echo "${OUTPUT_BAD}" | grep -q "End of the session. Exiting."; then
  echo "FAIL: opentrep-dbmgr did not terminate cleanly after the malformed connection string" >&2
  exit 1
fi

# //////////////////////////////////////////////////////////////////
# Scenario 2: well-formed (libpq-style 'dbname=') connection string
# //////////////////////////////////////////////////////////////////
OUTPUT_OK=$("${DBMGR_BIN}" -t pg \
  -c "reset_connection_string dbname=trep user=trep password=trep host=localhost" 2>&1)
STATUS_OK=$?

echo "${OUTPUT_OK}"

if [ ${STATUS_OK} -ne 0 ]; then
  echo "FAIL: opentrep-dbmgr exited with status ${STATUS_OK} (expected 0) for a well-formed PG connection string" >&2
  exit 1
fi

if echo "${OUTPUT_OK}" | grep -qi "Assertion failed\|terminating due to uncaught exception"; then
  echo "FAIL: opentrep-dbmgr crashed on a well-formed PG connection string" >&2
  exit 1
fi

if ! echo "${OUTPUT_OK}" | grep -q "The connection string has been reset"; then
  echo "FAIL: 'reset_connection_string' did not accept a well-formed PG connection string" >&2
  exit 1
fi

echo "PASS: opentrep-dbmgr reset_connection_string PG parsing test"
exit 0
