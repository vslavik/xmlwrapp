//
//  boost/assert.hpp - XMLWRAPP_BOOST_ASSERT(expr)
//
//  Copyright (c) 2001, 2002 Peter Dimov and Multi Media Ltd.
//
//  Permission to copy, use, modify, sell and distribute this software
//  is granted provided this copyright notice appears in all copies.
//  This software is provided "as is" without express or implied
//  warranty, and with no claim as to its suitability for any purpose.
//
//  Note: There are no include guards. This is intentional.
//
//  See http://www.boost.org/libs/utility/assert.html for documentation.
//

#undef XMLWRAPP_BOOST_ASSERT

#if defined(XMLWRAPP_BOOST_DISABLE_ASSERTS)

# define XMLWRAPP_BOOST_ASSERT(expr) ((void)0)

#elif defined(XMLWRAPP_BOOST_ENABLE_ASSERT_HANDLER)

#include <xmlwrapp/boost/current_function.hpp>

namespace xmlwrapp_boost
{

void assertion_failed(char const * expr, char const * function, char const * file, long line); // user defined

} // namespace xmlwrapp_boost

#define XMLWRAPP_BOOST_ASSERT(expr) ((expr)? ((void)0): ::xmlwrapp_boost::assertion_failed(#expr, XMLWRAPP_BOOST_CURRENT_FUNCTION, __FILE__, __LINE__))

#else
# include <assert.h>
# define XMLWRAPP_BOOST_ASSERT(expr) assert(expr)
#endif
