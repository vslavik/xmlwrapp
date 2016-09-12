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

#include <boost/iostreams/filtering_stream.hpp>

// Allow disabling the test using zlib if it's not available.
// Also never compile this test with Sun CC as it fails to compile
// gzip_decompressor() anyhow.
#if !defined(XMLWRAPP_NO_ZLIB) && !defined(__SUNPRO_CC)
    #define XMLWRAPP_USE_ZLIB
#endif

#ifdef XMLWRAPP_USE_ZLIB
    #include <boost/iostreams/filter/gzip.hpp>
#endif

BOOST_AUTO_TEST_SUITE( document )

/*
 * This test checks xml::document iteration.
 */

BOOST_AUTO_TEST_CASE( dump_type )
{
    xml::init::substitute_entities(false);

    xml::tree_parser parser(test_file_path("document/data/01.xml").c_str());

    std::ostringstream ostr;
    xml::node::iterator i = parser.get_document().begin(),
                        end = parser.get_document().end();
    for (; i!=end; ++i)
        dump_node_type(ostr, *i);

    BOOST_CHECK( is_same_as_file(ostr, "document/data/01.out") );
}


/*
 * This test checks xml::document default constructor.
 */

BOOST_AUTO_TEST_CASE( default_ctor )
{
    xml::document doc;
    BOOST_CHECK( is_same_as_file( doc, "document/data/02.out") );
}


/*
 * This test checks xml::document constructor that takes the name of the root
 * node.
 */

BOOST_AUTO_TEST_CASE( ctor_root_name )
{
    xml::document doc(xml::node("root"));
    BOOST_CHECK( is_same_as_file( doc, "document/data/03.out") );
}


/*
 * This test checks xml::document constructor that takes a node.
 */

BOOST_AUTO_TEST_CASE( ctor_root_node )
{
    xml::node n("root", "pcdata");
    xml::document doc(n);
    BOOST_CHECK( is_same_as_file( doc, "document/data/04.out") );
}


/*
 * This test checks xml::document copy constructor.
 */

BOOST_AUTO_TEST_CASE( copy_ctor )
{
    xml::node n("root", "pcdata");
    xml::document doc(n);

    xml::document doc_copy(doc);

    BOOST_CHECK( is_same_as_file( doc_copy, "document/data/04.out") );
}


/*
 * This test checks xml::document assignment operator.
 */

BOOST_AUTO_TEST_CASE( assignment_operator )
{
    xml::node n("root", "pcdata");
    xml::document doc(n);

    xml::document doc_copy;
    doc_copy = doc;

    BOOST_CHECK( is_same_as_file( doc_copy, "document/data/04.out") );
}


/*
 * This test checks xml::document::get_root_node.
 */

BOOST_AUTO_TEST_CASE( get_root_node )
{
    xml::node n("root", "pcdata");
    xml::document doc(n);

    BOOST_CHECK( is_same_as_file( doc.get_root_node(), "document/data/04.out") );
}


/*
 * This test checks xml::document::set_root_node().
 */

BOOST_AUTO_TEST_CASE( set_root_node )
{
    std::ostringstream ostr;

    xml::node n("root", "pcdata");

    xml::document doc;
    ostr << doc; // blank document

    doc.set_root_node(n);
    ostr << doc;

    BOOST_CHECK( is_same_as_file( ostr, "document/data/08.out") );
}


/*
 * This test checks xml::document::get_version().
 */

BOOST_AUTO_TEST_CASE( get_version )
{
    xml::tree_parser parser(test_file_path("document/data/09.xml").c_str());

    BOOST_CHECK_EQUAL( parser.get_document().get_version(), "1.1" );
}


/*
 * This test checks xml::document::set_version().
 */

BOOST_AUTO_TEST_CASE( set_version )
{
    xml::document doc(xml::node("root"));
    doc.set_version("1.1");

    BOOST_CHECK( is_same_as_file( doc, "document/data/10.out") );
}


/*
 * This test checks xml::document::get_encoding().
 */

