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
 * This file contains the implementation of the xml::node class.
**/

// xmlwrapp includes
#include "xmlwrapp/node.h"
#include "xmlwrapp/attributes.h"
#include "utility.h"
#include "ait_impl.h"
#include "node_manip.h"

// standard includes
#include <new>
#include <memory>
#include <string>
#include <vector>
#include <iostream>
#include <iterator>
#include <algorithm>
#include <stdexcept>
#include <functional>

// libxml includes
#include <libxml/tree.h>
#include <libxml/parser.h>

//####################################################################
struct xml::node_impl {
    //####################################################################
    node_impl (void) : xmlnode_(0), owner_(true), attrs_(0)
    { }
    //####################################################################
    ~node_impl (void)
    { release(); }
    //####################################################################
    void release (void) {
	if (xmlnode_ && owner_) {
	    xmlFreeNode(xmlnode_);
	}
    }
    //####################################################################

    xmlNodePtr xmlnode_;
    bool owner_;
    attributes attrs_;
    std::string tmp_string;
};
//####################################################################
struct xml::node_cmp : public std::binary_function<xmlNodePtr, xmlNodePtr, bool> {
    //####################################################################
    node_cmp (cbfo_node_compare &cb) : cb_(cb) { }
    //####################################################################
    bool operator() (xmlNodePtr lhs, xmlNodePtr rhs) {
	xml::node l_node, r_node;
	l_node.set_node_data(lhs);
	r_node.set_node_data(rhs);

	return cb_(l_node, r_node);
    }
    //####################################################################

    cbfo_node_compare &cb_;
};
//####################################################################
namespace {
    // help turn a node into a document
    class node2doc {
    public:
	node2doc (xmlNodePtr xmlnode_) : xmlnode__(xmlnode_), prev_(0), next_(0) {
	    xmldoc_ = xmlNewDoc(0);
	    if (!xmldoc_) throw std::bad_alloc();

	    xmldoc_->children	= xmlnode__;
	    xmldoc_->last	= xmlnode__;

	    std::swap(prev_, xmlnode__->prev);
	    std::swap(next_, xmlnode__->next);
	}

	~node2doc (void) {
	    xmldoc_->children	= 0;
	    xmldoc_->last	= 0;

	    xmlFreeDoc(xmldoc_);

	    std::swap(prev_, xmlnode__->prev);
	    std::swap(next_, xmlnode__->next);
	}

	xmlDocPtr get_doc (void)
	{ return xmldoc_; }
    private:
	xmlDocPtr xmldoc_;
	xmlNodePtr xmlnode__;
	xmlNodePtr prev_;
	xmlNodePtr next_;
    };

    // sort compare function to sort based on attribute
    struct compare_attr : public std::binary_function<xmlNodePtr, xmlNodePtr, bool> {
	compare_attr (const char *attr_name) : name_(attr_name) { }

	bool operator() (xmlNodePtr lhs, xmlNodePtr rhs) {
	    xmlAttrPtr attr_l, attr_r;
	    xmlAttributePtr dtd_l(0), dtd_r(0);

	    attr_l = xml::find_prop(lhs, name_);
	    if (attr_l == 0 && (dtd_l = xml::find_default_prop(lhs, name_)) == 0) return true;

	    attr_r = xml::find_prop(rhs, name_);
	    if (attr_r == 0 && (dtd_r = xml::find_default_prop(rhs, name_)) == 0) return false;

	    xmlChar *value_l, *value_r;
	    
	    if (dtd_l) value_l = const_cast<xmlChar*>(dtd_l->defaultValue);
	    else value_l = xmlNodeListGetString(lhs->doc, attr_l->children, 1);

	    if (dtd_r) value_r = const_cast<xmlChar*>(dtd_r->defaultValue);
	    else value_r = xmlNodeListGetString(rhs->doc, attr_r->children, 1);

	    int rc = xmlStrcmp(value_l, value_r);

	    if (!dtd_l) xmlFree(value_l);
	    if (!dtd_r) xmlFree(value_r);

	    return rc < 0;
	}

