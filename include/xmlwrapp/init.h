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
 * This file contains the definition of the xml::init class.
**/

#ifndef _xmlwrapp_init_h_
#define _xmlwrapp_init_h_

namespace xml {

/**
 * The xml::init class is used to initialize the XML parser. For thread
 * safety it should be instantiated one time in the main thread before any
 * other threads use xmlwrapp. Non-threaded programs should instantiate a
 * xml::init class before using xmlwrapp as well, at least for
 * consistency.
 *
 * If you want to use and of the xml::init member functions, do so before
 * you start any threads or use any other part of xmlwrapp. The member
 * functions may alter global and/or static variables. In other words, this
 * class is not thread safe.
**/
class init {
public:
    //####################################################################
    /** 
     * Create a new xml::init object. This constructor will prepare the XML
     * parser and set some default values for the parsers global variables.
     *
     * @author Peter Jones
    **/
    //####################################################################
    init (void);

    //####################################################################
    /** 
     * Clean up the XML parser. Don't let the xml::init object go out of
     * scope before you are done using the xmlwrapp library!
     *
     * @author Peter Jones
    **/
    //####################################################################
    virtual ~init (void);

    //####################################################################
    /** 
     * This member function controls whether or not the XML parser should
     * add text nodes for indenting when generating XML text output from a
     * node tree. The default, set in the xml::init constructor, is true.
     *
     * @param flag True to turn on indenting, false to turn it off.
     * @author Peter Jones
    **/
    //####################################################################
    void indent_output (bool flag);

    //####################################################################
    /** 
     * This member function controls whether or not the XML parser should
     * remove ignorable whitespace around XML elements. The default, set in
     * the xml::init constructor, is false.
     *
     * @param flag True to remove whitespace, false to leave alone.
     * @author Peter Jones
    **/
    //####################################################################
    void remove_whitespace (bool flag);

    //####################################################################
    /** 
     * This member function controls whether or not the XML parser should
     * substitute entities while parsing. The default, set in the xml::init
     * constructor, is true.
     *
     * @param flag True to turn on substitution, false to turn off.
     * @author Peter Jones
    **/
    //####################################################################
    void substitute_entities (bool flag);

    //####################################################################
    /** 
     * This member function controls whether or not the XML parser should
     * load external (DTD) subsets while parsing. This will only affect the
     * loading of the subsets, it does not cause files to be validated. The
     * default, set in the xml::init constructor, is true.
     *
     * @param flag True to turn on loading, false to turn it off.
     * @author Peter Jones
    **/
    //####################################################################
    void load_external_subsets (bool flag);

    //####################################################################
    /** 
     * This member function controls whether or not the XML parser should
     * validate every XML document that is parses with its DTD. The default,
     * set in the xml::init constructor, is false.
     *
     * @return flag True to turn on validation, false to turn it off.
     * @author Peter Jones
    **/
    //####################################################################
    void validate_xml (bool flag);

private:
    init (const init&);
    init& operator= (const init&);
}; // end xml::init class
    
} // end xml namespace
#endif
