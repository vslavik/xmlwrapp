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

#ifndef XPATH_OBJECT_H_20030714T1820
#define XPATH_OBJECT_H_20030714T1820

//####################################################################
/** @file
 * Definition of xpath::object_T
 **/

#include "access.h"
#include "expression.h" // included to allow implicit conversion
#include "node_set_iterator.h"
#include "pimpl.h"
#include "reference.h"
#include "xpath_fwd.h"
#include <string>

namespace xml {
  class node;
}

namespace xpath {
  //####################################################################
  //! XPath polymorphic object.
  //! Can be any one of boolean, floating-point number, string, or node set.
  //! @see xpath::object
  //! @see xpath::const_object
  template <XMLWRAPP_ACCESS_SPECIFIER Access>
  class object_T {
  public:
    //####################################################################
    //! Type of object represented by a object_T.
    enum object_type {
      undefined_type = 0, //!< unknown type
      node_set_type  = 1, //!< node set
      boolean_type   = 2, //!< boolean
      number_type    = 3, //!< floating-point number
      string_type    = 4  //!< string
    };
    //####################################################################
    //! \link xmlwrapp::access Const-correct\endlink instantiation of context_T.
    typedef context_T<Access>             restricted_context;
    //####################################################################
    //! context_T with read-write access rights.
    //! Allows read-only object_T to operate on read-write context_T.
    //! @see #XMLWRAPP_RW_ACCESS
    typedef context_T<XMLWRAPP_RW_ACCESS> rw_context;
    //####################################################################
    //! Type used to describe offsets from zero for indices.
    typedef unsigned int                  size_type;
    //####################################################################
    //! node_set_iterator_T allowing the same access as this object_T has.
    typedef node_set_iterator_T<Access>   iterator;
    //####################################################################
    //! node_set_iterator_T allowing read-only access.
    typedef node_set_iterator_T<XMLWRAPP_RO_ACCESS>   const_iterator;
    //####################################################################
    //! Pointer returned by iterator.
    typedef typename iterator::pointer    pointer;
    //####################################################################
    //! Reference returned by iterator.
    typedef typename iterator::reference  reference;

    //####################################################################
    //! Create a new XPath object.
    //! @author Shane Beasley
    object_T ();
    //####################################################################
    //! Create a new XPath object.
    //! @author Shane Beasley
    object_T (bool val);
    //####################################################################
    //! Create a new XPath object.
    //! @author Shane Beasley
    object_T (double val);
    //####################################################################
    //! Create a new XPath object.
    //! @author Shane Beasley
    object_T (const char *val);
    //####################################################################
    //! Create a new XPath object.
    //! @author Shane Beasley
    object_T (const std::string &val);
    //####################################################################
    //! Allow copy/conversion to read-write or read-only from read-write.
    //! @author Shane Beasley
    object_T (const object_T<XMLWRAPP_RW_ACCESS> &);
    //####################################################################
    //! Perform an XPath query.
    //! @author Shane Beasley
    object_T (restricted_context &ctxt, const expression &query);
    //####################################################################
    //! Perform an XPath query.
    //! @author Shane Beasley
    object_T (detail::reference<rw_context> ctxt, const expression &query);
    //####################################################################
    //! Swap with another object.
    //! @author Shane Beasley
    void swap (object_T &);
    //####################################################################
    //! Destroy this object.
    //! @author Shane Beasley
    ~object_T ();
    //####################################################################
    //! Query the type of the object.
    //! @author Shane Beasley
    object_type type () const;
    //####################################################################
    //! Convert this object to a number.
    //! @author Shane Beasley
    double to_number () const;
    //####################################################################
    //! Convert this object to a string.
    //! @author Shane Beasley
    std::string to_string () const;
    //####################################################################
    //! Convert this object to a boolean.
    //! @author Shane Beasley
    bool to_boolean () const;
    //####################################################################
    //! Determine whether this object is a node set.
    //! @author Shane Beasley
    bool is_node_set () const;
    //####################################################################
    //! Determine whether the node set is empty.
    //! @author Shane Beasley
    bool empty () const;
    //####################################################################
    //! Get the number of elements in the set.
    //! @author Shane Beasley
    size_type size () const;
    //####################################################################
    //! Get an iterator to the first node in the set.
    //! @author Shane Beasley
    iterator begin () const;
    //####################################################################
    //! Get an iterator just past the last node in the set.
    //! @author Shane Beasley
    iterator end () const;
    //####################################################################
    //! Get an element of the set by index.
    //! @author Shane Beasley
    reference operator[] (size_type i) const;
    //####################################################################
    //! Get the first node in the set.
    //! @author Shane Beasley
    reference operator* () const { return begin().operator*(); }
    //####################################################################
    //! Get a pointer to the first node in the set.
    //! @author Shane Beasley
    pointer operator-> () const { return begin().operator->(); }

    //####################################################################
    //! @internal Private implementation class for Pimpl idiom.
    //! @author Shane Beasley
    typedef XMLWRAPP_IMPL_T(object_T) impl;
    //####################################################################
    //! @internal Pointer to private implementation class for Pimpl idiom.
    //! @author Shane Beasley
    typedef XMLWRAPP_PIMPL(shared_ptr, impl) pimpl_type;
    //####################################################################
    //! @internal Get a const reference to the Pimpl pointer.
    //! @author Shane Beasley
    const pimpl_type &pimpl () const { return pimpl_; }
    //####################################################################
    //! @internal Construct an object from its internal representation type.
    //! @author Shane Beasley
    explicit object_T (impl *impl);

  private:
    pimpl_type pimpl_;
  };
}

#endif
