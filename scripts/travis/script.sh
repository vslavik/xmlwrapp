#!/bin/sh
# Main build script for Travis CI builds.
set -e

. ${TRAVIS_BUILD_DIR}/scripts/travis/common.sh

autoreconf --symlink --install

CXXFLAGS="-Werror $CXXFLAGS"

if [ -n "$HOST" ]; then
    configure_args="--host=$HOST"
fi
./configure CXXFLAGS="$CXXFLAGS" $configure_args

# Test building from a distribution archive, rather than from Git sources.
if [ "$TEST_DIST" = 1 ]; then
    # We need bkl in order to build the distribution archives, so get it.
    BKL_VERSION=1.2.6
    BKL_ARCHIVE=bakefile-${BKL_VERSION}-bin.tar.bz2

    curl --remote-name --location https://github.com/vslavik/bakefile/releases/download/v${BKL_VERSION}/${BKL_ARCHIVE}

    # We assume we're using GNU tar, which can uncompress .bz2 files on its own.
    tar xf ${BKL_ARCHIVE}

    PATH=$(pwd)/bakefile-${BKL_VERSION}:${PATH}

    make dist

    XMLWRAPP_VERSION=$(sed -n -e 's/AC_INIT(xmlwrapp, \([0-9.]*\),.*)$/\1/p' configure.ac)

    # Same as above, we assume tar can uncompress .gz files on the fly.
    tar xf xmlwrapp-${XMLWRAPP_VERSION}.tar.gz

    cd xmlwrapp-${XMLWRAPP_VERSION}
    ./configure CXXFLAGS="$CXXFLAGS" $configure_args
fi

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

if ! make check; then
    echo 'Test suite log contents'
    echo '-----------------------'
    cat tests/test-suite.log
    echo '-----------------------'
    exit 1
fi
