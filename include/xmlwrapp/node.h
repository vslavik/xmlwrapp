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
 * This file contains the definition of the xml::node class.
**/

#ifndef _xmlwrapp_node_h_
#define _xmlwrapp_node_h_

// hidden stuff
#include <xmlwrapp/_cbfo.h>

// standard includes
#include <cstddef>
#include <iosfwd>
#include <string>

namespace xml {

// forward declarations
class document;
class attributes;
struct node_impl;
struct nipimpl;
struct node_cmp;

/**
 * The xml::node class is used to hold information about one XML node. This
 * includes the name of the node, the namespace of the node and attributes
 * for the node. It also has an iterator whereby you can get to the children
 * nodes.
 *
 * It should be noted that any member function that returns a const char*
 * returns a temporary value. The pointer that is returned will change with
 * ANY operation to the xml::node. If you need the data to stick around a
 * little longer you should put it inside a std::string.
**/
class node {
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
     * Construct a new blank xml::node.
     *
     * @author Peter Jones
    **/
    //####################################################################
    node (void);

    //####################################################################
    /** 
     * Construct a new xml::node and set the name of the node.
     *
     * @param name The name of the new node.
     * @author Peter Jones
    **/
    //####################################################################
    explicit node (const char *name);

    //####################################################################
    /** 
     * Construct a new xml::node given a name and content. The content will
     * be used to create a new child text node.
     *
     * @param name The name of the new element.
     * @param content The text that will be used to create a child node.
     * @author Peter Jones
    **/
    //####################################################################
    node (const char *name, const char *content);

    //####################################################################
    /** 
     * Construct a new xml::node by copying another xml::node.
     *
     * @param other The other node to copy.
     * @author Peter Jones
    **/
    //####################################################################
    node (const node &other);

    //####################################################################
    /** 
     * Make this node equal to some other node via assignment.
     *
     * @param other The other node to copy.
     * @return A reference to this node.
     * @author Peter Jones
    **/
    //####################################################################
    node& operator= (const node &other);

    //####################################################################
    /** 
     * Class destructor
     *
     * @author Peter Jones
    **/
    //####################################################################
    ~node (void);

    //####################################################################
    /** 
     * Set the name of this xml::node.
     *
     * @param name The new name for this xml::node.
     * @author Peter Jones
    **/
    //####################################################################
    void set_name (const char *name);

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
     * Set the content of a node. If this node is an element node, this
     * function will remove all of its children nodes and replace them
     * with one text node set to the given string.
     *
     * @param content The content of the text node.
     * @author Peter Jones
    **/
    //####################################################################
    void set_content (const char *content);

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
     * and set the attributes for this node. Make sure you use a reference
     * to this returned object, to prevent a copy.
     *
     * @return The xml::attributes object for this node.
     * @author Peter Jones
    **/
    //####################################################################
    xml::attributes& get_attributes (void);

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

    //####################################################################
    /** 
     * Add a child xml::node to this node.
     *
     * @param child The child xml::node to add.
     * @author Peter Jones
    **/
    //####################################################################
    void push_back (const node &child);

    //####################################################################
    /** 
     * Swap this node with another one.
     *
     * @param other The other node to swap with.
     * @author Peter Jones
    **/
    //####################################################################
    void swap (node &other);

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
	friend class node;
    };

    //####################################################################
    /** 
     * Get an iterator that points to the beginning of this node's children.
     *
     * @return An iterator that points to the beginning of the children.
     * @author Peter Jones
    **/
    //####################################################################
    iterator begin (void);

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
     * Get an iterator that points one past the last child for this node.
     *
     * @return A "one past the end" iterator.
     * @author Peter Jones
    **/
    //####################################################################
    iterator end (void);

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
     * Get an iterator that points back at this node.
     *
     * @return An iterator that points at this node.
     * @author Peter Jones
    **/
    //####################################################################
    iterator self (void);

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
     * Get an iterator that points at the parent of this node. If this node
     * does not have a parent, this member function will return an "end"
     * iterator.
     *
     * @return An iterator that points to this nodes parent.
     * @return If no parent, returns the same iterator that xml::node::end() returns.
     * @author Peter Jones
    **/
    //####################################################################
    iterator parent (void);

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
     * can be found, this function will return the same iterator that end()
     * would return.
     *
     * This function is not recursive. That is, it will not search down the
     * tree for the requested node. Instead, it will only search one level
     * deep, only checking the children of this node.
     *
     * @param name The name of the node you want to find.
     * @return An iterator that points to the node if found.
     * @return An end() iterator if the node was not found.
     * @author Peter Jones
    **/
    //####################################################################
    iterator find (const char *name);

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
     * Find the first child node, starting with the given iterator, that has
     * the given name. If no such node can be found, this function will
     * return the same iterator that end() would return.
     *
     * This function should be given an iterator to one of this node's
     * children. The search will begin with that node and continue with all
     * its sibliings. This function will not recurse down the tree, it only
     * searches in one level.
     *
     * @param name The name of the node you want to find.
     * @param start Where to begin the search.
     * @return An iterator that points to the node if found.
     * @return An end() iterator if the node was not found.
     * @author Peter Jones
    **/
    //####################################################################
    iterator find (const char *name, iterator start);

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
     * Insert a new child node. The new node will be inserted at the end of
     * the child list. This is similar to the xml::node::push_back member
     * function except that an iterator to the inserted node is returned.
     *
     * @param n The node to insert as a child of this node.
     * @return An iterator that points to the newly inserted node.
     * @author Peter Jones
    **/
    //####################################################################
    iterator insert (const node &n);

