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

#ifndef XPATH_NODE_REF_H_20030714T1820
#define XPATH_NODE_REF_H_20030714T1820

//####################################################################
/** @file
 * Definition of xpath::node_ref
 **/

#include "access.h"
#include "node.h"
#include "xpath_fwd.h"

namespace xpath {
  //####################################################################
  //! Similar to xml::node but suitable as a proxy for nodes in a set.
  //!
  //! Dereferencing a node_set_iterator_T ideally would result in an xml::node,
  //! except for the following:
  //!
  //! <ul>
  //! <li>
  //! Returning xml::node by value makes a copy, changes to which do not affect
  //! the original. On the other hand, node_ref implements reference semantics;
  //! copies refer to the original, with changes reflected accordingly.
  //!
  //! </li>
  //!
  //! <li>
  //! operator-> cannot return a pointer to a temporary because temporaries are
  //! destroyed when they go out of scope. However, it can return an object which
  //! itself has an operator->.
  //!
  //! If <var>i</var> is not a pointer, i->m means (i.operator->())->m.
  //! In other words, i->m will keep calling overloaded operator-> until it gets
  //! a pointer. Moreover, the values returned by each operator-> exist until the
  //! full expression completes. As such, an object's operator-> can return
  //! "this" and be guaranteed that the object still exists afterward.
  //!
  //! </li>
  //! </ul>
  //!
  //! Additionally, node_ref has the following features:
  //! <ul>
  //! <li>
  //! node_ref derives publicly from xml::node so that it can be used as an
  //! xml::node. It can be bound to a reference to const xml::node, copied
  //! as an xml::node, and used to get or set data in the underlying xml::node.
  //! Furthermore...
  //!
  //! </li>
  //! <li>
  //! A const xml::node cannot be used to modify the underlying node, and
  //! neither can a const node_ref. Therefore, non-mutating iterators disallow
  //! changes to a node by returning a const node_ref to that node.
  //! Additionally, callers cannot defeat constness by copying a const node_ref
  //! into a non-const node_ref because its copy and assignment are private.
  //! They can <em>only</em> bind to a const reference; access xml::node
  //! members or the special node_ref::as_param function; or make a copy
  //! of the xml::node portion, changes to which do not affect the original.
  //!
  //! </li>
  //! </ul>
  class node_ref : public xml::node {
  public:
    //####################################################################
    //! Return "this" so that node_set_iterator_T::operator-> works properly.
    //! @author Shane Beasley
    node_ref *operator-> ();
    //####################################################################
    //! Return "this" so that node_set_iterator_T::operator-> works properly.
    //! @author Shane Beasley
    const node_ref *operator-> () const;
    //####################################################################
    //! Get a non-const reference to a non-const node_ref rvalue for use as a
    //! function parameter.
    //!
    //! @note
    //! The returned reference is valid only for the life of this object,
    //! which is likely only until the full expression which called this
    //! function has completed. Do not store a pointer or reference to the
    //! returned reference -- use it only as a function parameter.
    //! Furthermore, make sure that the function which accepts the reference
    //! does not store any such pointers or references which may exist when
    //! it returns.
    //! @author Shane Beasley
    node_ref &as_param ();
    //####################################################################
    //! Get a const reference to a node_ref rvalue for use as a function
    //! parameter. Exists mostly for symmetry with the non-const version.
    //!
    //! @note
    //! The returned reference is valid only for the life of this object,
    //! which is likely only until the full expression which called this
    //! function has completed. Do not store a pointer or reference to the
    //! returned reference -- use it only as a function parameter.
    //! Furthermore, make sure that the function which accepts the reference
    //! does not store any such pointers or references which may exist when
    //! it returns.
    //! @author Shane Beasley
    const node_ref &as_param () const;

  private:
    XMLWRAPP_FRIEND_T(node_set_iterator_T);
    XMLWRAPP_FRIEND_T(object_T);
    node_ref ();
    node_ref (const node_ref &);
    node_ref &operator= (const node_ref &);
  };
}

#endif
