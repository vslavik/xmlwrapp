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


BOOST_AUTO_TEST_SUITE( tree )

/*
 * tests that a good XML is parsed correctly.
 */

BOOST_AUTO_TEST_CASE( good_xml_1 )
{
    xml::tree_parser parser(test_file_path("tree/data/good.xml").c_str());
    BOOST_CHECK( parser );
}


/*
 * tests that a good XML is parsed correctly.
 */

BOOST_AUTO_TEST_CASE( good_xml_2 )
{
    xml::tree_parser parser(test_file_path("tree/data/good.xml").c_str(), false);
    BOOST_CHECK( parser );
}


/*
 * this test expects xml data as the first argument, it also expectes good
 * XML data.
 */

BOOST_AUTO_TEST_CASE( good_xml_data_1 )
{
    xml::tree_parser parser(XMLDATA_GOOD.c_str(), XMLDATA_GOOD.size());
    BOOST_CHECK( parser );
}

BOOST_AUTO_TEST_CASE( good_xml_data_2 )
{
    xml::tree_parser parser(XMLDATA_GOOD.c_str(), XMLDATA_GOOD.size(), false);
    BOOST_CHECK( parser );
}


/*
 * this test should be passed a bad XML file and it should fail without an
 * exception!
 */

BOOST_AUTO_TEST_CASE( bad_xml_no_throw )
{
    xml::tree_parser parser(test_file_path("tree/data/bad.xml").c_str(), false);
    BOOST_CHECK( !parser ); // failed
}


/*
 * this test should be passed a bad XML file and it should catch an
 * exception.
 */

BOOST_AUTO_TEST_CASE( bad_xml_throw )
{
    BOOST_CHECK_THROW
    (
        xml::tree_parser parser(test_file_path("tree/data/bad.xml").c_str()),
        xml::exception
    );
}


/*
 * this test should be passed a bad XML data and it should fail without an
 * exception!
 */

BOOST_AUTO_TEST_CASE( bad_xml_data_no_throw )
{
    xml::tree_parser parser(XMLDATA_BAD.c_str(), XMLDATA_BAD.size(), false);
    BOOST_CHECK( !parser ); // failed
}


/*
 * this test should be passed a bad XML data and it should catch an
 * exception.
 */

BOOST_AUTO_TEST_CASE( bad_xml_data_throw )
{
    BOOST_CHECK_THROW
    (
        xml::tree_parser parser(XMLDATA_BAD.c_str(), XMLDATA_BAD.size()),
        xml::exception
    );
}


// test reporting of nonexistent files
BOOST_AUTO_TEST_CASE( nonexistent_file )
{
    xml::tree_parser parser("doesnt_exist.xml", false);
    BOOST_CHECK( parser.messages().print().find("doesnt_exist.xml") != std::string::npos );
    BOOST_CHECK( !parser ); // failed
}

BOOST_AUTO_TEST_CASE( nonexistent_file_throw )
{
    BOOST_CHECK_THROW
    (
        xml::tree_parser parser("doesnt_exist.xml"),
        xml::exception
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

BOOST_AUTO_TEST_CASE( dump_tree )
{
    xml::tree_parser parser(test_file_path("tree/data/good.xml").c_str());
    BOOST_CHECK( parser );

    std::ostringstream ostr;
    dump_node(ostr, parser.get_document().get_root_node());

    BOOST_CHECK( is_same_as_file(ostr, "tree/data/output") );
}

BOOST_AUTO_TEST_CASE( dump_tree_data )
{
    xml::tree_parser parser(XMLDATA_GOOD.c_str(), XMLDATA_GOOD.length());
    BOOST_CHECK( parser );

    std::ostringstream ostr;
    dump_node(ostr, parser.get_document().get_root_node());

    BOOST_CHECK( is_same_as_file(ostr, "tree/data/output") );
}



namespace
{

// this is invalid markup, libxml2 fails with
// namespace error : Namespace prefix a on book is not defined
const std::string XMLDATA_BAD_NS =
      "<a:book> xmlns:a=\"a\" <title>title</title></a:book>";

} // anonymous namespace

BOOST_AUTO_TEST_CASE( bad_ns_xml_data_throw )
{

    BOOST_CHECK_THROW
    (
        xml::tree_parser parser( XMLDATA_BAD_NS.c_str(), XMLDATA_BAD_NS.size()),
        xml::exception
    );
}

BOOST_AUTO_TEST_CASE( bad_ns_xml_data_no_throw )
{
    xml::tree_parser parser(XMLDATA_BAD_NS.c_str(), XMLDATA_BAD_NS.size(), false);
    BOOST_CHECK( !parser ); // failed
}

BOOST_AUTO_TEST_SUITE_END()
