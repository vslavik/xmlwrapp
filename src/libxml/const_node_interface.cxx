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

#include <xmlwrapp/const_node_interface.h>

#include <xmlwrapp/node.h>
#include "ait_impl.h"
#include "node_impl.h"
#include "utility.h"
#include <libxml/tree.h>
#include <stdexcept>

//####################################################################
xml::detail::const_node_interface::const_node_interface (void) {
  std::auto_ptr<node_impl> ap(pimpl_ = new node_impl);

  pimpl_->xmlnode_ = xmlNewNode(0, reinterpret_cast<const xmlChar*>("blank"));
  if (!pimpl_->xmlnode_) throw std::bad_alloc();

  ap.release();
}
//####################################################################
xml::detail::const_node_interface::const_node_interface (const char *name) {
  std::auto_ptr<node_impl> ap(pimpl_ = new node_impl);

  pimpl_->xmlnode_ = xmlNewNode(0, reinterpret_cast<const xmlChar*>(name));
  if (!pimpl_->xmlnode_) throw std::bad_alloc();

  ap.release();
}
//####################################################################
xml::detail::const_node_interface::const_node_interface (const char *name, const char *content) {
  std::auto_ptr<node_impl> ap(pimpl_ = new node_impl);

  pimpl_->xmlnode_ = xmlNewNode(0, reinterpret_cast<const xmlChar*>(name));
  if (!pimpl_->xmlnode_) throw std::bad_alloc();

  xmlNodePtr content_node = xmlNewText(reinterpret_cast<const xmlChar*>(content));
  if (!content_node) throw std::bad_alloc();

  if (!xmlAddChild(pimpl_->xmlnode_, content_node)) {
    xmlFreeNode(content_node);
    throw std::bad_alloc();
  }

  ap.release();
}
//####################################################################
xml::detail::const_node_interface::const_node_interface (const const_node_interface &other) {
  std::auto_ptr<node_impl> ap(pimpl_ = new node_impl);

  pimpl_->xmlnode_ = xmlCopyNode(other.pimpl_->xmlnode_, 1);
  if (!pimpl_->xmlnode_) throw std::bad_alloc();

  ap.release();
}
//####################################################################
xml::detail::const_node_interface &
xml::detail::const_node_interface::operator= (const const_node_interface &other) {
  const_node_interface(other).do_swap(*this);
  return *this;
}
//####################################################################
xml::detail::const_node_interface::~const_node_interface (void) {
  delete pimpl_;
}
//####################################################################
void* xml::detail::const_node_interface::get_node_data (void) const {
  return pimpl_->xmlnode_;
}
//####################################################################
void xml::detail::const_node_interface::set_node_data (void *data) {
  pimpl_->release();
  pimpl_->xmlnode_ = static_cast<xmlNodePtr>(data);
  pimpl_->owner_ = false;
}
//####################################################################
void* xml::detail::const_node_interface::release_node_data (void) {
  pimpl_->owner_ = false;
  return pimpl_->xmlnode_;
}
//####################################################################
const char* xml::detail::const_node_interface::get_name (void) const {
  return reinterpret_cast<const char*>(pimpl_->xmlnode_->name);
}
//####################################################################
const char* xml::detail::const_node_interface::get_content (void) const {
  xmlchar_helper content(xmlNodeGetContent(pimpl_->xmlnode_));
  if (!content.get()) return 0;

  pimpl_->tmp_string = content.get();
  return pimpl_->tmp_string.c_str();
}
//####################################################################
xml::detail::const_node_interface::node_type
xml::detail::const_node_interface::get_type (void) const {
  switch (pimpl_->xmlnode_->type) {
  case XML_ELEMENT_NODE:		    return type_element;
  case XML_TEXT_NODE:		    return type_text;
  case XML_CDATA_SECTION_NODE:	    return type_cdata;
  case XML_ENTITY_REF_NODE:	    return type_entity_ref;
  case XML_ENTITY_NODE:		    return type_entity;
  case XML_PI_NODE:		    return type_pi;
  case XML_COMMENT_NODE:		    return type_comment;
  case XML_DOCUMENT_NODE:		    return type_document;
  case XML_DOCUMENT_TYPE_NODE:	    return type_document_type;
  case XML_DOCUMENT_FRAG_NODE:	    return type_document_frag;
  case XML_NOTATION_NODE:		    return type_notation;
  case XML_DTD_NODE:		    return type_dtd;
  case XML_ELEMENT_DECL:		    return type_dtd_element;
  case XML_ATTRIBUTE_DECL:	    return type_dtd_attribute;
  case XML_ENTITY_DECL:		    return type_dtd_entity;
  case XML_NAMESPACE_DECL:	    return type_dtd_namespace;
  case XML_XINCLUDE_START:	    return type_xinclude;
  case XML_XINCLUDE_END:		    return type_xinclude;
  default:			    return type_element;
  }
}
//####################################################################
const xml::attributes& xml::detail::const_node_interface::get_attributes (void) const {
  if (pimpl_->xmlnode_->type != XML_ELEMENT_NODE) {
    throw std::runtime_error("get_attributes called on non-element node");
  }

  pimpl_->attrs_.set_data(pimpl_->xmlnode_);
  return pimpl_->attrs_;
}
//####################################################################
bool xml::detail::const_node_interface::is_text (void) const {
  return xmlNodeIsText(pimpl_->xmlnode_) != 0;
}
//####################################################################
xml::detail::const_node_interface::const_iterator
xml::detail::const_node_interface::begin (void) const {
  return const_iterator(pimpl_->xmlnode_->children);
}
//####################################################################
xml::detail::const_node_interface::const_iterator
xml::detail::const_node_interface::end (void) const {
  return const_iterator();
}
//####################################################################
xml::detail::const_node_interface::const_iterator
xml::detail::const_node_interface::self (void) const {
  return const_iterator(pimpl_->xmlnode_);
}
//####################################################################
xml::detail::const_node_interface::const_iterator
xml::detail::const_node_interface::parent (void) const {
  if (pimpl_->xmlnode_->parent) return const_iterator(pimpl_->xmlnode_->parent);
  return const_iterator();
}
//####################################################################
xml::detail::const_node_interface::const_iterator
xml::detail::const_node_interface::find (const char *name) const {
  xmlNodePtr found = find_node(name, pimpl_->xmlnode_->children);
  if (found) return const_iterator(found);
  return end();
}
//####################################################################
xml::detail::const_node_interface::const_iterator
xml::detail::const_node_interface::find (const char *name, const_iterator start) const {
  xmlNodePtr n = static_cast<xmlNodePtr>(start.get_raw_node());
  if ( (n = find_node(name, n))) return const_iterator(n);
  return end();
}
//####################################################################
void xml::detail::const_node_interface::node_to_string (std::string &xml) const {
  node2doc n2d(pimpl_->xmlnode_);
  xmlDocPtr doc = n2d.get_doc();

  xmlChar *xml_string;
  int xml_string_length;

  xmlDocDumpFormatMemory(doc, &xml_string, &xml_string_length, 1);

  xmlchar_helper helper(xml_string);
  if (xml_string_length) xml.assign(helper.get(), xml_string_length);
}
//####################################################################
void xml::detail::const_node_interface::do_swap (const_node_interface &rhs) {
  std::swap(pimpl_, rhs.pimpl_);
}
//####################################################################
xml::detail::const_node_interface::const_node_interface (node_impl *impl)
  : pimpl_(new node_impl(impl->xmlnode_)) {
}
//####################################################################
std::ostream& xml::operator<< (std::ostream &stream,
			       const xml::detail::const_node_interface &n) {
  std::string xmldata;
  n.node_to_string(xmldata);
  stream << xmldata;
  return stream;
}
