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
 * Definition of xml::detail::const_node_interface.
 **/

#ifndef XMLWRAPP_CONST_NODE_INTERFACE_H_20030829T202924
#define XMLWRAPP_CONST_NODE_INTERFACE_H_20030829T202924

#include <xmlwrapp/access.h>
#include <xmlwrapp/xpath_fwd.h>
#include <iterator>
#include <string>
#include <cstddef>

namespace xpath {
  class xpath_helper;
}

#define XMLWRAPP_READ_ONLY  XMLWRAPP_RO_ACCESS
#define XMLWRAPP_READ_WRITE XMLWRAPP_RW_ACCESS

namespace xml {
  class attributes;
  class document;
  class node;
  struct doc_impl;
  struct node_impl;
  struct nipimpl;
  struct node_cmp;
  class node_iterator;

  template <XMLWRAPP_ACCESS_SPECIFIER Access> class node_reference_T;
  typedef node_reference_T<XMLWRAPP_READ_WRITE> node_reference;
  typedef node_reference_T<XMLWRAPP_READ_ONLY>  const_node_reference;

  namespace detail {
    //####################################################################
    //! Interface allowing read-only access to an XML node.
    class const_node_interface {
    public:
      /// size type
      typedef std::size_t size_type;

      /// enum for the different types of XML nodes
      enum node_type {
	type_element,		///< XML element such as <chapter/>
	type_text,		///< Text node
	type_cdata,		///< <![CDATA[text]]>
	type_pi,		///< Processing Instruction
	type_comment,		///< XML comment
	type_entity,		///< Entity as in &amp;amp;
	type_entity_ref,	///< Entity ref
	type_xinclude,		///< <xi:include/> node
	type_document,		///< Document node
	type_document_type,	///< DOCTYPE node
	type_document_frag,	///< Document Fragment
	type_notation,		///< Notation
	type_dtd,		///< DTD node
	type_dtd_element,	///< DTD <!ELEMENT> node
	type_dtd_attribute,	///< DTD <!ATTRLIST> node
	type_dtd_entity,	///< DTD <!ENTITY>
	type_dtd_namespace	///< ?
      };

      //####################################################################
      /** 
       * Get the name of this xml::node.
       *
       * This function may change in the future to return std::string.
       * Feedback is welcome.
       *
       * @return The name of this node.
       * @author Peter Jones
       **/
      //####################################################################
      const char* get_name (void) const;

      //####################################################################
      /** 
       * Get the content for this text node. If this node is not a text node
       * but it has children nodes that are text nodes, the contents of those
       * child nodes will be returned. If there is no content or these
       * conditions do not apply, zero will be returned.
       *
       * This function may change in the future to return std::string.
       * Feedback is welcome.
       *
       * @return The content or 0.
       * @author Peter Jones
       **/
      //####################################################################
      const char* get_content (void) const;

      //####################################################################
      /** 
       * Get this node's "type". You can use that information to know what you
       * can and cannot do with it.
       *
       * @return The node's type.
       * @author Peter Jones
       **/
      //####################################################################
      node_type get_type (void) const;

      //####################################################################
      /** 
       * Get the list of attributes. You can use the returned object to get
       * the attributes for this node. Make sure you use a reference to this
       * returned object, to prevent a copy.
       *
       * @return The xml::attributes object for this node.
       * @author Peter Jones
       **/
      //####################################################################
      const xml::attributes& get_attributes (void) const;

      //####################################################################
      /** 
       * Find out if this node is a text node or sometiming like a text node,
       * CDATA for example.
       *
       * @return True if this node is a text node; false otherwise.
       * @author Peter Jones
       **/
      //####################################################################
      bool is_text (void) const;

      class const_iterator; // forward declaration

      /**
       * The xml::node::iterator provides a way to access children nodes
       * similar to a standard C++ container. The nodes that are pointed to by
       * the iterator can be changed.
       */
      class iterator {
      public:
	typedef node value_type;
	typedef std::ptrdiff_t difference_type;
	typedef value_type* pointer;
	typedef value_type& reference;
	typedef std::forward_iterator_tag iterator_category;

	iterator  (void);
	iterator  (const iterator &other);
	iterator& operator= (const iterator& other);
	~iterator (void);

	reference operator*  (void) const;
	pointer   operator-> (void) const;

	/// prefix increment
	iterator& operator++ (void);

	/// postfix increment (avoid if possible for better performance)
	iterator  operator++ (int);

	friend bool operator== (const iterator &lhs, const iterator &rhs);
	friend bool operator!= (const iterator &lhs, const iterator &rhs);
      private:
	nipimpl *pimpl_;
	explicit iterator (void *data);
	void* get_raw_node (void);
	void swap (iterator &other);
	friend class node;
	friend class document;
	friend class const_iterator;
      };

