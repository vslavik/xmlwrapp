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

/** @file
 * forward-declarations and typedefs for the various XPath types. */

#ifndef XPATH_FWD_H_20030714T1820
#define XPATH_FWD_H_20030714T1820

#include "access.h"

namespace xmlwrapp {
  namespace access { }
  namespace detail { }
  namespace pimpl { }
}

//####################################################################
//! Identifiers pertaining to the xmlwrapp XPath implementation.
namespace xpath {
  //####################################################################
  // forward declarations
  class expression;
  template <XMLWRAPP_ACCESS_SPECIFIER> class context_T;
  template <XMLWRAPP_ACCESS_SPECIFIER> class node_set_iterator_T;
  template <XMLWRAPP_ACCESS_SPECIFIER> class object_T;

  //####################################################################
  //! @name Imported namespaces
  //! @author Shane Beasley
  //@{
  //! @brief
  namespace access = xmlwrapp::access;
  //! @brief
  namespace detail = xmlwrapp::detail;
  //! @brief
  namespace pimpl  = xmlwrapp::pimpl;
  //@}
  //####################################################################
  //! @name Read-write XPath objects
  //! Classes for making read-write queries on an XML document.
  //! @see xmlwrapp::access
  //@{
    typedef context_T<XMLWRAPP_RW_ACCESS>           context;
    typedef node_set_iterator_T<XMLWRAPP_RW_ACCESS> node_set_iterator;
    typedef object_T<XMLWRAPP_RW_ACCESS>            object;
  //@}
  //####################################################################
  //! @name Read-only XPath objects
  //! Classes for making read-only queries on an XML document.
  //! @see xmlwrapp::access
  //@{
    typedef context_T<XMLWRAPP_RO_ACCESS>           const_context;
    typedef node_set_iterator_T<XMLWRAPP_RO_ACCESS> const_node_set_iterator;
    typedef object_T<XMLWRAPP_RO_ACCESS>            const_object;
  //@}
}

#endif
