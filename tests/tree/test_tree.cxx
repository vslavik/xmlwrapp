/*
 * Copyright (C) 2001-2003 Peter J Jones (pjones@pmade.org)
 * Copyright (C) 2009      Vaclav Slavik (vslavik@gmail.com)
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

namespace
{

const std::string XMLDATA_GOOD =
    "<root><a><b><c><d><e><f><g/></f></e></d></c></b></a></root>";
const std::string XMLDATA_BAD =
    "root><a><b></c></d></root>";

}

inline bool is_parser_valid(const xml::tree_parser& parser)
{
    // test overloaded tree_parser::operator!()
    return !!parser;
}

/*
 * tests that a good XML is parsed correctly.
 */

TEST_CASE_METHOD( SrcdirConfig, "tree/good_xml_1", "[tree]" )
{
    xml::tree_parser parser(test_file_path("tree/data/good.xml").c_str());
    CHECK( is_parser_valid(parser) );
}


/*
 * tests that a good XML is parsed correctly.
 */

TEST_CASE_METHOD( SrcdirConfig, "tree/good_xml_2", "[tree]" )
{
    xml::tree_parser parser(test_file_path("tree/data/good.xml").c_str(), false);
    CHECK( is_parser_valid(parser) );
}


/*
 * this test expects xml data as the first argument, it also expectes good
 * XML data.
 */

TEST_CASE_METHOD( SrcdirConfig, "tree/good_xml_data_1", "[tree]" )
{
    xml::tree_parser parser(XMLDATA_GOOD.c_str(), XMLDATA_GOOD.size());
    CHECK( is_parser_valid(parser) );
}

TEST_CASE_METHOD( SrcdirConfig, "tree/good_xml_data_2", "[tree]" )
{
    xml::tree_parser parser(XMLDATA_GOOD.c_str(), XMLDATA_GOOD.size(), false);
    CHECK( is_parser_valid(parser) );
}


/*
 * this test should be passed a bad XML file and it should fail without an
 * exception!
 */

TEST_CASE_METHOD( SrcdirConfig, "tree/bad_xml_no_throw", "[tree]" )
{
    xml::tree_parser parser(test_file_path("tree/data/bad.xml").c_str(), false);
    CHECK( !parser ); // failed
}


/*
 * this test should be passed a bad XML file and it should catch an
 * exception.
 */

TEST_CASE_METHOD( SrcdirConfig, "tree/bad_xml_throw", "[tree]" )
{
    CHECK_THROWS_AS
    (
        xml::tree_parser(test_file_path("tree/data/bad.xml").c_str()),
        xml::exception&
    );
}


/*
 * this test should be passed a bad XML data and it should fail without an
 * exception!
 */

TEST_CASE_METHOD( SrcdirConfig, "tree/bad_xml_data_no_throw", "[tree]" )
{
    xml::tree_parser parser(XMLDATA_BAD.c_str(), XMLDATA_BAD.size(), false);
    CHECK( !parser ); // failed
}


/*
 * this test should be passed a bad XML data and it should catch an
 * exception.
 */

TEST_CASE_METHOD( SrcdirConfig, "tree/bad_xml_data_throw", "[tree]" )
{
    CHECK_THROWS_AS
    (
        xml::tree_parser(XMLDATA_BAD.c_str(), XMLDATA_BAD.size()),
        xml::exception&
    );
}


// test reporting of nonexistent files
TEST_CASE_METHOD( SrcdirConfig, "tree/nonexistent_file", "[tree]" )
{
    xml::tree_parser parser("doesnt_exist.xml", false);
    CHECK( parser.messages().print().find("doesnt_exist.xml") != std::string::npos );
    CHECK( !parser ); // failed
}

TEST_CASE_METHOD( SrcdirConfig, "tree/nonexistent_file_throw", "[tree]" )
{
    CHECK_THROWS_AS
    (
        xml::tree_parser("doesnt_exist.xml"),
        xml::exception&
    );
}


/*
 * this test should print out an outline of the input xml doc
 */

namespace
{

void dump_node(std::ostream& s, const xml::node &n, int space_counter = 0)
{
    if ( n.is_text() )
        return;

    for ( int space = 0; space < space_counter; ++space )
        s << " ";
    s << n.get_name() << std::endl;

    xml::node::const_iterator i(n.begin()), end(n.end());
    for (; i != end; ++i)
        dump_node(s, *i, space_counter + 4);
}

} // anonymous namespace

TEST_CASE_METHOD( SrcdirConfig, "tree/dump_tree", "[tree]" )
{
    xml::tree_parser parser(test_file_path("tree/data/good.xml").c_str());
    CHECK( is_parser_valid(parser) );

    std::ostringstream ostr;
    dump_node(ostr, parser.get_document().get_root_node());

    CHECK( is_same_as_file(ostr, "tree/data/output") );
}

TEST_CASE_METHOD( SrcdirConfig, "tree/dump_tree_data", "[tree]" )
{
    xml::tree_parser parser(XMLDATA_GOOD.c_str(), XMLDATA_GOOD.length());
    CHECK( is_parser_valid(parser) );

    std::ostringstream ostr;
    dump_node(ostr, parser.get_document().get_root_node());

    CHECK( is_same_as_file(ostr, "tree/data/output") );
}



namespace
{

// this is invalid markup, libxml2 fails with
// namespace error : Namespace prefix a on book is not defined
const std::string XMLDATA_BAD_NS =
      "<a:book> xmlns:a=\"a\" <title>title</title></a:book>";

} // anonymous namespace

TEST_CASE_METHOD( SrcdirConfig, "tree/bad_ns_xml_data_throw", "[tree]" )
{

    CHECK_THROWS_AS
    (
        xml::tree_parser( XMLDATA_BAD_NS.c_str(), XMLDATA_BAD_NS.size()),
        xml::exception&
    );
}

TEST_CASE_METHOD( SrcdirConfig, "tree/bad_ns_xml_data_no_throw", "[tree]" )
{
    xml::tree_parser parser(XMLDATA_BAD_NS.c_str(), XMLDATA_BAD_NS.size(), false);
    CHECK( !parser ); // failed
}
