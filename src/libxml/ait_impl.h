/*
 * Copyright (C) 2001-2003 Peter J Jones (pjones@pmade.org)
 * All Rights Reserved
 * 
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the
 *    distribution.
 * 3. Neither the name of the Author nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 * 
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR AND CONTRIBUTORS ``AS IS''
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED
 * TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A
 * PARTICULAR PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR
 * OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF
 * USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 * AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT
 * OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */

/** @file
 * This file defines the xml::ait_impl class.
**/

#ifndef _xmlwrapp_attr_iterator_h_
#define _xmlwrapp_attr_iterator_h_

// xmlwrapp includes
#include "xmlwrapp/attributes.h"

// libxml2 includes
#include <libxml/tree.h>

namespace xml {

/**
 * the class that does all the work behind xml::attributes::iterator and
 * xml::attributes::const_iterator.
 */
class ait_impl {
public:
    ait_impl (xmlNodePtr node, xmlAttrPtr prop);
    ait_impl (const char *name, const char *value, bool);
    ait_impl (const ait_impl &other);
    ait_impl& operator= (const ait_impl &other);

    attributes::attr* get (void);
    xmlAttrPtr get_raw_attr (void);

    ait_impl& operator++ (void);
    ait_impl  operator++ (int);

    friend bool operator== (const ait_impl &lhs, const ait_impl &rhs);
    friend bool operator!= (const ait_impl &lhs, const ait_impl &rhs);
private:
    xmlNodePtr xmlnode_;
    xmlAttrPtr xmlattr_;
    attributes::attr attr_;
    bool fake_;

}; // end xml::ait_impl class

// a couple helper functions
xmlAttrPtr find_prop (xmlNodePtr xmlnode, const char *name);
xmlAttributePtr find_default_prop (xmlNodePtr xmlnode, const char *name);

} // end xml namespace
#endif
