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

/**
    @file

    This file contains the definition of the xml::node class.
 */

#ifndef _xmlwrapp_node_h_
#define _xmlwrapp_node_h_

// xmlwrapp includes
#include "xmlwrapp/init.h"
#include "xmlwrapp/export.h"

// hidden stuff
#include "xmlwrapp/_cbfo.h"

// standard includes
#include <cstddef>
#include <iosfwd>
#include <string>

namespace xml
{

// forward declarations
class document;
class attributes;
class nodes_view;
class const_nodes_view;

namespace impl
{
class node_iterator;
class iter_advance_functor;
struct node_impl;
struct doc_impl;
struct nipimpl;
struct node_cmp;
struct xpath_context_impl;
}


/**
    The xml::node class is used to hold information about one XML node.

    This includes the name of the node, the namespace of the node and
    attributes for the node. It also has an iterator whereby you can get to the
    children nodes.

    It should be noted that any member function that returns a const char*
    returns a temporary value. The pointer that is returned will change with
    ANY operation to the xml::node. If you need the data to stick around a
    little longer you should put it inside a std::string.
 */
class XMLWRAPP_API node
{
public:
    /// size type
    typedef std::size_t size_type;

    /// enum for the different types of XML nodes
    enum node_type
    {
        type_element,       ///< XML element such as "<chapter/>"
        type_text,          ///< Text node
        type_cdata,         ///< <![CDATA[text]]>
        type_pi,            ///< Processing Instruction
        type_comment,       ///< XML comment
        type_entity,        ///< Entity as in &amp;amp;
        type_entity_ref,    ///< Entity ref
        type_xinclude,      ///< <xi:include/> node
        type_document,      ///< Document node
        type_document_type, ///< DOCTYPE node
        type_document_frag, ///< Document Fragment
        type_notation,      ///< Notation
        type_dtd,           ///< DTD node
        type_dtd_element,   ///< DTD <!ELEMENT> node
        type_dtd_attribute, ///< DTD <!ATTRLIST> node
        type_dtd_entity,    ///< DTD <!ENTITY>
        type_dtd_namespace  ///< ?
    };

    /**
        Helper struct for creating a xml::node of type_cdata.

        @code
        xml::node mynode(xml::node::cdata("This is a CDATA section"));
        @endcode
     */
    struct cdata
    {
        explicit cdata(const char *text) : t(text) {}
        const char *t;
    };

    /**
        Helper struct for creating a xml::node of type_comment.

        @code
        xml::node mynode(xml::node::comment("This is an XML comment"));
        @endcode
     */
    struct comment
    {
        explicit comment (const char *text) : t(text) {}
        const char *t;
    };

    /**
        Helper struct for creating a xml::node of type_pi.

        @code
        xml::node mynode(xml::node::pi("xslt", "stylesheet=\"test.xsl\""));
        @endcode
     */
    struct pi
    {
        explicit pi (const char *name, const char *content = NULL)
            : n(name), c(content) {}
        const char *n, *c;
    };

    /**
        Helper struct for creating a xml::node of type_text.

        @code
        xml::node mynode(xml::node::text("This is an XML text fragment"));
        @endcode
     */
    struct text
    {
        explicit text (const char *text) : t(text) {}
        const char *t;
    };

    /**
        Construct a new blank xml::node.
     */
    node();

    /**
        Construct a new xml::node and set the name of the node.

        @param name The name of the new node.
     */
    explicit node(const char *name);

    /**
        Construct a new xml::node given a name and content.

        The content, if it's not an empty string, will be used to create a new
        child text node.

        @param name The name of the new element.
        @param content The text that will be used to create a child node.
     */
    node(const char *name, const char *content);

    /**
        Construct a new xml::node that is of type_cdata. The cdata_info
        parameter should contain the contents of the CDATA section.

        @note Sample Use Example:
        @code
        xml::node mynode(xml::node::cdata("This is a CDATA section"));
        @endcode

        @param cdata_info A cdata struct that tells xml::node what the
                          content will be.
     */
    explicit node(cdata cdata_info);

    /**
        Construct a new xml::node that is of type_comment. The comment_info
        parameter should contain the contents of the XML comment.

        @note Sample Use Example:
        @code
        xml::node mynode(xml::node::comment("This is an XML comment"));
        @endcode

        @param comment_info A comment struct that tells xml::node what the comment will be.
     */
    explicit node(comment comment_info);

