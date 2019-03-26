/*
 * Copyright (C) 2001-2003 Peter J Jones (pjones@pmade.org)
 * Copyright (C) 2013 Vaclav Slavik <vslavik@gmail.com>
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

    This file contains the definition of the xml::document class.
 */

#ifndef _xmlwrapp_document_h_
#define _xmlwrapp_document_h_

// xmlwrapp includes
#include "xmlwrapp/init.h"
#include "xmlwrapp/node.h"
#include "xmlwrapp/export.h"
#include "xmlwrapp/errors.h"

// standard includes
#include <iosfwd>
#include <string>
#include <cstddef>

// forward declaration
namespace xslt
{

class stylesheet;
namespace impl
{
class result;
}

} // end xslt namespace

namespace xml
{

// forward declarations
class relaxng;
class schema;
class tree_parser;

namespace impl
{
struct doc_impl;
struct xpath_context_impl;
}

/**
    The xml::document class is used to hold the XML tree and various bits of
    information about it.
 */
class XMLWRAPP_API document
{
public:
    /// size type
    typedef std::size_t size_type;

    /**
        Create a new XML document with the default settings. The new document
        will contain a root node with a name of "blank".
     */
    document();

    /**
        Create a new XML document and set the name of the root element to the
        given text.

        @param root_name What to set the name of the root element to.

        @deprecated Use `xml::document(xml::node(root_name))` constructor instead.
     */
    XMLWRAPP_DEPRECATED("use xml::document(xml::node(root_name)) instead")
    explicit document(const char *root_name);

    /**
        Create a new XML document and set the root node.

        @param n The node to use as the root node. n will be copied.
     */
    explicit document(const node& n);

    /**
        Load XML document from given file.

        Errors are handled by @a on_error handler; if you pass
        xml::throw_on_error, xml::exception is thrown on errors. If there's a
        fatal error that prevents the document from being loaded and the error
        handler doesn't throw an exception, the constructor will throw
        xml::exception anyway.

        @param filename The name of the file to parse.
        @param on_error Handler called to process errors and warnings.

        @since 0.7.0
     */
    explicit document(const char *filename, error_handler& on_error);

    /**
        Load XML document from given data.

        Errors are handled by @a on_error handler; by default, xml::exception
        is thrown on errors. If there's a fatal error that prevents the document
        from being loaded and the error handler doesn't throw an exception, the
        constructor will throw xml::exception anyway.

        @param data The XML data to parse.
        @param len The length of the XML data to parse.
        @param on_error Handler called to process errors and warnings.

        @since 0.7.0
     */
    explicit document(const char *data, size_type len, error_handler& on_error = throw_on_error);

    /**
        Copy construct a new XML document. The new document will be an exact
        copy of the original.

        @param other The other document object to copy from.
     */
    document(const document& other);

    /**
        Copy another document object into this one using the assignment
        operator. This document object will be an exact copy of the other
        document after the assignment.

        @param other The document to copy from.
        @return *this.
     */
    document& operator=(const document& other);

    /**
        Swap one xml::document object for another.

        @param other The other document to swap
     */
    void swap(document& other);

    /**
        Clean up after an XML document object.
     */
    ~document();

    /**
        Get a reference to the root node of this document. If no root node
        has been set, the returned node will be a blank node. You should take
        caution to use a reference so that you don't copy the whole node
        tree!

        @return A const reference to the root node.
     */
    const node& get_root_node() const;

    /**
        Get a reference to the root node of this document. If no root node
        has been set, the returned node will be a blank node. You should take
        caution to use a reference so that you don't copy the whole node
        tree!

        @return A reference to the root node.
     */
    node& get_root_node();

    /**
        Set the root node to the given node. A full copy is made and stored
        in the document object.

        @param n The new root node to use.
     */
    void set_root_node(const node& n);

    /**
        Get the XML version for this document. For generated documents, the
        version will be the default. For parsed documents, this will be the
        version from the XML processing instruction.

        @return The XML version string for this document.
     */
    const std::string& get_version() const;

    /**
        Set the XML version number for this document. This version string
        will be used when generating the XML output.

        @param version The version string to use, like "1.0".
     */
    void set_version(const char *version);

