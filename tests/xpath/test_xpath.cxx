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


TEST_CASE_METHOD( SrcdirConfig, "xpath/create_context", "[xpath]" )
{
    xml::tree_parser parser(test_file_path("xpath/data/01.xml").c_str());
    xml::xpath_context ctxt(parser.get_document());
}

TEST_CASE_METHOD( SrcdirConfig, "xpath/evaluate", "[xpath]" )
{
    xml::tree_parser parser(test_file_path("xpath/data/01.xml").c_str());
    xml::xpath_context ctxt(parser.get_document());
    xml::const_nodes_view ns = ctxt.evaluate("//child");

    CHECK( !ns.empty() );
}

TEST_CASE_METHOD( SrcdirConfig, "xpath/evaluate_with_ns", "[xpath]" )
{
    xml::tree_parser parser(test_file_path("xpath/data/02.xml").c_str());
    xml::xpath_context ctxt(parser.get_document());
    
    xml::const_nodes_view ns1 = ctxt.evaluate("//child");
    CHECK( ns1.empty() );

    ctxt.register_namespace("p", "href");
    xml::const_nodes_view ns2 = ctxt.evaluate("//p:child");
    CHECK( !ns2.empty() );
    CHECK( std::distance(ns2.begin(), ns2.end()) == 3 );
}

TEST_CASE_METHOD( SrcdirConfig, "xpath/node_set_iterators", "[xpath]" )
{
    xml::tree_parser parser(test_file_path("xpath/data/02.xml").c_str());
    xml::xpath_context ctxt(parser.get_document());
    ctxt.register_namespace("p", "href");
    xml::const_nodes_view ns = ctxt.evaluate("//p:child");

    CHECK( ns.begin() == ns.begin() );
    REQUIRE( !ns.empty() );
    CHECK( ns.begin() != ns.end() );

    xml::const_nodes_view::iterator it = ns.begin();
    CHECK( it++ == ns.begin() );
    CHECK( it != ns.begin() );
    it = ns.begin();
    CHECK( ++it != ns.begin() );


    for (auto const& n : ns)
    {
        CHECK( n.get_name() );
    }
}

TEST_CASE_METHOD( SrcdirConfig, "xpath/node_set_contains", "[xpath]" )
{
    xml::tree_parser parser(test_file_path("xpath/data/02.xml").c_str());
    xml::xpath_context ctxt(parser.get_document());

    ctxt.register_namespace("p", "href");

    xml::const_nodes_view ns = ctxt.evaluate("//p:child");
    CHECK( std::distance(ns.begin(), ns.end()) == 3 );
}

TEST_CASE_METHOD( SrcdirConfig, "xpath/illegal_xpath", "[xpath]" )
{
    xml::tree_parser parser(test_file_path("xpath/data/02.xml").c_str());
    xml::xpath_context ctxt(parser.get_document());

    xml::const_nodes_view ns = ctxt.evaluate("ILLEGAL XPATH-QUERY", xml::ignore_errors);
    CHECK( ns.begin() == ns.end() );

    CHECK_THROWS_AS
    (
        ctxt.evaluate("ANOTHER ILLEGAL QUERY"),
        xml::exception&
    );
}
