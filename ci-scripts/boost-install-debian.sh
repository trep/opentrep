#!/bin/bash

# Boost (as of November 2021, Ubuntu 20.04 has only Boost 1.71, whereas Boost 1.77 is already out)

BOOST_VER="1_77_0"
BOOST_VER_DOT="$(echo ${BOOST_VER}|sed -e s/_/./g)"
BOOST_DIR="/opt/boost"

if [ "$1" != "" ]
then
	if [ ! -d "$1" ]
	then
		echo
		echo "The first parameter of $0 must be an existing directory; it is not ($1)"
		echo
		exit -1
	fi
	BOOST_DIR="$1"
fi

#
echo
echo "Boost ${BOOST_VER} will be downloaded and built in ${BOOST_DIR}"
echo

# Download Boost
mkdir ${BOOST_DIR}/archives
echo
echo "Downloading Boost ${BOOST_VER} into ${BOOST_DIR}"
rm -f ${BOOST_DIR}/archives/boost_${BOOST_VER}.tar.bz2
wget https://boostorg.jfrog.io/artifactory/main/release/${BOOST_VER_DOT}/source/boost_${BOOST_VER}.tar.bz2 -O ${BOOST_DIR}/archives/boost_${BOOST_VER}.tar.bz2

# Unpack Boost
pushd ${BOOST_DIR}
echo
echo "Unpacking Boost ${BOOST_VER} into ${BOOST_DIR}"
rm -rf boost_${BOOST_VER}
tar jxf archives/boost_${BOOST_VER}.tar.bz2
pushd boost_${BOOST_VER}

# Build Boost
echo
echo "Configuring the build of Boost ${SOCI_VER} (with bootstrap)"
./bootstrap.sh --prefix=/usr/
echo "Build Boost ${SOCI_VER} (with b2)"
./b2 cxxflags="-std=c++14"

# Install Boost
echo
echo "Installing Boost ${SOCI_VER} into /usr"
./b2 cxxflags="-std=c++14" install

popd
popd