    /**
        Get the XML encoding for this document. The default encoding is
        UTF-8.

        @return The encoding string.
     */
    std::string get_encoding() const;

    /**
        Set the XML encoding string. If you don't set this, it will default
        to UTF-8.

        Note that all strings in the XML document must be encoded in the
        document encoding.

        @param encoding The XML encoding to use.
     */
    void set_encoding(const char *encoding);

    /**
        Find out if the current document is a standalone document. For
        generated documents, this will be the default. For parsed documents
        this will be set based on the XML processing instruction.

        @return True if this document is standalone.
        @return False if this document is not standalone.
     */
    bool get_is_standalone() const;

    /**
        Set the standalone flag. This will show up in the XML output in the
        correct processing instruction.

        @param sa What to set the standalone flag to.
     */
    void set_is_standalone(bool sa);

    /**
        Walk through the document and expand <xi:include> elements. For more
        information, please see the w3c recommendation for XInclude.
        http://www.w3.org/2001/XInclude.

        The return value of this function may change to int after a bug has
        been fixed in libxml2 (xmlXIncludeDoProcess).

        @return False if there was an error with substitutions.
        @return True if there were no errors (with or without substitutions).
     */
    bool process_xinclude();

    /**
        Test to see if this document has an internal subset. That is, DTD
        data that is declared within the XML document itself.

        @return True if this document has an internal subset.
        @return False otherwise.
     */
    bool has_internal_subset() const;

    /**
        Test to see if this document has an external subset. That is, it
        references a DTD from an external source, such as a file or URL.

        @return True if this document has an external subset.
        @return False otherwise.
     */
    bool has_external_subset() const;

    /**
        Validate this document against the DTD that has been attached to it.
        This would happen at parse time if there was a !DOCTYPE definition.
        If the DTD is valid, and the document is valid, this member function
        will return true.

        If it returns false, you may want to send the document through
        xmllint to get the actual error messages.

        @return True if the document is valid.
        @return False if there was a problem with the DTD or XML doc.
     */
    bool validate();

    /**
        Parse the given DTD and try to validate this document against it. If
        the DTD is valid, and the document is valid, this member function
        will return true.

        If it returns false, you may want to send the document through
        xmllint to get the actual error messages.

        This member function will add the parsed DTD to this document as the
        external subset after the validation. If there is already an external
        DTD attached to this document it will be removed and deleted.

        @param dtdname A filename or URL for the DTD to use.
        @return True if the document is valid.
        @return False if there was a problem with the DTD or XML doc.
     */
    bool validate(const char *dtdname);

    /**
        Returns the number of child nodes of this document. This will always
        be at least one, since all xmlwrapp documents must have a root node.
        This member function is useful to find out how many document children
        there are, including processing instructions, comments, etc.

        @return The number of children nodes that this document has.
     */
    size_type size() const;

    /**
        Get an iterator to the first child node of this document. If what you
        really wanted was the root node (the first element) you should use
        the get_root_node() member function instead.

        @return A xml::node::iterator that points to the first child node.
        @return An end iterator if there are no children in this document
     */
    node::iterator begin();

    /**
        Get a const_iterator to the first child node of this document. If
        what you really wanted was the root node (the first element) you
        should use the get_root_node() member function instead.

        @return A xml::node::const_iterator that points to the first child node.
        @return An end const_iterator if there are no children in this document.
     */
    node::const_iterator begin() const;

    /**
        Get an iterator that points one past the last child node for this
        document.

        @return An end xml::node::iterator.
     */
    node::iterator end();

    /**
        Get a const_iterator that points one past the last child node for
        this document.

        @return An end xml::node::const_iterator.
     */
    node::const_iterator end() const;

    /**
        Add a child xml::node to this document. You should not add a element
        type node, since there can only be one root node. This member
        function is only useful for adding processing instructions, comments,
        etc.. If you do try to add a node of type element, an exception will
        be thrown.

        @param child The child xml::node to add.
     */
    void push_back (const node &child);

    /**
        Insert a new child node. The new node will be inserted at the end of
        the child list. This is similar to the xml::node::push_back member
        function except that an iterator to the inserted node is returned.

        The rules from the push_back member function apply here. Don't add a
        node of type element.

        @param n The node to insert as a child of this document.
        @return An iterator that points to the newly inserted node.
        @see xml::document::push_back
     */
    node::iterator insert (const node &n);

