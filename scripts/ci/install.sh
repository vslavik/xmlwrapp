#!/bin/sh
# Used to install dependencies for the CI builds.
set -e

. ${XMLWRAPP_SOURCE_DIR}/scripts/ci/common.sh

case $(uname -s) in
  Linux)
    if [ -f /etc/apt/sources.list ]; then
      platform=debian
    else
      echo 'Unknown Linux distribution.' >&2
      exit 1
    fi
    ;;

  Darwin)
    platform=macos
    ;;

  *)
    echo "Unknown platform `uname`." >&2
    exit 1
esac

. ${XMLWRAPP_SOURCE_DIR}/scripts/ci/install-${platform}.sh