    //####################################################################
    /** 
     * Insert a new child node. The new node will be inserted before the
     * node pointed to by the given iterator.
     *
     * @param position An iterator that points to the location where the new node should be inserted (before it).
     * @param n The node to insert as a child of this node.
     * @return An iterator that points to the newly inserted node.
     * @author Peter Jones
    **/
    //####################################################################
    iterator insert (iterator position, const node &n);

    //####################################################################
    /** 
     * Replace the node pointed to by the given iterator with another node.
     * The old node will be removed, including all its children, and
     * replaced with the new node. This will invalidate any iterators that
     * point to the node to be replaced, or any pointers or references to
     * that node.
     *
     * @param old_node An iterator that points to the node that should be removed.
     * @param new_node The node to put in old_node's place.
     * @return An iterator that points to the new node.
     * @author Peter Jones
    **/
    //####################################################################
    iterator replace (iterator old_node, const node &new_node);

    //####################################################################
    /** 
     * Erase the node that is pointed to by the given iterator. The node
     * and all its children will be removed from this node. This will
     * invalidate any iterators that point to the node to be erased, or any
     * pointers or references to that node.
     *
     * @param to_erase An iterator that points to the node to be erased.
     * @return An iterator that points to the node after the one being erased.
     * @author Peter Jones
     * @author Gary A. Passero
    **/
    //####################################################################
    iterator erase (iterator to_erase);

    //####################################################################
    /** 
     * Erase all nodes in the given range, from frist to last. This will
     * invalidate any iterators that point to the nodes to be erased, or any
     * pointers or references to those nodes.
     * 
     * @param first The first node in the range to be removed.
     * @param last An iterator that points one past the last node to erase. Think xml::node::end().
     * @return An iterator that points to the node after the last one being erased.
     * @author Peter Jones
    **/
    //####################################################################
    iterator erase (iterator first, iterator last);

    //####################################################################
    /** 
     * Erase all children nodes with the given name. This will find all
     * nodes that have the given node name and remove them from this node.
     * This will invalidate any iterators that point to the nodes to be
     * erased, or any pointers or references to those nodes.
     *
     * @param name The name of nodes to remove.
     * @return The number of nodes removed.
     * @author Peter Jones
    **/
    //####################################################################
    size_type erase (const char *name);

    //####################################################################
    /** 
     * Sort all the children nodes of this node using one of thier
     * attributes. Only nodes that are of xml::node::type_element will be
     * sorted, and they must have the given node_name.
     *
     * The sorting is done by calling std::strcmp on the value of the given
     * attribute.
     *
     * @param node_name The name of the nodes to sort.
     * @param attr_name The attribute to sort on.
     * @author Peter Jones
    **/
    //####################################################################
    void sort (const char *node_name, const char *attr_name);

    //####################################################################
    /** 
     * Sort all the children nodes of this node using the given comparison
     * function object. All element type nodes will be considered for
     * sorting.
     *
     * @param compare The binary function object to call in order to sort all child nodes.
     * @author Peter Jones
    **/
    //####################################################################
    template <typename T> void sort (T compare)
    { sort_callback<T> cb(compare); sort_fo(cb); }

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

    //####################################################################
    /** 
     * Write a node and all of its children to the given stream.
     *
     * @param stream The stream to write the node as XML.
     * @param n The node to write to the stream.
     * @return The stream.
     * @author Peter Jones
    **/
    //####################################################################
    friend std::ostream& operator<< (std::ostream &stream, const node &n);
private:
    node_impl *pimpl_;
    void set_node_data (void *data);
    void* get_node_data (void);
    void* release_node_data (void);
    friend class tree_parser;
    friend class node_iterator;
    friend struct doc_impl;
    friend struct node_cmp;

    void sort_fo (cbfo_node_compare &fo);
}; // end xml::node class

} // end xml namespace
#endif
