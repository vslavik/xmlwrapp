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
 * This file contains the implementation of the xml::document class.
**/

// xmlwrapp includes
#include "xmlwrapp/document.h"
#include "xmlwrapp/node.h"
#include "utility.h"
#include "dtd_impl.h"

// configure.pl generated file
#ifndef WIN32
#  include "../xmlwrapp_config.h"
#endif

// standard includes
#include <new>
#include <memory>
#include <algorithm>
#include <iostream>

// libxml includes
#include <libxml/tree.h>
#include <libxml/xinclude.h>

// bring in libxslt stuff if we need it.
#if defined(XMLWRAPP_WITH_XSLT)
#  include <libxslt/xslt.h>
#  include <libxslt/xsltInternals.h>
#  include <libxslt/transform.h>
#  include <libxslt/xsltutils.h>
#endif

//####################################################################
namespace {
    const char const_default_encoding[] = "ISO-8859-1";
}
//####################################################################
struct xml::doc_impl {
    //####################################################################
    doc_impl (void) : doc_(0), from_xslt_(0) { 
	xmlDocPtr tmpdoc;
	if ( (tmpdoc = xmlNewDoc(0)) == 0) throw std::bad_alloc();
	set_doc_data(tmpdoc, true);
    }
    //####################################################################
    doc_impl (const char *root_name) : doc_(0), from_xslt_(0), root_(root_name) {
	xmlDocPtr tmpdoc;
	if ( (tmpdoc = xmlNewDoc(0)) == 0) throw std::bad_alloc();
	set_doc_data(tmpdoc, true);
    }
    //####################################################################
    doc_impl (const doc_impl &other) : doc_(0), from_xslt_(0) {
	xmlDocPtr tmpdoc;
	if ( (tmpdoc = xmlCopyDoc(other.doc_, 1)) == 0) throw std::bad_alloc();
	set_doc_data(tmpdoc, false);
    }
    //####################################################################
    void set_doc_data (xmlDocPtr newdoc, bool root_is_okay) {
	if (doc_) xmlFreeDoc(doc_);
	doc_ = newdoc;

	if (doc_->version)  version_  = reinterpret_cast<const char*>(doc_->version);
	if (doc_->encoding) encoding_ = reinterpret_cast<const char*>(doc_->encoding);

	if (root_is_okay) {
	    xmlDocSetRootElement(doc_, static_cast<xmlNodePtr>(root_.release_node_data()));
	} else {
	    xmlNodePtr libxml_root_node = xmlDocGetRootElement(doc_);

	    if (libxml_root_node)  {
		root_.set_node_data(libxml_root_node);
	    } else { 
		node tmpnode;
		root_.swap(tmpnode);

		xmlDocSetRootElement(doc_, static_cast<xmlNodePtr>(root_.release_node_data()));
	    }
	}
    }
    //####################################################################
    void set_root_node (const node &n) {
	node &non_const_node = const_cast<node&>(n);
	xmlNodePtr new_root_node = xmlCopyNode(static_cast<xmlNodePtr>(non_const_node.get_node_data()), 1);
	if (!new_root_node) throw std::bad_alloc();

	xmlNodePtr old_root_node = xmlDocSetRootElement(doc_, new_root_node);
	root_.set_node_data(new_root_node);
	if (old_root_node) xmlFreeNode(old_root_node);

	from_xslt_ = 0;
    }
    //####################################################################
    ~doc_impl (void) {
	if (doc_) xmlFreeDoc(doc_);
    }
    //####################################################################

