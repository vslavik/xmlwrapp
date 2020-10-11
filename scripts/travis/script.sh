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
    # As long as we don't have any other tests (e.g. written in interpreted
    # language), just setting LOG_COMPILER to Wine is good enough. If we do
    # need to run e.g. Python tests too, we would need a script selecting the
    # right way to run the test depending on its extension as, unfortunately,
    # setting EXE_LOG_COMPILER doesn't work (although PY_LOG_COMPILER does).
    export LOG_COMPILER=wine-development

    # Set up the path to allow the tests to find libstdc++ and libxml2 DLLs.
    export WINEPATH="$(dirname $($HOST-g++ -print-libgcc-file-name));/usr/local/bin"
fi

make check
