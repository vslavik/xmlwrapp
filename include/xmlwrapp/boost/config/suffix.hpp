//  Boost config.hpp configuration header file  ------------------------------//

//  (C) Copyright Boost.org 2001. Permission to copy, use, modify, sell and
//  distribute this software is granted provided this copyright notice appears
//  in all copies. This software is provided "as is" without express or implied
//  warranty, and with no claim as to its suitability for any purpose.

//  See http://www.boost.org for most recent version.

//  Boost config.hpp policy and rationale documentation has been moved to
//  http://www.boost.org/libs/config
//
//  This file is intended to be stable, and relatively unchanging.
//  It should contain boilerplate code only - no compiler specific
//  code unless it is unavoidable - no changes unless unavoidable.

#ifndef XMLWRAPP_BOOST_CONFIG_SUFFIX_HPP
#define XMLWRAPP_BOOST_CONFIG_SUFFIX_HPP

//
// look for long long by looking for the appropriate macros in <limits.h>.
// Note that we use limits.h rather than climits for maximal portability,
// remember that since these just declare a bunch of macros, there should be
// no namespace issues from this.
//
#include <limits.h>
# if !defined(XMLWRAPP_BOOST_HAS_LONG_LONG)                                              \
   && !(defined(XMLWRAPP_BOOST_MSVC) && XMLWRAPP_BOOST_MSVC <=1300) && !defined(__BORLANDC__)     \
   && (defined(ULLONG_MAX) || defined(ULONG_LONG_MAX) || defined(ULONGLONG_MAX))
#  define XMLWRAPP_BOOST_HAS_LONG_LONG
#endif
#if !defined(XMLWRAPP_BOOST_HAS_LONG_LONG) && !defined(XMLWRAPP_BOOST_NO_INTEGRAL_INT64_T)
#  define XMLWRAPP_BOOST_NO_INTEGRAL_INT64_T
#endif

// GCC 3.x will clean up all of those nasty macro definitions that
// XMLWRAPP_BOOST_NO_CTYPE_FUNCTIONS is intended to help work around, so undefine
// it under GCC 3.x.
#if defined(__GNUC__) && (__GNUC__ >= 3) && defined(XMLWRAPP_BOOST_NO_CTYPE_FUNCTIONS)
#  undef XMLWRAPP_BOOST_NO_CTYPE_FUNCTIONS
#endif


//
// Assume any extensions are in namespace std:: unless stated otherwise:
//
#  ifndef XMLWRAPP_BOOST_STD_EXTENSION_NAMESPACE
#    define XMLWRAPP_BOOST_STD_EXTENSION_NAMESPACE std
#  endif

//
// If cv-qualified specializations are not allowed, then neither are cv-void ones:
//
#  if defined(XMLWRAPP_BOOST_NO_CV_SPECIALIZATIONS) \
      && !defined(XMLWRAPP_BOOST_NO_CV_VOID_SPECIALIZATIONS)
#     define XMLWRAPP_BOOST_NO_CV_VOID_SPECIALIZATIONS
#  endif

//
// If there is no numeric_limits template, then it can't have any compile time
// constants either!
//
#  if defined(XMLWRAPP_BOOST_NO_LIMITS) \
      && !defined(XMLWRAPP_BOOST_NO_LIMITS_COMPILE_TIME_CONSTANTS)
#     define XMLWRAPP_BOOST_NO_LIMITS_COMPILE_TIME_CONSTANTS
#     define XMLWRAPP_BOOST_NO_MS_INT64_NUMERIC_LIMITS
#     define XMLWRAPP_BOOST_NO_LONG_LONG_NUMERIC_LIMITS
#  endif

//
// if there is no long long then there is no specialisation
// for numeric_limits<long long> either:
//
#if !defined(XMLWRAPP_BOOST_HAS_LONG_LONG) && !defined(XMLWRAPP_BOOST_NO_LONG_LONG_NUMERIC_LIMITS)
#  define XMLWRAPP_BOOST_NO_LONG_LONG_NUMERIC_LIMITS
#endif

//
// if there is no __int64 then there is no specialisation
// for numeric_limits<__int64> either:
//
#if !defined(XMLWRAPP_BOOST_HAS_MS_INT64) && !defined(XMLWRAPP_BOOST_NO_MS_INT64_NUMERIC_LIMITS)
#  define XMLWRAPP_BOOST_NO_MS_INT64_NUMERIC_LIMITS
#endif

//
// if member templates are supported then so is the
// VC6 subset of member templates:
//
#  if !defined(XMLWRAPP_BOOST_NO_MEMBER_TEMPLATES) \
       && !defined(XMLWRAPP_BOOST_MSVC6_MEMBER_TEMPLATES)
