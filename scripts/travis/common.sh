# This file is not executable and is only sourced by the other scripts.

# Default setting of CC=gcc and CXX=g++ results in the native compiler being
# picked even when cross-compiling, which breaks the build, so unset these
# variables in this case (we could also set them to $HOST-gcc...).
if [ -n "$HOST" ]; then
    unset CC
    unset CXX
fi

# When using non-default compiler, .travis.yml defines SET_CXX to contain CXX
# (and CC) definitions.
if [ -n "$SET_CXX" ]; then
    eval "$SET_CXX"
fi
