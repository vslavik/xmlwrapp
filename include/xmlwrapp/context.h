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

#ifndef XPATH_CONTEXT_H_20030714T1820
#define XPATH_CONTEXT_H_20030714T1820

#include "access.h"
#include "expression.h" // included to allow implicit conversion
#include "pimpl.h"
#include "xpath_types.h"

//####################################################################
/** @file
 * Definition of xpath::context_T
 **/

namespace xml {
  class document;
  class node;
}

namespace xpath {
  //####################################################################
  //! XPath evaluation context.
  //! XPath expressions are evaluated in a context, which includes the
  //! document/node from which they are issued, plus any state information
  //! the XPath engine would like to keep around during evaluation.
  template <XMLWRAPP_ACCESS_SPECIFIER Access>
  class context_T {
  public:
    //####################################################################
    //! Result of an XPath query.
    typedef result_T<Access> result_type;
    //####################################################################
    //! xml::node with required access rights.
    //! Prevents read-write context_T from operating on read-only node.
    //! @see xmlwrapp::access::restrict
    typedef XMLWRAPP_RESTRICT(Access, xml::node) node;
    //####################################################################
    //! xml::document with required access rights.
    //! Prevents read-write context_T from operating on read-only document.
    //! @see xmlwrapp::access::restrict
    typedef XMLWRAPP_RESTRICT(Access, xml::document) document;

    //####################################################################
    //! Create an empty context.
    //! @author Shane Beasley
    context_T ();
    //####################################################################
    //! Create a context with respect to the given document.
    //! @author Shane Beasley
    explicit context_T (document &);
    //####################################################################
    //! Create a context with respect to the given node.
    //! @author Shane Beasley
    explicit context_T (node &);
    //####################################################################
    //! Create a context with respect to the same document (and node)
    //! as another context.
    //! @author Shane Beasley
    context_T (const context_T &);
    //####################################################################
    //! Alter the context to use the document (and node) of another context.
    //! @author Shane Beasley
    context_T &operator= (const context_T &);
    //####################################################################
    //! Perform future XPath queries in this context on the given document.
    //! @author Shane Beasley
    void set_document (document &);
    //####################################################################
    //! Perform future XPath queries in this context at the given node.
    //! @author Shane Beasley
    void set_node (node &);
    //####################################################################
    //! Swap with another context.
    //! @author Shane Beasley
    void swap (context_T &rhs);
    //####################################################################
    //! Perform an XPath query.
    //! @author Shane Beasley
    result_type operator[] (const expression &query);

  private:
    typedef XMLWRAPP_IMPL_T(context_T) impl;
    XMLWRAPP_PIMPL(shared_ptr, impl) pimpl_;
    XMLWRAPP_FRIEND_T(result_T);
  };

  //####################################################################
  //! Perform an XPath query.
  //! @author Shane Beasley
  template <typename T>
  read_write::result query (T &src, const expression &query) {
    return read_write::context(src)[query];
  }
  //####################################################################
  //! Perform an XPath query.
  //! @author Shane Beasley
  template <typename T>
  read_only::result query (const T &src, const expression &query) {
    return read_only::context(src)[query];
  }
}

#endif
