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

#ifndef XPATH_HELPER_H_20030714T1820
#define XPATH_HELPER_H_20030714T1820

#include "xmlwrapp/document.h"
#include "xmlwrapp/node.h"
#include <libxml/tree.h>

namespace xpath {
  class xpath_helper {
  public:
    static xmlDocPtr get (const xml::document &doc) {
      return static_cast<xmlDocPtr>(doc.get_doc_data());
    }

    static xmlNodePtr get (const xml::node &n) {
      return static_cast<xmlNodePtr>(n.get_node_data());
    }

    static void set (xml::node &n, xmlNodePtr p) {
      n.set_node_data(p);
    }
  };
}

#endif
