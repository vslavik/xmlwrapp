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
#include "xmlwrapp/tree_parser.h"
#include "xmlwrapp/document.h"
#include "xmlwrapp/errors.h"
#include "cpp11.h"
#include "utility.h"
#include "errors_impl.h"

// libxml includes
#include <libxml/parser.h>
#include <libxml/parserInternals.h>
#include <libxml/xmlversion.h>
#if LIBXML_VERSION >= 20600
    #define xmlwrapp_initDefaultSAXHandler xmlSAX2InitDefaultSAXHandler
    #include <libxml/SAX2.h>
#else
    #define xmlwrapp_initDefaultSAXHandler initxmlDefaultSAXHandler
    #include <libxml/SAX.h>
#endif


// standard includes
#include <stdexcept>
#include <cstring>
#include <cstdio>
#include <string>
#include <memory>

namespace xml
{

using namespace impl;

// ------------------------------------------------------------------------
// xml::impl::tree_impl
// ------------------------------------------------------------------------

struct impl::tree_impl
{
    tree_impl();

    document doc_;
    xmlSAXHandler sax_;
    errors_collector messages_;

    // for backward compatibility only, to be removed
    std::string get_error_message_cache_;
};

namespace
{

const char DEFAULT_ERROR[] = "unknown XML parsing error";

extern "C" void cb_tree_error(void *v, const char *message, ...)
{
    xmlParserCtxtPtr ctxt = static_cast<xmlParserCtxtPtr>(v);
    tree_impl *p = static_cast<tree_impl*>(ctxt->_private);
    if (!p)
        return; // handle bug in older versions of libxml

    xmlStopParser(ctxt);
    CALL_CB_MESSAGES_ERROR(&p->messages_, message);
}

extern "C" void cb_tree_warning(void *v, const char *message, ...)
{
    xmlParserCtxtPtr ctxt = static_cast<xmlParserCtxtPtr>(v);
    tree_impl *p = static_cast<tree_impl*>(ctxt->_private);
    if (!p)
        return; // handle bug in older versions of libxml

    CALL_CB_MESSAGES_WARNING(&p->messages_, message);
}


extern "C" void cb_tree_ignore(void*, const xmlChar*, int)
{
}

} // anonymous namespace


impl::tree_impl::tree_impl()
{
    std::memset(&sax_, 0, sizeof(sax_));
    xmlwrapp_initDefaultSAXHandler(&sax_, 0);

    sax_.warning    = cb_tree_warning;
    sax_.error      = cb_tree_error;
    sax_.fatalError = cb_tree_error;

    if (xmlKeepBlanksDefaultValue == 0)
        sax_.ignorableWhitespace =  cb_tree_ignore;
}


// ------------------------------------------------------------------------
// xml::tree_parser
// ------------------------------------------------------------------------

tree_parser::tree_parser(const char *name, bool allow_exceptions)
{
    init(name, allow_exceptions ? &throw_on_error : NULL);
}

tree_parser::tree_parser(const char *name, error_handler& on_error)
{
    init(name, &on_error);
}

void tree_parser::init(const char *name, error_handler *on_error)
{
    xml::impl::auto_ptr<tree_impl> ap(pimpl_ = new tree_impl);

    // Errors happening before the document is parsed, e.g. IO errors, are
    // logged using the global function and not the SAX handler callbacks, so
    // it's important to install our sink as global one in order to receive
    // these messages too.
    impl::global_errors_installer install_as_global(pimpl_->messages_);

    xmlDocPtr tmpdoc = xmlSAXParseFileWithData(&(pimpl_->sax_), name, 0, pimpl_);

    if (tmpdoc && !pimpl_->messages_.has_errors())
    {
        // all is fine
        pimpl_->doc_.set_doc_data(tmpdoc);
    }
    else
    {
        if ( !pimpl_->messages_.has_errors() )
        {
            // Provide at least some error message.
            pimpl_->messages_.on_error(DEFAULT_ERROR);
        }

        // a problem appeared
        if (tmpdoc)
            xmlFreeDoc(tmpdoc);

        if (on_error)
            pimpl_->messages_.replay(*on_error);
    }

    ap.release();
}


tree_parser::tree_parser(const char *data, size_type size, bool allow_exceptions)
{
    init(data, size, allow_exceptions ? &throw_on_error : NULL);
}

tree_parser::tree_parser(const char *data, size_type size, error_handler& on_error)
{
    init(data, size, &on_error);
}

void tree_parser::init(const char *data, size_type size, error_handler *on_error)
{
    xml::impl::auto_ptr<tree_impl> ap(pimpl_ = new tree_impl);
    xmlParserCtxtPtr ctxt;

    if ( (ctxt = xmlCreateMemoryParserCtxt(data, xml::impl::checked_int_cast(size))) == 0)
        throw std::bad_alloc();

    if (ctxt->sax)
        xmlFree(ctxt->sax);

    ctxt->sax = &(pimpl_->sax_);

    ctxt->_private = pimpl_;

    const int retval = xmlParseDocument(ctxt);

    if (!ctxt->wellFormed || retval != 0 || pimpl_->messages_.has_errors())
    {
        xmlFreeDoc(ctxt->myDoc);
        ctxt->myDoc = 0;
        ctxt->sax = 0;
        xmlFreeParserCtxt(ctxt);

        if (on_error)
            pimpl_->messages_.replay(*on_error);

        ap.release();
        return; // handle non-exception case
    }

    pimpl_->doc_.set_doc_data(ctxt->myDoc);
    ctxt->sax = 0;

    xmlFreeParserCtxt(ctxt);
    ap.release();
}


tree_parser::~tree_parser()
{
    delete pimpl_;
}


bool tree_parser::operator!() const
{
    return messages().has_errors();
}

const error_messages& tree_parser::messages() const
{
    return pimpl_->messages_;
}


const std::string& tree_parser::get_error_message() const
{
    if ( pimpl_->get_error_message_cache_.empty() )
    {
        if ( messages().has_errors() )
            pimpl_->get_error_message_cache_ = messages().print();
        else
            pimpl_->get_error_message_cache_ = DEFAULT_ERROR;
    }
    return pimpl_->get_error_message_cache_;
}


bool tree_parser::had_warnings() const
{
    return messages().has_warnings();
}


document& tree_parser::get_document()
{
    return pimpl_->doc_;
}


const document& tree_parser::get_document() const
{
    return pimpl_->doc_;
}

} // namespace xml
