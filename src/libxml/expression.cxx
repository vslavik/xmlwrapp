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

#include "expression_rep.h"
#include <stdexcept>
#include <string>

using namespace xmlwrapp::impl_cast;

namespace {
  struct factory : factory_base<XMLWRAPP_IMPL(xpath::expression)> {
    static impl_ptr create (const char *expr) {
      rep_ptr new_expr = xmlXPathCompile(reinterpret_cast<const xmlChar *>(expr));
      if (!new_expr) throw std::runtime_error(std::string("expression compilation failed"));
      return as_impl(new_expr);
    }

    static void destroy (impl_ptr expr) { xmlXPathFreeCompExpr(as_rep(expr)); }
  };
}

xpath::expression::expression (const char *expr)
  : pimpl_(factory::create(expr), &factory::destroy) {
}

xpath::expression::expression (const std::string &expr)
  : pimpl_(factory::create(expr.c_str()), &factory::destroy) {
}

void xpath::expression::swap (expression &rhs) {
  pimpl_.swap(rhs.pimpl_);
}
