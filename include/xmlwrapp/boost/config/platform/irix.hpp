//  (C) Copyright Boost.org 2001. Permission to copy, use, modify, sell and
//  distribute this software is granted provided this copyright notice appears
//  in all copies. This software is provided "as is" without express or implied
//  warranty, and with no claim as to its suitability for any purpose.

//  See http://www.boost.org for most recent version.

//  SGI Irix specific config options:

#define XMLWRAPP_BOOST_PLATFORM "SGI Irix"

#define XMLWRAPP_BOOST_NO_SWPRINTF 
//
// these are not auto detected by POSIX feature tests:
//
#define XMLWRAPP_BOOST_HAS_GETTIMEOFDAY
#define XMLWRAPP_BOOST_HAS_PTHREAD_MUTEXATTR_SETTYPE


// boilerplate code:
#define XMLWRAPP_BOOST_HAS_UNISTD_H
#include <xmlwrapp/boost/config/posix_features.hpp>


