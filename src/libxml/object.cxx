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

#include "object_rep.h"

#include "context_rep.h"
#include "expression_rep.h"
#include "node_set_iterator_rep.h"
#include "xpath_helper.h"
#include <stdexcept>
#include <libxml/xpath.h>
#include <libxml/xpathInternals.h>

using namespace xmlwrapp::impl_cast;

namespace {
  struct factory : factory_base<XMLWRAPP_IMPL_T(xpath::object_T)> {
    typedef get_rep<XMLWRAPP_IMPL_T(xpath::context_T)>::type *ctxt_rep_ptr;
    typedef get_rep<XMLWRAPP_IMPL(xpath::expression)>::type  *expr_rep_ptr;

    static impl_ptr create () {
      rep_ptr obj = static_cast<rep_ptr>(xmlMalloc(sizeof(xmlXPathObject)));
      if (!obj) throw std::bad_alloc();
      obj->type = XPATH_UNDEFINED;
      return as_impl(obj);
    }

    static impl_ptr create (bool val) {
      return as_impl(xmlXPathNewBoolean(val));
    }

    static impl_ptr create (double val) {
      return as_impl(xmlXPathNewFloat(val));
    }

    static impl_ptr create (const char *val) {
      return as_impl(xmlXPathNewString(reinterpret_cast<const xmlChar *>(val)));
    }

    static impl_ptr create (ctxt_rep_ptr ctxt, expr_rep_ptr expr) {
      rep_ptr new_obj = xmlXPathCompiledEval(expr, ctxt);
      if (!new_obj) throw std::runtime_error("error evaluating XPath expression");
      return as_impl(new_obj);
    }

    static impl_ptr clone (impl_ptr obj) {
      rep_ptr new_obj = xmlXPathObjectCopy(as_rep(obj));
      if (!new_obj) throw std::bad_alloc();
      return as_impl(new_obj);
    }

    static void destroy (impl_ptr obj) {
      xmlXPathFreeObject(as_rep(obj));
    }
  };
}

template <XMLWRAPP_ACCESS_SPECIFIER Access>
xpath::object_T<Access>::object_T ()
  : pimpl_(factory::create(), &factory::destroy) {
}

template <XMLWRAPP_ACCESS_SPECIFIER Access>
xpath::object_T<Access>::object_T (bool val)
  : pimpl_(factory::create(val), &factory::destroy) {
}

template <XMLWRAPP_ACCESS_SPECIFIER Access>
xpath::object_T<Access>::object_T (double val)
  : pimpl_(factory::create(val), &factory::destroy) {
}

template <XMLWRAPP_ACCESS_SPECIFIER Access>
xpath::object_T<Access>::object_T (const char *val)
  : pimpl_(factory::create(val), &factory::destroy) {
}

template <XMLWRAPP_ACCESS_SPECIFIER Access>
xpath::object_T<Access>::object_T (const std::string &val)
  : pimpl_(factory::create(val.c_str()), &factory::destroy) {
}

template <XMLWRAPP_ACCESS_SPECIFIER Access>
xpath::object_T<Access>::object_T (const read_write::object &rhs)
  : pimpl_(rhs.pimpl()) {
}

template <XMLWRAPP_ACCESS_SPECIFIER Access>
xpath::object_T<Access>::object_T (restricted_context &ctxt,
				   const expression &query)
  : pimpl_(factory::create(as_rep(ctxt.pimpl_), as_rep(query.pimpl_)),
	   &factory::destroy) {
}

template <XMLWRAPP_ACCESS_SPECIFIER Access>
xpath::object_T<Access>::object_T (detail::reference<rw_context> ctxt,
				   const expression &query)
  : pimpl_(factory::create(as_rep(ctxt.ref().pimpl_), as_rep(query.pimpl_)),
	   &factory::destroy) {
}

template <XMLWRAPP_ACCESS_SPECIFIER Access>
void
xpath::object_T<Access>::swap (xpath::object_T<Access> &rhs) {
  pimpl_.swap(rhs.pimpl_);
}

template <XMLWRAPP_ACCESS_SPECIFIER Access>
xpath::object_T<Access>::~object_T () {
}

template <XMLWRAPP_ACCESS_SPECIFIER Access>
typename xpath::object_T<Access>::object_type
xpath::object_T<Access>::type () const {
  switch (as_rep(pimpl_)->type) {
  case XPATH_NODESET: return node_set_type;
  case XPATH_BOOLEAN: return boolean_type;
  case XPATH_NUMBER:  return number_type;
  case XPATH_STRING:  return string_type;
  default:            return undefined_type;
  }
}

template <XMLWRAPP_ACCESS_SPECIFIER Access>
double
xpath::object_T<Access>::to_number () const {
  return xmlXPathCastToNumber(as_rep(pimpl_));
}

template <XMLWRAPP_ACCESS_SPECIFIER Access>
bool
xpath::object_T<Access>::to_boolean () const {
  return (xmlXPathCastToBoolean(as_rep(pimpl_)) ? true : false);
}

template <XMLWRAPP_ACCESS_SPECIFIER Access>
std::string
xpath::object_T<Access>::to_string () const {
  std::string retval;

  if (xmlChar *s = xmlXPathCastToString(as_rep(pimpl_))) {
    retval = reinterpret_cast<char *>(s);
    xmlFree(s);
  }

  return retval;
}