    /**
        Insert a new child node. The new node will be inserted before the
        node pointed to by the given iterator.

        The rules from the push_back member function apply here. Don't add a
        node of type element.

        @param position An iterator that points to the location where the new node should be inserted (before it).
        @param n The node to insert as a child of this document.
        @return An iterator that points to the newly inserted node.
        @see xml::document::push_back
     */
    node::iterator insert(node::iterator position, const node &n);

    /**
        Replace the node pointed to by the given iterator with another node.
        The old node will be removed, including all its children, and
        replaced with the new node. This will invalidate any iterators that
        point to the node to be replaced, or any pointers or references to
        that node.

        Do not replace this root node with this member function. The same
        rules that apply to push_back apply here. If you try to replace a
        node of type element, an exception will be thrown.

        @param old_node An iterator that points to the node that should be removed.
        @param new_node The node to put in old_node's place.
        @return An iterator that points to the new node.
        @see xml::document::push_back
     */
    node::iterator replace(node::iterator old_node, const node& new_node);

    /**
        Erase the node that is pointed to by the given iterator. The node
        and all its children will be removed from this node. This will
        invalidate any iterators that point to the node to be erased, or any
        pointers or references to that node.

        Do not remove the root node using this member function. The same
        rules that apply to push_back apply here. If you try to erase the
        root node, an exception will be thrown.

        @param to_erase An iterator that points to the node to be erased.
        @return An iterator that points to the node after the one being erased.
        @see xml::document::push_back
     */
    node::iterator erase(node::iterator to_erase);

    /**
        Erase all nodes in the given range, from first to last. This will
        invalidate any iterators that point to the nodes to be erased, or any
        pointers or references to those nodes.

        Do not remove the root node using this member function. The same
        rules that apply to push_back apply here. If you try to erase the
        root node, an exception will be thrown.

        @param first The first node in the range to be removed.
        @param last An iterator that points one past the last node to erase. Think xml::node::end().
        @return An iterator that points to the node after the last one being erased.
        @see xml::document::push_back
     */
    node::iterator erase(node::iterator first, node::iterator last);

    /**
        Convert the XML document tree into XML text data and place it into
        the given string.

        Any errors occurring while converting the document to string are passed
        to @a on_error handler. By default, an exception will be thrown if
        anything goes wrong.

        @param s The string to place the XML text data.
        @param on_error Handler called to process errors and warnings (new
            since 0.8.0).
     */
    void save_to_string(std::string& s, error_handler& on_error = throw_on_error) const;

    /**
        Convert the XML document tree into XML text data and place it into
        the given filename.

        This function throws an exception if saving fails for any reason by
        default and allows to customize this behaviour by passing a non-default
        @a on_error handler.

        @param filename The name of the file to place the XML text data into.
        @param on_error Handler called to process errors and warnings (new
            since 0.8.0).
        @param compression_level 0 is no compression, 1-9 allowed, where 1 is
                                 for better speed, and 9 is for smaller size
        @return True if the data was saved successfully.
        @return False otherwise (notice that this is only possible if a custom
            error handler not throwing on error is specified).
     */
    bool save_to_file(const char *filename,
                      int compression_level = 0,
                      error_handler& on_error = throw_on_error) const;

    /**
        Convert the XML document tree into XML text data and then insert it
        into the given stream.

        @param stream The stream to insert the XML into.
        @param doc The document to insert.
        @return The stream from the first parameter.
     */
    friend XMLWRAPP_API std::ostream& operator<< (std::ostream &stream, const document &doc);

private:
    impl::doc_impl *pimpl_;

    void set_doc_data (void *data);
    void set_doc_data_from_xslt (void *data, xslt::impl::result *xr);
    void* get_doc_data();
    void* get_doc_data_read_only() const;
    void* release_doc_data();

    friend class tree_parser;
    friend class relaxng;
    friend class schema;
    friend class xslt::stylesheet;
    friend struct impl::xpath_context_impl;
};

} // namespace xml

#endif // _xmlwrapp_document_h_
