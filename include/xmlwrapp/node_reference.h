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
 * Definition of xml::node_reference_T.
 **/

#ifndef XMLWRAPP_NODE_REFERENCE_H_20030830T011402
#define XMLWRAPP_NODE_REFERENCE_H_20030830T011402

#include "node.h"

namespace xml {
  namespace detail {
    //####################################################################
    // MSVC++6 requires that this template be defined, even though only
    // specializations may be used.
    template <XMLWRAPP_ACCESS_SPECIFIER Access> struct node_ref_traits {
      //####################################################################
      // MSVC++6 requires that this type be defined, even though only
      // specializations may be used.
      typedef class uninstantiable {
      private:
	uninstantiable ();
	uninstantiable (const uninstantiable &);
	uninstantiable &operator= (const uninstantiable &);
	~uninstantiable ();
	friend class no_friends; // g++ warns "all private, no friends"
      } type;
    };

    //####################################################################
    //! Specify the interface to be used for read-only access to a node.
    template <> struct node_ref_traits<XMLWRAPP_READ_ONLY> {
      typedef const_node_interface type;
    };
    //####################################################################
    //! Specify the interface to be used for read-write access to a node.
    template <> struct node_ref_traits<XMLWRAPP_READ_WRITE> {
      typedef node type;
    };
  }

  //####################################################################
  //! A reference with the specified access (read-write or read-only)
  //! to an XML node.
  template <XMLWRAPP_ACCESS_SPECIFIER Access>
  class node_reference_T : public detail::node_ref_traits<Access>::type {
  public:
    typedef typename detail::node_ref_traits<Access>::type base_type;

    //####################################################################
    //! Type accepted when forming a reference (read-write or read-only)
    //! from a read-write reference in order to avoid ambiguity. If forced
    //! to choose between a copy constructor and implicit conversion to
    //! this type, C++ will prefer the copy constructor.
    typedef
    xmlwrapp::detail::reference< const node_reference_T<XMLWRAPP_READ_WRITE> >
    rw_reference;

    //####################################################################
    //! Type of node to which this class is allowed to form a reference.
    //! Forbids a read-write reference from being formed from a reference
    //! to a const node.
    typedef XMLWRAPP_RESTRICT(Access, node) node_type;

    //####################################################################
    //! Create a "null" reference.
    node_reference_T ();
    //####################################################################
    //! Copy a reference.
    node_reference_T (const node_reference_T &rhs);
    //####################################################################
    //! Copy a reference.
    node_reference_T &operator= (const node_reference_T &rhs);
    //####################################################################
    //! Cleanup.
    ~node_reference_T ();

    //####################################################################
    //! Copy a read-write reference.
    node_reference_T (const rw_reference &rhs);
    //####################################################################
    //! Copy a read-write reference.
    node_reference_T &operator= (const rw_reference &rhs);

    //####################################################################
    //! Form a reference to a node with the required constness.
    node_reference_T (node_type &rhs);
    //####################################################################
    //! Form a reference to a node with the required constness.
    node_reference_T &operator= (node_type &rhs);

    //####################################################################
    //! Swap with another reference of the same type.
    void swap (node_reference_T &rhs);
    //####################################################################
    //! Return "this" so that node_set_iterator_T::operator-> works properly.
    //! @author Shane Beasley
    node_reference_T *operator-> ();
    //####################################################################
    //! Return "this" so that node_set_iterator_T::operator-> works properly.
    //! @author Shane Beasley
    const node_reference_T *operator-> () const;
    //####################################################################
    //! @internal Create a node reference from its implementation.
    //! @author Shane Beasley
    explicit node_reference_T (node_impl *impl);
  };
}

#endif
