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

#include "result_rep.h"

#include "context_rep.h"
#include "expression_rep.h"
#include "node_set_iterator_rep.h"
#include "xpath_helper.h"

using namespace xmlwrapp::impl_cast;

namespace {
  struct factory : factory_base<XMLWRAPP_IMPL_T(xpath::result_T)> {
    typedef get_rep<XMLWRAPP_IMPL_T(xpath::context_T)>::type *ctxt_rep_ptr;
    typedef get_rep<XMLWRAPP_IMPL(xpath::expression)>::type  *expr_rep_ptr;

    static impl_ptr create (ctxt_rep_ptr ctxt, const char *query) {
      rep_ptr
	new_obj = xmlXPathEvalExpression(reinterpret_cast<const xmlChar *>(query),
					 ctxt);
      if (!new_obj) throw std::bad_alloc();
      return as_impl(new_obj);
    }

    static impl_ptr create (ctxt_rep_ptr ctxt, expr_rep_ptr expr) {
      rep_ptr new_obj = xmlXPathCompiledEval(expr, ctxt);
      if (!new_obj) throw std::bad_alloc();
      return as_impl(new_obj);
    }

    static impl_ptr clone (impl_ptr obj) {
      rep_ptr new_obj = xmlXPathObjectCopy(as_rep(obj));
      if (!new_obj) throw std::bad_alloc();
      return as_impl(new_obj);
    }

    static void destroy (impl_ptr obj) { xmlXPathFreeObject(as_rep(obj)); }
  };
}

template <XMLWRAPP_ACCESS_SPECIFIER Access>
xpath::result_T<Access>::result_T (const read_write::result &rhs)
  : pimpl_(rhs.pimpl()) {
}

template <XMLWRAPP_ACCESS_SPECIFIER Access>
xpath::result_T<Access>::result_T (restricted_context &ctxt,
				   const expression &query)
  : pimpl_(factory::create(as_rep(ctxt.pimpl_), as_rep(query.pimpl_)),
	   &factory::destroy) {
}

template <XMLWRAPP_ACCESS_SPECIFIER Access>
xpath::result_T<Access>::result_T (detail::reference<rw_context> ctxt,
				   const expression &query)
  : pimpl_(factory::create(as_rep(ctxt.ref().pimpl_), as_rep(query.pimpl_)),
	   &factory::destroy) {
}

template <XMLWRAPP_ACCESS_SPECIFIER Access>
void
xpath::result_T<Access>::swap (xpath::result_T<Access> &rhs) {
  pimpl_.swap(rhs.pimpl_);
}

template <XMLWRAPP_ACCESS_SPECIFIER Access>
xpath::result_T<Access>::~result_T () {
}

template <XMLWRAPP_ACCESS_SPECIFIER Access>
typename xpath::result_T<Access>::result_type
xpath::result_T<Access>::type () const {
  switch (as_rep(pimpl_)->type) {
  case XPATH_NODESET: return node_set_result;
  case XPATH_BOOLEAN: return boolean_result;
  case XPATH_NUMBER:  return number_result;
  case XPATH_STRING:  return string_result;
  default:            return undefined_result;
  }
}

template <XMLWRAPP_ACCESS_SPECIFIER Access>
double
xpath::result_T<Access>::to_number () const {
  return xmlXPathCastToNumber(as_rep(pimpl_));
}

template <XMLWRAPP_ACCESS_SPECIFIER Access>
bool
xpath::result_T<Access>::to_boolean () const {
  return (xmlXPathCastToBoolean(as_rep(pimpl_)) ? true : false);
}

template <XMLWRAPP_ACCESS_SPECIFIER Access>
std::string
xpath::result_T<Access>::to_string () const {
  std::string retval;

  if (xmlChar *s = xmlXPathCastToString(as_rep(pimpl_))) {
    retval = reinterpret_cast<char *>(s);
    xmlFree(s);
  }

  return retval;
}

template <XMLWRAPP_ACCESS_SPECIFIER Access>
bool
xpath::result_T<Access>::is_node_set () const {
  return as_rep(pimpl_)->type == XPATH_NODESET;
}

// node set functions

namespace {
  xmlNodeSetPtr get_node_set (xmlXPathObjectPtr obj) {
    return obj->nodesetval;
  }

  template <typename P>
  xmlNodeSetPtr get_node_set (const P &p) {
    return as_rep(p)->nodesetval;
  }
}

template <XMLWRAPP_ACCESS_SPECIFIER Access>
bool xpath::result_T<Access>::empty () const {
  return xmlXPathNodeSetIsEmpty(get_node_set(pimpl_));
} 

template <XMLWRAPP_ACCESS_SPECIFIER Access>
typename xpath::result_T<Access>::size_type
xpath::result_T<Access>::size () const {
  return (!empty()
	  ? xmlXPathNodeSetGetLength(get_node_set(pimpl_))
	  : 0);
} 

template <XMLWRAPP_ACCESS_SPECIFIER Access>
typename xpath::result_T<Access>::iterator
xpath::result_T<Access>::begin () const {
  return (!empty()
	  ? as_my_impl<iterator>(&get_node_set(pimpl_)->nodeTab[0])
	  : iterator());
}

template <XMLWRAPP_ACCESS_SPECIFIER Access>
typename xpath::result_T<Access>::iterator
xpath::result_T<Access>::end () const {
  return (!empty()
	  ? as_my_impl<iterator>(&get_node_set(pimpl_)->nodeTab[size()])
	  : iterator());
}

template <XMLWRAPP_ACCESS_SPECIFIER Access>
typename xpath::result_T<Access>::reference
xpath::result_T<Access>::operator[] (size_type i) const {
  return *(begin() + i);
}

// explicit instantiations

template class xpath::result_T<xmlwrapp::access::read_write>;
template class xpath::result_T<xmlwrapp::access::read_only>;
