#!/bin/bash

# SOCI (as of April 2019, SOCI v4.0 has still not been released)
pushd /opt/soci
patch -p1 < ./soci-debian-cmake.patch
mkdir -p build
pushd build
cmake -DCMAKE_INSTALL_PREFIX=/usr -DCMAKE_BUILD_TYPE=Release -DSOCI_CXX11=ON -DSOCI_TESTS=OFF ..
sudo make install
popd
popd

