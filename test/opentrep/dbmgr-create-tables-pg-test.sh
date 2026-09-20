#!/usr/bin/env bash
#
# Regression test for opentrep-dbmgr's 'create_user', 'create_tables' and
# 'create_indexes' commands, specifically for the PostgreSQL ('-t pg') SQL
# database type:
#  - When the deployment-slot-suffixed target database (e.g., 'trep0') does
#    not exist, and/or cannot be reached (e.g., no PostgreSQL server is
#    listening), connecting to it used to make the whole application abort
#    with an uncaught OPENTREP::SQLDatabaseImpossibleConnectionException; it
#    now reports a user-friendly, actionable error message instead, and the
#    command-mode session terminates cleanly (exit code 0).
#
# This scenario is reproduced deterministically, without requiring a real
# PostgreSQL server: the connection string targets an address ('127.0.0.1',
# port 1) that is not expected to have anything listening, so the
# connection reliably fails the same way a missing/inaccessible database
# would.
#
# Usage: dbmgr-create-tables-pg-test.sh <path-to-opentrep-dbmgr-binary>
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

# Connection string pointing at an address with (in practice) nothing
# listening, so that the connection attempt fails in the same way as it
# would for a not-yet-created/unreachable deployment-slot database.
UNREACHABLE_CONN_STRING="dbname=trep0 user=trep password=trep host=127.0.0.1 port=1"

for CMD in create_user create_tables create_indexes; do
  OUTPUT=$("${DBMGR_BIN}" -t pg \
    -c "reset_connection_string ${UNREACHABLE_CONN_STRING} ; ${CMD}" 2>&1)
  STATUS=$?

  echo "${OUTPUT}"

  if [ ${STATUS} -ne 0 ]; then
    echo "FAIL: opentrep-dbmgr exited with status ${STATUS} (expected 0) for '${CMD}' against an unreachable PG database" >&2
    exit 1
  fi

  if echo "${OUTPUT}" | grep -qi "Assertion failed\|terminating due to uncaught exception"; then
    echo "FAIL: opentrep-dbmgr crashed on '${CMD}' against an unreachable PG database (regression)" >&2
    exit 1
  fi

  if ! echo "${OUTPUT}" | grep -qi "could not be created"; then
    echo "FAIL: '${CMD}' did not report the expected connection-failure message" >&2
    exit 1
  fi

  if ! echo "${OUTPUT}" | grep -q "End of the session. Exiting."; then
    echo "FAIL: opentrep-dbmgr did not terminate cleanly after '${CMD}' failed" >&2
    exit 1
  fi
done

echo "PASS: opentrep-dbmgr create_user/create_tables/create_indexes PG connection-failure test"
exit 0
