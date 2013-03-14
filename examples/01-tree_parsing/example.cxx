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
 * This file demonstrates how to use the xml::tree_parser class to parse XML
 * data and create a tree of xml::node objects. It also shows how you can
 * walk the tree using xml::node iterators.
 */

// xmlwrapp include
#include <xmlwrapp/xmlwrapp.h>

// standard includes
#include <iostream>
#include <exception>

int main (int argc, char *argv[]) {
    if (argc != 2) {
	std::cerr << argv[0] << ": you must give one and only one XML file name\n";
	return 1;
    }

    try {

	xml::tree_parser parser(argv[1]);

	xml::node &root = parser.get_document().get_root_node();
	std::cout << "root node is '" << root.get_name() << "'\n";

	xml::namespaces::definitions& nsdefs = root.get_namespace_definitions();
	std::cout << "EMPTY: " << nsdefs.empty() << std::endl;
	for (xml::namespaces::iterator it = nsdefs.begin(); it != nsdefs.end(); ++it)
	{
		std::cout << "ns: " << it->get_href() << " " << it->get_prefix() << std::endl;
	}

	nsdefs.push_back (xml::namespaces::ns ("href", "prefix"));

	xml::node::iterator child(root.begin()), child_end(root.end());
	for (; child != child_end; ++child) {
	    if (child->is_text()) continue;
	    std::cout << "child node '" << child->get_name() << "'\n";
            std::cout << "child node: find prefix=p. href=" << child->get_namespace_definitions().find("p")->get_href() << std::endl;
            std::cout << "assigning namespace: " << std::endl; child->set_namespace("prefix");
	}

	std::cout << root << std::endl;


    } catch (std::exception &e) {
	std::cerr << argv[0] << ": " << e.what() << "\n";
	return 1;
    }

    return 0;
}
