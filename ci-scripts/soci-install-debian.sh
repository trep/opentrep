#!/bin/bash

# SOCI (as of November 2020, SOCI v4 has still not been released on Debian/Ubuntu)

SOCI_VER="4.0.2"
SOCI_DIR="/opt/soci"

if [ "$1" != "" ]
then
	if [ ! -d "$1" ]
	then
		echo
		echo "The first parameter of $0 must be an existing directory; it is not ($1)"
		echo
		exit -1
	fi
	SOCI_DIR="$1"
fi

#
echo
echo "SOCI v${SOCI_VER} will be downloaded and built in ${SOCI_DIR}"
echo

# Download SOCI
mkdir ${SOCI_DIR}/archives
echo
echo "Downloading SOCI v${SOCI_VER} into ${SOCI_DIR}"
rm -f ${SOCI_DIR}/archives/soci-${SOCI_VER}.tar.gz
wget https://github.com/SOCI/soci/archive/v${SOCI_VER}.tar.gz -O ${SOCI_DIR}/archives/soci-${SOCI_VER}.tar.gz

# Unpack SOCI
pushd ${SOCI_DIR}
echo
echo "Unpacking SOCI v${SOCI_VER} into ${SOCI_DIR}"
rm -rf soci-${SOCI_VER}
tar zxf archives/soci-${SOCI_VER}.tar.gz
pushd soci-${SOCI_VER}
patch -p1 < ../soci-debian-cmake.patch

# Build SOCI
mkdir build
pushd build
echo
echo "Preparing the build of SOCI v${SOCI_VER} (with CMake)"
cmake -DCMAKE_INSTALL_PREFIX=/usr -DCMAKE_BUILD_TYPE=Release -DSOCI_CXX11=ON -DSOCI_TESTS=OFF ..
echo "Building SOCI v${SOCI_VER}"
make

# Install SOCI
echo
echo "Installing SOCI v${SOCI_VER} into /usr"
make install

popd
popd
popd

