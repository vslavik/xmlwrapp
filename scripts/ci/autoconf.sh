#!/bin/sh
# Main build script for the CI builds.
set -e

. ${XMLWRAPP_SOURCE_DIR}/scripts/ci/common.sh

autoreconf --symlink --install

CXXFLAGS="-Werror $CXXFLAGS"

case "$HOST" in
    *-mingw*)
        configure_args="--host=$HOST"

        # As long as we don't have any other tests (e.g. written in interpreted
        # language), just setting LOG_COMPILER to Wine is good enough. If we do
        # need to run e.g. Python tests too, we would need a script selecting the
        # right way to run the test depending on its extension as, unfortunately,
        # setting EXE_LOG_COMPILER doesn't work (although PY_LOG_COMPILER does).
        export LOG_COMPILER=wine-development

        # Set up the path to allow the tests to find libstdc++ and libxml2 DLLs.
        export WINEPATH="$(dirname $($HOST-g++ -print-libgcc-file-name));/usr/local/bin"
        ;;
esac

if [ "$XMLWRAPP_WITH_DOCS" != 1 ]; then
    configure_args="$configure_args --disable-docs"
fi

./configure CXXFLAGS="$CXXFLAGS" LDFLAGS="$LDFLAGS" $configure_args

make --keep-going

if ! make check; then
    echo 'Test suite log contents'
    echo '-----------------------'
    cat tests/test-suite.log
    echo '-----------------------'
    exit 1
fi
