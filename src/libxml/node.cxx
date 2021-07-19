/*
 * Copyright (C) 2001-2003 Peter J Jones (pjones@pmade.org)
 *               2009      Vaclav Slavik <vslavik@gmail.com>
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
#include "xmlwrapp/node.h"
#include "xmlwrapp/nodes_view.h"
#include "xmlwrapp/attributes.h"
#include "xmlwrapp/errors.h"
#include "cpp11.h"
#include "utility.h"
#include "ait_impl.h"
#include "node_manip.h"
#include "node_iterator.h"

// standard includes
#include <cstring>
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

namespace xml
{

using namespace xml::impl;

// ------------------------------------------------------------------------
// xml::impl::node_impl
// ------------------------------------------------------------------------

namespace impl
{

struct node_impl
{
    node_impl() : xmlnode_(0), owner_(true), attrs_(0) {}
    ~node_impl() { release(); }

    void release()
    {
        if (xmlnode_ && owner_)
            xmlFreeNode(xmlnode_);
    }

    xmlNodePtr xmlnode_;
    bool owner_;
    attributes attrs_;
    std::string tmp_string;
};


struct node_cmp : public std::binary_function<xmlNodePtr, xmlNodePtr, bool>
{
    node_cmp (cbfo_node_compare &cb) : cb_(cb) {}

    bool operator()(xmlNodePtr lhs, xmlNodePtr rhs)
    {
        xml::node l_node, r_node;
        l_node.set_node_data(lhs);
        r_node.set_node_data(rhs);

        return cb_(l_node, r_node);
    }

    cbfo_node_compare &cb_;
};

} // namespace impl


// ------------------------------------------------------------------------
// misc helpers
// ------------------------------------------------------------------------

namespace
{

// help turn a node into a document
class node2doc
{
public:
    node2doc(xmlNodePtr xmlnode_)
        : xmlnode__(xmlnode_), prev_(0), next_(0)
    {
        xmldoc_ = xmlNewDoc(0);
        if (!xmldoc_)
            throw std::bad_alloc();

        xmldoc_->children   = xmlnode__;
        xmldoc_->last   = xmlnode__;

        std::swap(prev_, xmlnode__->prev);
        std::swap(next_, xmlnode__->next);
    }

    ~node2doc()
    {
        xmldoc_->children   = 0;
        xmldoc_->last   = 0;

        xmlFreeDoc(xmldoc_);

        std::swap(prev_, xmlnode__->prev);
        std::swap(next_, xmlnode__->next);
    }

    xmlDocPtr get_doc()  { return xmldoc_; }

private:
    xmlDocPtr xmldoc_;
    xmlNodePtr xmlnode__;
    xmlNodePtr prev_;
    xmlNodePtr next_;
};


// sort compare function to sort based on attribute
struct compare_attr : public std::binary_function<xmlNodePtr, xmlNodePtr, bool>
{
    compare_attr(const char *attr_name) : name_(attr_name) {}

    bool operator()(xmlNodePtr lhs, xmlNodePtr rhs)
    {
        xmlAttrPtr attr_l, attr_r;
        xmlAttributePtr dtd_l(0), dtd_r(0);

        attr_l = find_prop(lhs, name_);
        if (attr_l == 0 && (dtd_l = find_default_prop(lhs, name_)) == 0)
            return true;

        attr_r = find_prop(rhs, name_);
        if (attr_r == 0 && (dtd_r = find_default_prop(rhs, name_)) == 0)
            return false;

        xmlChar *value_l, *value_r;

        if (dtd_l)
            value_l = const_cast<xmlChar*>(dtd_l->defaultValue);
        else
            value_l = xmlNodeListGetString(lhs->doc, attr_l->children, 1);

        if (dtd_r)
            value_r = const_cast<xmlChar*>(dtd_r->defaultValue);
        else
            value_r = xmlNodeListGetString(rhs->doc, attr_r->children, 1);

        int rc = xmlStrcmp(value_l, value_r);

        if (!dtd_l)
            xmlFree(value_l);
        if (!dtd_r)
            xmlFree(value_r);

        return rc < 0;
    }

    const char *name_;
};


// add a node as a child
struct insert_node : public std::unary_function<xmlNodePtr, void>
{
    insert_node(xmlNodePtr parent) : parent_(parent) {}
    void operator()(xmlNodePtr child) { xmlAddChild(parent_, child); }

    xmlNodePtr parent_;
};


// an element node finder
xmlNodePtr find_element(const char *name, xmlNodePtr first)
{
    while (first != 0)
    {
        if (first->type == XML_ELEMENT_NODE && xmlStrcmp(first->name, reinterpret_cast<const xmlChar*>(name)) == 0)
        {
            return first;
        }
        first = first->next;
    }

    return 0;
}


xmlNodePtr find_element(xmlNodePtr first)
{
    while (first != 0)
    {
        if (first->type == XML_ELEMENT_NODE)
            return first;
        first = first->next;
    }

    return 0;
}


class next_element_functor : public iter_advance_functor
{
public:
    virtual xmlNodePtr operator()(xmlNodePtr node) const
        { return find_element(node->next); }
};


class next_named_element_functor : public iter_advance_functor
{
public:
    next_named_element_functor(const char *name) : name_(name) {}
    virtual xmlNodePtr operator()(xmlNodePtr node) const
        { return find_element(name_.c_str(), node->next); }
private:
    std::string name_;
};

} // anonymous namespace


// ------------------------------------------------------------------------
// xml::node
// ------------------------------------------------------------------------

node::node(int)
{
    pimpl_ = new node_impl;
}


node::node()
{
    xml::impl::auto_ptr<node_impl> ap(pimpl_ = new node_impl);

    pimpl_->xmlnode_ = xmlNewNode(0, reinterpret_cast<const xmlChar*>("blank"));
    if (!pimpl_->xmlnode_)
        throw std::bad_alloc();

    ap.release();
}


node::node (const char *name)
{
    xml::impl::auto_ptr<node_impl> ap(pimpl_ = new node_impl);

    pimpl_->xmlnode_ = xmlNewNode(0, reinterpret_cast<const xmlChar*>(name));
    if (!pimpl_->xmlnode_)
        throw std::bad_alloc();

    ap.release();
}


node::node (const char *name, const char *content)
{
    xml::impl::auto_ptr<node_impl> ap(pimpl_ = new node_impl);

    pimpl_->xmlnode_ = xmlNewNode(0, reinterpret_cast<const xmlChar*>(name));
    if (!pimpl_->xmlnode_)
        throw std::bad_alloc();

    if (std::strlen(content))
    {
        xmlNodePtr content_node = xmlNewText(reinterpret_cast<const xmlChar*>(content));
        if (!content_node)
            throw std::bad_alloc();

        if (!xmlAddChild(pimpl_->xmlnode_, content_node))
        {
            xmlFreeNode(content_node);
            throw std::bad_alloc();
        }
    }

    ap.release();
}


node::node(cdata cdata_info)
{
    const int len = xml::impl::checked_int_cast(std::strlen(cdata_info.t));

    xml::impl::auto_ptr<node_impl> ap(pimpl_ = new node_impl);

    if ( (pimpl_->xmlnode_ = xmlNewCDataBlock(0, reinterpret_cast<const xmlChar*>(cdata_info.t), len)) == 0)
    {
        throw std::bad_alloc();
    }

    ap.release();
}


node::node(comment comment_info)
{
    xml::impl::auto_ptr<node_impl> ap(pimpl_ = new node_impl);

    if ( (pimpl_->xmlnode_ =  xmlNewComment(reinterpret_cast<const xmlChar*>(comment_info.t))) == 0)
    {
        throw std::bad_alloc();
    }

    ap.release();
}


node::node(pi pi_info)
{
    xml::impl::auto_ptr<node_impl> ap(pimpl_ = new node_impl);

    if ( (pimpl_->xmlnode_ = xmlNewPI(reinterpret_cast<const xmlChar*>(pi_info.n), reinterpret_cast<const xmlChar*>(pi_info.c))) == 0)
    {
        throw std::bad_alloc();
    }

    ap.release();
}


node::node(text text_info)
{
    xml::impl::auto_ptr<node_impl> ap(pimpl_ = new node_impl);

    if ( (pimpl_->xmlnode_ =  xmlNewText(reinterpret_cast<const xmlChar*>(text_info.t))) == 0)
    {
        throw std::bad_alloc();
    }

    ap.release();
}


node::node(const node& other)
{
    xml::impl::auto_ptr<node_impl> ap(pimpl_ = new node_impl);

    pimpl_->xmlnode_ = xmlCopyNode(other.pimpl_->xmlnode_, 1);
    if (!pimpl_->xmlnode_)
        throw std::bad_alloc();

    ap.release();
}


node& node::operator=(const node& other)
{
    node tmp_node(other);
    swap(tmp_node);
    return *this;
}


void node::swap(node& other)
{
    std::swap(pimpl_, other.pimpl_);
}


void node::move_under(node& new_parent)
{
    xmlNodePtr& this_node = pimpl_->xmlnode_;
    xmlNodePtr& new_parent_node = new_parent.pimpl_->xmlnode_;

    if (this_node->parent == new_parent_node)
        return;

    // If we wanted to support inter document moves, we would need to deal with
    // the namespaces, e.g. we could need to define the namespace of this node
    // in the new document. While this should be possible, currently this is
    // not implemented.
    if (this_node->doc != new_parent_node->doc)
        throw xml::exception("moving nodes between documents not supported");

    // Check that we're not moving the node under itself, loops are not allowed
    // in XML trees.
    for (xmlNodePtr p = new_parent_node; p; p = p->parent)
    {
        if (p == this_node)
            throw xml::exception("can't move a node under itself");
    }

    // Note that this must be non-NULL as we're not the tree root, otherwise
    // the new parent would have been our child and we would have thrown above.
    xmlNodePtr& old_parent_node = this_node->parent;

    // Remove the node from the old parent children list.
    if (this_node->prev)
        this_node->prev->next = this_node->next;
    else
        old_parent_node->children = this_node->next;

    if (this_node->next)
        this_node->next->prev = this_node->prev;
    else
        old_parent_node->last = this_node->prev;

    // Update the pointers inside this node itself.
    this_node->parent = new_parent_node;
    this_node->prev = new_parent_node->last;
    this_node->next = NULL;

    // And update the new parent too.
    if (this_node->prev)
        this_node->prev->next = this_node;
    else
        new_parent_node->children = this_node;
    new_parent_node->last = this_node;
}


node::~node()
{
    delete pimpl_;
}


void node::set_node_data(void *data)
{
    pimpl_->release();
    pimpl_->xmlnode_ = static_cast<xmlNodePtr>(data);
    pimpl_->owner_ = false;
}


void* node::get_node_data()
{
    return pimpl_->xmlnode_;
}


void* node::release_node_data()
{
    pimpl_->owner_ = false;
    return pimpl_->xmlnode_;
}


void node::set_name(const char *name)
{
    xmlNodeSetName(pimpl_->xmlnode_, reinterpret_cast<const xmlChar*>(name));
}


const char* node::get_name() const
{
    return reinterpret_cast<const char*>(pimpl_->xmlnode_->name);
}


void node::set_content(const char *content)
{
    xmlNodeSetContent(pimpl_->xmlnode_, reinterpret_cast<const xmlChar*>(content));
}


void node::set_text_content(const char *content)
{
    xmlChar *escaped = xmlEncodeSpecialChars(pimpl_->xmlnode_->doc,
                                             reinterpret_cast<const xmlChar*>(content));
    xmlNodeSetContent(pimpl_->xmlnode_, escaped);
    if ( escaped )
        xmlFree(escaped);
}


const char* node::get_content() const
{
    xmlchar_helper content(xmlNodeGetContent(pimpl_->xmlnode_));
    if (!content.get())
        return NULL;

    pimpl_->tmp_string = content.get();
    return pimpl_->tmp_string.c_str();
}


node::node_type node::get_type() const
{
    // Note that XML_xxx values are listed here in order of their declaration
    // in xmlElementType, for ease of comparison.
    switch (pimpl_->xmlnode_->type)
    {
        case XML_ELEMENT_NODE:          return type_element;
        // List for completeness, but this is impossible here as attrubutes are
        // represented by xml::attribute objects, not xml::node ones.
        case XML_ATTRIBUTE_NODE:        break;
        case XML_TEXT_NODE:             return type_text;
        case XML_CDATA_SECTION_NODE:    return type_cdata;
        case XML_ENTITY_REF_NODE:       return type_entity_ref;
        case XML_ENTITY_NODE:           return type_entity;
        case XML_PI_NODE:               return type_pi;
        case XML_COMMENT_NODE:          return type_comment;
        case XML_DOCUMENT_NODE:         return type_document;
        case XML_DOCUMENT_TYPE_NODE:    return type_document_type;
        case XML_DOCUMENT_FRAG_NODE:    return type_document_frag;
        case XML_NOTATION_NODE:         return type_notation;
        // Another impossible case: we never call htmlNewDoc(), so we are never
        // going to have objects of this type.
        case XML_HTML_DOCUMENT_NODE:    break;
        case XML_DTD_NODE:              return type_dtd;
        case XML_ELEMENT_DECL:          return type_dtd_element;
        case XML_ATTRIBUTE_DECL:        return type_dtd_attribute;
        case XML_ENTITY_DECL:           return type_dtd_entity;
        case XML_NAMESPACE_DECL:        return type_dtd_namespace;
        case XML_XINCLUDE_START:        return type_xinclude;
        case XML_XINCLUDE_END:          return type_xinclude;
        // This is also impossible as we don't use DocBook and, in addition,
        // needs to be made conditional as this symbols is not always defined.
#ifdef LIBXML_DOCB_ENABLED
        case XML_DOCB_DOCUMENT_NODE:    break;
#endif
    }

    throw std::logic_error("unknown or unexpected node type");
}


xml::attributes& node::get_attributes()
{
    if (pimpl_->xmlnode_->type != XML_ELEMENT_NODE)
    {
        throw xml::exception("get_attributes called on non-element node");
    }

    pimpl_->attrs_.set_data(pimpl_->xmlnode_);
    return pimpl_->attrs_;
}


const xml::attributes& node::get_attributes() const
{
    if (pimpl_->xmlnode_->type != XML_ELEMENT_NODE)
    {
        throw xml::exception("get_attributes called on non-element node");
    }

    pimpl_->attrs_.set_data(pimpl_->xmlnode_);
    return pimpl_->attrs_;
}


const char *node::get_namespace() const
{
    return pimpl_->xmlnode_->ns
        ? reinterpret_cast<const char*>(pimpl_->xmlnode_->ns->href)
        : NULL;
}


void node::set_namespace(const std::string& href)
{
    const xmlChar *xmlHref = reinterpret_cast<const xmlChar*>(href.c_str());

    if (pimpl_->xmlnode_->type != XML_ELEMENT_NODE)
        throw xml::exception("set_namespace called on non-element node");

    xmlNsPtr ns = xmlNewNs(pimpl_->xmlnode_, xmlHref, NULL);

    if ( !ns )
    {
        // Looks like the default namespace already exists on this node,
        // we must change its URI.
        for ( ns = pimpl_->xmlnode_->nsDef; ns; ns = ns->next )
        {
            if ( ns->prefix == NULL )
            {
                xmlFree(const_cast<xmlChar*>(ns->href));
                ns->href = xmlStrdup(xmlHref);
                break;
            }
        }
        if ( !ns )
            throw xml::exception("set_namespace failed to create namespace object");
    }

    // If we already have a namespace, all our children without their own
    // namespaces should already use the same pointer, so their namespaces will
    // be updated when our namespace is changed. However if we don't have any
    // namespace yet, children namespaces will remain unset, which would break
    // the expected namespace inheritance and so we need to set them explicitly
    // to avoid this.
    if ( pimpl_->xmlnode_->ns )
        xmlSetNs(pimpl_->xmlnode_, ns);
    else
        node_set_ns_recursively(pimpl_->xmlnode_, ns);
}


bool node::is_text() const
{
    return xmlNodeIsText(pimpl_->xmlnode_) != 0;
}


void node::push_back (const node &child)
{
    xml::impl::node_insert(pimpl_->xmlnode_, 0, child.pimpl_->xmlnode_);
}


node::size_type node::size() const
{
    using namespace std;
    return checked_size_t_cast(distance(begin(), end()));
}


bool node::empty() const
{
    return pimpl_->xmlnode_->children == 0;
}


node::iterator node::begin()
{
    return iterator(pimpl_->xmlnode_->children);
}


node::const_iterator node::begin() const
{
    return const_iterator(pimpl_->xmlnode_->children);
}


node::iterator node::self()
{
    return iterator(pimpl_->xmlnode_);
}


node::const_iterator node::self() const
{
    return const_iterator(pimpl_->xmlnode_);
}


node::iterator node::parent()
{
    if (pimpl_->xmlnode_->parent)
        return iterator(pimpl_->xmlnode_->parent);
    return iterator();
}


node::const_iterator node::parent() const
{
    if (pimpl_->xmlnode_->parent)
        return const_iterator(pimpl_->xmlnode_->parent);
    return const_iterator();
}


node::iterator node::find(const char *name)
{
    xmlNodePtr found = find_element(name, pimpl_->xmlnode_->children);
    if (found)
        return iterator(found);
    return end();
}


node::const_iterator node::find(const char *name) const
{
    xmlNodePtr found = find_element(name, pimpl_->xmlnode_->children);
    if (found)
        return const_iterator(found);
    return end();
}


node::iterator node::find(const char *name, const iterator& start)
{
    xmlNodePtr n = static_cast<xmlNodePtr>(start.get_raw_node());
    if ((n = find_element(name, n)) != NULL)
        return iterator(n);
    return end();
}


node::const_iterator node::find(const char *name, const const_iterator& start) const
{
    xmlNodePtr n = static_cast<xmlNodePtr>(start.get_raw_node());
    if ((n = find_element(name, n)) != NULL)
        return const_iterator(n);
    return end();
}


nodes_view node::elements()
{
    return nodes_view
           (
               find_element(pimpl_->xmlnode_->children),
               new next_element_functor
           );
}

xml::const_nodes_view node::elements() const
{
    return const_nodes_view
           (
               find_element(pimpl_->xmlnode_->children),
               new next_element_functor
           );
}

nodes_view node::elements(const char *name)
{
    return nodes_view
           (
               find_element(name, pimpl_->xmlnode_->children),
               new next_named_element_functor(name)
           );
}

xml::const_nodes_view node::elements(const char *name) const
{
    return const_nodes_view
           (
               find_element(name, pimpl_->xmlnode_->children),
               new next_named_element_functor(name)
           );
}

node::iterator node::insert(const node &n)
{
    return iterator(xml::impl::node_insert(pimpl_->xmlnode_, 0, n.pimpl_->xmlnode_));
}


node::iterator node::insert(const iterator& position, const node &n)
{
    return iterator(xml::impl::node_insert(pimpl_->xmlnode_, static_cast<xmlNodePtr>(position.get_raw_node()), n.pimpl_->xmlnode_));
}


node::iterator node::replace(const iterator& old_node, const node &new_node)
{
    return iterator(xml::impl::node_replace(static_cast<xmlNodePtr>(old_node.get_raw_node()), new_node.pimpl_->xmlnode_));
}


node::iterator node::erase(const iterator& to_erase)
{
    return iterator(xml::impl::node_erase(static_cast<xmlNodePtr>(to_erase.get_raw_node())));
}


node::iterator node::erase(iterator first, const iterator& last)
{
    while (first != last)
        first = erase(first);
    return first;
}


node::size_type node::erase(const char *name)
{
    size_type removed_count(0);
    iterator to_remove(begin()), the_end(end());

    while ( (to_remove = find(name, to_remove)) != the_end)
    {
        ++removed_count;
        to_remove = erase(to_remove);
    }

    return removed_count;
}


void node::clear()
{
    xmlNodePtr n = pimpl_->xmlnode_;

    if ( !n->children )
        return;

    xmlFreeNodeList(n->children);
    n->children =
    n->last = NULL;
}


void node::sort(const char *node_name, const char *attr_name)
{
    xmlNodePtr i(pimpl_->xmlnode_->children), next(0);
    std::vector<xmlNodePtr> node_list;

    while (i!=0)
    {
        next = i->next;

        if (i->type == XML_ELEMENT_NODE && xmlStrcmp(i->name, reinterpret_cast<const xmlChar*>(node_name)) == 0)
        {
            xmlUnlinkNode(i);
            node_list.push_back(i);
        }

        i = next;
    }

    if (node_list.empty())
        return;

    std::sort(node_list.begin(), node_list.end(), compare_attr(attr_name));
    std::for_each(node_list.begin(), node_list.end(), insert_node(pimpl_->xmlnode_));
}


void node::sort_fo(cbfo_node_compare& cb)
{
    xmlNodePtr i(pimpl_->xmlnode_->children), next(0);
    std::vector<xmlNodePtr> node_list;

    while (i!=0)
    {
        next = i->next;

        if (i->type == XML_ELEMENT_NODE) {
            xmlUnlinkNode(i);
            node_list.push_back(i);
        }

        i = next;
    }

    if (node_list.empty())
        return;

    std::sort(node_list.begin(), node_list.end(), node_cmp(cb));
    std::for_each(node_list.begin(), node_list.end(), insert_node(pimpl_->xmlnode_));
}


std::string node::node_to_string() const
{
    node2doc n2d(pimpl_->xmlnode_);
    xmlDocPtr doc = n2d.get_doc();

    xmlChar *xml_string;
    int xml_string_length;

    xmlDocDumpFormatMemory(doc, &xml_string, &xml_string_length, 1);

    xmlchar_helper helper(xml_string);
    std::string xml;
    if (xml_string_length)
        xml.assign(helper.get(), checked_size_t_cast(xml_string_length));
    return xml;
}


std::ostream& operator<<(std::ostream &stream, const xml::node& n)
{
    stream << n.node_to_string();
    return stream;
}

} // namespace xml
