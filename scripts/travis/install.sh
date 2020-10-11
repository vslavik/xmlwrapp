#!/bin/sh
# Used to install dependencies for Travis CI builds.
set -e

. ${TRAVIS_BUILD_DIR}/scripts/travis/common.sh

# Arch-independent dependencies.
sudo apt-get install -qq --no-install-recommends doxygen graphviz

case "$HOST" in
    *-w64-mingw32)
        arch=`echo $HOST | sed -e 's/-w64-mingw32$//;s/_/-/'`

        case "$arch" in
            i686)
                sudo dpkg --add-architecture i386
                wine_package=wine32-development
                ;;

            x86-64)
                wine_package=wine64-development
                ;;

            *)
                echo "Unknown architecture $arch when cross-compiling"
                exit 1
                ;;
        esac

        sudo apt-get update
        sudo apt-get install -qq --no-install-recommends g++-mingw-w64-$arch \
            $wine_package wine-development

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
