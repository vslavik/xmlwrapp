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

#ifndef XPATH_CAST_H_20030714T1820
#define XPATH_CAST_H_20030714T1820

//####################################################################
/** @file
 * Various casting functions.
 **/

#include "xpath_types.h"
#include <string>

namespace xml {
  class node;
}

namespace xpath {
  //####################################################################
  //! Convert a number to a boolean.
  //! @author Shane Beasley
  bool to_boolean (double val);
  //####################################################################
  //! Convert a string to a boolean.
  //! @author Shane Beasley
  bool to_boolean (const char *val);
  //####################################################################
  //! Convert a string to a boolean.
  //! @author Shane Beasley
  bool to_boolean (const std::string &val);
  //####################################################################
  //! Convert an XPath query result to a boolean.
  //! @author Shane Beasley
  template <XMLWRAPP_ACCESS_SPECIFIER Access>
  bool to_boolean (const result_T<Access> &val);
  //####################################################################
  //! Convert a boolean to a number.
  //! @author Shane Beasley
  double to_number (bool val);
  //####################################################################
  //! Convert a string to a number.
  //! @author Shane Beasley
  double to_number (const char *val) ;
  //####################################################################
  //! Convert a string to a boolean.
  //! @author Shane Beasley
  double to_number (const std::string val);
  //####################################################################
  //! Convert a node to a number.
  //! @author Shane Beasley
  double to_number (const xml::node &node);
  //####################################################################
  //! Convert an XPath query result to a number.
  //! @author Shane Beasley
  template <XMLWRAPP_ACCESS_SPECIFIER Access>
  double to_number (const result_T<Access> &val);
  //####################################################################
  //! Convert a boolean to a string.
  //! @author Shane Beasley
  std::string to_string (bool val);
  //####################################################################
  //! Convert a number to a string.
  //! @author Shane Beasley
  std::string to_string (double val);
  //####################################################################
  //! Convert a node to a string.
  //! @author Shane Beasley
  std::string to_string (const xml::node &node);
  //####################################################################
  //! Convert an XPath query result to a string.
  //! @author Shane Beasley
  template <XMLWRAPP_ACCESS_SPECIFIER Access>
  std::string to_string (const result_T<Access> &val);
}

#endif