template <XMLWRAPP_ACCESS_SPECIFIER Access>
bool
xpath::object_T<Access>::is_node_set () const {
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
bool xpath::object_T<Access>::empty () const {
  return xmlXPathNodeSetIsEmpty(get_node_set(pimpl_));
} 

template <XMLWRAPP_ACCESS_SPECIFIER Access>
typename xpath::object_T<Access>::size_type
xpath::object_T<Access>::size () const {
  return (!empty()
	  ? xmlXPathNodeSetGetLength(get_node_set(pimpl_))
	  : 0);
} 

template <XMLWRAPP_ACCESS_SPECIFIER Access>
typename xpath::object_T<Access>::iterator
xpath::object_T<Access>::begin () const {
  return (!empty()
	  ? as_my_impl<iterator>(&get_node_set(pimpl_)->nodeTab[0])
	  : iterator());
}

template <XMLWRAPP_ACCESS_SPECIFIER Access>
typename xpath::object_T<Access>::iterator
xpath::object_T<Access>::end () const {
  return (!empty()
	  ? as_my_impl<iterator>(&get_node_set(pimpl_)->nodeTab[size()])
	  : iterator());
}

template <XMLWRAPP_ACCESS_SPECIFIER Access>
typename xpath::object_T<Access>::reference
xpath::object_T<Access>::operator[] (size_type i) const {
  return !empty() ? *(begin() + i) : reference();
}

template <XMLWRAPP_ACCESS_SPECIFIER Access>
xpath::object_T<Access>::object_T (impl *impl)
  : pimpl_(impl, &factory::destroy) {
}

// explicit instantiations

template class xpath::object_T<xmlwrapp::access::read_write>;
template class xpath::object_T<xmlwrapp::access::read_only>;


//####################################################################
// if at some point the libxml people let us see the actual error
// generated by evaluating an XPath expression, this code might
// actually be useful.
#if 0

namespace {
  void do_throw (int code) {
#define XPATH_THROW_ERR(msg) \
  throw std::runtime_error("error evaluating XPath expression: " #msg)
    switch (code) {
    case XPATH_EXPRESSION_OK:
      XPATH_THROW_ERR(XPATH_EXPRESSION_OK);
    case XPATH_NUMBER_ERROR:
      XPATH_THROW_ERR(XPATH_NUMBER_ERROR);
    case XPATH_UNFINISHED_LITERAL_ERROR:
      XPATH_THROW_ERR(XPATH_UNFINISHED_LITERAL_ERROR);
    case XPATH_START_LITERAL_ERROR:
      XPATH_THROW_ERR(XPATH_START_LITERAL_ERROR);
    case XPATH_VARIABLE_REF_ERROR:
      XPATH_THROW_ERR(XPATH_VARIABLE_REF_ERROR);
    case XPATH_UNDEF_VARIABLE_ERROR:
      XPATH_THROW_ERR(XPATH_UNDEF_VARIABLE_ERROR);
    case XPATH_INVALID_PREDICATE_ERROR:
      XPATH_THROW_ERR(XPATH_INVALID_PREDICATE_ERROR);
    case XPATH_EXPR_ERROR:
      XPATH_THROW_ERR(XPATH_EXPR_ERROR);
    case XPATH_UNCLOSED_ERROR:
      XPATH_THROW_ERR(XPATH_UNCLOSED_ERROR);
    case XPATH_UNKNOWN_FUNC_ERROR:
      XPATH_THROW_ERR(XPATH_UNKNOWN_FUNC_ERROR);
    case XPATH_INVALID_OPERAND:
      XPATH_THROW_ERR(XPATH_INVALID_OPERAND);
    case XPATH_INVALID_TYPE:
      XPATH_THROW_ERR(XPATH_INVALID_TYPE);
    case XPATH_INVALID_ARITY:
      XPATH_THROW_ERR(XPATH_INVALID_ARITY);
    case XPATH_INVALID_CTXT_SIZE:
      XPATH_THROW_ERR(XPATH_INVALID_CTXT_SIZE);
    case XPATH_INVALID_CTXT_POSITION:
      XPATH_THROW_ERR(XPATH_INVALID_CTXT_POSITION);
    case XPATH_MEMORY_ERROR:
      XPATH_THROW_ERR(XPATH_MEMORY_ERROR);
    case XPTR_SYNTAX_ERROR:
      XPATH_THROW_ERR(XPTR_SYNTAX_ERROR);
    case XPTR_RESOURCE_ERROR:
      XPATH_THROW_ERR(XPTR_RESOURCE_ERROR);
    case XPTR_SUB_RESOURCE_ERROR:
      XPATH_THROW_ERR(XPTR_SUB_RESOURCE_ERROR);
    case XPATH_UNDEF_PREFIX_ERROR:
      XPATH_THROW_ERR(XPATH_UNDEF_PREFIX_ERROR);
    case XPATH_ENCODING_ERROR:
      XPATH_THROW_ERR(XPATH_ENCODING_ERROR);
    case XPATH_INVALID_CHAR_ERROR:
      XPATH_THROW_ERR(XPATH_INVALID_CHAR_ERROR);
    }
#undef XPATH_THROW_ERR
  }
}

#endif
