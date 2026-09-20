#!/usr/bin/env bash
#
# Regression/feature test for opentrep-datasync:
#  1. The '-h'/'--help' option prints usage information and exits
#     cleanly (exit code 0).
#  2. An invalid '-n'/'--noniata' value (only 0 and 1 are accepted) is
#     rejected with a clear error message and a non-zero exit code.
#  3. A '-p'/'--porpath' destination that cannot be created (e.g.,
#     because it already exists as a regular, non-directory file) is
#     reported with a clear error message and a non-zero exit code.
#
# This test does not perform any network access; a real download is
# validated manually (see the README).
#
# Usage: datasync-help-and-args-test.sh <path-to-opentrep-datasync-script>
#
set -u

DATASYNC_BIN="${1:-}"

if [ -z "${DATASYNC_BIN}" ] || [ ! -x "${DATASYNC_BIN}" ]; then
  echo "FAIL: opentrep-datasync script not found or not executable: '${DATASYNC_BIN}'" >&2
  exit 1
fi

# Run in an isolated, throw-away working directory.
WORKDIR=$(mktemp -d)
trap 'rm -rf "${WORKDIR}"' EXIT
cd "${WORKDIR}" || exit 1

# 1. '-h'/'--help'
OUTPUT=$("${DATASYNC_BIN}" -h 2>&1)
STATUS=$?

if [ ${STATUS} -ne 0 ]; then
  echo "FAIL: 'opentrep-datasync -h' exited with status ${STATUS} (expected 0)" >&2
  echo "${OUTPUT}" >&2
  exit 1
fi

if ! echo "${OUTPUT}" | grep -q "noniata"; then
  echo "FAIL: 'opentrep-datasync -h' output did not mention '--noniata'" >&2
  echo "${OUTPUT}" >&2
  exit 1
fi

if ! echo "${OUTPUT}" | grep -q "porpath"; then
  echo "FAIL: 'opentrep-datasync -h' output did not mention '--porpath'" >&2
  echo "${OUTPUT}" >&2
  exit 1
fi

# 2. Invalid '-n'/'--noniata' value
OUTPUT=$("${DATASYNC_BIN}" -n 2 2>&1)
STATUS=$?

if [ ${STATUS} -eq 0 ]; then
  echo "FAIL: 'opentrep-datasync -n 2' unexpectedly exited with status 0" >&2
  exit 1
fi

if ! echo "${OUTPUT}" | grep -qi "invalid choice"; then
  echo "FAIL: 'opentrep-datasync -n 2' did not report an 'invalid choice' error" >&2
  echo "${OUTPUT}" >&2
  exit 1
fi

# 3. Non-creatable destination directory (a regular file already exists
#    at that path)
BLOCKED_PATH="${WORKDIR}/blocked-porpath"
touch "${BLOCKED_PATH}"

OUTPUT=$("${DATASYNC_BIN}" -p "${BLOCKED_PATH}" 2>&1)
STATUS=$?

if [ ${STATUS} -eq 0 ]; then
  echo "FAIL: 'opentrep-datasync -p <blocked-file>' unexpectedly exited with status 0" >&2
  exit 1
fi

if ! echo "${OUTPUT}" | grep -qi "cannot create destination directory"; then
  echo "FAIL: 'opentrep-datasync -p <blocked-file>' did not report the expected error" >&2
  echo "${OUTPUT}" >&2
  exit 1
fi

echo "PASS: opentrep-datasync '-h', invalid '--noniata' and destination-directory error handling"
exit 0
