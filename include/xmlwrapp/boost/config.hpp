//  Boost config.hpp configuration header file  ------------------------------//

//  (C) Copyright Boost.org 2001. Permission to copy, use, modify, sell and
//  distribute this software is granted provided this copyright notice appears
//  in all copies. This software is provided "as is" without express or implied
//  warranty, and with no claim as to its suitability for any purpose.

//  See http://www.boost.org/libs/config for most recent version.

//  Boost config.hpp policy and rationale documentation has been moved to
//  http://www.boost.org/libs/config
//
//  CAUTION: This file is intended to be completely stable -
//           DO NOT MODIFY THIS FILE!
//

#ifndef XMLWRAPP_BOOST_CONFIG_HPP
#define XMLWRAPP_BOOST_CONFIG_HPP

// if we don't have a user config, then use the default location:
#if !defined(XMLWRAPP_BOOST_USER_CONFIG) && !defined(XMLWRAPP_BOOST_NO_USER_CONFIG)
#  define XMLWRAPP_BOOST_USER_CONFIG <xmlwrapp/boost/config/user.hpp>
#endif
// include it first:
#ifdef XMLWRAPP_BOOST_USER_CONFIG
#  include XMLWRAPP_BOOST_USER_CONFIG
#endif

// if we don't have a compiler config set, try and find one:
#if !defined(XMLWRAPP_BOOST_COMPILER_CONFIG) && !defined(XMLWRAPP_BOOST_NO_COMPILER_CONFIG) && !defined(XMLWRAPP_BOOST_NO_CONFIG)
#  include <xmlwrapp/boost/config/select_compiler_config.hpp>
#endif
// if we have a compiler config, include it now:
#ifdef XMLWRAPP_BOOST_COMPILER_CONFIG
#  include XMLWRAPP_BOOST_COMPILER_CONFIG
#endif

// if we don't have a std library config set, try and find one:
#if !defined(XMLWRAPP_BOOST_STDLIB_CONFIG) && !defined(XMLWRAPP_BOOST_NO_STDLIB_CONFIG) && !defined(XMLWRAPP_BOOST_NO_CONFIG)
#  include <xmlwrapp/boost/config/select_stdlib_config.hpp>
#endif
// if we have a std library config, include it now:
#ifdef XMLWRAPP_BOOST_STDLIB_CONFIG
#  include XMLWRAPP_BOOST_STDLIB_CONFIG
#endif

// if we don't have a platform config set, try and find one:
#if !defined(XMLWRAPP_BOOST_PLATFORM_CONFIG) && !defined(XMLWRAPP_BOOST_NO_PLATFORM_CONFIG) && !defined(XMLWRAPP_BOOST_NO_CONFIG)
#  include <xmlwrapp/boost/config/select_platform_config.hpp>
#endif
// if we have a platform config, include it now:
#ifdef XMLWRAPP_BOOST_PLATFORM_CONFIG
#  include XMLWRAPP_BOOST_PLATFORM_CONFIG
#endif

// get config suffix code:
#include <xmlwrapp/boost/config/suffix.hpp>

#endif  // XMLWRAPP_BOOST_CONFIG_HPP










