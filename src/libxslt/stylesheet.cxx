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
 * This file contains the implementation of the xslt::stylesheet class.
**/

// xmlwrapp includes
#include "xsltwrapp/stylesheet.h"
#include "xmlwrapp/document.h"
#include "xmlwrapp/tree_parser.h"

// libxslt includes
#include <libxslt/xslt.h>
#include <libxslt/xsltInternals.h>
#include <libxslt/transform.h>
#include <libxslt/xsltutils.h>

// standard includes
#include <stdexcept>
#include <memory>
#include <string>
#include <vector>
#include <map>

//####################################################################
namespace {
    void make_vector_param (std::vector<const char*> &v, const xslt::stylesheet::param_type &p);
    xmlDocPtr apply_stylesheet (xsltStylesheetPtr s, xmlDocPtr d, const xslt::stylesheet::param_type *p=0);
}
//####################################################################
struct xslt::stylesheet::pimpl {
    pimpl (void) : ss_(0) { }

    xsltStylesheetPtr ss_;
    xml::document doc_;
    std::string error_;
};
//####################################################################
xslt::stylesheet::stylesheet (const char *filename) {
    std::auto_ptr<pimpl> ap(pimpl_ = new pimpl);

    xml::tree_parser parser(filename);
    xmlDocPtr xmldoc = static_cast<xmlDocPtr>(parser.get_document().get_doc_data());

    if ( (pimpl_->ss_ = xsltParseStylesheetDoc(xmldoc)) == 0) {
	// TODO error_ can't get set yet. Need changes from libxslt first
	if (pimpl_->error_.empty()) pimpl_->error_ = "unknown XSLT parser error";
	throw std::runtime_error(pimpl_->error_);
    }

    /*
     * if we got this far, the xmldoc we gave to xsltParseStylesheetDoc is
     * now owned by the stylesheet and will be cleaned up in our destructor.
     */
    parser.get_document().release_doc_data();
    ap.release();
}
//####################################################################
xslt::stylesheet::stylesheet (xml::document doc) {
    xmlDocPtr xmldoc = static_cast<xmlDocPtr>(doc.get_doc_data());
    std::auto_ptr<pimpl> ap(pimpl_ = new pimpl);

    if ( (pimpl_->ss_ = xsltParseStylesheetDoc(xmldoc)) == 0) {
	// TODO error_ can't get set yet. Need changes from libxslt first
	if (pimpl_->error_.empty()) pimpl_->error_ = "unknown XSLT parser error";
	throw std::runtime_error(pimpl_->error_);
    }

    /*
     * if we got this far, the xmldoc we gave to xsltParseStylesheetDoc is
     * now owned by the stylesheet and will be cleaned up in our destructor.
     */
    doc.release_doc_data();
    ap.release();
}
//####################################################################
xslt::stylesheet::~stylesheet (void) {
    if (pimpl_->ss_) xsltFreeStylesheet(pimpl_->ss_);
    delete pimpl_;
}
//####################################################################
bool xslt::stylesheet::apply (xml::document &doc, xml::document &result) {
    xmlDocPtr input = static_cast<xmlDocPtr>(doc.get_doc_data());
    xmlDocPtr xmldoc = apply_stylesheet(pimpl_->ss_, input); 

    if (xmldoc) {
	result.set_doc_data_from_xslt(xmldoc, pimpl_->ss_);
	return true;
    }

    return false;
}
//####################################################################
bool xslt::stylesheet::apply (xml::document &doc, xml::document &result, const param_type &with_params) {
    xmlDocPtr input = static_cast<xmlDocPtr>(doc.get_doc_data());
    xmlDocPtr xmldoc = apply_stylesheet(pimpl_->ss_, input, &with_params); 

    if (xmldoc) {
	result.set_doc_data_from_xslt(xmldoc, pimpl_->ss_);
	return true;
    }

    return false;
}
//####################################################################
xml::document& xslt::stylesheet::apply (xml::document &doc) {
    xmlDocPtr input = static_cast<xmlDocPtr>(doc.get_doc_data());
    xmlDocPtr xmldoc = apply_stylesheet(pimpl_->ss_, input);

    if (xmldoc == 0) {
	if (pimpl_->error_.empty()) pimpl_->error_ = "unknown XSLT transformation error";
	throw std::runtime_error(pimpl_->error_);
    }

    pimpl_->doc_.set_doc_data_from_xslt(xmldoc, pimpl_->ss_);
    return pimpl_->doc_;
}
//####################################################################
xml::document& xslt::stylesheet::apply (xml::document &doc, const param_type &with_params) {
    xmlDocPtr input = static_cast<xmlDocPtr>(doc.get_doc_data());
    xmlDocPtr xmldoc = apply_stylesheet(pimpl_->ss_, input, &with_params);

    if (xmldoc == 0) {
	if (pimpl_->error_.empty()) pimpl_->error_ = "unknown XSLT transformation error";
	throw std::runtime_error(pimpl_->error_);
    }

    pimpl_->doc_.set_doc_data_from_xslt(xmldoc, pimpl_->ss_);
    return pimpl_->doc_;
}
//####################################################################
const std::string& xslt::stylesheet::get_error_message (void) const {
    return pimpl_->error_;
}
//####################################################################
namespace {
    //####################################################################
    void make_vector_param (std::vector<const char*> &v, const xslt::stylesheet::param_type &p) {
	v.reserve(p.size());

	xslt::stylesheet::param_type::const_iterator i=p.begin(), end=p.end();
	for (; i!=end; ++i) {
	    v.push_back(i->first.c_str());
	    v.push_back(i->second.c_str());
	}

	v.push_back(static_cast<const char*>(0));
    }
    //####################################################################
    xmlDocPtr apply_stylesheet (xsltStylesheetPtr s, xmlDocPtr d, const xslt::stylesheet::param_type *p) {
	/*
	 * TODO TODO TODO TODO
	 *
	 * use a transform context to capture error messages
	 *
	 * TODO TODO TODO TODO
	 */
	std::vector<const char*> v;
	if (p) make_vector_param(v, *p);
	return xsltApplyStylesheet(s, d, p ? &v[0] : 0);
    }
    //####################################################################
}
//####################################################################
