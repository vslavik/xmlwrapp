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

#include "node_set_iterator_rep.h"
#include "xpath_helper.h"

using namespace xmlwrapp::impl_cast;

template <XMLWRAPP_ACCESS_SPECIFIER Access>
xpath::node_set_iterator_T<Access>::node_set_iterator_T ()
  : pimpl_(0) {
}

template <XMLWRAPP_ACCESS_SPECIFIER Access>
xpath::node_set_iterator_T<Access>::node_set_iterator_T (impl *impl)
  : pimpl_(impl) {
}

template <XMLWRAPP_ACCESS_SPECIFIER Access>
xpath::node_set_iterator_T<Access>::node_set_iterator_T
(const node_set_iterator &rhs)
  : pimpl_(rhs.pimpl()) {
}

template <XMLWRAPP_ACCESS_SPECIFIER Access>
typename xpath::node_set_iterator_T<Access>::reference
xpath::node_set_iterator_T<Access>::operator* () const {
  reference ref;
  xpath_helper::set(ref, *as_rep(pimpl_));
  return ref;
}

template <XMLWRAPP_ACCESS_SPECIFIER Access>
typename xpath::node_set_iterator_T<Access>::reference
xpath::node_set_iterator_T<Access>::operator[] (size_type i) const {
  return *(*this + i);
}

template <XMLWRAPP_ACCESS_SPECIFIER Access>
typename xpath::node_set_iterator_T<Access>::pointer
xpath::node_set_iterator_T<Access>::operator-> () const {
  reference ref;
  xpath_helper::set(ref, *as_rep(pimpl_));
  return ref;
}

template <XMLWRAPP_ACCESS_SPECIFIER Access>
xpath::node_set_iterator_T<Access> &
xpath::node_set_iterator_T<Access>::operator++ () {
  pimpl_ = as_impl<impl>(as_rep(pimpl_) + 1);
  return *this;
}

template <XMLWRAPP_ACCESS_SPECIFIER Access>
xpath::node_set_iterator_T<Access>
xpath::node_set_iterator_T<Access>::operator++ (int) {
  node_set_iterator_T<Access> temp(*this);
  ++*this;
  return temp;
}

template <XMLWRAPP_ACCESS_SPECIFIER Access>
xpath::node_set_iterator_T<Access> &
xpath::node_set_iterator_T<Access>::operator+= (int i) {
  pimpl_ = as_impl<impl>(as_rep(pimpl_) + i);
  return *this;
}

template <XMLWRAPP_ACCESS_SPECIFIER Access>
xpath::node_set_iterator_T<Access> &
xpath::node_set_iterator_T<Access>::operator-- () {
  pimpl_ = as_impl<impl>(as_rep(pimpl_) - 1);
  return *this;
}

template <XMLWRAPP_ACCESS_SPECIFIER Access>
xpath::node_set_iterator_T<Access>
xpath::node_set_iterator_T<Access>::operator-- (int) {
  node_set_iterator_T<Access> temp(*this);
  --*this;
  return temp;
}

template <XMLWRAPP_ACCESS_SPECIFIER Access>
xpath::node_set_iterator_T<Access> &
xpath::node_set_iterator_T<Access>::operator-= (int i) {
  pimpl_ = as_impl<impl>(as_rep(pimpl_) - i);
  return *this;
}

template <XMLWRAPP_ACCESS_SPECIFIER Access>
typename xpath::node_set_iterator_T<Access>::difference_type
xpath::node_set_iterator_T<Access>::distance (const xpath::node_set_iterator_T<Access> &rhs) const {
  return as_rep(rhs.pimpl_) - as_rep(pimpl_);
}

// explicit instantiations

template class xpath::node_set_iterator_T<xpath::access::read_write>;
template class xpath::node_set_iterator_T<xpath::access::read_only>;
