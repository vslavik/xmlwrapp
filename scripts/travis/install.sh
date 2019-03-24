#!/bin/sh
# Used to install dependencies for Travis CI builds.
sudo apt-get install -qq --no-install-recommends doxygen graphviz
sudo apt-get install -qq libxml2-dev libxslt1-dev
