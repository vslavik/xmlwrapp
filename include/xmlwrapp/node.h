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
 * Definition of xml::node.
 **/

#ifndef _xmlwrapp_node_h_
#define _xmlwrapp_node_h_

#include "const_node_interface.h"
#include <xmlwrapp/_cbfo.h> // hidden
#include <xmlwrapp/reference.h>
#include <iosfwd>
#include <string>

namespace xpath {
  class xpath_helper;
}

namespace xml {
  struct node_cmp;

  //####################################################################
  //! An XML node.
  //! Provides functions for read-write access to node properties
  //! and uses detail::const_node_interface to provide read-only access.
  class node : public detail::const_node_interface {
  public:
    typedef detail::const_node_interface base_type;
    typedef base_type::iterator          iterator;

    //####################################################################
    //! Reference to a node, which may be used to alter the node.
    typedef node_reference               reference;

    //####################################################################
    //! Const reference to a node, which may be used to access (but not
    //! modify) the node.
    typedef const_node_reference         const_reference;

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
     * Get an iterator that points one past the last child for this node.
     *
     * @return A "one past the end" iterator.
     * @author Peter Jones
     **/
    //####################################################################
    iterator end (void);

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

    // prevent hiding (defined both here and in const interface)
    using base_type::begin;
    using base_type::end;
    using base_type::find;
    using base_type::get_attributes;
    using base_type::parent;
    using base_type::self;

  private:
    explicit node (node_impl *impl) : base_type(impl) { }
    XMLWRAPP_FRIEND_T(node_reference_T);

    void sort_fo (cbfo_node_compare &fo);
  };
}

#endif
