//  Boost compiler configuration selection header file

//  (C) Copyright Boost.org 2001. Permission to copy, use, modify, sell and
//  distribute this software is granted provided this copyright notice appears
//  in all copies. This software is provided "as is" without express or implied
//  warranty, and with no claim as to its suitability for any purpose.

//  See http://www.boost.org for most recent version.

// locate which compiler we are using and define
// XMLWRAPP_BOOST_COMPILER_CONFIG as needed: 

#if defined __GNUC__
//  GNU C++:
#   define XMLWRAPP_BOOST_COMPILER_CONFIG "boost/config/compiler/gcc.hpp"

# elif defined __COMO__
//  Comeau C++
#   define XMLWRAPP_BOOST_COMPILER_CONFIG "boost/config/compiler/comeau.hpp"

#elif defined __KCC
//  Kai C++
#   define XMLWRAPP_BOOST_COMPILER_CONFIG "boost/config/compiler/kai.hpp"

#elif defined __sgi
//  SGI MIPSpro C++
#   define XMLWRAPP_BOOST_COMPILER_CONFIG "boost/config/compiler/sgi_mipspro.hpp"

#elif defined __DECCXX
//  Compaq Tru64 Unix cxx
#   define XMLWRAPP_BOOST_COMPILER_CONFIG "boost/config/compiler/compaq_cxx.hpp"

#elif defined __ghs
//  Greenhills C++
#   define XMLWRAPP_BOOST_COMPILER_CONFIG "boost/config/compiler/greenhills.hpp"

#elif defined __BORLANDC__
//  Borland
#   define XMLWRAPP_BOOST_COMPILER_CONFIG "boost/config/compiler/borland.hpp"

#elif defined(__ICL) || defined(__ICC)
//  Intel
#   define XMLWRAPP_BOOST_COMPILER_CONFIG "boost/config/compiler/intel.hpp"

#elif defined  __MWERKS__
//  Metrowerks CodeWarrior
#   define XMLWRAPP_BOOST_COMPILER_CONFIG "boost/config/compiler/metrowerks.hpp"

#elif defined  __SUNPRO_CC
//  Sun Workshop Compiler C++
#   define XMLWRAPP_BOOST_COMPILER_CONFIG "boost/config/compiler/sunpro_cc.hpp"

#elif defined __HP_aCC
//  HP aCC
#   define XMLWRAPP_BOOST_COMPILER_CONFIG "boost/config/compiler/hp_acc.hpp"

#elif defined(__MRC__) || defined(__SC__)
//  MPW MrCpp or SCpp
#   define XMLWRAPP_BOOST_COMPILER_CONFIG "boost/config/compiler/mpw.hpp"

#elif defined(__IBMCPP__)
//  IBM Visual Age
#   define XMLWRAPP_BOOST_COMPILER_CONFIG "boost/config/compiler/vacpp.hpp"

#elif defined _MSC_VER
//  Microsoft Visual C++
//
//  Must remain the last #elif since some other vendors (Metrowerks, for
//  example) also #define _MSC_VER
#   define XMLWRAPP_BOOST_COMPILER_CONFIG "boost/config/compiler/visualc.hpp"

#elif defined (XMLWRAPP_BOOST_ASSERT_CONFIG)
// this must come last - generate an error if we don't
// recognise the compiler:
#  error "Unknown compiler - please configure and report the results to boost.org"

#endif

