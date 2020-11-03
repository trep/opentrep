#!/bin/bash

SOCI_VER="4.0.1"
SOCI_DIR="/opt/soci"

# SOCI (as of November 2020, SOCI v4 has still not been released on Debian/Ubuntu)
mkdir -p ${SOCI_DIR}/archives
wget https://github.com/SOCI/soci/archive/${SOCI_VER}.tar.gz -O ${SOCI_DIR}/archives/soci-${SOCI_VER}.tar.gz
pushd ${SOCI_DIR}
tar zxf archives/soci-${SOCI_VER}.tar.gz
mkdir -p soci-${SOCI_VER}/build
pushd soci-${SOCI_VER}/build
cmake -DCMAKE_INSTALL_PREFIX=/usr -DCMAKE_BUILD_TYPE=Release -DSOCI_CXX11=ON -DSOCI_TESTS=OFF ..
sudo make install
popd
popd