	const char *name_;
    };


    // add a node as a child
    struct insert_node : public std::unary_function<xmlNodePtr, void> {
	insert_node (xmlNodePtr parent) : parent_(parent) { }
	void operator() (xmlNodePtr child) { xmlAddChild(parent_, child); }

	xmlNodePtr parent_;
    };

    // a node finder
    xmlNodePtr find_node(const char *name, xmlNodePtr first);
}
//####################################################################
xml::node::node (void) {
    std::auto_ptr<node_impl> ap(pimpl_ = new node_impl);

    pimpl_->xmlnode_ = xmlNewNode(0, reinterpret_cast<const xmlChar*>("blank"));
    if (!pimpl_->xmlnode_) throw std::bad_alloc();

    ap.release();
}
//####################################################################
xml::node::node (const char *name) {
    std::auto_ptr<node_impl> ap(pimpl_ = new node_impl);

    pimpl_->xmlnode_ = xmlNewNode(0, reinterpret_cast<const xmlChar*>(name));
    if (!pimpl_->xmlnode_) throw std::bad_alloc();

    ap.release();
}
//####################################################################
xml::node::node (const char *name, const char *content) {
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
xml::node::node (cdata cdata_info) {
    std::auto_ptr<node_impl> ap(pimpl_ = new node_impl);

    if ( (pimpl_->xmlnode_ = xmlNewCDataBlock(0, reinterpret_cast<const xmlChar*>(cdata_info.t), std::strlen(cdata_info.t))) == 0) {
	throw std::bad_alloc();
    }

    ap.release();
}
//####################################################################
xml::node::node (comment comment_info) {
    std::auto_ptr<node_impl> ap(pimpl_ = new node_impl);

    if ( (pimpl_->xmlnode_ =  xmlNewComment(reinterpret_cast<const xmlChar*>(comment_info.t))) == 0) {
	throw std::bad_alloc();
    }

    ap.release();
}
//####################################################################
xml::node::node (pi pi_info) {
    std::auto_ptr<node_impl> ap(pimpl_ = new node_impl);

    if ( (pimpl_->xmlnode_ = xmlNewPI(reinterpret_cast<const xmlChar*>(pi_info.n), reinterpret_cast<const xmlChar*>(pi_info.c))) == 0) {
	throw std::bad_alloc();
    }

    ap.release();
}
//####################################################################
xml::node::node (const node &other) {
    std::auto_ptr<node_impl> ap(pimpl_ = new node_impl);

    pimpl_->xmlnode_ = xmlCopyNode(other.pimpl_->xmlnode_, 1);
    if (!pimpl_->xmlnode_) throw std::bad_alloc();

    ap.release();
}
//####################################################################
xml::node& xml::node::operator= (const node &other) {
    node tmp_node(other);
    swap(tmp_node);
    return *this;
}
//####################################################################
void xml::node::swap (node &other) {
    std::swap(pimpl_, other.pimpl_);
}
//####################################################################
xml::node::~node (void) {
    delete pimpl_;
}
//####################################################################
void xml::node::set_node_data (void *data) {
    pimpl_->release();
    pimpl_->xmlnode_ = static_cast<xmlNodePtr>(data);
    pimpl_->owner_ = false;
}
//####################################################################
void* xml::node::get_node_data (void) {
    return pimpl_->xmlnode_;
}
//####################################################################
void* xml::node::release_node_data (void) {
    pimpl_->owner_ = false;
    return pimpl_->xmlnode_;
}
//####################################################################
void xml::node::set_name (const char *name) {
    xmlNodeSetName(pimpl_->xmlnode_, reinterpret_cast<const xmlChar*>(name));
}
//####################################################################
const char* xml::node::get_name (void) const {
    return reinterpret_cast<const char*>(pimpl_->xmlnode_->name);
}
//####################################################################
void xml::node::set_content (const char *content) {
    xmlNodeSetContent(pimpl_->xmlnode_, reinterpret_cast<const xmlChar*>(content));
}
//####################################################################
const char* xml::node::get_content (void) const {
    xmlchar_helper content(xmlNodeGetContent(pimpl_->xmlnode_));
    if (!content.get()) return 0;

    pimpl_->tmp_string = content.get();
    return pimpl_->tmp_string.c_str();
}
//####################################################################
xml::node::node_type xml::node::get_type (void) const {
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
xml::attributes& xml::node::get_attributes (void) {
    if (pimpl_->xmlnode_->type != XML_ELEMENT_NODE) {
	throw std::runtime_error("get_attributes called on non-element node");
    }

    pimpl_->attrs_.set_data(pimpl_->xmlnode_);
    return pimpl_->attrs_;
}
//####################################################################
const xml::attributes& xml::node::get_attributes (void) const {
    if (pimpl_->xmlnode_->type != XML_ELEMENT_NODE) {
	throw std::runtime_error("get_attributes called on non-element node");
    }

    pimpl_->attrs_.set_data(pimpl_->xmlnode_);
    return pimpl_->attrs_;
}
//####################################################################
bool xml::node::is_text (void) const {
    return xmlNodeIsText(pimpl_->xmlnode_) != 0;
}
//####################################################################
void xml::node::push_back (const node &child) {
    xmlwrapp::node_insert(pimpl_->xmlnode_, 0, child.pimpl_->xmlnode_);
}
//####################################################################
xml::node::size_type xml::node::size (void) const {
    return std::distance(begin(), end());
}
//####################################################################
bool xml::node::empty (void) const {
    return pimpl_->xmlnode_->children == 0;
}
//####################################################################
xml::node::iterator xml::node::begin (void) {
    return iterator(pimpl_->xmlnode_->children);
}
//####################################################################
xml::node::const_iterator xml::node::begin (void) const {
    return const_iterator(pimpl_->xmlnode_->children);
}
//####################################################################
xml::node::iterator xml::node::end (void) {
    return iterator();
}
//####################################################################
xml::node::const_iterator xml::node::end (void) const {
    return const_iterator();
}
//####################################################################
xml::node::iterator xml::node::self (void) {
    return iterator(pimpl_->xmlnode_);
}
//####################################################################
xml::node::const_iterator xml::node::self (void) const {
    return const_iterator(pimpl_->xmlnode_);
}
//####################################################################
xml::node::iterator xml::node::parent (void) {
    if (pimpl_->xmlnode_->parent) return iterator(pimpl_->xmlnode_->parent);
    return iterator();
}
//####################################################################
xml::node::const_iterator xml::node::parent (void) const {
    if (pimpl_->xmlnode_->parent) return const_iterator(pimpl_->xmlnode_->parent);
    return const_iterator();
}
//####################################################################
xml::node::iterator xml::node::find (const char *name) {
    xmlNodePtr found = find_node(name, pimpl_->xmlnode_->children);
    if (found) return iterator(found);
    return end();
}
//####################################################################
xml::node::const_iterator xml::node::find (const char *name) const {
    xmlNodePtr found = find_node(name, pimpl_->xmlnode_->children);
    if (found) return const_iterator(found);
    return end();
}
//####################################################################
xml::node::iterator xml::node::find (const char *name, iterator start) {
    xmlNodePtr n = static_cast<xmlNodePtr>(start.get_raw_node());
    if ( (n = find_node(name, n))) return iterator(n);
    return end();
}
//####################################################################
xml::node::const_iterator xml::node::find (const char *name, const_iterator start) const {
    xmlNodePtr n = static_cast<xmlNodePtr>(start.get_raw_node());
    if ( (n = find_node(name, n))) return const_iterator(n);
    return end();
}
//####################################################################
xml::node::iterator xml::node::insert (const node &n) {
    return iterator(xmlwrapp::node_insert(pimpl_->xmlnode_, 0, n.pimpl_->xmlnode_));
}
//####################################################################
xml::node::iterator xml::node::insert (iterator position, const node &n) {
    return iterator(xmlwrapp::node_insert(pimpl_->xmlnode_, static_cast<xmlNodePtr>(position.get_raw_node()), n.pimpl_->xmlnode_));
}
//####################################################################
xml::node::iterator xml::node::replace (iterator old_node, const node &new_node) {
    return iterator(xmlwrapp::node_replace(static_cast<xmlNodePtr>(old_node.get_raw_node()), new_node.pimpl_->xmlnode_));
}
//####################################################################
xml::node::iterator xml::node::erase (iterator to_erase) {
    return iterator(xmlwrapp::node_erase(static_cast<xmlNodePtr>(to_erase.get_raw_node())));
}
//####################################################################
xml::node::iterator xml::node::erase (iterator first, iterator last) {
    while (first != last) first = erase(first);
    return first;
}
//####################################################################
xml::node::size_type xml::node::erase (const char *name) {
    size_type removed_count(0);
    iterator to_remove(begin()), the_end(end());

    while ( (to_remove = find(name, to_remove)) != the_end) {
	++removed_count;
	to_remove = erase(to_remove);
    }

    return removed_count;
}
//####################################################################
void xml::node::sort (const char *node_name, const char *attr_name) {
    xmlNodePtr i(pimpl_->xmlnode_->children), next(0);
    std::vector<xmlNodePtr> node_list;

    while (i!=0) {
	next = i->next;

	if (i->type == XML_ELEMENT_NODE && xmlStrcmp(i->name, reinterpret_cast<const xmlChar*>(node_name)) == 0) {
	    xmlUnlinkNode(i);
	    node_list.push_back(i);
	}

	i = next;
    }

    if (node_list.empty()) return;
    
    std::sort(node_list.begin(), node_list.end(), compare_attr(attr_name));
    std::for_each(node_list.begin(), node_list.end(), insert_node(pimpl_->xmlnode_));
}
//####################################################################
void xml::node::sort_fo (cbfo_node_compare &cb) {
    xmlNodePtr i(pimpl_->xmlnode_->children), next(0);
    std::vector<xmlNodePtr> node_list;

    while (i!=0) {
	next = i->next;

	if (i->type == XML_ELEMENT_NODE) {
	    xmlUnlinkNode(i);
	    node_list.push_back(i);
	}

	i = next;
    }

    if (node_list.empty()) return;
    
    std::sort(node_list.begin(), node_list.end(), node_cmp(cb));
    std::for_each(node_list.begin(), node_list.end(), insert_node(pimpl_->xmlnode_));
}
//####################################################################
void xml::node::node_to_string (std::string &xml) const {
    node2doc n2d(pimpl_->xmlnode_);
    xmlDocPtr doc = n2d.get_doc();

    xmlChar *xml_string;
    int xml_string_length;

    xmlDocDumpFormatMemory(doc, &xml_string, &xml_string_length, 1);

    xmlchar_helper helper(xml_string);
    if (xml_string_length) xml.assign(helper.get(), xml_string_length);
}
//####################################################################
namespace {
    xmlNodePtr find_node(const char *name, xmlNodePtr first) {
	while (first != 0) {
	    if (first->type == XML_ELEMENT_NODE && xmlStrcmp(first->name, reinterpret_cast<const xmlChar*>(name)) == 0) return first;
	    first = first->next;
	}

	return 0;
    }
}
//####################################################################
std::ostream& xml::operator<< (std::ostream &stream, const xml::node &n) {
    std::string xmldata;
    n.node_to_string(xmldata);
    stream << xmldata;
    return stream;
}
//####################################################################