BOOST_AUTO_TEST_CASE( get_encoding )
{
    xml::tree_parser parser(test_file_path("document/data/11.xml").c_str());

    BOOST_CHECK_EQUAL( parser.get_document().get_encoding(), "UTF-8" );
}


/*
 * This test checks xml::document::set_encoding().
 */

BOOST_AUTO_TEST_CASE( set_encoding )
{
    xml::document doc(xml::node("root"));
    doc.set_encoding("UTF-8");

    BOOST_CHECK( is_same_as_file( doc, "document/data/12.out") );
}


/*
 * This test checks xml::document::get_is_standalone().
 */

BOOST_AUTO_TEST_CASE( get_is_standalone )
{
    xml::tree_parser parser1(test_file_path("document/data/13a.xml").c_str());
    BOOST_CHECK_EQUAL( parser1.get_document().get_is_standalone(), false );

    xml::tree_parser parser2(test_file_path("document/data/13b.xml").c_str());
    BOOST_CHECK_EQUAL( parser2.get_document().get_is_standalone(), true );
}


/*
 * This test checks xml::document::set_is_standalone().
 */

BOOST_AUTO_TEST_CASE( set_is_standalone )
{
    xml::document doc1(xml::node("root"));
    doc1.set_is_standalone(true);
    BOOST_CHECK( is_same_as_file( doc1, "document/data/13a.out") );

    xml::document doc2(xml::node("root"));
    doc2.set_is_standalone(false);
    BOOST_CHECK( is_same_as_file( doc2, "document/data/13b.out") );
}


/*
 * This test checks xml::document::process_xinclude()
 */
BOOST_AUTO_TEST_CASE( process_xinclude )
{
    xml::tree_parser parser(test_file_path("document/data/14.xml").c_str());

    BOOST_CHECK( parser.get_document().process_xinclude() );
    BOOST_CHECK( is_same_as_file( parser.get_document(), "document/data/14.out") );
}


/*
 * This test checks xml::document::size()
 */

BOOST_AUTO_TEST_CASE( size )
{
    xml::document doc_01(xml::node("root"));
    BOOST_CHECK_EQUAL( doc_01.size(), 1u );

    doc_01.push_back(xml::node(xml::node::comment("This is a comment")));
    BOOST_CHECK_EQUAL( doc_01.size(), 2u );

    xml::document doc_02(doc_01);
    BOOST_CHECK_EQUAL( doc_02.size(), 2u );

    xml::document doc_03;
    BOOST_CHECK_EQUAL( doc_03.size(), 1u );

    xml::node n("root");
    xml::document doc_04(n);
    BOOST_CHECK_EQUAL( doc_04.size(), 1u );
}


/*
 * This test checks xml::document::push_back and insert
 */

BOOST_AUTO_TEST_CASE( push_back_and_insert )
{
    xml::document doc(xml::node("root"));

    doc.push_back(xml::node(xml::node::comment(" Comment From push_back ")));

    xml::node::iterator n(doc.insert(xml::node(xml::node::comment("This Will Be Changed"))));
    n->set_content(" Comment From insert ");

    n = doc.insert(doc.begin(), xml::node(xml::node::pi("test")));
    n->set_content("one=\"1\"");

    BOOST_CHECK( is_same_as_file( doc, "document/data/17.out") );
}


/*
 * This test checks xml::document::push_back and insert to make sure they
 * throw exceptions
 */

BOOST_AUTO_TEST_CASE( push_back_and_insert_throw )
{
    xml::document doc(xml::node("root"));

    BOOST_CHECK_THROW
    (
        doc.push_back(xml::node("noway")),
        xml::exception
    );

    BOOST_CHECK_THROW
    (
        doc.insert(xml::node("noway")),
        xml::exception
    );

    BOOST_CHECK_THROW
    (
        doc.insert(doc.end(), xml::node("noway")),
        xml::exception
    );
}


/*
 * This test checks xml::document::replace()
 */

