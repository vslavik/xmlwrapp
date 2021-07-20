#!/bin/sh
# Script installing the dependencies for this library.
#
# It is used by the CI scripts, but could be used directly if these libraries
# are not available as packages on your system.
#
# Note that the script makes no attempt to clean up after itself and assumes
# it can use sudo to install the libraries to /usr/local.
set -e

# Takes base name of the library, its version and an arbitrary number of
# arguments to pass to the library configure.
install_lib() {
    lib_basename=$1-$2
    shift
    shift
    lib_archive=$lib_basename.tar.gz

    [ -e $lib_archive ] || wget http://xmlsoft.org/sources/$lib_archive
    tar xzf $lib_archive
    cd $lib_basename
    # Use minimal and fastest options.
    configure_args='--disable-dependency-tracking --disable-static --without-python'
    if [ -n "$HOST" ]; then
        configure_args="$configure_args --host=$HOST"
    fi
    ./configure $configure_args "$@"
    make
    sudo make install
}

install_lib libxml2 2.9.9 --without-lzma --without-modules --without-zlib
install_lib libxslt 1.1.33 --without-crypto
