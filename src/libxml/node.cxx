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

#include <xmlwrapp/node.h>

#include "ait_impl.h"
#include "node_impl.h"
#include "utility.h"
#include <xmlwrapp/attributes.h>

// standard includes
#include <new>
#include <memory>
#include <string>
#include <vector>
#include <iostream>
#include <algorithm>
#include <stdexcept>
#include <functional>

// libxml includes
#include <libxml/tree.h>
#include <libxml/parser.h>

namespace {
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
}
//####################################################################
xml::node::node () { }
xml::node::node (const char *name) : base_type(name) { }
xml::node::node (const char *name, const char *content) : base_type(name, content) { }
xml::node::node (const node &rhs) : base_type(rhs) { }
xml::node &xml::node::operator= (const node &rhs) { base_type::operator=(rhs); return *this; }
xml::node::~node (void) { }
void xml::node::swap (node &other) { do_swap(other); }
//####################################################################
void xml::node::set_name (const char *name) {
  xmlNodeSetName(pimpl_->xmlnode_, reinterpret_cast<const xmlChar*>(name));
}
//####################################################################
void xml::node::set_content (const char *content) {
  xmlNodeSetContent(pimpl_->xmlnode_, reinterpret_cast<const xmlChar*>(content));
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
void xml::node::push_back (const node &child) {
  xmlNodePtr node_to_add = xmlCopyNode(child.pimpl_->xmlnode_, 1);
  if (!node_to_add) return;

  if (!xmlAddChild(pimpl_->xmlnode_, node_to_add)) {
    xmlFreeNode(node_to_add);
  }
}
//####################################################################
xml::node::iterator xml::node::begin (void) {
  return iterator(pimpl_->xmlnode_->children);
}
//####################################################################
xml::node::iterator xml::node::end (void) {
  return iterator();
}
//####################################################################
xml::node::iterator xml::node::self (void) {
  return iterator(pimpl_->xmlnode_);
}
//####################################################################
xml::node::iterator xml::node::parent (void) {
  if (pimpl_->xmlnode_->parent) return iterator(pimpl_->xmlnode_->parent);
  return iterator();
}
//####################################################################
xml::node::iterator xml::node::find (const char *name) {
  xmlNodePtr found = find_node(name, pimpl_->xmlnode_->children);
  if (found) return iterator(found);
  return end();
}
//####################################################################
xml::node::iterator xml::node::find (const char *name, iterator start) {
  xmlNodePtr n = static_cast<xmlNodePtr>(start.get_raw_node());
  if ( (n = find_node(name, n))) return iterator(n);
  return end();
}
//####################################################################
xml::node::iterator xml::node::insert (const node &n) {
  push_back(n);
  return iterator(pimpl_->xmlnode_->last);
}
//####################################################################
xml::node::iterator xml::node::insert (iterator position, const node &n) {
  xmlNodePtr before_xml_node = static_cast<xmlNodePtr>(position.get_raw_node());

  if (before_xml_node == 0) { // xml::node::end() given?
    push_back(n);
    return iterator(pimpl_->xmlnode_->last);
  }

  xmlNodePtr new_xml_node =  xmlCopyNode(n.pimpl_->xmlnode_, 1);
  if (!new_xml_node) throw std::bad_alloc();

  if (xmlAddPrevSibling(before_xml_node, new_xml_node) == 0) {
    xmlFreeNode(new_xml_node);
    throw std::runtime_error("failed to insert xml::node. xmlAddPrevSibling failed");
  }

  return iterator(new_xml_node);
}
//####################################################################
xml::node::iterator xml::node::replace (iterator old_node, const node &new_node) {
  xmlNodePtr old_xml_node = static_cast<xmlNodePtr>(old_node.get_raw_node());
  xmlNodePtr new_xml_node =  xmlCopyNode(new_node.pimpl_->xmlnode_, 1);
  if (!new_xml_node) throw std::bad_alloc();

  // hack to see if xmlReplaceNode was successful
  new_xml_node->doc = reinterpret_cast<xmlDocPtr>(old_xml_node);
  xmlReplaceNode(old_xml_node, new_xml_node);

  if (new_xml_node->doc == reinterpret_cast<xmlDocPtr>(old_xml_node)) {
    xmlFreeNode(new_xml_node);
    throw std::runtime_error("failed to replace xml::node. xmlReplaceNode() failed");
  }

  xmlFreeNode(old_xml_node);
  return iterator(new_xml_node);
}
//####################################################################
xml::node::iterator xml::node::erase (iterator to_erase) {
  xmlNodePtr old_xml_node = static_cast<xmlNodePtr>(to_erase.get_raw_node());
  ++to_erase;

  xmlUnlinkNode(old_xml_node);
  xmlFreeNode(old_xml_node);

  return to_erase;
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
