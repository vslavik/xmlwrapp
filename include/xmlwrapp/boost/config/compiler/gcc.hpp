//  (C) Copyright Boost.org 2001. Permission to copy, use, modify, sell and
//  distribute this software is granted provided this copyright notice appears
//  in all copies. This software is provided "as is" without express or implied
//  warranty, and with no claim as to its suitability for any purpose.

//  See http://www.boost.org for most recent version.

//  GNU C++ compiler setup:

#   if __GNUC__ == 2 && __GNUC_MINOR__ == 91
       // egcs 1.1 won't parse shared_ptr.hpp without this:
#      define BOOST_NO_AUTO_PTR
#   endif
#   if __GNUC__ == 2 && __GNUC_MINOR__ < 95
      //
      // Prior to gcc 2.95 member templates only partly
      // work - define BOOST_MSVC6_MEMBER_TEMPLATES
      // instead since inline member templates mostly work.
      //
#     define BOOST_NO_MEMBER_TEMPLATES
#     if __GNUC_MINOR__ >= 9
#       define BOOST_MSVC6_MEMBER_TEMPLATES
#     endif
#   endif

#   if __GNUC__ == 2 && __GNUC_MINOR__ <= 97
#     define BOOST_NO_MEMBER_TEMPLATE_FRIENDS
#     define BOOST_NO_OPERATORS_IN_NAMESPACE
#   endif

//
// Threading support: Turn this on unconditionally here (except for
// MinGW, where we can know for sure). It will get turned off again
// later if no threading API is detected.
//
#if !defined(__MINGW32__) || defined(_MT)
# define BOOST_HAS_THREADS
#endif 

//
// gcc has "long long"
//
#define BOOST_HAS_LONG_LONG

//
// gcc implements the named return value optimization since version 3.1
//
#if __GNUC__ > 3 || ( __GNUC__ == 3 && __GNUC_MINOR__ >= 1 )
#define BOOST_HAS_NRVO
#endif

#define BOOST_COMPILER "GNU C++ version " __VERSION__

//
// versions check:
// we don't know gcc prior to version 2.90:
#if (__GNUC__ == 2) && (__GNUC_MINOR__ < 90)
#  error "Compiler not configured - please reconfigure"
#endif
//
// last known and checked version is 3.3:
#if (__GNUC__ > 3) || ((__GNUC__ == 3) && (__GNUC_MINOR__ > 3))
#  if defined(BOOST_ASSERT_CONFIG)
#     error "Unknown compiler version - please run the configure tests and report the results"
#  else
#     warning "Unknown compiler version - please run the configure tests and report the results"
#  endif
#endif
