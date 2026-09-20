#!/usr/bin/env bash
#
# Regression/feature test for opentrep-dbmgr:
#  1. The non-interactive '-c'/'--command' mode accepts several
#     semicolon-separated dbmgr commands, executes them by reusing the
#     very same command parser/dispatcher as the interactive session,
#     and then exits cleanly (exit code 0), without prompting for input.
#  2. Entering a 'list_by_*' command (e.g., 'list_by_iata nce') when no
#     SQL database has been configured (the default) no longer crashes
#     with an assertion failure in OPENTREP_Service::listByIataCode();
#     instead, a user-friendly error message is displayed.
#
# Usage: dbmgr-command-mode-test.sh <path-to-opentrep-dbmgr-binary>
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

# 'help' does not require any Xapian index nor SQL database, and is only
# used here as a harmless filler command to validate that several
# semicolon-separated commands are indeed all executed in sequence.
# 'list_by_iata nce' is the command which used to crash (see above).
OUTPUT=$("${DBMGR_BIN}" -c "help ; list_by_iata nce" 2>&1)
STATUS=$?

echo "${OUTPUT}"

if [ ${STATUS} -ne 0 ]; then
  echo "FAIL: opentrep-dbmgr exited with status ${STATUS} (expected 0)" >&2
  exit 1
fi

if echo "${OUTPUT}" | grep -qi "Assertion failed"; then
  echo "FAIL: opentrep-dbmgr crashed with an assertion failure (regression)" >&2
  exit 1
fi

if ! echo "${OUTPUT}" | grep -q "^Commands: $"; then
  echo "FAIL: the 'help' command output was not found; commands may not be chained correctly" >&2
  exit 1
fi

if ! echo "${OUTPUT}" | grep -q "requires a SQL database"; then
  echo "FAIL: 'list_by_iata' did not report the expected 'no SQL database configured' message" >&2
  exit 1
fi

if ! echo "${OUTPUT}" | grep -q "End of the session. Exiting."; then
  echo "FAIL: opentrep-dbmgr did not terminate cleanly after the command-mode commands" >&2
  exit 1
fi

echo "PASS: opentrep-dbmgr command mode (-c) and list_by_iata regression test"
exit 0
