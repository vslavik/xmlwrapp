/*
 * Copyright (c) 2003 Shane Beasley
 *
 * Permission is hereby granted, free of charge, to any person
 * obtaining a copy of this software and associated documentation
 * files (the "Software"), to deal in the Software without restriction,
 * including without limitation the rights to use, copy, modify, merge,
 * publish, distribute, sublicense, and/or sell copies of the Software,
 * and to permit persons to whom the Software is furnished to do so,
 * subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 * IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
 * CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
 * TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
 * SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

#ifndef XMLWRAPP_PIMPL_H_20030714T1820
#define XMLWRAPP_PIMPL_H_20030714T1820

/** @file
 * Functions and classes related to the Pimpl idiom.
 * @see http://www.gotw.ca/gotw/024.htm
 **/

#include "boost/smart_ptr.hpp"

namespace xmlwrapp {
  //####################################################################
  //! Code related to the Pimpl idiom.
  //! @see http://www.gotw.ca/gotw/024.htm
  namespace pimpl {
    //####################################################################
    //! @struct impl<T>
    //! Declared but undefined private implementation type.
    template <typename T> struct impl;
    //####################################################################
  }
}
//####################################################################
//! A smart pointer (ptr = shared_ptr or scoped_ptr) to T.
//! @see http://boost.org/libs/smart_ptr/smart_ptr.htm
//! @author Shane Beasley
#define XMLWRAPP_PIMPL(ptr, T) ::boost::ptr<T>
//####################################################################
//! The private implementation type for T.
//! @author Shane Beasley
#define XMLWRAPP_IMPL(T) ::xmlwrapp::pimpl::impl<T>
//####################################################################
//! The private implementation type for
//! T<\link #XMLWRAPP_RW_ACCESS XMLWRAPP_RW_ACCESS\endlink>.
//! @author Shane Beasley
#define XMLWRAPP_IMPL_T(T) \
  ::xmlwrapp::pimpl::impl< T<XMLWRAPP_RW_ACCESS> >
//####################################################################

#endif
