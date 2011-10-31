#!/bin/sh

EXEC=../xapian/string_search
XAPIAN_DB=traveldb
QUERY=$1

if [ -x ${EXEC} ]; then
	${EXEC} ${XAPIAN_DB} ${QUERY}
else
	echo "The ${EXEC} binary must be built. Hint: go to ../simple and type make."
fi

