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

#include "context_rep.h"

#include "xmlwrapp/result.h"
#include "xpath_helper.h"

using namespace xmlwrapp::impl_cast;

namespace {
  struct factory : factory_base<XMLWRAPP_IMPL_T(xpath::context_T)> {
    static impl_ptr create (xmlDocPtr doc) {
      rep_ptr new_ctxt = xmlXPathNewContext(doc);
      if (!new_ctxt) throw std::bad_alloc();
      return as_impl(new_ctxt);
    }

    static impl_ptr create (xmlNodePtr node) {
      rep_ptr new_ctxt = xmlXPathNewContext(node->doc);
      if (!new_ctxt) throw std::bad_alloc();
      new_ctxt->node = node;
      return as_impl(new_ctxt);
    }

    static impl_ptr clone (impl_ptr ctxt) {
      rep_ptr new_ctxt = xmlXPathNewContext(as_rep(ctxt)->doc);
      if (!new_ctxt) throw std::bad_alloc();
      new_ctxt->node = as_rep(ctxt)->node;
      return as_impl(new_ctxt);
    }
    
    static void destroy (impl_ptr ctxt) { xmlXPathFreeContext(as_rep(ctxt)); }
  };
}

template <XMLWRAPP_ACCESS_SPECIFIER Access>
xpath::context_T<Access>::context_T ()
  : pimpl_(factory::create(xmlDocPtr()), &factory::destroy) {
}

template <XMLWRAPP_ACCESS_SPECIFIER Access>
xpath::context_T<Access>::context_T (document &doc)
  : pimpl_(factory::create(xpath_helper::get(doc)),
	   &factory::destroy) {
}

template <XMLWRAPP_ACCESS_SPECIFIER Access>
xpath::context_T<Access>::context_T (node &node)
  : pimpl_(factory::create(xpath_helper::get(node)),
	   &factory::destroy) {
}

template <XMLWRAPP_ACCESS_SPECIFIER Access>
xpath::context_T<Access>::context_T (const context_T &rhs)
  : pimpl_(factory::clone(rhs.pimpl_.get()), &factory::destroy) {
}

template <XMLWRAPP_ACCESS_SPECIFIER Access>
void
xpath::context_T<Access>::set_document (document &doc) {
  as_rep(pimpl_)->doc  = xpath_helper::get(doc);
  as_rep(pimpl_)->node = 0;
}

template <XMLWRAPP_ACCESS_SPECIFIER Access>
void
xpath::context_T<Access>::set_node (node &node) {
  as_rep(pimpl_)->doc = (as_rep(pimpl_)->node = xpath_helper::get(node))->doc;
}

template <XMLWRAPP_ACCESS_SPECIFIER Access>
xpath::context_T<Access> &
xpath::context_T<Access>::operator= (const context_T &rhs) {
  as_rep(pimpl_)->doc = as_rep(rhs.pimpl_)->doc;
  as_rep(pimpl_)->node = as_rep(rhs.pimpl_)->node;
  return *this;
}

template <XMLWRAPP_ACCESS_SPECIFIER Access>
void
xpath::context_T<Access>::swap (context_T &rhs) {
  pimpl_.swap(rhs.pimpl_);
}

namespace xpath {
  template <XMLWRAPP_ACCESS_SPECIFIER Access>
  result_T<Access>
  context_T<Access>::operator[] (const char *query) {
    return result_type(*this, query);
  }
}

template <XMLWRAPP_ACCESS_SPECIFIER Access>
typename xpath::context_T<Access>::result_type
xpath::context_T<Access>::operator[] (const expression &query) {
  return result_type(*this, query);
}

template class xpath::context_T<xmlwrapp::access::read_write>;
template class xpath::context_T<xmlwrapp::access::read_only>;
