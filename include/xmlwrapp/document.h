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
 * This file contains the definition of the xml::document class.
**/

#ifndef _xmlwrapp_document_h_
#define _xmlwrapp_document_h_

// xmlwrapp includes
#include <xmlwrapp/node.h>

// standard includes
#include <iosfwd>
#include <string>

// forward declaration
namespace xslt { class stylesheet; }
namespace xpath { class xpath_helper; }

namespace xml {

// forward declarations
class tree_parser;
struct doc_impl;

/**
 * The xml::document class is used to hold the XML tree and various bits of
 * information about it.
**/
class document {
public:
    //####################################################################
    /** 
     * Create a new XML document with the default settings. The new document
     * will contain a root node with a name of "blank".
     *
     * @author Peter Jones
    **/
    //####################################################################
    document (void);

    //####################################################################
    /** 
     * Create a new XML document and set the name of the root element to the
     * given text.
     *
     * @param root_name What to set the name of the root element to.
     * @author Peter Jones
    **/
    //####################################################################
    explicit document (const char *root_name);

    //####################################################################
    /** 
     * Create a new XML document and set the root node.
     *
     * @param n The node to use as the root node. n will be copied.
     * @author Peter Jones
    **/
    //####################################################################
    explicit document (const node &n);

    //####################################################################
    /** 
     * Copy construct a new XML document. The new document will be an exact
     * copy of the original.
     *
     * @param other The other document object to copy from.
     * @author Peter Jones
    **/
    //####################################################################
    document (const document &other);

    //####################################################################
    /** 
     * Copy another document object into this one using the assignment
     * operator. This document object will be an exact copy of the other
     * document after the assignement.
     *
     * @param other The document to copy from.
     * @return *this.
     * @author Peter Jones
    **/
    //####################################################################
    document& operator= (const document &other);

    //####################################################################
    /** 
     * Swap one xml::document object for another.
     *
     * @param other The other document to swap
     * @author Peter Jones
    **/
    //####################################################################
    void swap (document &other);

    //####################################################################
    /** 
     * Clean up after an XML document object.
     *
     * @author Peter Jones
    **/
    //####################################################################
    ~document (void);

    //####################################################################
    /** 
     * Get a reference to the root node of this document. If no root node
     * has been set, the returned node will be a blank node. You should take
     * caution to use a reference so that you don't copy the whole node
     * tree!
     *
     * @return A const reference to the root node.
     * @author Peter Jones
    **/
    //####################################################################
    const node& get_root_node (void) const;

    //####################################################################
    /** 
     * Get a reference to the root node of this document. If no root node
     * has been set, the returned node will be a blank node. You should take
     * caution to use a reference so that you don't copy the whole node
     * tree!
     *
     * @return A reference to the root node.
     * @author Peter Jones
    **/
    //####################################################################
    node& get_root_node (void);

    //####################################################################
    /** 
     * Set the root node to the given node. A full copy is made and stored
     * in the document object.
     *
     * @param n The new root node to use.
     * @author Peter Jones
    **/
    //####################################################################
    void set_root_node (const node &n);

    //####################################################################
    /** 
     * Get the XML version for this document. For generated documents, the
     * version will be the default. For parsed documents, this will be the
     * version from the XML processing instruction.
     *
     * @return The XML version string for this document.
     * @author Peter Jones
    **/
    //####################################################################
    const std::string& get_version (void) const;

    //####################################################################
    /** 
     * Set the XML version number for this document. This version string
     * will be used when generating the XML output.
     *
     * @param version The version string to use, like "1.0".
     * @author Peter Jones
    **/
    //####################################################################
    void set_version (const char *version);

    //####################################################################
    /** 
     * Get the XML encoding for this document. The default encoding is
     * ISO-8859-1.
     *
     * @return The encoding string.
     * @author Peter Jones
    **/
    //####################################################################
    const std::string& get_encoding (void) const;

    //####################################################################
    /** 
     * Set the XML encoding string. If you don't set this, it will default
     * to ISO-8859-1.
     *
     * @param encoding The XML encoding to use.
     * @author Peter Jones
     * @author Dmitriy Nikitinskiy
    **/
    //####################################################################
    void set_encoding (const char *encoding);

    //####################################################################
    /** 
     * Find out if the current document is a standalone document. For
     * generated documents, this will be the default. For parsed documents
     * this will be set based on the XML processing instruction.
     *
     * @return True if this document is standalone.
     * @return False if this document is not standalone.
     * @author Peter Jones
    **/
    //####################################################################
    bool get_is_standalone (void) const;

    //####################################################################
    /** 
     * Set the standalone flag. This will show up in the XML output in the
     * correct processing instruction.
     *
     * @param sa What to set the standalone flag to.
     * @author Peter Jones
    **/
    //####################################################################
    void set_is_standalone (bool sa);

