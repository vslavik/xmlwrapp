/*
 * Copyright (C) 2011 Jonas Weber <mail@jonasw.de>
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

#include "../test.h"

#include <functional>


BOOST_AUTO_TEST_SUITE( namespaces )

BOOST_AUTO_TEST_CASE ( read_file_get_namespace_definitions )
{
    xml::tree_parser parser(test_file_path("namespaces/data/01.xml").c_str());
    
    xml::document& doc = parser.get_document();
    xml::node& root = doc.get_root_node();

    xml::namespaces::definitions& nsdefs = root.get_namespace_definitions();
    BOOST_CHECK( !nsdefs.empty() );

    xml::namespaces::definitions::iterator it = nsdefs.begin();
    
    BOOST_CHECK( (std::string(it->get_prefix()) == std::string("a")) && (std::string(it->get_href()) == std::string("href_a")) );
}

BOOST_AUTO_TEST_CASE ( read_file_get_namespace_definitions_iterate )
{
    xml::tree_parser parser(test_file_path("namespaces/data/02.xml").c_str());
    xml::node& root = parser.get_document().get_root_node();

    xml::namespaces::definitions& nsdefs = root.get_namespace_definitions();

    xml::namespaces::definitions::iterator it = nsdefs.begin();

    BOOST_CHECK( (std::string(it->get_prefix()) == std::string("a")) && (std::string(it->get_href()) == std::string("href_a")) );

    xml::namespaces::definitions::iterator it2 = it;
    BOOST_CHECK ( it2 == it++ );
    
    BOOST_CHECK( (std::string(it->get_prefix()) == std::string("b")) && (std::string(it->get_href()) == std::string("href_b")) );

    it2 = it;
    BOOST_CHECK ( it2 != ++it );
    
    BOOST_CHECK ( it == nsdefs.end() );
}

BOOST_AUTO_TEST_CASE ( nsdef_getters )
{
    xml::namespaces::ns mynsdef ("href", "prefix");

    BOOST_CHECK ( std::string(mynsdef.get_href()) == "href" && std::string(mynsdef.get_prefix()) == "prefix" );
}

BOOST_AUTO_TEST_CASE ( read_file_add_namespace_definition )
{
    xml::tree_parser parser(test_file_path("namespaces/data/03.xml").c_str());
    xml::node& root = parser.get_document().get_root_node();

    xml::namespaces::definitions& nsdefs = root.get_namespace_definitions();

    nsdefs.push_back (xml::namespaces::ns("href", "prefix"));

    std::ostringstream ostr;
    ostr << parser.get_document();

    BOOST_CHECK( is_same_as_file (ostr, "namespaces/data/03.out") );
}

BOOST_AUTO_TEST_CASE ( find_namespaces )
{
    xml::tree_parser parser(test_file_path("namespaces/data/04.xml").c_str());
    xml::node& root = parser.get_document().get_root_node();

    xml::node::iterator child_it = root.find("child");
    BOOST_CHECK (child_it != root.end());
    
    xml::node& child = *child_it;

    xml::namespaces::definitions& nsdefs = child.get_namespace_definitions();
    xml::namespaces::definitions::iterator res = nsdefs.find("p1");
    BOOST_CHECK ( std::string(res->get_href()) == std::string("href_1"));

    res = nsdefs.findHref("href_2");
    BOOST_CHECK ( std::string(res->get_prefix()) == std::string("p2"));

    res = nsdefs.find("pXXX");
    BOOST_CHECK ( res == nsdefs.end() );

    res = nsdefs.findHref("href_XXX");
    BOOST_CHECK ( res == nsdefs.end() );
}

BOOST_AUTO_TEST_CASE ( check_empty )
{
    xml::tree_parser parser(test_file_path("namespaces/data/04.xml").c_str());
    xml::node& root = parser.get_document().get_root_node();

    xml::node::iterator child_it = root.find("child");
    xml::node& child = *child_it;

    BOOST_CHECK( !root.get_namespace_definitions().empty() );
    BOOST_CHECK ( child.get_namespace_definitions().empty() );

}

BOOST_AUTO_TEST_CASE ( set_namespaces )
{
    xml::tree_parser parser(test_file_path("namespaces/data/04.xml").c_str());
    xml::node& root = parser.get_document().get_root_node();
    xml::node::iterator child_it = root.find("child");
    xml::node& child = *child_it;
    
    child.set_namespace("p1");

    std::ostringstream ostr1;
    ostr1 << parser.get_document();

    BOOST_CHECK( is_same_as_file (ostr1, "namespaces/data/04_01.out"));

    child.set_namespace_href("href_2");
    
    std::ostringstream ostr2;
    ostr2 << parser.get_document();
    BOOST_CHECK ( is_same_as_file (ostr2, "namespaces/data/04_02.out"));


    child.set_namespace(xml::namespaces::ns("href_1", "p1"));
    std::ostringstream ostr3;
    ostr3 << parser.get_document();
    BOOST_CHECK ( is_same_as_file (ostr3, "namespaces/data/04_01.out"));

}

BOOST_AUTO_TEST_CASE ( get_namespaces )
{
    xml::tree_parser parser(test_file_path("namespaces/data/05.xml").c_str());
    xml::node& root = parser.get_document().get_root_node();
    
    xml::namespaces::ns ns = root.get_namespace_o();
    
    BOOST_CHECK (std::string("p1") == ns.get_prefix() && std::string("href1") == ns.get_href());
}

BOOST_AUTO_TEST_SUITE_END()
