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
  //! Instantiations of templates using read-write access policy.
  namespace read_write {
    //! @name Read-write XPath objects
    //! @see xmlwrapp::access::read_write
    //@{
    typedef context_T<XMLWRAPP_RW_ACCESS>           context;
    typedef node_set_iterator_T<XMLWRAPP_RW_ACCESS> node_set_iterator;
    typedef object_T<XMLWRAPP_RW_ACCESS>            object, result;
    //@}
  }
  //####################################################################
  //! Instantiations of templates using read-only access policy.
  namespace read_only {
    //! @name Read-only XPath objects
    //! @see xmlwrapp::access::read_only
    //@{
    typedef context_T<XMLWRAPP_RO_ACCESS>           context;
    typedef node_set_iterator_T<XMLWRAPP_RO_ACCESS> node_set_iterator;
    typedef object_T<XMLWRAPP_RO_ACCESS>            object, result;
    //@}
  }

  //! @name Read-write XPath objects
  //! @see xmlwrapp::access::read_write
  //@{
  typedef read_write::context           context;
  typedef read_write::object            object;
  typedef read_write::node_set_iterator node_set_iterator;
  //@}

  //! @name Read-only XPath objects
  //! @see xmlwrapp::access::read_only
  //@{
  typedef read_only::context            const_context;
  typedef read_only::object             const_object;
  typedef read_only::node_set_iterator  const_node_set_iterator;
  //@}
}

#endif