BOOST_AUTO_TEST_CASE( replace )
{
    xml::document doc(xml::node("root"));

    xml::node::iterator n(doc.insert(xml::node(xml::node::comment(" To Be Replaced "))));
    doc.replace(n, xml::node(xml::node::comment(" This is the replacement comment ")));

    BOOST_CHECK( is_same_as_file( doc, "document/data/19.out") );
}


/*
 * This test checks xml::document::replace() to make sure it throws exceptions
 */

BOOST_AUTO_TEST_CASE( replace_throw )
{
    xml::document doc(xml::node("root"));
    xml::node::iterator n(doc.insert(xml::node(xml::node::comment(" To Be Replaced "))));

    BOOST_CHECK_THROW
    (
        doc.replace(n, xml::node("noway")),
        xml::exception
    );

    BOOST_CHECK_THROW
    (
        doc.replace(doc.begin(), xml::node(xml::node::comment(" no way "))),
        xml::exception
    );
}


/*
 * This test checks xml::document::erase().
 */

BOOST_AUTO_TEST_CASE( erase )
{
    xml::document doc(xml::node("root"));
    doc.push_back(xml::node(xml::node::comment(" Comment from push_back ")));

    xml::node::iterator n(doc.insert(xml::node(xml::node::comment(" You should not see me "))));
    doc.erase(n);

    BOOST_CHECK( is_same_as_file(doc, "document/data/21.out") );
}

/*
 * This test checks xml::document::erase() to make sure it throws an
 * exception.
 */

BOOST_AUTO_TEST_CASE( cant_erase_root )
{
    xml::document doc(xml::node("root"));
    doc.push_back(xml::node(xml::node::comment(" Comment from push_back ")));

    BOOST_CHECK_THROW
    (
        doc.erase(doc.begin(), doc.end()),
        xml::exception
    );
}


// Simple RAII helper to remove a temporary test file.
class temp_test_file
{
public:
    temp_test_file() :
        used_(false)
    {
    }

    ~temp_test_file()
    {
        if (used_)
            remove(get_name());
    }

    const char *get_name()
    {
        used_ = true;
        return "test_temp_file";
    }

private:
    bool used_;

    temp_test_file(const temp_test_file&);
    temp_test_file& operator=(const temp_test_file&);
};

/*
 * These tests check xml::docment::save_to_file()
 */

BOOST_AUTO_TEST_CASE( save_to_file )
{
    xml::document doc(xml::node("root"));
    doc.get_root_node().push_back(xml::node("child"));

    temp_test_file test_file;
    doc.save_to_file(test_file.get_name());

    std::ifstream stream(test_file.get_name());
    BOOST_CHECK( is_same_as_file(read_file_into_string(stream), "document/data/15.out") );
}


BOOST_AUTO_TEST_CASE( save_throws_on_failure )
{
    xml::document doc(xml::node("root"));
    doc.get_root_node().push_back(xml::node(xml::node::text("invalid character: \x7")));

    std::string s;
    BOOST_CHECK_THROW
    (
        doc.save_to_string(s),
        xml::exception
    );

    temp_test_file test_file;
    BOOST_CHECK_THROW
    (
        doc.save_to_file(test_file.get_name()),
        xml::exception
    );
}


#ifdef XMLWRAPP_USE_ZLIB
BOOST_AUTO_TEST_CASE( save_to_file_gzip )
{
    xml::document doc(xml::node("root"));
    doc.get_root_node().push_back(xml::node("child"));

    temp_test_file test_file;
    doc.save_to_file(test_file.get_name(), 9);

    // verify that the file was can be read back as compressed
    std::ifstream stream(test_file.get_name());
    boost::iostreams::filtering_stream<boost::iostreams::input> filter;
    filter.push(boost::iostreams::gzip_decompressor());
    filter.push(stream);
    BOOST_CHECK( is_same_as_file(read_file_into_string(filter), "document/data/15.out") );

    // ...and by libxml2 directly too
    xml::tree_parser parser(test_file.get_name());
}
#endif // XMLWRAPP_USE_ZLIB


BOOST_AUTO_TEST_SUITE_END()
