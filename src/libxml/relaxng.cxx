/*
 * Copyright (C) 2018 Vadim Zeitlin <vz-xmlwrapp@zeitlins.org>
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
 *    the relaxngation and/or other materials provided with the
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
#include "xmlwrapp/relaxng.h"
#include "xmlwrapp/document.h"
#include "xmlwrapp/errors.h"

#include "errors_impl.h"

// libxml includes
#include <libxml/relaxng.h>

namespace xml
{

using namespace impl;

// ------------------------------------------------------------------------
// xml::impl::relaxng_impl
// ------------------------------------------------------------------------

namespace impl
{

struct relaxng_impl
{
    relaxng_impl(xmlDocPtr xmldoc, error_handler& on_error)
    {
        impl::errors_collector err;

        xmlRelaxNGParserCtxtPtr ctxt = xmlRelaxNGNewDocParserCtxt(xmldoc);
        if ( !ctxt )
            throw std::bad_alloc();
        xmlRelaxNGSetParserErrors(ctxt,
                                  cb_messages_error, cb_messages_warning,
                                  &err);

        relaxng_ = xmlRelaxNGParse(ctxt);
        xmlRelaxNGFreeParserCtxt(ctxt);

        if ( !relaxng_ )
        {
            err.replay(on_error);
            // if the handler didn't throw, do it ourselves -- it's the only
            // way to signal fatal errors from a ctor:
            throw exception(err);
        }
    }

    ~relaxng_impl()
    {
        if (relaxng_)
            xmlRelaxNGFree(relaxng_);
    }

    xmlRelaxNGPtr relaxng_{nullptr};
};

} // namespace impl


// ------------------------------------------------------------------------
// xml::relaxng
// ------------------------------------------------------------------------

relaxng::relaxng(const document& doc, error_handler& on_error)
{
    // Note that, unlike xmlSchemaNewDocParserCtxt(), we don't need to make a
    // copy of the document here as xmlRelaxNGNewDocParserCtxt() already does
    // it internally.
    auto xmldoc = static_cast<xmlDocPtr>(doc.get_doc_data_read_only());

    pimpl_.reset(new relaxng_impl(xmldoc, on_error));
}

relaxng::~relaxng() = default;


bool relaxng::validate(const document& doc, error_handler& on_error) const
{
    auto xmldoc = static_cast<xmlDocPtr>(doc.get_doc_data_read_only());

    xmlRelaxNGValidCtxtPtr ctxt = xmlRelaxNGNewValidCtxt(pimpl_->relaxng_);
    if ( !ctxt )
        throw std::bad_alloc();

    impl::errors_collector err;
    xmlRelaxNGSetValidErrors(ctxt,
                             cb_messages_error, cb_messages_warning,
                             &err);

    int ret = xmlRelaxNGValidateDoc(ctxt, xmldoc);

    xmlRelaxNGFreeValidCtxt(ctxt);

    if ( ret == -1 )
        throw xml::exception("internal validation error");

    err.replay(on_error);

    return ret == 0;
}

} // namespace xml
