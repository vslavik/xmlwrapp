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

#ifndef XPATH_NODE_SET_ITERATOR_H_20030714T1820
#define XPATH_NODE_SET_ITERATOR_H_20030714T1820

//####################################################################
/** @file
 * Definition of xpath::node_set_iterator_T
 **/

#include "access.h"
#include "node_reference.h"
#include "pimpl.h"
#include "xpath_fwd.h"
#include <iterator>

namespace xpath {
  //####################################################################
  //! Random-access iterator over a node set.
  template <XMLWRAPP_ACCESS_SPECIFIER Access>
  class node_set_iterator_T {
  public:
    //####################################################################
    //! Inform algorithms that this is a random-access iterator.
    typedef std::random_access_iterator_tag                   iterator_category;
    //####################################################################
    //! Properly restricted xml::node_reference_T.
    typedef xml::node_reference_T<Access>                     value_type;
    //####################################################################
    //! Type used to represent distance between two iterators.
    typedef std::ptrdiff_t                                    difference_type;
    //####################################################################
    //! operator-> returns a properly restricted node_ref.
    typedef value_type                                        pointer;
    //####################################################################
    //! operator* returns a properly restricted node_ref.
    typedef value_type                                        reference;
    //####################################################################
    //! Type used for offsets from zero.
    typedef std::size_t                                       size_type;

    //####################################################################
    //! Default-construct an iterator.
    //! @author Shane Beasley
    node_set_iterator_T ();
    //####################################################################
    //! Copy a read-write iterator.
    //! @author Shane Beasley
    node_set_iterator_T (const read_write::node_set_iterator &);
    //####################################################################
    //! Get a proxy for an xml::node.
    //! @author Shane Beasley
    pointer   operator-> () const;
    //####################################################################
    //! Get a proxy for an xml::node.
    //! @author Shane Beasley
    reference operator* () const;
    //####################################################################
    //! Get a proxy for an xml::node.
    //! @author Shane Beasley
    reference operator[] (size_type i) const;
    //####################################################################
    //! Advance the iterator.
    //! @author Shane Beasley
    node_set_iterator_T &operator++ ();
    //####################################################################
    //! Advance the iterator, returning a copy of its previous value.
    //! @author Shane Beasley
    node_set_iterator_T operator++ (int);
    //####################################################################
    //! Advance the iterator the given number of elements.
    //! @author Shane Beasley
    node_set_iterator_T &operator+= (difference_type);
    //####################################################################
    //! Rewind the iterator.
    //! @author Shane Beasley
    node_set_iterator_T &operator-- ();
    //####################################################################
    //! Rewind the iterator, returning a copy of its previous value.
    //! @author Shane Beasley
    node_set_iterator_T operator-- (int);
    //####################################################################
    //! Rewind the iterator the given number of elements.
    //! @author Shane Beasley
    node_set_iterator_T &operator-= (difference_type);
    //####################################################################
    //! Returns the forward distance from *this to rhs, or (rhs - *this).
    //! @author Shane Beasley
    difference_type distance (const node_set_iterator_T &rhs) const;

    //####################################################################
    //! @internal Private implementation class for Pimpl idiom.
    typedef XMLWRAPP_IMPL_T(node_set_iterator_T) impl;
    //####################################################################
    //! @internal Pointer to private implementation class for Pimpl idiom.
    typedef impl *pimpl_type;
    //####################################################################
    //! @internal Construct an iterator from its private implementation.
    //! @author Shane Beasley
    node_set_iterator_T (impl *);
    //####################################################################
    //! @internal Get a const reference to the Pimpl pointer.
    //! @author Shane Beasley
    const pimpl_type &pimpl () const { return pimpl_; }
    //####################################################################

  private:
    impl *pimpl_;
  };

  //####################################################################
  //####################################################################
  //! @name Arithmetic operators
  //! @author Shane Beasley
  //@{
  //####################################################################
  template <XMLWRAPP_ACCESS_SPECIFIER Access>
  node_set_iterator_T<Access>
  operator+ (const node_set_iterator_T<Access> &it, int i) {
    return node_set_iterator_T<Access>(it) += i;
  }
  //####################################################################
  template <XMLWRAPP_ACCESS_SPECIFIER Access>
  node_set_iterator_T<Access>
  operator+ (int i, const node_set_iterator_T<Access> &it) {
    return node_set_iterator_T<Access>(it) += i;
  }
  //####################################################################
  template <XMLWRAPP_ACCESS_SPECIFIER Access>
  node_set_iterator_T<Access>
  operator- (const node_set_iterator_T<Access> &it, int i) {
    return node_set_iterator_T<Access>(it) -= i;
  }
  //####################################################################
  template <XMLWRAPP_ACCESS_SPECIFIER Access>
  node_set_iterator_T<Access>
  operator- (int i, const node_set_iterator_T<Access> &it) {
    return node_set_iterator_T<Access>(it) -= i;
  }
  //####################################################################
  template <XMLWRAPP_ACCESS_SPECIFIER Access>
  typename node_set_iterator_T<Access>::difference_type
  operator- (const node_set_iterator_T<Access> &lhs,
	     const node_set_iterator_T<Access> &rhs) {
    return lhs.distance(rhs);
  }
  //####################################################################
  //@}
  //! @name Comparison operators
  //! @author Shane Beasley
  //@{
  //####################################################################
  template <XMLWRAPP_ACCESS_SPECIFIER Access>
  bool operator< (const node_set_iterator_T<Access> &lhs,
		  const node_set_iterator_T<Access> &rhs) {
    return rhs - lhs < 0;
  }
  //####################################################################
  template <XMLWRAPP_ACCESS_SPECIFIER Access>
  bool operator== (const node_set_iterator_T<Access> &lhs,
		   const node_set_iterator_T<Access> &rhs) {
    return rhs - lhs == 0;
  }
  //####################################################################
  template <XMLWRAPP_ACCESS_SPECIFIER Access>
  bool operator<= (const node_set_iterator_T<Access> &lhs,
		   const node_set_iterator_T<Access> &rhs) {
    return !(rhs < lhs);
  }
  //####################################################################
  template <XMLWRAPP_ACCESS_SPECIFIER Access>
  bool operator!= (const node_set_iterator_T<Access> &lhs,
		   const node_set_iterator_T<Access> &rhs) {
    return !(rhs == lhs);
  }
  //####################################################################
  template <XMLWRAPP_ACCESS_SPECIFIER Access>
  bool operator>= (const node_set_iterator_T<Access> &lhs,
		   const node_set_iterator_T<Access> &rhs) {
    return !(lhs < rhs);
  }
  //####################################################################
  template <XMLWRAPP_ACCESS_SPECIFIER Access>
  bool operator> (const node_set_iterator_T<Access> &lhs,
		  const node_set_iterator_T<Access> &rhs) {
    return rhs < lhs;
  }
  //####################################################################
  //! @}
  //####################################################################
}

#endif
