//  (C) Copyright Boost.org 2001. Permission to copy, use, modify, sell and
//  distribute this software is granted provided this copyright notice appears
//  in all copies. This software is provided "as is" without express or implied
//  warranty, and with no claim as to its suitability for any purpose.

//  See http://www.boost.org for most recent version.

//  config for libstdc++ v3
//  not much to go in here:

#define XMLWRAPP_BOOST_STDLIB "GNU libstdc++ version " XMLWRAPP_BOOST_STRINGIZE(__GLIBCPP__)

#ifndef _GLIBCPP_USE_WCHAR_T
#  define XMLWRAPP_BOOST_NO_CWCHAR
#  define XMLWRAPP_BOOST_NO_CWCTYPE
#  define XMLWRAPP_BOOST_NO_STD_WSTRING
#  define XMLWRAPP_BOOST_NO_STD_WSTREAMBUF
#endif
 
#ifndef _GLIBCPP_USE_LONG_LONG
// May have been set by compiler/*.hpp, but "long long" without library
// support is useless.
#  undef XMLWRAPP_BOOST_HAS_LONG_LONG
#endif

