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
 * This test checks xml::node::get_type()
 */

#include <xmlwrapp/xmlwrapp.h>
#include <iostream>
#include <exception>

void dump_node_type (const xml::node &n, int indent=0);

int main (int argc, char *argv[]) {
    if (argc != 2) {
	std::cerr << "Usage: " << argv[0] << " xmlfile\n";
	return 1;
    }

    try {
	xml::init xml; xml.substitute_entities(false);
	xml::tree_parser parser(argv[1]);

	xml::node &root = parser.get_document().get_root_node();
	dump_node_type(root);
    } catch (const std::exception &e) {
	std::cout << e.what() << std::endl;
	return 1;
    }

    return 0;
}

void dump_node_type (const xml::node &n, int indent) {
    for (int j=0; j<indent; ++j) std::cout << " ";

    switch (n.get_type()) {
	case xml::node::type_element:
	    std::cout << "type_element\n";
	    break;

	case xml::node::type_text:
	    std::cout << "type_text\n";
	    break;

	case xml::node::type_cdata:
	    std::cout << "type_cdata\n";
	    break;

	case xml::node::type_pi:
	    std::cout << "type_pi\n";
	    break;

	case xml::node::type_comment:
	    std::cout << "type_comment\n";
	    break;

	case xml::node::type_entity:
	    std::cout << "type_entity\n";
	    break;

	case xml::node::type_entity_ref:
	    std::cout << "type_entity_ref\n";
	    break;

	case xml::node::type_xinclude:
	    std::cout << "type_xinclude\n";
	    break;

	case xml::node::type_document:
	    std::cout << "type_document\n";
	    break;

	case xml::node::type_document_type:
	    std::cout << "type_document_type\n";
	    break;

	case xml::node::type_document_frag:
	    std::cout << "type_document_frag\n";
	    break;

	case xml::node::type_notation:
	    std::cout << "type_notation\n";
	    break;

	case xml::node::type_dtd:
	    std::cout << "type_dtd\n";
	    break;

	case xml::node::type_dtd_element:
	    std::cout << "type_dtd_element\n";
	    break;

	case xml::node::type_dtd_attribute:
	    std::cout << "type_dtd_attribute\n";
	    break;

	case xml::node::type_dtd_entity:
	    std::cout << "type_dtd_entity\n";
	    break;

	case xml::node::type_dtd_namespace:
	    std::cout << "type_dtd_namespace\n";
	    break;
    }

    xml::node::const_iterator i=n.begin(), end=n.end();
    for (; i!=end; ++i) dump_node_type(*i, indent+4);
}
