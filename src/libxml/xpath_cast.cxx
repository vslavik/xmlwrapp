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

#include "xmlwrapp/xpath_cast.h"

#include "xpath_helper.h"
#include "object_rep.h"

using xmlwrapp::impl_cast::as_rep;

namespace {
  std::string convert (xmlChar *s) {
    struct eraser { eraser (xmlChar *s) : s(s) { } ~eraser () { xmlFree(s); } xmlChar *s; } e(s);
    return reinterpret_cast<const char *>(s);
  }

  const xmlChar *convert (const char *s) { return reinterpret_cast<const xmlChar *>(s); }
  const xmlChar *convert (const std::string &s) { return convert(s.c_str()); }
}

namespace xpath {
  bool to_boolean (double val) {
    return xmlXPathCastNumberToBoolean(val) ? 1 : 0;
  }
  bool to_boolean (const char *val) {
    return xmlXPathCastStringToBoolean(convert(val)) ? 1 : 0;
  }
  bool to_boolean (const std::string &val) {
    return xmlXPathCastStringToBoolean(convert(val)) ? 1 : 0;
  }
  template <XMLWRAPP_ACCESS_SPECIFIER Access>
  bool to_boolean (const object_T<Access> &val) {
    return xmlXPathCastToBoolean(as_rep(val.pimpl())) ? 1 : 0;
  }

  double to_number (bool val) {
    return xmlXPathCastBooleanToNumber(val);
  }
  double to_number (const char *val)  {
    return xmlXPathCastStringToNumber(convert(val));
  }
  double to_number (const std::string val) {
    return xmlXPathCastStringToNumber(convert(val));
  }
  double to_number (const xml::node::const_reference &node) {
    return xmlXPathCastNodeToNumber(xpath_helper::get(node));
  }
  template <XMLWRAPP_ACCESS_SPECIFIER Access>
  double to_number (const object_T<Access> &val) {
    return xmlXPathCastToNumber(as_rep(val.pimpl()));
  }

  std::string to_string (bool val) {
    return convert(xmlXPathCastBooleanToString(val));
  }
  std::string to_string (double val) {
    return convert(xmlXPathCastNumberToString(val));
  }
  std::string to_string (const xml::node::const_reference &node) {
    return convert(xmlXPathCastNodeToString(xpath_helper::get(node)));
  }
  template <XMLWRAPP_ACCESS_SPECIFIER Access>
  std::string to_string (const object_T<Access> &val) {
    return convert(xmlXPathCastToString(as_rep(val.pimpl())));
  }

  template bool to_boolean (const object_T<XMLWRAPP_RO_ACCESS> &);
  template bool to_boolean (const object_T<XMLWRAPP_RW_ACCESS> &);
  template double to_number (const object_T<XMLWRAPP_RO_ACCESS> &);
  template double to_number (const object_T<XMLWRAPP_RW_ACCESS> &);
  template std::string to_string (const object_T<XMLWRAPP_RO_ACCESS> &);
  template std::string to_string (const object_T<XMLWRAPP_RW_ACCESS> &);
}
