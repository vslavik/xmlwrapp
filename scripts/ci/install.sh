#!/bin/sh
# Used to install dependencies for the CI builds.
set -e

# This function sets the environment variable to the given value not only for
# this script itself, but persistently, by using GITHUB_ENV feature provided
# by the CI environment.
set_env_var() {
  echo "Setting environment variable $1=$2"
  echo $1=$2 >> $GITHUB_ENV
  eval "export $1=$2"
}

echo 'Compiler version:'
${CXX-c++} --version

case $(uname -s) in
  Linux)
    if [ -f /etc/apt/sources.list ]; then
      platform=debian
    elif [ -f /etc/redhat-release ]; then
      platform=redhat
    else
      echo 'Unknown Linux distribution.' >&2
      exit 1
    fi
    ccache_path=/usr/lib/ccache
    ;;

  Darwin)
    platform=macos
    # The directory used for ccache symlinks has changed and differs between
    # macOS 13 and 15, so detect it dynamically.
    if [ -d /opt/homebrew/opt/ccache/libexec ]; then
      ccache_path=/opt/homebrew/opt/ccache/libexec
    elif [ -d /usr/local/opt/ccache/libexec ]; then
      ccache_path=/usr/local/opt/ccache/libexec
    else
      echo 'Warning: unknown macOS ccache path.' >&2
    fi
    ;;

  *)
    echo "Unknown platform `uname`." >&2
    exit 1
esac

set_env_var PATH ${ccache_path}:$PATH
set_env_var XMLWRAPP_CI_BRANCH $GITHUB_REF
set_env_var XMLWRAPP_SOURCE_DIR $GITHUB_WORKSPACE

. ${XMLWRAPP_SOURCE_DIR}/scripts/ci/common.sh

. ${XMLWRAPP_SOURCE_DIR}/scripts/ci/install-${platform}.sh