    /**
        Construct a new xml::node that is of type_pi. The pi_info parameter
        should contain the name of the XML processing instruction (PI), and
        optionally, the contents of the XML PI.

        @note Sample Use Example:
        @code
        xml::node mynode(xml::node::pi("xslt", "stylesheet=\"test.xsl\""));
        @endcode

        @param pi_info A pi struct that tells xml::node what the name and contents of the XML PI are.
     */
    explicit node(pi pi_info);

    /**
        Construct a new xml::node that is of type_text. The text_info
        parameter should contain the text.

        @note Sample Use Example:
        @code
        xml::node mynode(xml::node::text("This is XML text"));
        @endcode

        @param text_info A text struct that tells xml::node what the text will be.
     */
    explicit node(text text_info);

    /**
        Construct a new xml::node by copying another xml::node.

        @param other The other node to copy.
     */
    node(const node& other);

    /**
        Make this node equal to some other node via assignment.

        @param other The other node to copy.
        @return A reference to this node.
     */
    node& operator=(const node& other);

    /**
        Class destructor
     */
    ~node();

    /**
        Set the name of this xml::node.

        @param name The new name for this xml::node.
     */
    void set_name(const char *name);

    /**
        Get the name of this xml::node.

        This function may change in the future to return std::string.
        Feedback is welcome.

        @return The name of this node.
     */
    const char* get_name() const;

    /**
        Set the content of a node. If this node is an element node, this
        function will remove all of its children nodes and replace them
        with one text node set to the given string.

        @param content The content of the text node.

        @note @a content is supposed to be a piece of XML CDATA, so it allows
              entity references, but XML special chars need to be escaped
              first. In particular, the '&' character @em must be escaped
              as "&amp;" unless it's part of entity reference. Not escaping
              @a content may result in truncation of data. Use
              set_text_content() if @a content may contain special characters.

        @see set_text_content()
     */
    void set_content(const char *content);

    /**
        Set the content of a node to given text.

        In contrast to set_content(), @a content is raw text, so unescaped XML
        special chars are allowed and entity references are not supported.

        If this node is an element node, this function will remove all of its
        children nodes and replace them with one text node set to the given
        string.

        @param content The content text.

        @see set_content()

        @since 0.7.0
     */
    void set_text_content(const char *content);

    /**
        Get the content for this text node. If this node is not a text node
        but it has children nodes that are text nodes, the contents of those
        child nodes will be returned. If there is no content or these
        conditions do not apply, zero will be returned.

        This function may change in the future to return std::string.
        Feedback is welcome.

        @return The content or 0.
     */
    const char* get_content() const;

    /**
        Get this node's "type". You can use that information to know what you
        can and cannot do with it.

        @return The node's type.
     */
    node_type get_type() const;

    /**
        Get the list of attributes. You can use the returned object to get
        and set the attributes for this node. Make sure you use a reference
        to this returned object, to prevent a copy.

        @return The xml::attributes object for this node.
     */
    xml::attributes& get_attributes();

    /**
        Get the list of attributes. You can use the returned object to get
        the attributes for this node. Make sure you use a reference to this
        returned object, to prevent a copy.

        @return The xml::attributes object for this node.
     */
    const xml::attributes& get_attributes() const;

    /**
        Get the namespace of this xml::node.

        @return The namespace of this node or NULL if no namespace is
                associated.
        @since  0.6.0
     */
    const char* get_namespace() const;

    /**
        Set the default namespace of this xml::node.

        If the default namespace is already set on this node, it is changed.

        Example:

        @code
        // n is <foo/>
        n.set_namespace("http://example.com")
        // n is <foo xmlns="http://example.com"/>
        @endcode

        @since 0.8.0
     */
    void set_namespace(const std::string& href);

    /**
        Find out if this node is a text node or something like a text node,
        CDATA for example.

        @return True if this node is a text node; false otherwise.
     */
    bool is_text() const;

    /**
        Add a child xml::node to this node.

        @param child The child xml::node to add.
     */
    void push_back(const node& child);

    /**
        Swap this node with another one.

        @param other The other node to swap with.
     */
    void swap(node& other);

