#!/usr/bin/env bash
#
# Regression/feature test for opentrep-dbmgr's 'reset_connection_string'
# command, specifically for the MySQL/MariaDB ('-t mysql') SQL database
# type:
#  1. A connection string containing a 'host=' key (and, combined, a
#     'port=' key) used to make the whole application abort with an
#     assertion failure in OPENTREP::parseMySQLConnectionString (the
#     parser only recognized 'db'/'user'/'password' via a fragile,
#     position-based state machine; any other key, such as 'host', threw
#     off that state and caused a duplicate-key insertion into an
#     internal STL map, tripping 'assert (isSuccess == true)'). The
#     parser has been rewritten as a generic key=value parser (mirroring
#     the PostgreSQL one), so 'host='/'port=' (and other keys) are now
#     properly recognized, and the command-mode session terminates
#     cleanly (exit code 0).
#  2. A connection string using only the previously-supported keys
#     ('db=', 'user=', 'password=') is still accepted and applied without
#     error (no regression).
#  3. A connection string missing the required 'db=' key is still
#     reported as a user-friendly parsing error (not a crash), exactly as
#     for the well-formed cases.
#
# None of these scenarios require a running MySQL/MariaDB server: the
# failure (and its fix) both occur at the connection-string
# parsing/dispatch stage, before any attempt is made to actually connect
# to a database.
#
# Usage: dbmgr-reset-connection-string-mysql-host-test.sh <path-to-opentrep-dbmgr-binary>
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

assert_no_crash_and_clean_exit () {
  local SCENARIO="$1"
  local OUTPUT="$2"
  local STATUS="$3"

  if [ "${STATUS}" -ne 0 ]; then
    echo "FAIL: opentrep-dbmgr exited with status ${STATUS} (expected 0) for scenario '${SCENARIO}'" >&2
    exit 1
  fi

  if echo "${OUTPUT}" | grep -qi "Assertion failed\|terminating due to uncaught exception"; then
    echo "FAIL: opentrep-dbmgr crashed for scenario '${SCENARIO}' (regression)" >&2
    exit 1
  fi

  if ! echo "${OUTPUT}" | grep -q "End of the session. Exiting."; then
    echo "FAIL: opentrep-dbmgr did not terminate cleanly for scenario '${SCENARIO}'" >&2
    exit 1
  fi
}

# //////////////////////////////////////////////////////////////////
# Scenario 1: 'host=' alone, combined with the existing 'db='/'user='/
# 'password=' keys
# //////////////////////////////////////////////////////////////////
OUTPUT_HOST=$("${DBMGR_BIN}" -t mysql \
  -c "reset_connection_string db=trep0 user=trep password=trep host=127.0.0.1" 2>&1)
STATUS_HOST=$?

echo "${OUTPUT_HOST}"
assert_no_crash_and_clean_exit "host=" "${OUTPUT_HOST}" "${STATUS_HOST}"

if ! echo "${OUTPUT_HOST}" | grep -q "The connection string has been reset"; then
  echo "FAIL: 'reset_connection_string' did not accept a MySQL connection string with 'host='" >&2
  exit 1
fi

# //////////////////////////////////////////////////////////////////
# Scenario 2: 'host=' combined with 'port='
# //////////////////////////////////////////////////////////////////
OUTPUT_HOST_PORT=$("${DBMGR_BIN}" -t mysql \
  -c "reset_connection_string db=trep0 user=trep password=trep host=127.0.0.1 port=3306" 2>&1)
STATUS_HOST_PORT=$?

echo "${OUTPUT_HOST_PORT}"
assert_no_crash_and_clean_exit "host=+port=" "${OUTPUT_HOST_PORT}" "${STATUS_HOST_PORT}"

if ! echo "${OUTPUT_HOST_PORT}" | grep -q "The connection string has been reset"; then
  echo "FAIL: 'reset_connection_string' did not accept a MySQL connection string with 'host=' and 'port='" >&2
  exit 1
fi

# //////////////////////////////////////////////////////////////////
# Scenario 3: no regression on the previously-supported keys only
# //////////////////////////////////////////////////////////////////
OUTPUT_OK=$("${DBMGR_BIN}" -t mysql \
  -c "reset_connection_string db=trep0 user=trep password=trep" 2>&1)
STATUS_OK=$?

echo "${OUTPUT_OK}"
assert_no_crash_and_clean_exit "db+user+password only" "${OUTPUT_OK}" "${STATUS_OK}"

if ! echo "${OUTPUT_OK}" | grep -q "The connection string has been reset"; then
  echo "FAIL: 'reset_connection_string' did not accept a well-formed MySQL connection string" >&2
  exit 1
fi

# //////////////////////////////////////////////////////////////////
# Scenario 4: missing required 'db=' key is still a friendly error,
# not a crash
# //////////////////////////////////////////////////////////////////
OUTPUT_BAD=$("${DBMGR_BIN}" -t mysql \
  -c "reset_connection_string user=trep password=trep host=127.0.0.1" 2>&1)
STATUS_BAD=$?

echo "${OUTPUT_BAD}"
assert_no_crash_and_clean_exit "missing db=" "${OUTPUT_BAD}" "${STATUS_BAD}"

if ! echo "${OUTPUT_BAD}" | grep -qi "cannot be understood"; then
  echo "FAIL: 'reset_connection_string' did not report the expected parsing-error message for a missing 'db=' key" >&2
  exit 1
fi

echo "PASS: opentrep-dbmgr reset_connection_string MySQL 'host='/'port=' parsing test"
exit 0