#     define XMLWRAPP_BOOST_MSVC6_MEMBER_TEMPLATES
#  endif

//
// Without partial specialization, can't test for partial specialisation bugs:
//
#  if defined(XMLWRAPP_BOOST_NO_TEMPLATE_PARTIAL_SPECIALIZATION) \
      && !defined(XMLWRAPP_BOOST_BCB_PARTIAL_SPECIALIZATION_BUG)
#     define XMLWRAPP_BOOST_BCB_PARTIAL_SPECIALIZATION_BUG
#  endif

//
// Without partial specialization, std::iterator_traits can't work:
//
#  if defined(XMLWRAPP_BOOST_NO_TEMPLATE_PARTIAL_SPECIALIZATION) \
      && !defined(XMLWRAPP_BOOST_NO_STD_ITERATOR_TRAITS)
#     define XMLWRAPP_BOOST_NO_STD_ITERATOR_TRAITS
#  endif

//
// Without member template support, we can't have template constructors
// in the standard library either:
//
#  if defined(XMLWRAPP_BOOST_NO_MEMBER_TEMPLATES) \
      && !defined(XMLWRAPP_BOOST_MSVC6_MEMBER_TEMPLATES) \
      && !defined(XMLWRAPP_BOOST_NO_TEMPLATED_ITERATOR_CONSTRUCTORS)
#     define XMLWRAPP_BOOST_NO_TEMPLATED_ITERATOR_CONSTRUCTORS
#  endif

//
// Without member template support, we can't have a conforming
// std::allocator template either:
//
#  if defined(XMLWRAPP_BOOST_NO_MEMBER_TEMPLATES) \
      && !defined(XMLWRAPP_BOOST_MSVC6_MEMBER_TEMPLATES) \
      && !defined(XMLWRAPP_BOOST_NO_STD_ALLOCATOR)
#     define XMLWRAPP_BOOST_NO_STD_ALLOCATOR
#  endif

//
// If we have a standard allocator, then we have a partial one as well:
//
#if !defined(XMLWRAPP_BOOST_NO_STD_ALLOCATOR)
#  define XMLWRAPP_BOOST_HAS_PARTIAL_STD_ALLOCATOR
#endif

//
// We can't have a working std::use_facet if there is no std::locale:
//
#  if defined(XMLWRAPP_BOOST_NO_STD_LOCALE) && !defined(XMLWRAPP_BOOST_NO_STD_USE_FACET)
#     define XMLWRAPP_BOOST_NO_STD_USE_FACET
#  endif

//
// We can't have a std::messages facet if there is no std::locale:
//
#  if defined(XMLWRAPP_BOOST_NO_STD_LOCALE) && !defined(XMLWRAPP_BOOST_NO_STD_MESSAGES)
#     define XMLWRAPP_BOOST_NO_STD_MESSAGES
#  endif

//
// We can't have a working std::wstreambuf if there is no std::locale:
//
#  if defined(XMLWRAPP_BOOST_NO_STD_LOCALE) && !defined(XMLWRAPP_BOOST_NO_STD_WSTREAMBUF)
#     define XMLWRAPP_BOOST_NO_STD_WSTREAMBUF
#  endif

//
// We can't have a <cwctype> if there is no <cwchar>:
//
#  if defined(XMLWRAPP_BOOST_NO_CWCHAR) && !defined(XMLWRAPP_BOOST_NO_CWCTYPE)
#     define XMLWRAPP_BOOST_NO_CWCTYPE
#  endif

//
// We can't have a swprintf if there is no <cwchar>:
//
#  if defined(XMLWRAPP_BOOST_NO_CWCHAR) && !defined(XMLWRAPP_BOOST_NO_SWPRINTF)
#     define XMLWRAPP_BOOST_NO_SWPRINTF
#  endif

//
// If Win32 support is turned off, then we must turn off
// threading support also, unless there is some other
// thread API enabled:
//
#if defined(XMLWRAPP_BOOST_DISABLE_WIN32) && defined(_WIN32) \
   && !defined(XMLWRAPP_BOOST_DISABLE_THREADS) && !defined(XMLWRAPP_BOOST_HAS_PTHREADS)
#  define XMLWRAPP_BOOST_DISABLE_THREADS
#endif

//
// Turn on threading support if the compiler thinks that it's in
// multithreaded mode.  We put this here because there are only a
// limited number of macros that identify this (if there's any missing
// from here then add to the appropriate compiler section):
//
#if (defined(__MT__) || defined(_MT) || defined(_REENTRANT) \
    || defined(_PTHREADS)) && !defined(XMLWRAPP_BOOST_HAS_THREADS)
