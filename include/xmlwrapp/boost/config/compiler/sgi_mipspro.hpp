//  (C) Copyright Boost.org 2001. Permission to copy, use, modify, sell and
//  distribute this software is granted provided this copyright notice appears
//  in all copies. This software is provided "as is" without express or implied
//  warranty, and with no claim as to its suitability for any purpose.

//  See http://www.boost.org for most recent version.

//  SGI C++ compiler setup:

#define XMLWRAPP_BOOST_COMPILER "SGI Irix compiler version " XMLWRAPP_BOOST_STRINGIZE(_COMPILER_VERSION)

#include "xmlwrapp/boost/config/compiler/common_edg.hpp"

//
// Threading support:
// Turn this on unconditionally here, it will get turned off again later
// if no threading API is detected.
//
#define XMLWRAPP_BOOST_HAS_THREADS
//
// version check:
// probably nothing to do here?

