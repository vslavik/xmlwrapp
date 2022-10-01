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

#include "errors_impl.h"
#include "node_iterator.h"
#include "utility.h"

// libxml includes
#include <libxml/xmlversion.h>
#include <libxml/tree.h>
#include <libxml/xpath.h>
#include <libxml/xpathInternals.h>

#include <map>

using namespace xml::impl;

namespace
{

// xmlXPathNodeEval was introduced in 2.9.1, use a helper reimplementation
// with older versions:
#if LIBXML_VERSION < 20901
xmlXPathObjectPtr xmlXPathNodeEval(xmlNodePtr node, const xmlChar *str, xmlXPathContextPtr ctx)
{
    ctx->node = node;
    return xmlXPathEval(str, ctx);
}
#endif // LIBXML_VERSION < 20901

} // anonymous namespace

namespace xml
{

namespace impl
{

// Function for iterating over a libxml nodeset.
// Takes ownership of the path object passed to it
class nodeset_next_functor : public impl::iter_advance_functor
{
public:
    explicit nodeset_next_functor(xmlXPathObjectPtr pathobj)
    {
        // TODO: This isn't efficient, it would be better if node_iterator was
        //       able to use a functor that remembers the index.

        const int length = pathobj->nodesetval->nodeNr;
        const xmlNodePtr *table = pathobj->nodesetval->nodeTab;

        for ( int i = 0; i < length-1; i++ )
            m_next[table[i]] = table[i+1];
    }

    virtual xmlNodePtr operator()(xmlNodePtr node) const
    {
        const NextNodeMap::const_iterator i = m_next.find(node);
        if ( i == m_next.end() )
            return nullptr;
        else
            return i->second;
    }

private:
    typedef std::map<xmlNodePtr, xmlNodePtr> NextNodeMap;
    NextNodeMap m_next;
};


// Need the wrapper with C++ linkage as extern "C" xmlXPathFreeObject itself
// can't be used as xml_scoped_ptr template parameter.
inline void wrap_xmlXPathFreeObject(xmlXPathObjectPtr ptr)
{
    xmlXPathFreeObject(ptr);
}

struct xpath_context_impl
{
    explicit xpath_context_impl(const document& doc) : doc_(doc)
    {
        ctxt_ = xmlXPathNewContext(static_cast<xmlDocPtr>(doc.get_doc_data_read_only()));
    }

    ~xpath_context_impl()
    {
        if (ctxt_)
            xmlXPathFreeContext(ctxt_);
    }

    template<typename NodesView>
    NodesView evaluate(const std::string& expr, node& n, error_handler& on_error)
    {
        xmlNodePtr xmlnode = reinterpret_cast<xmlNodePtr>(n.get_node_data());
        if ( xmlnode->doc != ctxt_->doc )
        {
            throw xml::exception("node doesn't belong to context's document");
        }

        impl::global_errors_collector err;

        xml_scoped_ptr<xmlXPathObjectPtr, wrap_xmlXPathFreeObject> nsptr(
            xmlXPathNodeEval(xmlnode, xml_string(expr), ctxt_));

        err.replay(on_error);

        if ( !nsptr )
            return NodesView();

        if ( xmlXPathNodeSetIsEmpty(nsptr->nodesetval) )
            return NodesView();

        return NodesView(nsptr->nodesetval->nodeTab[0],
                         new nodeset_next_functor(nsptr));
    }

    const document&    doc_;
    xmlXPathContextPtr ctxt_;

private:
    // non-copyable
    xpath_context_impl(const xpath_context_impl&);
    xpath_context_impl& operator=(const xpath_context_impl&);
};

} // namespace impl


xpath_context::xpath_context(const document& doc)
{
    pimpl_ = new impl::xpath_context_impl(doc);
}

xpath_context::~xpath_context()
{
    delete pimpl_;
}

void xpath_context::register_namespace(const std::string& prefix, const std::string& href)
{
    xmlXPathRegisterNs(pimpl_->ctxt_, xml_string(prefix), xml_string(href));
}

const_nodes_view xpath_context::evaluate(const std::string& expr, error_handler& on_error)
{
    return evaluate(expr, const_cast<document&>(pimpl_->doc_).get_root_node(), on_error);
}

const_nodes_view xpath_context::evaluate(const std::string& expr, const node& n, error_handler& on_error)
{
    return pimpl_->evaluate<const_nodes_view>(expr, const_cast<node&>(n), on_error);
}

nodes_view xpath_context::evaluate(const std::string& expr, node& n, error_handler& on_error)
{
    return pimpl_->evaluate<nodes_view>(expr, n, on_error);
}

} // namespace xml