#  define XMLWRAPP_BOOST_HAS_THREADS
#endif

//
// Turn threading support off if XMLWRAPP_BOOST_DISABLE_THREADS is defined:
//
#if defined(XMLWRAPP_BOOST_DISABLE_THREADS) && defined(XMLWRAPP_BOOST_HAS_THREADS)
#  undef XMLWRAPP_BOOST_HAS_THREADS
#endif

//
// Turn threading support off if we don't recognise the threading API:
//
#if defined(XMLWRAPP_BOOST_HAS_THREADS) && !defined(XMLWRAPP_BOOST_HAS_PTHREADS)\
      && !defined(XMLWRAPP_BOOST_HAS_WINTHREADS) && !defined(XMLWRAPP_BOOST_HAS_BETHREADS)\
      && !defined(XMLWRAPP_BOOST_HAS_MPTASKS)
#  undef XMLWRAPP_BOOST_HAS_THREADS
#endif

//
// If the compiler claims to be C99 conformant, then it had better
// have a <stdint.h>:
//
#  if defined(__STDC_VERSION__) && (__STDC_VERSION__ >= 199901)
#     define XMLWRAPP_BOOST_HAS_STDINT_H
#  endif

//
// Define XMLWRAPP_BOOST_NO_SLIST and XMLWRAPP_BOOST_NO_HASH if required.
// Note that this is for backwards compatibility only.
//
#  ifndef XMLWRAPP_BOOST_HAS_SLIST
#     define XMLWRAPP_BOOST_NO_SLIST
#  endif

#  ifndef XMLWRAPP_BOOST_HAS_HASH
#     define XMLWRAPP_BOOST_NO_HASH
#  endif

//  XMLWRAPP_BOOST_NO_STDC_NAMESPACE workaround  --------------------------------------//
//  Because std::size_t usage is so common, even in boost headers which do not
//  otherwise use the C library, the <cstddef> workaround is included here so
//  that ugly workaround code need not appear in many other boost headers.
//  NOTE WELL: This is a workaround for non-conforming compilers; <cstddef> 
//  must still be #included in the usual places so that <cstddef> inclusion
//  works as expected with standard conforming compilers.  The resulting
//  double inclusion of <cstddef> is harmless.

# ifdef XMLWRAPP_BOOST_NO_STDC_NAMESPACE
#   include <cstddef>
    namespace std { using ::ptrdiff_t; using ::size_t; }
# endif

//  XMLWRAPP_BOOST_NO_STD_MIN_MAX workaround  -----------------------------------------//

#  ifdef XMLWRAPP_BOOST_NO_STD_MIN_MAX

namespace std {
  template <class _Tp>
  inline const _Tp& min(const _Tp& __a, const _Tp& __b) {
    return __b < __a ? __b : __a;
  }
  template <class _Tp>
  inline const _Tp& max(const _Tp& __a, const _Tp& __b) {
    return  __a < __b ? __b : __a;
  }
}

#  endif

// XMLWRAPP_BOOST_STATIC_CONSTANT workaround --------------------------------------- //
// On compilers which don't allow in-class initialization of static integral
// constant members, we must use enums as a workaround if we want the constants
// to be available at compile-time. This macro gives us a convenient way to
// declare such constants.

#  ifdef XMLWRAPP_BOOST_NO_INCLASS_MEMBER_INITIALIZATION
#       define XMLWRAPP_BOOST_STATIC_CONSTANT(type, assignment) enum { assignment }
#  else
#     define XMLWRAPP_BOOST_STATIC_CONSTANT(type, assignment) static const type assignment
#  endif

// XMLWRAPP_BOOST_USE_FACET workaround ----------------------------------------------//
// When the standard library does not have a conforming std::use_facet there
// are various workarounds available, but they differ from library to library.
// This macro provides a consistent way to access a locale's facets.
// Usage:
//    replace
//       std::use_facet<Type>(loc);
//    with
//       XMLWRAPP_BOOST_USE_FACET(Type, loc);
//    Note do not add a std:: prefix to the front of XMLWRAPP_BOOST_USE_FACET!

