#!/bin/sh
# Build the given version of libxml2 from sources and install it into the
# given prefix.
#
# This is used by the CI builds to test with many different libxml2 versions,
# but can also be used locally to reproduce problems with a specific version.
#
# Usage: build-libxml2.sh <version> <prefix>
#
# where <version> is either a full release version, e.g. 2.9.14, or "master"
# to build the latest sources from git.
#
# If libxml2 is already installed in the given prefix (e.g. because it was
# restored from the cache), nothing is done.
set -e

if [ $# -ne 2 ]; then
    echo "Usage: $0 <version>|master <prefix>" >&2
    exit 2
fi

version=$1
prefix=$2

case "$prefix" in
    /*) ;;
    *) prefix="$(pwd)/$prefix" ;;
esac

if [ -f "$prefix/lib/pkgconfig/libxml-2.0.pc" ]; then
    echo "libxml2 $(PKG_CONFIG_PATH="$prefix/lib/pkgconfig" pkg-config --modversion libxml-2.0) already installed in $prefix."
    exit 0
fi

workdir=$(mktemp -d)
trap 'rm -rf "$workdir"' EXIT
cd "$workdir"

case "$version" in
    master)
        git clone --quiet --depth 1 https://gitlab.gnome.org/GNOME/libxml2.git libxml2
        git -C libxml2 log -1 --format='Building libxml2 master at %H (%cs)'
        (cd libxml2 && autoreconf --force --install)
        srcdir=libxml2
        ;;

    [0-9]*.[0-9]*.[0-9]*)
        series=$(echo "$version" | cut -d. -f1,2)
        echo "Building libxml2 $version"
        curl --fail --silent --show-error --location --retry 3 \
            --output libxml2.tar.xz \
            "https://download.gnome.org/sources/libxml2/$series/libxml2-$version.tar.xz"
        tar xf libxml2.tar.xz
        srcdir=libxml2-$version
        ;;

    *)
        echo "Invalid libxml2 version \"$version\"." >&2
        exit 2
esac

# Old libxml2 versions don't compile with gcc 14 or later, which turned some
# warnings into errors by default, so turn them back into warnings, nor with
# gcc 15 or later, which uses C23 by default, in which "int f();" declares a
# function without any parameters, so use the previous C standard explicitly.
CFLAGS="-O2 -std=gnu17 -Wno-error=incompatible-pointer-types -Wno-error=implicit-function-declaration -Wno-error=int-conversion"

# Disable everything not needed by xmlwrapp to speed up the build and avoid
# extra dependencies. Note that older versions don't recognize some of these
# options, but configure only gives a warning about them.
mkdir build
cd build
"../$srcdir/configure" --prefix="$prefix" \
    --disable-static \
    --without-python \
    --without-lzma \
    --without-zlib \
    CFLAGS="$CFLAGS" \
    >configure.log 2>&1 || {
    cat configure.log
    echo "Configuring libxml2 failed." >&2
    exit 1
}

# Use --output-sync to avoid interleaving the output of parallel jobs, making
# the log readable in case of an error.
make --no-print-directory --output-sync=target -j"$(nproc)" >make.log 2>&1 || {
    cat make.log
    echo "Building libxml2 failed." >&2
    exit 1
}

make --no-print-directory install >install.log 2>&1 || {
    cat install.log
    echo "Installing libxml2 failed." >&2
    exit 1
}

installed=$(PKG_CONFIG_PATH="$prefix/lib/pkgconfig" pkg-config --modversion libxml-2.0)
echo "Installed libxml2 $installed into $prefix."

if [ "$version" != master ] && [ "$installed" != "$version" ]; then
    echo "Unexpected installed libxml2 version $installed, expected $version." >&2
    exit 1
fi