    /**
        Move this node under another parent.

        This node will become the last child of @a new_parent. Notice that this
        node must not be an ancestor of @a new_parent, in particular it
        shouldn't be the document root.

        Currently this method can only be used to move nodes inside the same
        document.

        All iterators pointing to this node are invalidated after the move.

        @param new_parent The new parent for the node.

        @since 0.7.2
     */
    void move_under(node& new_parent);


    class const_iterator; // forward declaration

    /**
        The xml::node::iterator provides a way to access children nodes
        similar to a standard C++ container. The nodes that are pointed to by
        the iterator can be changed.
     */
    class iterator
    {
    public:
        typedef node value_type;
        typedef int difference_type;
        typedef value_type* pointer;
        typedef value_type& reference;
        typedef std::forward_iterator_tag iterator_category;

        iterator() : pimpl_(0) {}
        iterator(const iterator& other);
        iterator& operator=(const iterator& other);
        ~iterator();

        reference operator* () const;
        pointer   operator->() const;

        /// prefix increment
        iterator& operator++();

        /// postfix increment (avoid if possible for better performance)
        iterator  operator++ (int);

    private:
        impl::nipimpl *pimpl_;

        explicit iterator (void *data);
        void* get_raw_node() const;
        void swap (iterator &other);

        friend class node;
        friend class document;
        friend class const_iterator;
        friend bool XMLWRAPP_API operator==(const iterator& lhs, const iterator& rhs);
    };

    /**
        The xml::node::const_iterator provides a way to access children nodes
        similar to a standard C++ container. The nodes that are pointed to by
        the const_iterator cannot be changed.
     */
    class const_iterator
    {
    public:
        typedef const node value_type;
        typedef int difference_type;
        typedef value_type* pointer;
        typedef value_type& reference;
        typedef std::forward_iterator_tag iterator_category;

        const_iterator() : pimpl_(0) {}
        const_iterator(const const_iterator &other);
        const_iterator(const iterator &other);
        const_iterator& operator=(const const_iterator& other);
        ~const_iterator();

        reference operator* () const;
        pointer   operator->() const;

        /// prefix increment
        const_iterator& operator++();

        /// postfix increment (avoid if possible for better performance)
        const_iterator  operator++ (int);

    private:
        impl::nipimpl *pimpl_;

        explicit const_iterator (void *data);
        void* get_raw_node() const;
        void swap (const_iterator &other);

        friend class document;
        friend class node;
        friend bool XMLWRAPP_API operator==(const const_iterator& lhs, const const_iterator& rhs);
    };

    /**
        Returns the number of children this nodes has. If you just want to
        know how if this node has children or not, you should use
        xml::node::empty() instead.

        @return The number of children this node has.
     */
    size_type size() const;

    /**
        Find out if this node has any children. This is the same as
        xml::node::size() == 0 except it is much faster.

        @return True if this node DOES NOT have any children.
        @return False if this node does have children.
     */
    bool empty() const;

    /**
        Get an iterator that points to the beginning of this node's children.

        @return An iterator that points to the beginning of the children.
     */
    iterator begin();

    /**
        Get a const_iterator that points to the beginning of this node's
        children.

        @return A const_iterator that points to the beginning of the children.
     */
    const_iterator begin() const;

    /**
        Get an iterator that points one past the last child for this node.

        @return A "one past the end" iterator.
     */
    iterator end() { return iterator(); }

    /**
        Get a const_iterator that points one past the last child for this
        node.

        @return A "one past the end" const_iterator
     */
    const_iterator end() const { return const_iterator(); }

    /**
        Get an iterator that points back at this node.

        @return An iterator that points at this node.
     */
    iterator self();

    /**
        Get a const_iterator that points back at this node.

        @return A const_iterator that points at this node.
     */
    const_iterator self() const;

    /**
        Get an iterator that points at the parent of this node. If this node
        does not have a parent, this member function will return an "end"
        iterator.

        @return An iterator that points to this nodes parent.
        @return If no parent, returns the same iterator that xml::node::end() returns.
     */
    iterator parent();

    /**
        Get a const_iterator that points at the parent of this node. If this
        node does not have a parent, this member function will return an
        "end" const_iterator.

        @return A const_iterator that points to this nodes parent.
        @return If no parent, returns the same const_iterator that xml::node::end() returns.
     */
    const_iterator parent() const;

