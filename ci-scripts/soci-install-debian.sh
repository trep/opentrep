#!/bin/bash

# SOCI (as of Novermber 2020, SOCI v4 has still not been released on Debian/Ubuntu)
pushd /opt/soci
mkdir -p build
pushd build
cmake -DCMAKE_INSTALL_PREFIX=/usr -DCMAKE_BUILD_TYPE=Release -DSOCI_CXX11=ON -DSOCI_TESTS=OFF ..
sudo make install
popd
popd

