//  (C) Copyright Boost.org 2001. Permission to copy, use, modify, sell and
//  distribute this software is granted provided this copyright notice appears
//  in all copies. This software is provided "as is" without express or implied
//  warranty, and with no claim as to its suitability for any purpose.

//  See http://www.boost.org for most recent version.

//  cygwin specific config options:

#define XMLWRAPP_BOOST_PLATFORM "Cygwin"
#define XMLWRAPP_BOOST_NO_CWCTYPE
#define XMLWRAPP_BOOST_NO_CWCHAR
#define XMLWRAPP_BOOST_NO_SWPRINTF
#define XMLWRAPP_BOOST_HAS_DIRENT_H

//
// Threading API:
// See if we have POSIX threads, if we do use them, otherwise
// revert to native Win threads.
#define XMLWRAPP_BOOST_HAS_UNISTD_H
#include <unistd.h>
#if defined(_POSIX_THREADS) && (_POSIX_THREADS+0 >= 0) && !defined(XMLWRAPP_BOOST_HAS_WINTHREADS)
#  define XMLWRAPP_BOOST_HAS_PTHREADS
#  define XMLWRAPP_BOOST_HAS_SCHED_YIELD
#  define XMLWRAPP_BOOST_HAS_GETTIMEOFDAY
#  define XMLWRAPP_BOOST_HAS_PTHREAD_MUTEXATTR_SETTYPE
#  define XMLWRAPP_BOOST_HAS_SIGACTION
#else
#  if !defined(XMLWRAPP_BOOST_HAS_WINTHREADS)
#     define XMLWRAPP_BOOST_HAS_WINTHREADS
#  endif
#  define XMLWRAPP_BOOST_HAS_FTIME
#endif

// boilerplate code:
#include <xmlwrapp/boost/config/posix_features.hpp>
 