    xmlDocPtr doc_;
    void *from_xslt_;
    node root_;
    std::string version_;
    mutable std::string encoding_;
};
//####################################################################
xml::document::document (void) {
    pimpl_ = new doc_impl;
}
//####################################################################
xml::document::document (const char *root_name) {
    pimpl_ = new doc_impl(root_name);
}
//####################################################################
xml::document::document (const node &n) {
    std::auto_ptr<doc_impl> ap(pimpl_ = new doc_impl);
    pimpl_->set_root_node(n);
    ap.release();
}
//####################################################################
xml::document::document (const document &other) {
    pimpl_ = new doc_impl(*(other.pimpl_));
}
//####################################################################
xml::document& xml::document::operator= (const document &other) {
    document tmp(other);
    swap(tmp);
    return *this;
}
//####################################################################
void xml::document::swap (document &other) {
    std::swap(pimpl_, other.pimpl_);
}
//####################################################################
xml::document::~document (void) {
    delete pimpl_;
}
//####################################################################
const xml::node& xml::document::get_root_node (void) const {
    return pimpl_->root_;
}
//####################################################################
xml::node& xml::document::get_root_node (void) {
    return pimpl_->root_;
}
//####################################################################
void xml::document::set_root_node (const node &n) {
    pimpl_->set_root_node(n);
}
//####################################################################
const std::string& xml::document::get_version (void) const {
    return pimpl_->version_;
}
//####################################################################
void xml::document::set_version (const char *version) {
    const xmlChar *old_version = pimpl_->doc_->version;
    if ( (pimpl_->doc_->version = xmlStrdup(reinterpret_cast<const xmlChar*>(version))) == 0) throw std::bad_alloc();
    pimpl_->version_ = version;
    if (old_version) xmlFree(const_cast<char*>(reinterpret_cast<const char*>(old_version)));
}
//####################################################################
const std::string& xml::document::get_encoding (void) const {
    if (pimpl_->encoding_.empty()) pimpl_->encoding_ = const_default_encoding;
    return pimpl_->encoding_;
}
//####################################################################
void xml::document::set_encoding (const char *encoding) {
    pimpl_->encoding_ = encoding;

    if (pimpl_->doc_->encoding) xmlFree(const_cast<xmlChar*>(pimpl_->doc_->encoding));
    pimpl_->doc_->encoding = xmlStrdup(reinterpret_cast<const xmlChar*>(encoding));

    if (!pimpl_->doc_->encoding) throw std::bad_alloc();
}
//####################################################################
bool xml::document::get_is_standalone (void) const {
    return pimpl_->doc_->standalone == 1;
}
//####################################################################
void xml::document::set_is_standalone (bool sa) {
    pimpl_->doc_->standalone = sa ? 1 : 0;
}
//####################################################################
bool xml::document::process_xinclude (void) {
    // xmlXIncludeProcess does not return what is says it does
    return xmlXIncludeProcess(pimpl_->doc_) >= 0;
}
//####################################################################
bool xml::document::has_internal_subset (void) const {
    return pimpl_->doc_->intSubset != 0;
}
//####################################################################
bool xml::document::has_external_subset (void) const {
    return pimpl_->doc_->extSubset != 0;
}
//####################################################################
bool xml::document::validate (void) {
    dtd_impl dtd;
    return dtd.validate(pimpl_->doc_);
}
//####################################################################
bool xml::document::validate (const char *dtdname) {
    dtd_impl dtd(dtdname);

    if (!dtd.error_.empty()) return false;
    if (!dtd.validate(pimpl_->doc_)) return false;

    // remove the old DTD
    if (pimpl_->doc_->extSubset != 0) xmlFreeDtd(pimpl_->doc_->extSubset);
    pimpl_->doc_->extSubset = dtd.release();

    return true;
}
//####################################################################
xml::node::iterator xml::document::begin (void) {
    return node::iterator(pimpl_->doc_->children);
}
//####################################################################
xml::node::const_iterator xml::document::begin (void) const {
    return node::const_iterator(pimpl_->doc_->children);
}
//####################################################################
xml::node::iterator xml::document::end (void) {
    return node::iterator(0);
}
//####################################################################
xml::node::const_iterator xml::document::end (void) const {
    return node::const_iterator(0);
}
//####################################################################
void xml::document::save_to_string (std::string &s) const {
    xmlChar *xml_string;
    int xml_string_length;

#if defined(XMLWRAPP_WITH_XSLT)
    if (pimpl_->from_xslt_ != 0) {
	if (xsltSaveResultToString(&xml_string, &xml_string_length, pimpl_->doc_, 
		    static_cast<xsltStylesheetPtr>(pimpl_->from_xslt_)) >= 0)
	{
	    xmlchar_helper helper(xml_string);
	    if (xml_string_length) s.assign(helper.get(), xml_string_length);
	}

	return;
    }
#endif

    const char *enc = pimpl_->encoding_.empty() ? 0 : pimpl_->encoding_.c_str();
    xmlDocDumpFormatMemoryEnc(pimpl_->doc_, &xml_string, &xml_string_length, enc, 1);

    xmlchar_helper helper(xml_string);
    if (xml_string_length) s.assign(helper.get(), xml_string_length);
}
//####################################################################
bool xml::document::save_to_file (const char *filename) const {
#if defined(XMLWRAPP_WITH_XSLT)
    if (pimpl_->from_xslt_ != 0) {
	return xsltSaveResultToFilename(filename, pimpl_->doc_, static_cast<xsltStylesheetPtr>(pimpl_->from_xslt_), 0) >= 0;
    }
#endif

    const char *enc = pimpl_->encoding_.empty() ? 0 : pimpl_->encoding_.c_str();
    return xmlSaveFormatFileEnc(filename, pimpl_->doc_, enc, 1) > 0;
}
//####################################################################
void xml::document::set_doc_data (void *data) {
    // we own the doc now, don't free it!
    pimpl_->set_doc_data(static_cast<xmlDocPtr>(data), false);
    pimpl_->from_xslt_ = 0;
}
//####################################################################
void xml::document::set_doc_data_from_xslt (void *data, void *ss) {
    // this document came from a XSLT transformation
    pimpl_->set_doc_data(static_cast<xmlDocPtr>(data), false);
    pimpl_->from_xslt_ = ss;
}
//####################################################################
void* xml::document::get_doc_data (void) const {
    return pimpl_->doc_;
}
//####################################################################
void* xml::document::release_doc_data (void) {
    xmlDocPtr xmldoc = pimpl_->doc_;
    pimpl_->doc_ = 0;

    return xmldoc;
}
//####################################################################
std::ostream& xml::operator<< (std::ostream &stream, const xml::document &doc) {
    std::string xmldata;
    doc.save_to_string(xmldata);
    stream << xmldata;
    return stream;
}
//####################################################################
