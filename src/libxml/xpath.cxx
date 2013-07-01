/*
 * Copyright (C) 2011 Jonas Weber (mail@jonasw.de)
 *               2013 Vaclav Slavik <vslavik@gmail.com>
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

#include "node_iterator.h"

// libxml includes
#include <libxml/tree.h>
#include <libxml/xpath.h>
#include <libxml/xpathInternals.h>

#include <map>

namespace xml {

namespace
{

// Function for iterating over a libxml nodeset.
// Takes ownership of the path object passed to it
class nodeset_next_functor : public impl::iter_advance_functor
{
public:
    nodeset_next_functor(xmlXPathObjectPtr pathobj)
    {
        // TODO: This isn't efficient, it would be better if node_iterator was
        //       able to use a functor that remembers the index.

        const size_t length = pathobj->nodesetval->nodeNr;
        const xmlNodePtr *table = pathobj->nodesetval->nodeTab;

        for ( size_t i = 0; i < length-1; i++ )
            m_next[table[i]] = table[i+1];

        xmlXPathFreeObject(pathobj);
    }

    virtual xmlNodePtr operator()(xmlNodePtr node) const
    {
        const NextNodeMap::const_iterator i = m_next.find(node);
        if ( i == m_next.end() )
            return NULL;
        else
            return i->second;
    }

private:
    typedef std::map<xmlNodePtr, xmlNodePtr> NextNodeMap;
    NextNodeMap m_next;
};

} // anonymous namespace

    namespace xpath {
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

            void context::register_namespace(const char* prefix, const char* href)
            {
                xmlXPathRegisterNs(static_cast<xmlXPathContextPtr>(ctxtptr),
                        reinterpret_cast<const xmlChar*>(prefix),
                        reinterpret_cast<const xmlChar*>(href));
            }

            const_nodes_view context::evaluate(const char* expr)
            {
                // TODO: use auto ptr for this
                xmlXPathObjectPtr nsptr = xmlXPathEval(reinterpret_cast<const xmlChar*>(expr),
                        static_cast<xmlXPathContextPtr>(ctxtptr));

                if ( !nsptr )
                    return const_nodes_view();
                if ( xmlXPathNodeSetIsEmpty(nsptr->nodesetval) )
                {
                    xmlXPathFreeObject(nsptr);
                    return const_nodes_view();
                }

                return const_nodes_view
                       (
                           nsptr->nodesetval->nodeTab[0],
                           new nodeset_next_functor(nsptr)
                       );
            }

        }
}
