#!/bin/sh
# Main build script for Travis CI builds.
set -e
./bootstrap
./configure CXXFLAGS="-Werror $CXXFLAGS"
make
make check
