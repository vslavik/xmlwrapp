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

#ifndef XMLWRAPP_NODE_IMPL_H_20030830T013157
#define XMLWRAPP_NODE_IMPL_H_20030830T013157

#include <xmlwrapp/node.h>
#include <libxml/tree.h>

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
struct xml::node_impl {
  //####################################################################
  node_impl (void) : xmlnode_(0), owner_(true), attrs_(0)
  { }
  //####################################################################
  node_impl (xmlNodePtr node) : xmlnode_(node), owner_(false), attrs_(0)
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

  //####################################################################
  inline xmlNodePtr find_node (const char *name, xmlNodePtr first) {
    while (first != 0) {
      if (first->type == XML_ELEMENT_NODE && xmlStrcmp(first->name, reinterpret_cast<const xmlChar*>(name)) == 0) return first;
      first = first->next;
    }

    return 0;
  }
}

#endif
