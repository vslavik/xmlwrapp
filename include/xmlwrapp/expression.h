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

#ifndef XPATH_EXPRESSION_H_20030714T1820
#define XPATH_EXPRESSION_H_20030714T1820

//####################################################################
/** @file
 * Definition of xpath::expression
 **/

#include "xpath_fwd.h"
#include "pimpl.h"
#include <string>

namespace xpath {
  //####################################################################
  //! Compiled XPath expression, useful when repeating a query many times.
  //!
  //! The XPath engine evaluates expressions in a compiled binary format.
  //! Ad hoc expressions can be compiled from text strings as necessary;
  //! but frequently used expressions should be compiled once, then reused
  //! in compiled form.
  class expression {
  public:
    //####################################################################
    //! Compile an XPath expression.
    //! @author Shane Beasley
    expression (const char *);
    //####################################################################
    //! Compile an XPath expression.
    //! @author Shane Beasley
    expression (const std::string &);
    //####################################################################
    //! Swap with another expression.
    //! @author Shane Beasley
    void swap (expression &);

    //####################################################################
    //! @internal Private implementation class for Pimpl idiom.
    typedef XMLWRAPP_IMPL(expression) impl;

  private:
    XMLWRAPP_PIMPL(shared_ptr, impl) pimpl_;
    XMLWRAPP_FRIEND_T(object_T);
  };
}

#endif
