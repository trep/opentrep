#!/bin/sh

INSTALL_DIR=`grep "^prefix =" ../Makefile | cut -d"=" -d" " -f3`
TST_PROG=../opentrep/batches/opentrep_indexer
OPENTREP=`grep "^PACKAGE_VERSION =" ../Makefile | cut -d"=" -d" " -f3`
OPENTREP_LIBRARY_NAME=`grep "^PACKAGE =" ../Makefile | cut -d"=" -d" " -f3`
OPENTREP_LIB=lib${OPENTREP_LIBRARY_NAME}-${OPENTREP_API_VERSION}.so

if [ ! -x ${TST_PROG} ];
then
	echo "The sample program does not seem to have been compiled. Try 'cd ../opentrep && make && cd ../test' first."
	exit -1
fi

if [ "$1" = "-h" -o "$1" = "-H" -o "$1" = "--h" -o "$1" = "--help" ];
then
	echo "Usage: $0 [<log file name> [<Xapian database name (directory of the index)>]]"
	echo "That program builds the Xapian database (index) for travel-related data."
	echo
	echo "The library (${OPENTREP_LIB}) is installed in: ${INSTALL_DIR}/lib"
	exit 0
fi

${TST_PROG} $1 $2

