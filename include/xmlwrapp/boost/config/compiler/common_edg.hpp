//  (C) Copyright Boost.org 2001. Permission to copy, use, modify, sell and
//  distribute this software is granted provided this copyright notice appears
//  in all copies. This software is provided "as is" without express or implied
//  warranty, and with no claim as to its suitability for any purpose.

//  See http://www.boost.org for most recent version.

//
// Options common to all edg based compilers.
//
// This is included from within the individual compiler mini-configs.

#ifndef  __EDG_VERSION__
#  error This file requires that __EDG_VERSION__ be defined.
#endif

#if (__EDG_VERSION__ <= 238)
#   define XMLWRAPP_BOOST_NO_INTEGRAL_INT64_T
#endif

#if (__EDG_VERSION__ <= 240)
#   define XMLWRAPP_BOOST_NO_VOID_RETURNS
#endif

#if (__EDG_VERSION__ <= 241) && !defined(XMLWRAPP_BOOST_NO_ARGUMENT_DEPENDENT_LOOKUP)
#   define XMLWRAPP_BOOST_NO_ARGUMENT_DEPENDENT_LOOKUP
#endif

#if (__EDG_VERSION__ <= 244) && !defined(XMLWRAPP_BOOST_NO_TEMPLATE_TEMPLATES)
#   define XMLWRAPP_BOOST_NO_TEMPLATE_TEMPLATES
#endif 

// See also kai.hpp which checks a Kai-specific symbol for EH
# if !defined(__KCC) && !defined(__EXCEPTIONS)
#     define XMLWRAPP_BOOST_NO_EXCEPTIONS
# endif

# if !defined(__NO_LONG_LONG)
#     define XMLWRAPP_BOOST_HAS_LONG_LONG
# endif

#ifdef c_plusplus
// EDG has "long long" in non-strict mode
// However, some libraries have insufficient "long long" support
// #define XMLWRAPP_BOOST_HAS_LONG_LONG
#endif