    //####################################################################
    /** 
     * Walk through the document and expand <xi:include> elements. For more
     * information, please see the w3c recomendation for XInclude.
     * http://www.w3.org/2001/XInclude.
     *
     * The return value of this function may change to int after a bug has
     * been fixed in libxml2 (xmlXIncludeDoProcess).
     *
     * @return False if there was an error with substitutions.
     * @return True if there were no errors (with or without substitutions).
     * @author Peter Jones
     * @author Daniel Evison
    **/
    //####################################################################
    bool process_xinclude (void);

    //####################################################################
    /** 
     * Test to see if this document has an internal subset. That is, DTD
     * data that is declared within the XML document itself.
     *
     * @return True if this document has an internal subset.
     * @return False otherwise.
     * @author Peter Jones
    **/
    //####################################################################
    bool has_internal_subset (void) const;

    //####################################################################
    /** 
     * Test to see if this document has an external subset. That is, it
     * references a DTD from an external source, such as a file or URL.
     *
     * @return True if this document has an external subset.
     * @return False otherwise.
     * @author Peter Jones
    **/
    //####################################################################
    bool has_external_subset (void) const;

    //####################################################################
    /** 
     * Validate this document against the DTD that has been attached to it.
     * This would happen at parse time if there was a !DOCTYPE definition.
     * If the DTD is valid, and the document is valid, this member function
     * will return true.
     *
     * If it returns false, you may want to send the document through
     * xmllint to get the actual error messages.
     *
     * @return True if the document is valid.
     * @return False if there was a problem with the DTD or XML doc.
     * @author Peter Jones
    **/
    //####################################################################
    bool validate (void);

    //####################################################################
    /** 
     * Parse the given DTD and try to validate this document against it. If
     * the DTD is valid, and the document is valid, this member function
     * will return true.
     *
     * If it returns false, you may want to send the document through
     * xmllint to get the actual error messages.
     *
     * This member function will add the parsed DTD to this document as the
     * external subset after the validation. If there is already an external
     * DTD attached to this document it will be removed and deleted.
     *
     * @param dtdname A filename or URL for the DTD to use.
     * @return True if the document is valid.
     * @return False if there was a problem with the DTD or XML doc.
     * @author Peter Jones
    **/
    //####################################################################
    bool validate (const char *dtdname);

    //####################################################################
    /** 
     * Get an iterator to the first child node of this document. If what you
     * really wanted was the root node (the first element) you should use
     * the get_root_node() member function instead.
     *
     * @return A xml::node::iterator that points to the first child node.
     * @return An end iterator if there are no children in this document
     * @author Peter Jones
    **/
    //####################################################################
    node::iterator begin (void);

    //####################################################################
    /** 
     * Get a const_iterator to the first child node of this document. If
     * what you really wanted was the root node (the first element) you
     * should use the get_root_node() member function instead.
     *
     * @return A xml::node::const_iterator that points to the first child node.
     * @return An end const_iterator if there are no children in this document.
     * @author Peter Jones
    **/
    //####################################################################
    node::const_iterator begin (void) const;

    //####################################################################
    /** 
     * Get an iterator that points one past the last child node for this
     * document.
     *
     * @return An end xml::node::iterator.
     * @author Peter Jones
    **/
    //####################################################################
    node::iterator end (void);

    //####################################################################
    /** 
     * Get a const_iterator that points one past the last child node for
     * this document.
     *
     * @return An end xml::node::const_iterator.
     * @author Peter Jones
    **/
    //####################################################################
    node::const_iterator end (void) const;

    //####################################################################
    /** 
     * Convert the XML document tree into XML text data and place it into
     * the given string.
     *
     * @param s The string to place the XML text data.
     * @author Peter Jones
    **/
    //####################################################################
    void save_to_string (std::string &s) const;

    //####################################################################
    /** 
     * Convert the XML document tree into XML text data and place it into
     * the given filename.
     *
     * @param filename The name of the file to place the XML text data into.
     * @return True if the data was saved successfully.
     * @return False otherwise.
     * @author Peter Jones
    **/
    //####################################################################
    bool save_to_file (const char *filename) const;

    //####################################################################
    /** 
     * Convert the XML document tree into XML text data and then insert it
     * into the given stream.
     *
     * @param stream The stream to insert the XML into.
     * @param doc The document to insert.
     * @return The stream from the first parameter.
     * @author Peter Jones
    **/
    //####################################################################
    friend std::ostream& operator<< (std::ostream &stream, const document &doc);

private:
    doc_impl *pimpl_;
    void set_doc_data (void *data);
    void set_doc_data_from_xslt (void *data, void *ss);
    void* get_doc_data (void) const;
    void* release_doc_data (void);

    friend class tree_parser;
    friend class xslt::stylesheet;
  friend class xpath::xpath_helper;
}; // end xml::document class

} // end xml namespace
#endif
