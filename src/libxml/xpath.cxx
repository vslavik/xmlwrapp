/*
 * Copyright (C) 2011 Jonas Weber (mail@jonasw.de)
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

// xmlwrapp includes
#include "xmlwrapp/xpath.h"
#include "xmlwrapp/document.h"
#include "xmlwrapp/node.h"

// libxml includes
#include <libxml/tree.h>
#include <libxml/xpath.h>
#include <libxml/xpathInternals.h>

#include <iostream>

namespace xml {
    namespace xpath {
        bool XMLWRAPP_API operator==(const xml::xpath::node_set::iterator& l, const xml::xpath::node_set::iterator& r)
        {
            return ((l.pos == r.pos) && (l.data == r.data));
        }

        bool XMLWRAPP_API operator!=(const xml::xpath::node_set::iterator& l, const xml::xpath::node_set::iterator& r)
        {
            return ((l.pos != r.pos) || (l.data != r.data));
        }}

        namespace impl
        {
            struct xpitimpl
            {
                xml::node node;
                xpitimpl() : node(0) { };
                void set_data(xmlNodePtr n) { node.set_node_data(n); };
                xml::node& get() { return node; }
            };
        }

        namespace xpath
        {

            //------------------------
            // node_set
            //------------------------
            //[data] is xmlXPathObjectPtr
            node_set::node_set(void* data) : data(data)
            {
            }

            node_set::~node_set()
            {
                xmlXPathFreeObject(static_cast<xmlXPathObjectPtr>(data));
            }

            int node_set::count() const
            {
                return static_cast<xmlXPathObjectPtr>(data)->nodesetval->nodeNr;
            }

            bool node_set::empty() const
            {
                return (count() == 0); // TODO: best way?
            }

            node_set::iterator node_set::begin()
            {
                xmlNodeSetPtr nset = static_cast<xmlXPathObjectPtr>(data)->nodesetval;
                return node_set::iterator(nset, 0);
            }

            node_set::iterator node_set::end()
            {
                xmlNodeSetPtr nset = static_cast<xmlXPathObjectPtr>(data)->nodesetval;
                return iterator(nset, nset->nodeNr);
            }


	    bool node_set::contains(const xml::node& n) const
	    {
                xmlNodeSetPtr nset = static_cast<xmlXPathObjectPtr>(data)->nodesetval;
                xmlNodePtr nodeptr = static_cast<xmlNodePtr>(const_cast<xml::node&>(n).get_node_data());

                return (xmlXPathNodeSetContains(nset, nodeptr) == 1);
            }


            //-----------------------
            //node_set::iterator
            //----------------------

            node_set::iterator::iterator (void* data, int pos) : data(data), pos(pos)
            {
                pimpl_ = new xml::impl::xpitimpl();
            }

            node_set::iterator& node_set::iterator::operator++()
            {
                if (pos < static_cast<xmlNodeSetPtr>(data)->nodeNr)
                    ++pos;

                return *this;
            }

            node_set::iterator node_set::iterator::operator++(int)
            {
                node_set::iterator tmp = *this;
                ++(*this);
                return tmp;
            }
            xml::node& xml::xpath::node_set::iterator::operator*()
            {
                pimpl_->set_data((static_cast<xmlNodeSetPtr>(data)->nodeTab)[pos]);
                return pimpl_->get();
            }

            xml::node* xml::xpath::node_set::iterator::operator->()
            {
                pimpl_->set_data((static_cast<xmlNodeSetPtr>(data)->nodeTab)[pos]);
                return &(pimpl_->get());
            }

            node_set::iterator::~iterator()
            {
                delete pimpl_;
            }

            node_set::iterator& node_set::iterator::operator=(const iterator& i)
            {
                data = i.data;
                pos = i.pos;
                return *this;
            }

            node_set::iterator::iterator(const node_set::iterator& i) : data(i.data), pos(i.pos), pimpl_(new impl::xpitimpl)
            {
            }

            //-----------------
            // xpath::context
            //-----------------

            context::context(const xml::document& doc)
            {
                ctxtptr = xmlXPathNewContext(static_cast<xmlDocPtr>(doc.get_doc_data_read_only()));
            }

            context::~context()
            {
                xmlXPathFreeContext(static_cast<xmlXPathContextPtr>(ctxtptr));
            }

            void context::registerNamespace(const char* prefix, const char* href)
            {
                xmlXPathRegisterNs(static_cast<xmlXPathContextPtr>(ctxtptr),
                        reinterpret_cast<const xmlChar*>(prefix),
                        reinterpret_cast<const xmlChar*>(href));
            }

            node_set context::evaluate(const char* expr)
            {
                xmlXPathObjectPtr nsptr = xmlXPathEvalExpression(reinterpret_cast<const xmlChar*>(expr),
                        static_cast<xmlXPathContextPtr>(ctxtptr));
                return node_set(nsptr);
            }

        }
}