    /**
        Find the first child node that has the given name. If no such node
        can be found, this function will return the same iterator that end()
        would return.

        This function is not recursive. That is, it will not search down the
        tree for the requested node. Instead, it will only search one level
        deep, only checking the children of this node.

        @param name The name of the node you want to find.
        @return An iterator that points to the node if found.
        @return An end() iterator if the node was not found.

        @see elements(const char*), find(const char*, iterator)
     */
    iterator find(const char *name);

    /**
        Find the first child node that has the given name. If no such node
        can be found, this function will return the same const_iterator that
        end() would return.

        This function is not recursive. That is, it will not search down the
        tree for the requested node. Instead, it will only search one level
        deep, only checking the children of this node.

        @param name The name of the node you want to find.
        @return A const_iterator that points to the node if found.
        @return An end() const_iterator if the node was not found.

        @see elements(const char*) const,
             find(const char*, const_iterator) const
     */
    const_iterator find(const char *name) const;

    /**
        Find the first child node, starting with the given iterator, that has
        the given name. If no such node can be found, this function will
        return the same iterator that end() would return.

        This function should be given an iterator to one of this node's
        children. The search will begin with that node and continue with all
        its sibliings. This function will not recurse down the tree, it only
        searches in one level.

        @param name The name of the node you want to find.
        @param start Where to begin the search.
        @return An iterator that points to the node if found.
        @return An end() iterator if the node was not found.

        @see elements(const char*)
     */
    iterator find(const char *name, const iterator& start);

    /**
        Find the first child node, starting with the given const_iterator,
        that has the given name. If no such node can be found, this function
        will return the same const_iterator that end() would return.

        This function should be given a const_iterator to one of this node's
        children. The search will begin with that node and continue with all
        its siblings. This function will not recurse down the tree, it only
        searches in one level.

        @param name The name of the node you want to find.
        @param start Where to begin the search.
        @return A const_iterator that points to the node if found.
        @return An end() const_iterator if the node was not found.

        @see elements(const char*) const
     */
    const_iterator find(const char *name, const const_iterator& start) const;

    /**
        Returns view of child nodes of type type_element. If no such node
        can be found, returns empty view.

        Example:
        @code
        xml::nodes_view view(root.elements());
        for (xml::nodes_view::iterator i = view.begin(); i != view.end(); ++i)
        {
          ...
        }
        @endcode

        @return View with all child elements or empty view if there aren't any.
        @since  0.6.0

        @see nodes_view
     */
    nodes_view elements();

    /**
        Returns view of child nodes of type type_element. If no such node
        can be found, returns empty view.

        Example:
        @code
        xml::const_nodes_view view(root.elements());
        for (xml::const_nodes_view::const_iterator i = view.begin();
             i != view.end();
             ++i)
        {
          ...
        }
        @endcode

        @return View with all child elements or empty view if there aren't any.
        @since  0.6.0

        @see const_nodes_view
     */
    const_nodes_view elements() const;

    /**
        Returns view of child nodes of type type_element with name @a name.
        If no such node can be found, returns empty view.

        Example:
        @code
        xml::nodes_view view(root.elements("person"));
        for (xml::nodes_view::iterator i = view.begin(); i != view.end(); ++i)
        {
          ...
        }
        @endcode

        @param  name Name of the elements to return.
        @return View that contains only elements @a name.
        @since  0.6.0
     */
    nodes_view elements(const char *name);

    /**
        Returns view of child nodes of type type_element with name @a name.
        If no such node can be found, returns empty view.

        Example:
        @code
        xml::const_nodes_view view(root.elements("person"));
        for (xml::const_nodes_view::const_iterator i = view.begin();
             i != view.end();
             ++i)
        {
          ...
        }
        @endcode

        @param  name Name of the elements to return.
        @return View that contains only elements @a name.
        @since  0.6.0
     */
    const_nodes_view elements(const char *name) const;

    /**
        Insert a new child node. The new node will be inserted at the end of
        the child list. This is similar to the xml::node::push_back member
        function except that an iterator to the inserted node is returned.

        @param n The node to insert as a child of this node.
        @return An iterator that points to the newly inserted node.
     */
    iterator insert(const node& n);

