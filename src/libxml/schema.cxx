/*
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
 *    the schemaation and/or other materials provided with the
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
#include "xmlwrapp/schema.h"
#include "xmlwrapp/document.h"
#include "xmlwrapp/errors.h"

#include "errors_impl.h"

// libxml includes
#include <libxml/xmlschemas.h>

namespace xml
{

using namespace impl;

// ------------------------------------------------------------------------
// xml::impl::schema_impl
// ------------------------------------------------------------------------

namespace impl
{

struct schema_impl
{
    schema_impl(xmlDocPtr xmldoc, error_handler& on_error)
    {
        impl::errors_collector err;

        xmlSchemaParserCtxtPtr ctxt = xmlSchemaNewDocParserCtxt(xmldoc);
        if ( !ctxt )
            throw std::bad_alloc();
        xmlSchemaSetParserErrors(ctxt,
                                 cb_messages_error, cb_messages_warning,
                                 &err);

        schema_ = xmlSchemaParse(ctxt);
        xmlSchemaFreeParserCtxt(ctxt);

        if ( !schema_ )
        {
            err.replay(on_error);
            // if the handler didn't throw, do it ourselves -- it's the only
            // way to signal fatal errors from a ctor:
            throw exception(err);
        }
    }

    ~schema_impl()
    {
        if (schema_)
            xmlSchemaFree(schema_);
        if (retainDoc_)
            xmlFreeDoc(retainDoc_);
    }

    xmlSchemaPtr schema_{nullptr};
    xmlDocPtr    retainDoc_{nullptr};
};

} // namespace impl


// ------------------------------------------------------------------------
// xml::schema
// ------------------------------------------------------------------------

schema::schema(const document& doc, error_handler& on_error)
{
    // xmlSchemaNewDocParserCtxt() modifies the document during parsing,
    // so we have to make a private copy first
    document mydoc(doc);

    auto xmldoc = static_cast<xmlDocPtr>(mydoc.get_doc_data_read_only());
    pimpl_ = new schema_impl(xmldoc, on_error);

    // xmldoc is still used in the schema_, but it's not owned (and thus freed)
    // by it, we have to manage its lifetime outselves. Transfer the ownership
    // here and clear 'mydoc' of any data.
    //
    // Note that the document was already modified by libxml2 in the process of
    // parsing it, so it's not usable by the caller anymore.
    mydoc.release_doc_data();
    pimpl_->retainDoc_ = xmldoc;
}

schema::~schema()
{
    delete pimpl_;
}

bool schema::validate(const document& doc, error_handler& on_error) const
{
    auto xmldoc = static_cast<xmlDocPtr>(doc.get_doc_data_read_only());

    xmlSchemaValidCtxtPtr ctxt = xmlSchemaNewValidCtxt(pimpl_->schema_);
    if ( !ctxt )
        throw std::bad_alloc();

    impl::errors_collector err;
    xmlSchemaSetValidErrors(ctxt,
                            cb_messages_error, cb_messages_warning,
                            &err);

    int ret = xmlSchemaValidateDoc(ctxt, xmldoc);

    xmlSchemaFreeValidCtxt(ctxt);

    if ( ret == -1 )
        throw xml::exception("internal validation error");

    err.replay(on_error);

    return ret == 0;
}

} // namespace xml