#if defined(XMLWRAPP_BOOST_NO_STD_USE_FACET)
#  ifdef XMLWRAPP_BOOST_HAS_TWO_ARG_USE_FACET
#     define XMLWRAPP_BOOST_USE_FACET(Type, loc) std::use_facet(loc, static_cast<Type*>(0))
#  elif defined(XMLWRAPP_BOOST_HAS_MACRO_USE_FACET)
#     define XMLWRAPP_BOOST_USE_FACET(Type, loc) std::_USE(loc, Type)
#  elif defined(XMLWRAPP_BOOST_HAS_STLP_USE_FACET)
#     define XMLWRAPP_BOOST_USE_FACET(Type, loc) (*std::_Use_facet<Type >(loc))
#  endif
#else
#  define XMLWRAPP_BOOST_USE_FACET(Type, loc) std::use_facet< Type >(loc)
#endif

// XMLWRAPP_BOOST_NESTED_TEMPLATE workaround ------------------------------------------//
// Member templates are supported by some compilers even though they can't use
// the A::template member<U> syntax, as a workaround replace:
//
// typedef typename A::template rebind<U> binder;
//
// with:
//
// typedef typename A::XMLWRAPP_BOOST_NESTED_TEMPLATE rebind<U> binder;

#ifndef XMLWRAPP_BOOST_NO_MEMBER_TEMPLATE_KEYWORD
#  define XMLWRAPP_BOOST_NESTED_TEMPLATE template
#else
#  define XMLWRAPP_BOOST_NESTED_TEMPLATE
#endif

// XMLWRAPP_BOOST_UNREACHABLE_RETURN(x) workaround -------------------------------------//
// Normally evaluates to nothing, unless XMLWRAPP_BOOST_NO_UNREACHABLE_RETURN_DETECTION
// is defined, in which case it evaluates to return x; Use when you have a return
// statement that can never be reached.

#ifdef XMLWRAPP_BOOST_NO_UNREACHABLE_RETURN_DETECTION
#  define XMLWRAPP_BOOST_UNREACHABLE_RETURN(x) return x;
#else
#  define XMLWRAPP_BOOST_UNREACHABLE_RETURN(x)
#endif

// XMLWRAPP_BOOST_DEDUCED_TYPENAME workaround ------------------------------------------//
//
// Some compilers don't support the use of `typename' for dependent
// types in deduced contexts, e.g.
//
//     template <class T> void f(T, typename T::type);
//                                  ^^^^^^^^
// Replace these declarations with:
//
//     template <class T> void f(T, XMLWRAPP_BOOST_DEDUCED_TYPENAME T::type);

#ifndef XMLWRAPP_BOOST_NO_DEDUCED_TYPENAME
#  define XMLWRAPP_BOOST_DEDUCED_TYPENAME typename
#else 
#  define XMLWRAPP_BOOST_DEDUCED_TYPENAME
#endif

// ---------------------------------------------------------------------------//

//
// Helper macro XMLWRAPP_BOOST_STRINGIZE:
// Converts the parameter X to a string after macro replacement
// on X has been performed.
//
#define XMLWRAPP_BOOST_STRINGIZE(X) XMLWRAPP_BOOST_DO_STRINGIZE(X)
#define XMLWRAPP_BOOST_DO_STRINGIZE(X) #X

//
// Helper macro XMLWRAPP_BOOST_JOIN:
// The following piece of macro magic joins the two 
// arguments together, even when one of the arguments is
// itself a macro (see 16.3.1 in C++ standard).  The key
// is that macro expansion of macro arguments does not
// occur in XMLWRAPP_BOOST_DO_JOIN2 but does in XMLWRAPP_BOOST_DO_JOIN.
//
#define XMLWRAPP_BOOST_JOIN( X, Y ) XMLWRAPP_BOOST_DO_JOIN( X, Y )
#define XMLWRAPP_BOOST_DO_JOIN( X, Y ) XMLWRAPP_BOOST_DO_JOIN2(X,Y)
#define XMLWRAPP_BOOST_DO_JOIN2( X, Y ) X##Y

//
// Set some default values for compiler/library/platform names.
// These are for debugging config setup only:
//
#  ifndef XMLWRAPP_BOOST_COMPILER
#     define XMLWRAPP_BOOST_COMPILER "Unknown ISO C++ Compiler"
#  endif
#  ifndef XMLWRAPP_BOOST_STDLIB
#     define XMLWRAPP_BOOST_STDLIB "Unknown ISO standard library"
#  endif
#  ifndef XMLWRAPP_BOOST_PLATFORM
#     if defined(unix) || defined(__unix) || defined(_XOPEN_SOURCE) \
         || defined(_POSIX_SOURCE)
#        define XMLWRAPP_BOOST_PLATFORM "Generic Unix"
#     else
#        define XMLWRAPP_BOOST_PLATFORM "Unknown"
#     endif
#  endif

#endif