    /**
        Insert a new child node. The new node will be inserted before the
        node pointed to by the given iterator.

        @param position An iterator that points to the location where the new node should be inserted (before it).
        @param n The node to insert as a child of this node.
        @return An iterator that points to the newly inserted node.
     */
    iterator insert(const iterator& position, const node& n);

    /**
        Replace the node pointed to by the given iterator with another node.
        The old node will be removed, including all its children, and
        replaced with the new node. This will invalidate any iterators that
        point to the node to be replaced, or any pointers or references to
        that node.

        @param old_node An iterator that points to the node that should be removed.
        @param new_node The node to put in old_node's place.
        @return An iterator that points to the new node.
     */
    iterator replace(const iterator& old_node, const node& new_node);

    /**
        Erase the node that is pointed to by the given iterator. The node
        and all its children will be removed from this node. This will
        invalidate any iterators that point to the node to be erased, or any
        pointers or references to that node.

        @param to_erase An iterator that points to the node to be erased.
        @return An iterator that points to the node after the one being erased.
     */
    iterator erase(const iterator& to_erase);

    /**
        Erase all nodes in the given range, from first to last. This will
        invalidate any iterators that point to the nodes to be erased, or any
        pointers or references to those nodes.

        @param first The first node in the range to be removed.
        @param last An iterator that points one past the last node to erase. Think xml::node::end().
        @return An iterator that points to the node after the last one being erased.
     */
    iterator erase(iterator first, const iterator& last);

    /**
        Erase all children nodes with the given name. This will find all
        nodes that have the given node name and remove them from this node.
        This will invalidate any iterators that point to the nodes to be
        erased, or any pointers or references to those nodes.

        @param name The name of nodes to remove.
        @return The number of nodes removed.
     */
    size_type erase(const char *name);

    /**
        Erases all children nodes.

        @since 0.7.0
     */
    void clear();

    /**
        Sort all the children nodes of this node using one of their
        attributes. Only nodes that are of xml::node::type_element will be
        sorted, and they must have the given node_name.

        The sorting is done by calling std::strcmp on the value of the given
        attribute.

        @param node_name The name of the nodes to sort.
        @param attr_name The attribute to sort on.
     */
    void sort(const char *node_name, const char *attr_name);

    /**
        Sort all the children nodes of this node using the given comparison
        function object. All element type nodes will be considered for
        sorting.

        @param compare The binary function object to call in order to sort all child nodes.
     */
    template <typename T> void sort (T compare)
        { impl::sort_callback<T> cb(compare); sort_fo(cb); }

    /**
        Convert the node and all its children into XML text and set the given
        string to that text.

        @param xml The string to set the node's XML data to.
     */
    void node_to_string(std::string& xml) const;

    /**
        Write a node and all of its children to the given stream.

        @param stream The stream to write the node as XML.
        @param n The node to write to the stream.
        @return The stream.
     */
    friend XMLWRAPP_API std::ostream& operator<< (std::ostream &stream, const node &n);

private:
    impl::node_impl *pimpl_;

    // private ctor to create uninitialized instance
    explicit node(int);

    void set_node_data(void *data);
    void* get_node_data();
    void* release_node_data();

    void sort_fo(impl::cbfo_node_compare &fo);

    friend class tree_parser;
    friend class impl::node_iterator;
    friend class document;
    friend struct impl::doc_impl;
    friend struct impl::node_cmp;
    friend class xml::const_nodes_view;
    friend struct impl::xpath_context_impl;
};

// Comparison operators for xml::node iterators

inline bool XMLWRAPP_API operator==(const node::iterator& lhs,
                                    const node::iterator& rhs)
    { return lhs.get_raw_node() == rhs.get_raw_node(); }
inline bool XMLWRAPP_API operator!=(const node::iterator& lhs,
                                    const node::iterator& rhs)
    { return !(lhs == rhs); }

inline bool XMLWRAPP_API operator==(const node::const_iterator& lhs,
                                    const node::const_iterator& rhs)
    { return lhs.get_raw_node() == rhs.get_raw_node(); }
inline bool XMLWRAPP_API operator!=(const node::const_iterator& lhs,
                                    const node::const_iterator& rhs)
    { return !(lhs == rhs); }

} // namespace xml

#endif // _xmlwrapp_node_h_
