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
#include "node_reference.h"
#include "pimpl.h"
#include "const_node_interface.h"

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
    typedef object_T<Access> result_type;
    //####################################################################
    //! Reference to an xml::node with required access rights.
    //! Prevents read-write context_T from operating on read-only node.
    //! @see xmlwrapp::access::restrict
    typedef xml::node_reference<Access> node_ref;
    //####################################################################
    //! Reference to an xml::document with required access rights.
    //! Prevents read-write context_T from operating on read-only document.
    //! @see xmlwrapp::access::restrict
    typedef XMLWRAPP_RESTRICT(Access, xml::document) &document_ref;

    //####################################################################
    //! Create an empty context.
    //! @author Shane Beasley
    context_T ();
    //####################################################################
    //! Create a context with respect to the given document.
    //! @author Shane Beasley
    explicit context_T (document_ref doc);
    //####################################################################
    //! Create a context with respect to the given node.
    //! @author Shane Beasley
    explicit context_T (node_ref node);
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
    void set_document (document_ref doc);
    //####################################################################
    //! Perform future XPath queries in this context at the given node.
    //! @author Shane Beasley
    void set_node (node_ref node);
    //####################################################################
    //! Swap with another context.
    //! @author Shane Beasley
    void swap (context_T &rhs);
    //####################################################################
    //! Perform an XPath query.
    //! @author Shane Beasley
    result_type operator[] (const expression &query);
    //####################################################################
    //! Set a variable.
    //! @author Shane Beasley
    void set (const std::string &name, const xpath::const_object &obj);
    //####################################################################
    //! Unset a variable.
    //! @author Shane Beasley
    void unset (const std::string &name);
    //####################################################################
    //! Get a variable.
    //! @author Shane Beasley
    result_type get (const std::string &name);

  private:
    typedef XMLWRAPP_IMPL_T(context_T) impl;
    XMLWRAPP_PIMPL(shared_ptr, impl) pimpl_;
    XMLWRAPP_FRIEND_T(object_T);
  };

  //####################################################################
  //! Perform an XPath query.
  //! @author Shane Beasley
  template <XMLWRAPP_ACCESS_SPECIFIER Access>
  object_T<Access>
  query (const xml::node_reference_T<Access> &node, const expression &query);

  //####################################################################
  //! Perform an XPath query.
  //! @author Shane Beasley
  read_write::object query (xml::document &doc, const expression &query);
  //####################################################################
  //! Perform an XPath query.
  //! @author Shane Beasley
  read_only::object query (const xml::document &doc, const expression &query);
}

#endif
