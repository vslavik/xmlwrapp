//  (C) Copyright Boost.org 2001. Permission to copy, use, modify, sell and
//  distribute this software is granted provided this copyright notice appears
//  in all copies. This software is provided "as is" without express or implied
//  warranty, and with no claim as to its suitability for any purpose.

//  See http://www.boost.org for most recent version.

//  hpux specific config options:

#define XMLWRAPP_BOOST_PLATFORM "HP-UX"

// In principle, HP-UX has a nice <stdint.h> under the name <inttypes.h>
// However, it has the following problem:
// Use of UINT32_C(0) results in "0u l" for the preprocessed source
// (verifyable with gcc 2.95.3, assumed for HP aCC)
// #define XMLWRAPP_BOOST_HAS_STDINT_H

#define XMLWRAPP_BOOST_NO_SWPRINTF 
#define XMLWRAPP_BOOST_NO_CWCTYPE

// boilerplate code:
#define XMLWRAPP_BOOST_HAS_UNISTD_H
#include <xmlwrapp/boost/config/posix_features.hpp>

// HPUX has an incomplete pthreads implementation, so it doesn't
// define _POSIX_THREADS, but it might be enough to implement
// Boost.Threads.
#if !defined(XMLWRAPP_BOOST_HAS_PTHREADS) && defined(_POSIX_THREAD_ATTR_STACKADDR)
# define XMLWRAPP_BOOST_HAS_PTHREADS 
#endif

// the following are always available:
#ifndef XMLWRAPP_BOOST_HAS_GETTIMEOFDAY
#  define XMLWRAPP_BOOST_HAS_GETTIMEOFDAY
#endif
#ifndef XMLWRAPP_BOOST_HAS_SCHED_YIELD
#    define XMLWRAPP_BOOST_HAS_SCHED_YIELD
#endif
#ifndef XMLWRAPP_BOOST_HAS_PTHREAD_MUTEXATTR_SETTYPE
#    define XMLWRAPP_BOOST_HAS_PTHREAD_MUTEXATTR_SETTYPE
#endif
#ifndef XMLWRAPP_BOOST_HAS_NL_TYPES_H
#    define XMLWRAPP_BOOST_HAS_NL_TYPES_H
#endif
#ifndef XMLWRAPP_BOOST_HAS_NANOSLEEP
#    define XMLWRAPP_BOOST_HAS_NANOSLEEP
#endif
#ifndef XMLWRAPP_BOOST_HAS_GETTIMEOFDAY
#    define XMLWRAPP_BOOST_HAS_GETTIMEOFDAY
#endif
#ifndef XMLWRAPP_BOOST_HAS_DIRENT_H
#    define XMLWRAPP_BOOST_HAS_DIRENT_H
#endif
#ifndef XMLWRAPP_BOOST_HAS_CLOCK_GETTIME
#    define XMLWRAPP_BOOST_HAS_CLOCK_GETTIME
#endif
#ifndef XMLWRAPP_BOOST_HAS_SIGACTION
#  define XMLWRAPP_BOOST_HAS_SIGACTION
#endif

