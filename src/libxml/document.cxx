/*
 * Copyright (C) 2001-2003 Peter J Jones (pjones@pmade.org)
 * Copyright (C) 2013 Vaclav Slavik <vslavik@gmail.com>
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
#include "xmlwrapp/document.h"
#include "xmlwrapp/node.h"
#include "xmlwrapp/errors.h"
#include "xmlwrapp/tree_parser.h"

#include "errors_impl.h"
#include "utility.h"
#include "cpp11.h"
#include "dtd_impl.h"
#include "node_manip.h"

// standard includes
#include <new>
#include <memory>
#include <iterator>
#include <iostream>
#include <algorithm>
#include <stdexcept>

// libxml includes
#include <libxml/tree.h>
#include <libxml/xinclude.h>

// bring in private libxslt stuff (see bug #1927398)
#include "../libxslt/result.h"

namespace xml
{

using namespace impl;

namespace
{
    const char DEFAULT_ENCODING[] = "ISO-8859-1";
}

// ------------------------------------------------------------------------
// xml::impl::doc_impl
// ------------------------------------------------------------------------

namespace impl
{

struct doc_impl
{
    doc_impl()
        : doc_(0), xslt_result_(0)
    {
        xmlDocPtr tmpdoc;
        if ( (tmpdoc = xmlNewDoc(0)) == 0)
            throw std::bad_alloc();
        set_doc_data(tmpdoc, true);
    }


    doc_impl(const char *root_name)
        : doc_(0), xslt_result_(0), root_(root_name)
    {
        xmlDocPtr tmpdoc;
        if ( (tmpdoc = xmlNewDoc(0)) == 0)
            throw std::bad_alloc();
        set_doc_data(tmpdoc, true);
    }


    doc_impl(const doc_impl& other)
        : doc_(0), xslt_result_(0)
    {
        xmlDocPtr tmpdoc;
        if ( (tmpdoc = xmlCopyDoc(other.doc_, 1)) == 0)
            throw std::bad_alloc();
        set_doc_data(tmpdoc, false);
    }


    void set_doc_data(xmlDocPtr newdoc, bool root_is_okay)
    {
        if (doc_)
            xmlFreeDoc(doc_);
        doc_ = newdoc;

        if (doc_->version)
            version_ = reinterpret_cast<const char*>(doc_->version);
        if (doc_->encoding)
            encoding_ = reinterpret_cast<const char*>(doc_->encoding);

        if (root_is_okay)
        {
            xmlDocSetRootElement(doc_, static_cast<xmlNodePtr>(root_.release_node_data()));
        }
        else
        {
            xmlNodePtr libxml_root_node = xmlDocGetRootElement(doc_);

            if (libxml_root_node)
            {
                root_.set_node_data(libxml_root_node);
            }
            else
            {
                node tmpnode;
                root_.swap(tmpnode);

                xmlDocSetRootElement(doc_, static_cast<xmlNodePtr>(root_.release_node_data()));
            }
        }
    }


    void set_root_node(const node& n)
    {
        node &non_const_node = const_cast<node&>(n);
        xmlNodePtr new_root_node = xmlCopyNode(static_cast<xmlNodePtr>(non_const_node.get_node_data()), 1);
        if (!new_root_node)
            throw std::bad_alloc();

        xmlNodePtr old_root_node = xmlDocSetRootElement(doc_, new_root_node);
        root_.set_node_data(new_root_node);
        if (old_root_node)
            xmlFreeNode(old_root_node);

        xslt_result_ = 0;
    }


    ~doc_impl()
    {
        if (doc_)
            xmlFreeDoc(doc_);
        delete xslt_result_;
    }

    xmlDocPtr doc_;
    xslt::impl::result *xslt_result_;
    node root_;
    std::string version_;
    mutable std::string encoding_;
};

} // namespace impl


// ------------------------------------------------------------------------
// xml::document
// ------------------------------------------------------------------------

document::document()
{
    pimpl_ = new doc_impl;
}


document::document(const char *root_name)
{
    pimpl_ = new doc_impl(root_name);
}


document::document(const node& n)
{
    xml::impl::auto_ptr<doc_impl> ap(pimpl_ = new doc_impl);
    pimpl_->set_root_node(n);
    ap.release();
}

document::document(const char *filename, error_handler& on_error)
{
    pimpl_ = new doc_impl;
    tree_parser p(filename, on_error);
    if ( !p )
        throw exception(p.messages());
    swap(p.get_document());
}

document::document(const char *data, size_type size, error_handler& on_error)
{
    pimpl_ = new doc_impl;
    tree_parser p(data, size, on_error);
    if ( !p )
        throw exception(p.messages());
    swap(p.get_document());
}

document::document(const document& other)
{
    pimpl_ = new doc_impl(*(other.pimpl_));
}


document& document::operator=(const document& other)
{
    document tmp(other);
    swap(tmp);
    return *this;
}


void document::swap(document& other)
{
    std::swap(pimpl_, other.pimpl_);
}


document::~document()
{
    delete pimpl_;
}


const node& document::get_root_node() const
{
    return pimpl_->root_;
}


node& document::get_root_node()
{
    return pimpl_->root_;
}


void document::set_root_node(const node& n)
{
    pimpl_->set_root_node(n);
}


const std::string& document::get_version() const
{
    return pimpl_->version_;
}


void document::set_version(const char *version)
{
    const xmlChar *old_version = pimpl_->doc_->version;
    if ( (pimpl_->doc_->version = xmlStrdup(reinterpret_cast<const xmlChar*>(version))) == 0)
        throw std::bad_alloc();

    pimpl_->version_ = version;
    if (old_version)
        xmlFree(const_cast<char*>(reinterpret_cast<const char*>(old_version)));
}


const std::string& document::get_encoding() const
{
    if (pimpl_->encoding_.empty())
        pimpl_->encoding_ = DEFAULT_ENCODING;
    return pimpl_->encoding_;
}


void document::set_encoding(const char *encoding)
{
    pimpl_->encoding_ = encoding;

    if (pimpl_->doc_->encoding)
        xmlFree(const_cast<xmlChar*>(pimpl_->doc_->encoding));

    pimpl_->doc_->encoding = xmlStrdup(reinterpret_cast<const xmlChar*>(encoding));

    if (!pimpl_->doc_->encoding)
        throw std::bad_alloc();
}


bool document::get_is_standalone() const
{
    return pimpl_->doc_->standalone == 1;
}


void document::set_is_standalone(bool sa)
{
    pimpl_->doc_->standalone = sa ? 1 : 0;
}


bool document::process_xinclude()
{
    // xmlXIncludeProcess does not return what is says it does
    return xmlXIncludeProcess(pimpl_->doc_) >= 0;
}


bool document::has_internal_subset() const
{
    return pimpl_->doc_->intSubset != 0;
}


bool document::has_external_subset() const
{
    return pimpl_->doc_->extSubset != 0;
}


bool document::validate()
{
    dtd_impl dtd;
    return dtd.validate(pimpl_->doc_);
}


bool document::validate(const char *dtdname)
{
    dtd_impl dtd(dtdname);

    if (!dtd.error_.empty())
        return false;
    if (!dtd.validate(pimpl_->doc_))
        return false;

    // remove the old DTD
    if (pimpl_->doc_->extSubset != 0)
        xmlFreeDtd(pimpl_->doc_->extSubset);

    pimpl_->doc_->extSubset = dtd.release();

    return true;
}


document::size_type document::size() const
{
    using namespace std;
    return distance(begin(), end());
}


node::iterator document::begin()
{
    return node::iterator(pimpl_->doc_->children);
}


node::const_iterator document::begin() const
{
    return node::const_iterator(pimpl_->doc_->children);
}


node::iterator document::end()
{
    return node::iterator(0);
}


node::const_iterator document::end() const
{
    return node::const_iterator(0);
}


void document::push_back(const node& child)
{
    if (child.get_type() == node::type_element)
        throw xml::exception("xml::document::push_back can't take element type nodes");

    impl::node_insert
          (
              reinterpret_cast<xmlNodePtr>(pimpl_->doc_),
              0,
              static_cast<xmlNodePtr>(const_cast<node&>(child).get_node_data())
          );
}


node::iterator document::insert(const node& n)
{
    if (n.get_type() == node::type_element)
        throw xml::exception("xml::document::insert can't take element type nodes");

    return node::iterator(xml::impl::node_insert(reinterpret_cast<xmlNodePtr>(pimpl_->doc_), 0, static_cast<xmlNodePtr>(const_cast<node&>(n).get_node_data())));
}


node::iterator document::insert(node::iterator position, const node& n)
{
    if (n.get_type() == node::type_element)
        throw xml::exception("xml::document::insert can't take element type nodes");

    return node::iterator(xml::impl::node_insert(reinterpret_cast<xmlNodePtr>(pimpl_->doc_), static_cast<xmlNodePtr>(position.get_raw_node()), static_cast<xmlNodePtr>(const_cast<node&>(n).get_node_data())));
}


node::iterator document::replace(node::iterator old_node, const node& new_node)
{
    if (old_node->get_type() == node::type_element || new_node.get_type() == node::type_element)
    {
        throw xml::exception("xml::document::replace can't replace element type nodes");
    }

    return node::iterator(xml::impl::node_replace(static_cast<xmlNodePtr>(old_node.get_raw_node()), static_cast<xmlNodePtr>(const_cast<node&>(new_node).get_node_data())));
}


node::iterator document::erase(node::iterator to_erase)
{
    if (to_erase->get_type() == node::type_element)
        throw xml::exception("xml::document::erase can't erase element type nodes");
    return node::iterator(xml::impl::node_erase(static_cast<xmlNodePtr>(to_erase.get_raw_node())));
}


node::iterator document::erase(node::iterator first, node::iterator last)
{
    while (first != last)
        first = erase(first);
    return first;
}


void document::save_to_string(std::string& s, error_handler& on_error) const
{
    impl::global_errors_collector err;

    if (pimpl_->xslt_result_ != 0)
    {
        pimpl_->xslt_result_->save_to_string(s);
    }
    else
    {
        xmlChar *xml_string;
        int xml_string_length;

        const char *enc = pimpl_->encoding_.empty() ? 0 : pimpl_->encoding_.c_str();
        xmlDocDumpFormatMemoryEnc(pimpl_->doc_, &xml_string, &xml_string_length, enc, 1);

        xmlchar_helper helper(xml_string);
        if (xml_string_length)
            s.assign(helper.get(), xml_string_length);
    }

    err.replay(on_error);
}


bool document::save_to_file(const char *filename, int compression_level, error_handler& on_error) const
{
    impl::global_errors_collector err;

    // Helper to temporarily change document compression parameter.
    class set_compression_in_scope
    {
    public:
        set_compression_in_scope(xmlDocPtr doc, int compression_level) :
            doc_(doc),
            compression_level_orig_(doc_->compression)
        {
            doc_->compression = compression_level;
        }

        ~set_compression_in_scope()
        {
            doc_->compression = compression_level_orig_;
        }

    private:
        const xmlDocPtr doc_;
        const int compression_level_orig_;
    } set_compression(pimpl_->doc_, compression_level);

    bool rc;
    if (pimpl_->xslt_result_ != 0)
    {
        rc = pimpl_->xslt_result_->save_to_file(filename, compression_level);
    }
    else
    {
        const char *enc = pimpl_->encoding_.empty() ? 0 : pimpl_->encoding_.c_str();
        rc = xmlSaveFormatFileEnc(filename, pimpl_->doc_, enc, 1) > 0;
    }

    err.replay(on_error);

    return rc;
}


void document::set_doc_data(void *data)
{
    // we own the doc now, don't free it!
    pimpl_->set_doc_data(static_cast<xmlDocPtr>(data), false);
    pimpl_->xslt_result_ = 0;
}


void document::set_doc_data_from_xslt(void *data, xslt::impl::result *xr)
{
    // this document came from a XSLT transformation
    pimpl_->set_doc_data(static_cast<xmlDocPtr>(data), false);
    pimpl_->xslt_result_ = xr;
}


void* document::get_doc_data()
{
    return pimpl_->doc_;
}


void* document::get_doc_data_read_only() const
{
    return pimpl_->doc_;
}


void* document::release_doc_data()
{
    xmlDocPtr xmldoc = pimpl_->doc_;
    pimpl_->doc_ = 0;

    return xmldoc;
}


std::ostream& operator<<(std::ostream& stream, const document& doc)
{
    std::string xmldata;
    doc.save_to_string(xmldata);
    stream << xmldata;
    return stream;
}

} // namespace xml
