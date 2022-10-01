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

/*
 * The following code demonstrates how to use the xml::node class to build
 * an XML tree and then convert it to XML text.
 *
 * Here is what we want to create:
 *
 *  <abook>
 *	<person id="01" name="Peter Jones">
 *	    <email>pjones@pmade.org</email>
 *	    <!-- Fake Phone Number -->
 *	    <phone type="home">555-1212</phone>
 *	</person>
 *  </abook>
 */

// xmlwrapp include
#include <xmlwrapp/xmlwrapp.h>

// standard includes
#include <iostream>
#include <exception>

int main () {
    // create a new XML document and set the root node
    xml::document xmldoc(xml::node("abook"));
    xml::node &root = xmldoc.get_root_node();

    // add a child to the root node, <person>
    xml::node::iterator it = root.insert(root.begin(), xml::node("person"));

    /*
     * set the attributes for this new <person> element using the
     * xml::attributes object returned from xml::node::get_attributes
     */
    it->get_attributes().insert("id", "01");
    it->get_attributes().insert("name", "Peter Jones");

    // add a node and set the content for that new node
    it->push_back(xml::node("email", "pjones@pmade.org"));

    // add an XML comment
    it->push_back(xml::node(xml::node::comment(" Fake Phone Number ")));

    // build a node one member function at a time
    it = it->insert(xml::node("phone"));
    it->get_attributes().insert("type", "home");
    it->set_content("555-1212");

    // set some document settings
    xmldoc.set_is_standalone(true);
    xmldoc.set_encoding("ISO-8859-1");

    // convert the document to XML
    std::cout << xmldoc;
    return 0;
}
