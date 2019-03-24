#!/bin/sh
# Used to install dependencies for Travis CI builds.
set -e

. ${TRAVIS_BUILD_DIR}/scripts/travis/common.sh

# Arch-independent dependencies.
sudo apt-get install -qq --no-install-recommends doxygen graphviz

case "$HOST" in
    *-w64-mingw32)
        arch=`echo $HOST | sed s/-w64-mingw32$//`
        sudo apt-get install -qq --no-install-recommends g++-mingw-w64-$arch

        echo -n "Cross-compiling for $HOST using "
        $HOST-g++ --version

        # Download and build the required dependencies ourselves.
        ${TRAVIS_BUILD_DIR}/scripts/install_deps.sh
        ;;

    *)
        # Assume native build.
        sudo apt-get install -qq libxml2-dev libxslt1-dev
        ;;
esac