      /**
       * The xml::node::const_iterator provides a way to access children nodes
       * similar to a standard C++ container. The nodes that are pointed to by
       * the const_iterator cannot be changed.
       */
      class const_iterator {
      public:
	typedef const node value_type;
	typedef std::ptrdiff_t difference_type;
	typedef value_type* pointer;
	typedef value_type& reference;
	typedef std::forward_iterator_tag iterator_category;

	const_iterator  (void);
	const_iterator  (const const_iterator &other);
	const_iterator  (const iterator &other);
	const_iterator& operator= (const const_iterator& other);
	~const_iterator (void);

	reference operator*  (void) const;
	pointer   operator-> (void) const;

	/// prefix increment
	const_iterator& operator++ (void);

	/// postfix increment (avoid if possible for better performance)
	const_iterator  operator++ (int);

	friend bool operator== (const const_iterator &lhs, const const_iterator &rhs);
	friend bool operator!= (const const_iterator &lhs, const const_iterator &rhs);
      private:
	nipimpl *pimpl_;
	explicit const_iterator (void *data);
	void* get_raw_node (void);
	void swap (const_iterator &other);
	friend class document;
	friend class const_node_interface;
      };

      //####################################################################
      /** 
       * Get a const_iterator that points to the beginning of this node's
       * children.
       *
       * @return A const_iterator that points to the beginning of the children.
       * @author Peter Jones
       **/
      //####################################################################
      const_iterator begin (void) const;

      //####################################################################
      /** 
       * Get a const_iterator that points one past the last child for this
       * node.
       *
       * @return A "one past the end" const_iterator
       * @author Peter Jones
       **/
      //####################################################################
      const_iterator end (void) const;

      //####################################################################
      /** 
       * Get a const_iterator that points back at this node.
       *
       * @return A const_iterator that points at this node.
       * @author Peter Jones
       **/
      //####################################################################
      const_iterator self (void) const;

      //####################################################################
      /** 
       * Get a const_iterator that points at the parent of this node. If this
       * node does not have a parent, this member function will return an
       * "end" const_iterator.
       *
       * @return A const_iterator that points to this nodes parent.
       * @return If no parent, returns the same const_iterator that xml::node::end() returns.
       * @author Peter Jones
       **/
      //####################################################################
      const_iterator parent (void) const;

      //####################################################################
      /** 
       * Find the first child node that has the given name. If no such node
       * can be found, this function will return the same const_iterator that
       * end() would return.
       *
       * This function is not recursive. That is, it will not search down the
       * tree for the requested node. Instead, it will only search one level
       * deep, only checking the children of this node.
       *
       * @param name The name of the node you want to find.
       * @return A const_iterator that points to the node if found.
       * @return An end() const_iterator if the node was not found.
       * @author Peter Jones
       **/
      //####################################################################
      const_iterator find (const char *name) const;


      //####################################################################
      /** 
       * Find the first child node, starting with the given const_iterator,
       * that has the given name. If no such node can be found, this function
       * will return the same const_iterator that end() would return.
       *
       * This function should be given a const_iterator to one of this node's
       * children. The search will begin with that node and continue with all
       * its sibliings. This function will not recurse down the tree, it only
       * searches in one level.
       *
       * @param name The name of the node you want to find.
       * @param start Where to begin the search.
       * @return A const_iterator that points to the node if found.
       * @return An end() const_iterator if the node was not found.
       * @author Peter Jones
       **/
      //####################################################################
      const_iterator find (const char *name, const_iterator start) const;

      //####################################################################
      /** 
       * Convert the node and all its children into XML text and set the given
       * string to that text.
       *
       * @param xml The string to set the node's XML data to.
       * @author Peter Jones
       **/
      //####################################################################
      void node_to_string (std::string &xml) const;

    private:
      const_node_interface ();
      explicit const_node_interface (const char *name);
      const_node_interface (const char *name, const char *content);
      const_node_interface (const const_node_interface &);
      const_node_interface &operator= (const const_node_interface &);
      ~const_node_interface ();

      explicit const_node_interface (node_impl *);

      node_impl *pimpl_;

      void* get_node_data (void) const;
      void set_node_data (void *data);
      void* release_node_data (void);
      friend class tree_parser;
      friend class node_iterator;
      friend struct doc_impl;
      friend struct node_cmp;
      friend class xpath::xpath_helper;
      friend class node;
      XMLWRAPP_FRIEND_T(node_reference_T);

      void do_swap (const_node_interface &rhs);
    };
  }
}

#endif
