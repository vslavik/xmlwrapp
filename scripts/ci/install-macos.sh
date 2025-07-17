# Used to install dependencies for the CI builds under macOS.

if [ -z "$XMLWRAPP_CMAKE" ]; then
  brew install automake
fi
