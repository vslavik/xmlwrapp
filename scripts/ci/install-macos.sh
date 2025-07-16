# Used to install dependencies for the CI builds under macOS.

if [ -z "$XMLWRAPP_CMAKE" ]; then
  packages="autoconf automake"
fi

brew install $packages doxygen graphviz
