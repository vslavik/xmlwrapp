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
 * This file contains the definition of the xml::tree_parser class.
**/

#ifndef _xmlwrapp_tree_parser_h_
#define _xmlwrapp_tree_parser_h_

// standard includes
#include <cstddef>
#include <string>

namespace xml {

// forward declarations
struct tree_impl;
class document;

/**
 * The xml::tree_parser class is used to parse an XML document and generate
 * a tree like structure of xml::node objects. After constructing a
 * tree_parser, with either a file to parse or some in memory data to parse,
 * you can walk the tree using the xml::node interface.
**/
class tree_parser {
public:
    typedef std::size_t size_type;

    //####################################################################
    /** 
     * xml::tree_parser class constructor. Given the name of a file, this
     * constructor will parse that file.
     *
     * There are two options for dealing with XML parsing errors. The
     * default it to throw an exception (std::runtime_error). The other
     * option is to pass false for the allow_exceptions flag. This will
     * prevent an exception from being thrown, instead, a flag will be set
     * that you can test with the operator! member function.
     *
     * No matter what option you choose, this constructor may still throw
     * exceptions for memory failure or other non-parsing related failures.
     *
     * @param filename The name of the file to parse.
     * @param allow_exceptions Whether or not you want an exception for parsing errors.
     * @author Peter Jones
    **/
    //####################################################################
    tree_parser (const char *filename, bool allow_exceptions=true);

    //####################################################################
    /** 
     * xml::tree_parser class constructor. Given some data and the size of
     * that data, parse that data as XML. To see if the data was parsed
     * successfully use operator!.
     *
     * @param data The XML data to parse.
     * @param size The size of the XML data to parse.
     * @param allow_exceptions Whether or not you want an exception for parsing errors.
     * @author Peter Jones
    **/
    //####################################################################
    tree_parser (const char *data, size_type size, bool allow_exceptions=true);

    //####################################################################
    /** 
     * xml::tree_parser class destructor.
     *
     * @author Peter Jones
    **/
    //####################################################################
    ~tree_parser (void);

    //####################################################################
    /** 
     * Check to see if a xml::tree_parser class is vaild. That is, check to
     * see if parsing XML data was successful and the tree_parser holds a
     * good XML node tree.
     *
     * @return True if the tree_parser is NOT VAILD; false if it is vaild.
     * @author Peter Jones
    **/
    //####################################################################
    bool operator! (void) const;

    //####################################################################
    /** 
     * If operator! indicates that there was an error parsing your XML data,
     * you can use this member function to get the error message that was
     * generated durring parsing.
     *
     * @return The error message generated durring XML parsing.
     * @author Peter Jones
    **/
    //####################################################################
    const std::string& get_error_message (void) const;

    //####################################################################
    /** 
     * Check to see if there were any warnings from the parser while
     * processing the given XML data. If there were, you may want to send
     * the same document through xmllint or the event_parser to catch and
     * review the warning messages.
     *
     * @return True if there were any warnings.
     * @return False if there were no warnings.
     * @author Peter Jones
    **/
    //####################################################################
    bool had_warnings (void) const;

    //####################################################################
    /** 
     * Get a reference to the xml::document that was generated during the
     * XML parsing. You should make sure to only use a reference to the
     * document to avoid a deep copy.
     *
     * @return A reference to the xml::document.
     * @author Peter Jones
    **/
    //####################################################################
    xml::document& get_document (void);

    //####################################################################
    /** 
     * Get a const reference to the xml::document that was generate during
     * the XML parsing. You should make sure to only use a reference to the
     * document to avoid a deep copy.
     *
     * @return A const reference to the xml::document.
     * @author Peter Jones
    **/
    //####################################################################
    const xml::document& get_document (void) const;
private:
    tree_impl *pimpl_; // private implementation

    /* 
     * Don't allow anyone to copy construct a xml::tree_parser or allow the
     * assignment operator to be called. It is not very useful to copy a
     * parser that has already parsed half a document.
     */
    tree_parser (const tree_parser&);
    tree_parser& operator= (const tree_parser&);
}; // end xml::tree_parser class

} // end xml namespace
#endif
