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

#include <xmlwrapp/node_reference.h>

namespace xml {
  template <XMLWRAPP_ACCESS_SPECIFIER Access>
  node_reference_T<Access>::node_reference_T () {
  }

  template <XMLWRAPP_ACCESS_SPECIFIER Access>
  node_reference_T<Access>::node_reference_T (const node_reference_T &rhs)
    : base_type(rhs.pimpl_) {
  }

  template <XMLWRAPP_ACCESS_SPECIFIER Access>
  node_reference_T<Access> &
  node_reference_T<Access>::operator= (const node_reference_T &rhs)
  { node_reference_T(rhs).swap(*this); return *this; }

  template <XMLWRAPP_ACCESS_SPECIFIER Access>
  node_reference_T<Access>::~node_reference_T () {
  }

  template <XMLWRAPP_ACCESS_SPECIFIER Access>
  node_reference_T<Access>::node_reference_T (const rw_reference &rhs)
    : base_type(rhs.ref().pimpl_) {
  }

  template <XMLWRAPP_ACCESS_SPECIFIER Access>
  node_reference_T<Access> &
  node_reference_T<Access>::operator= (const rw_reference &rhs)
  { node_reference_T(rhs).swap(*this); return *this; }

  template <XMLWRAPP_ACCESS_SPECIFIER Access>
  node_reference_T<Access>::node_reference_T (node_type &rhs)
    : base_type(rhs.pimpl_) { }

  template <XMLWRAPP_ACCESS_SPECIFIER Access>
  node_reference_T<Access> &
  node_reference_T<Access>::operator= (node_type &rhs)
  { node_reference_T(rhs).swap(*this); return *this; }

  template <XMLWRAPP_ACCESS_SPECIFIER Access>
  void node_reference_T<Access>::swap (node_reference_T &rhs)
  { base_type::do_swap(rhs); }

  template <XMLWRAPP_ACCESS_SPECIFIER Access>
  node_reference_T<Access> *
  node_reference_T<Access>::operator-> () { return this; }

  template <XMLWRAPP_ACCESS_SPECIFIER Access>
  const node_reference_T<Access> *
  node_reference_T<Access>::operator-> () const { return this; }

  template <XMLWRAPP_ACCESS_SPECIFIER Access>
  node_reference_T<Access>::node_reference_T (node_impl *impl)
    : base_type(impl) { }

  // explicit instantiations
  template class node_reference_T<XMLWRAPP_READ_ONLY>;
  template class node_reference_T<XMLWRAPP_READ_WRITE>;
}
