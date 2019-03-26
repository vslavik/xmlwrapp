#!/bin/sh
# Main build script for Travis CI builds.
set -e

. ${TRAVIS_BUILD_DIR}/scripts/travis/common.sh

./bootstrap

CXXFLAGS="-Werror $CXXFLAGS"

if [ -n "$HOST" ]; then
    configure_args="--host=$HOST"
fi
./configure CXXFLAGS="$CXXFLAGS" $configure_args

make

if [ -n "$HOST" ]; then
    # TODO: Use Wine.
    echo "*** Skipping tests for cross-builds."
else
    make check
fi
