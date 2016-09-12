/*
 * Copyright (C) 2001-2003 Peter J Jones (pjones@pmade.org)
 * Copyright (C) 2009-2010 Vaclav Slavik (vslavik@gmail.com)
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


BOOST_AUTO_TEST_SUITE( node )

/*
 * This test should make sure that we can construct a tree using nodes and
 * output the nodes using stream insertion.
 */
BOOST_AUTO_TEST_CASE( create_node )
{
    xml::node root_node("root");
    xml::node one("count", "one");
    xml::node two("count", "two");
    xml::node nested_out("outter");
    xml::node nested_in("inner");

    nested_out.push_back(nested_in);
    root_node.push_back(one);
    root_node.push_back(two);
    root_node.push_back(nested_out);

    BOOST_CHECK( is_same_as_file(root_node, "node/data/01.out") );
}


/*
 * This test checks xml::node::iterator xml::node::find (const char *name);
 */

BOOST_AUTO_TEST_CASE( find )
{
    xml::tree_parser parser(test_file_path("node/data/02.xml").c_str());

    xml::node &root = parser.get_document().get_root_node();
    xml::node::iterator i = root.find("person");

    BOOST_REQUIRE( i != root.end() );
    BOOST_CHECK( is_same_as_file(*i, "node/data/02a.out") );
}


/*
 * This test checks xml::node::const_iterator xml::node::find (const char *name) const;
 */

BOOST_AUTO_TEST_CASE( find_consts )
{
    xml::tree_parser parser(test_file_path("node/data/02.xml").c_str());

    const xml::node& root = parser.get_document().get_root_node();
    xml::node::const_iterator i = root.find("person");

    BOOST_REQUIRE( i != root.end() );
    BOOST_CHECK( is_same_as_file(*i, "node/data/02a.out") );
}


/*
 * This test checks xml::node::iterator xml::node::find (const char *name, * iterator start);
 */

BOOST_AUTO_TEST_CASE( find_with_start )
{
    xml::tree_parser parser(test_file_path("node/data/02.xml").c_str());

    xml::node &root = parser.get_document().get_root_node();
    xml::node::iterator i = root.find("person", root.begin());

    std::ostringstream ostr;

    while (i != root.end())
    {
        ostr << *i;
        i = root.find("person", ++i);
    }

    BOOST_CHECK( is_same_as_file( ostr, "node/data/02c.out") );
}


/*
 * This test checks xml::node::const_iterator xml::node::find (const char *name, * iterator start) const;
 */

BOOST_AUTO_TEST_CASE( find_with_start_const )
{
    xml::tree_parser parser(test_file_path("node/data/02.xml").c_str());

    const xml::node &root = parser.get_document().get_root_node();
    xml::node::const_iterator i = root.find("person", root.begin());

    std::ostringstream ostr;

    while (i != root.end())
    {
        ostr << *i;
        i = root.find("person", ++i);
    }

    BOOST_CHECK( is_same_as_file( ostr, "node/data/02c.out") );
}


/*
 * This test checks xml::node::elements (const char *name);
 */

BOOST_AUTO_TEST_CASE( elements )
{
    xml::tree_parser parser(test_file_path("node/data/02.xml").c_str());

    xml::node &root = parser.get_document().get_root_node();
    xml::nodes_view persons(root.elements("person"));

    BOOST_CHECK_EQUAL( persons.size(), 3u );

    std::ostringstream ostr;

    for (xml::nodes_view::const_iterator i = persons.begin(); i != persons.end(); ++i)
    {
        ostr << *i;
    }

    BOOST_CHECK( is_same_as_file( ostr, "node/data/02c.out") );
}


/*
 * This test checks xml::node::elements (const char *name) const;
 */

BOOST_AUTO_TEST_CASE( elements_const )
{
    xml::tree_parser parser(test_file_path("node/data/02.xml").c_str());

    const xml::node &root = parser.get_document().get_root_node();
    xml::const_nodes_view persons(root.elements("person"));

    BOOST_CHECK_EQUAL( persons.size(), 3u );

    std::ostringstream ostr;

    for (xml::const_nodes_view::const_iterator i = persons.begin(); i != persons.end(); ++i)
    {
        ostr << *i;
    }

    BOOST_CHECK( is_same_as_file( ostr, "node/data/02c.out") );
}


/*
 * This test checks xml::node::elements();
 */

BOOST_AUTO_TEST_CASE( elements_all )
{
    xml::tree_parser parser(test_file_path("node/data/02.xml").c_str());

    xml::node &root = parser.get_document().get_root_node();
    xml::nodes_view all(root.elements());

    std::ostringstream ostr;

    for (xml::nodes_view::const_iterator i = all.begin(); i != all.end(); ++i)
    {
        ostr << *i;
    }

    BOOST_CHECK( is_same_as_file( ostr, "node/data/02g.out") );
}


/*
 * This test checks xml::node::elements() const;
 */

BOOST_AUTO_TEST_CASE( elements_all_const )
{
    xml::tree_parser parser(test_file_path("node/data/02.xml").c_str());

    const xml::node &root = parser.get_document().get_root_node();
    xml::const_nodes_view all(root.elements());

    std::ostringstream ostr;

    for (xml::const_nodes_view::const_iterator i = all.begin(); i != all.end(); ++i)
    {
        ostr << *i;
    }

    BOOST_CHECK( is_same_as_file( ostr, "node/data/02g.out") );
}


/*
 * Tests that elements() returns empty set when it should.
 */

BOOST_AUTO_TEST_CASE( elements_empty )
{
    xml::tree_parser parser(test_file_path("node/data/02.xml").c_str());
    const xml::node &root = parser.get_document().get_root_node();

    xml::const_nodes_view view(root.elements("nonexistent"));
    BOOST_CHECK( view.empty() );
    BOOST_CHECK( view.begin() == view.end() );
}


/*
 * Tests that elements() returns empty set when it should.
 */

BOOST_AUTO_TEST_CASE( elements_empty_all )
{
    xml::tree_parser parser(test_file_path("node/data/empty.xml").c_str());
    const xml::node &root = parser.get_document().get_root_node();

    xml::const_nodes_view view(root.elements());
    BOOST_CHECK( view.empty() );
    BOOST_CHECK( view.begin() == view.end() );
}


/*
 * These tests check xml::node::replace()
 */

BOOST_AUTO_TEST_CASE( replace )
{
    xml::tree_parser parser(test_file_path("node/data/03.xml").c_str());

    xml::node &root = parser.get_document().get_root_node();
    xml::node::iterator i = root.find("to_replace", root.begin());

    BOOST_REQUIRE( i != root.end() );

    xml::node replacement("replace");
    replacement.push_back(xml::node("replace_child"));
    root.replace(i, replacement);

    BOOST_CHECK( is_same_as_file(root, "node/data/03a.out") );
}


BOOST_AUTO_TEST_CASE( replace_returned_iter )
{
    xml::tree_parser parser(test_file_path("node/data/03.xml").c_str());

    xml::node &root = parser.get_document().get_root_node();
    xml::node::iterator i = root.find("to_replace", root.begin());

    BOOST_REQUIRE( i != root.end() );

    xml::node replacement("replace");
    replacement.push_back(xml::node("replace_child"));
    i = root.replace(i, replacement);

    BOOST_REQUIRE( i != root.end() );
    BOOST_CHECK( is_same_as_file(*i, "node/data/03b.out") );
}


/*
 * These tests check xml::node::erase()
 */

BOOST_AUTO_TEST_CASE( erase )
{
    xml::tree_parser parser(test_file_path("node/data/04.xml").c_str());

    xml::node &root = parser.get_document().get_root_node();
    xml::node::iterator i = root.find("to_remove", root.begin());

    BOOST_REQUIRE( i != root.end() );

    root.erase(i);

    BOOST_CHECK( is_same_as_file(root, "node/data/04a.out") );
}


BOOST_AUTO_TEST_CASE( erase_returned_iter )
{
    xml::tree_parser parser(test_file_path("node/data/04.xml").c_str());

    xml::node &root = parser.get_document().get_root_node();
    xml::node::iterator i = root.find("to_remove", root.begin());

    BOOST_REQUIRE( i != root.end() );

    i = root.erase(i);

    BOOST_REQUIRE( i != root.end() );
    BOOST_CHECK( is_same_as_file(*i, "node/data/04b.out") );
}


BOOST_AUTO_TEST_CASE( clear )
{
    xml::tree_parser parser(test_file_path("node/data/04.xml").c_str());

    xml::node &root = parser.get_document().get_root_node();

    BOOST_REQUIRE( root.size() > 0 );

    root.clear();

    BOOST_CHECK( root.empty() );
    BOOST_CHECK( is_same_as_file(root, "node/data/04c.out") );
}


/*
 * These tests check xml::node::insert()
 */

BOOST_AUTO_TEST_CASE( insert )
{
    xml::tree_parser parser(test_file_path("node/data/05.xml").c_str());

    xml::node &root = parser.get_document().get_root_node();
    xml::node::iterator i = root.find("insert_before", root.begin());

    BOOST_REQUIRE( i != root.end() );

    root.insert(i, xml::node("inserted"));

    BOOST_CHECK( is_same_as_file(root, "node/data/05a.out") );
}


BOOST_AUTO_TEST_CASE( insert_returned_iter )
{
    xml::tree_parser parser(test_file_path("node/data/05.xml").c_str());

    xml::node &root = parser.get_document().get_root_node();
    xml::node::iterator i = root.find("insert_before", root.begin());

    BOOST_REQUIRE( i != root.end() );

    i = root.insert(i, xml::node("inserted"));

    BOOST_REQUIRE( i != root.end() );
    BOOST_CHECK( is_same_as_file(*i, "node/data/05b.out") );
}


BOOST_AUTO_TEST_CASE( insert_in_front )
{
    xml::tree_parser parser(test_file_path("node/data/05.xml").c_str());

    xml::node &root = parser.get_document().get_root_node();

    root.insert(root.begin(), xml::node("inserted"));

    BOOST_CHECK( is_same_as_file(root, "node/data/05c.out") );
}


BOOST_AUTO_TEST_CASE( insert_in_front_returned_iter )
{
    xml::tree_parser parser(test_file_path("node/data/05.xml").c_str());

    xml::node &root = parser.get_document().get_root_node();

    xml::node::iterator i = root.insert(root.begin(), xml::node("inserted"));

    BOOST_REQUIRE( i != root.end() );
    BOOST_CHECK( is_same_as_file(*i, "node/data/05d.out") );
}


/*
 * This test checks xml::node::move_under()
 */
BOOST_AUTO_TEST_CASE( node_move_under )
{
    xml::tree_parser parser(test_file_path("node/data/02.xml").c_str());
    xml::document doc(parser.get_document());

    xml::node& root = doc.get_root_node();

    xml::nodes_view::iterator i = root.elements().begin();

    // Check that moving the node under itself doesn't work.
    BOOST_CHECK_THROW(i->move_under(*i->find("name")), xml::exception);

    i->move_under(*root.find("unrelated_element"));

    BOOST_CHECK( is_same_as_file(doc, "node/data/02_after_move.out") );
}


/*
 * This test checks xml::node::empty()
 */

BOOST_AUTO_TEST_CASE( node_empty )
{
    xml::node n("root");
    BOOST_CHECK( n.empty() );

    n.push_back(xml::node("one"));
    BOOST_CHECK( !n.empty() );

    n.push_back(xml::node("two"));
    BOOST_CHECK( !n.empty() );
}


/*
 * This test checks xml::node::size()
 */

BOOST_AUTO_TEST_CASE( node_size )
{
    xml::node n("root");
    BOOST_CHECK_EQUAL( n.size(), 0u );

    n.push_back(xml::node("one"));
    BOOST_CHECK_EQUAL( n.size(), 1u );

    n.push_back(xml::node("two"));
    BOOST_CHECK_EQUAL( n.size(), 2u );
}

/*
 * This test checks xml::node::get_type()
 */

namespace
{

void do_dump_node_types(const char *xmlfile, const char *outfile)
{
    xml::init::substitute_entities(false);

    xml::tree_parser parser(test_file_path(xmlfile).c_str());
    xml::node &root = parser.get_document().get_root_node();

    std::ostringstream ostr;
    dump_node_type(ostr, root);

    BOOST_CHECK( is_same_as_file(ostr, outfile) );

    // FIXME: don't rely on the default being 'true', read the current value
    //        from xml::init
    xml::init::substitute_entities(true);
}

}

BOOST_AUTO_TEST_CASE( dump_node_types )
{
    do_dump_node_types("node/data/06a.xml", "node/data/06a.out");
    do_dump_node_types("node/data/06b.xml", "node/data/06b.out");
    do_dump_node_types("node/data/06c.xml", "node/data/06c.out");
}


/*
 * This test checks xml::node::sort()
 */

static void do_sort_by_attr(const char *xmlfile, const char *outfile)
{
    xml::init::remove_whitespace(true);

    xml::tree_parser parser(test_file_path(xmlfile).c_str());

    xml::node &root = parser.get_document().get_root_node();
    root.sort("child", "order");

    BOOST_CHECK( is_same_as_file(parser.get_document(), outfile) );

    // FIXME: don't rely on the default being 'false', read the current value
    //        from xml::init
    xml::init::remove_whitespace(false);
}

BOOST_AUTO_TEST_CASE( sort_by_attr )
{
    do_sort_by_attr("node/data/07a.xml", "node/data/07a.out");
    do_sort_by_attr("node/data/07b.xml", "node/data/07b.out");
}


/*
 * This test checks xml::node::sort_fo
 */

namespace
{

struct node_sort_cmp : public std::binary_function<xml::node, xml::node, bool>
{
    bool operator() (const xml::node &lhs, const xml::node &rhs)
    {
        if (lhs.get_type() != xml::node::type_element) return true;
        if (rhs.get_type() != xml::node::type_element) return false;
        return std::strcmp(lhs.get_name(), rhs.get_name()) < 0;
    }
};

}

BOOST_AUTO_TEST_CASE( sort_with_predicate )
{
    xml::init::remove_whitespace(true);


    xml::tree_parser parser(test_file_path("node/data/08a.xml").c_str());

    xml::node &root = parser.get_document().get_root_node();
    root.sort(node_sort_cmp());

    BOOST_CHECK( is_same_as_file(parser.get_document(), "node/data/08a.out") );

    // FIXME: don't rely on the default being 'false', read the current value
    //        from xml::init
    xml::init::remove_whitespace(false);
}


/*
 * This test checks xml::node::node(text)
 */

BOOST_AUTO_TEST_CASE( text_node )
{
    xml::node root("root");
    xml::node n(xml::node::text("some text"));
    root.push_back(n);
    BOOST_CHECK( is_same_as_file(root, "node/data/14.out") );
}


/*
 * This test checks xml::node::node(cdata)
 */

BOOST_AUTO_TEST_CASE( cdata_node )
{
    xml::node n(xml::node::cdata("This is a CDATA section"));
    BOOST_CHECK( is_same_as_file(n, "node/data/09.out") );
}


/*
 * This test checks xml::node::node(comment)
 */

BOOST_AUTO_TEST_CASE( comment_node )
{
    xml::node n(xml::node::comment("This is an XML comment"));
    BOOST_CHECK( is_same_as_file(n, "node/data/10.out") );
}


/*
 * This test checks xml::node::node(pi)
 */

BOOST_AUTO_TEST_CASE( processing_instruction )
{
    xml::node n(xml::node::pi("xslt", "stylesheet=\"test.xsl\""));
    BOOST_CHECK( is_same_as_file(n, "node/data/11.out") );
}


/*
 * Test correct (expected) handling of text escaping.
 */

BOOST_AUTO_TEST_CASE( text_substitute_entities )
{
    xml::tree_parser parser(test_file_path("node/data/special_chars.xml").c_str());
    xml::node &root = parser.get_document().get_root_node();

    const std::string content = root.get_content();

    BOOST_CHECK_EQUAL( content, "Marlow & Sons" );
}


BOOST_AUTO_TEST_CASE( escape_node_with_content )
{
    xml::node n("root", "Marlow & Sons");

    BOOST_CHECK_EQUAL( n.get_content(), "Marlow & Sons" );
    BOOST_CHECK( is_same_as_file(n, "node/data/special_chars.xml") );
}


BOOST_AUTO_TEST_CASE( escape_text_node )
{
    xml::node n("root");
    n.push_back(xml::node(xml::node::text("Marlow & Sons")));

    BOOST_CHECK_EQUAL( n.get_content(), "Marlow & Sons" );
    BOOST_CHECK( is_same_as_file(n, "node/data/special_chars.xml") );
}


BOOST_AUTO_TEST_CASE( escape_set_content )
{
    xml::node n("root");
    // Note that set_content() takes CDATA content as argument, so "&"
    // has to be written as "&amp;".
    n.set_content("Marlow &amp; Sons");

    BOOST_CHECK_EQUAL( n.get_content(), "Marlow & Sons" );
    BOOST_CHECK( is_same_as_file(n, "node/data/special_chars.xml") );
}


BOOST_AUTO_TEST_CASE( escape_set_text_content )
{
    xml::node n("root");
    n.set_text_content("Marlow & Sons");

    BOOST_CHECK_EQUAL( n.get_content(), "Marlow & Sons" );
    BOOST_CHECK( is_same_as_file(n, "node/data/special_chars.xml") );
}


BOOST_AUTO_TEST_CASE( compare_node_iterators )
{
    xml::node n("root");
    xml::node::iterator i = n.begin();
    xml::node::const_iterator ci = n.begin();

    BOOST_CHECK( i == i );
    BOOST_CHECK( !(i != i) );
    BOOST_CHECK( ci == ci );
    BOOST_CHECK( !(ci != ci) );
    BOOST_CHECK( i == ci );
    BOOST_CHECK( !(i != ci) );
    BOOST_CHECK( ci == i );
    BOOST_CHECK( !(ci != i) );
}


BOOST_AUTO_TEST_CASE( compare_nodes_view_iterators )
{
    xml::node n("root");
    xml::nodes_view view(n.elements("foo"));
    xml::const_nodes_view cview(n.elements("foo"));

    xml::nodes_view::iterator i = view.begin();
    xml::nodes_view::const_iterator ci = view.begin();

    xml::const_nodes_view::iterator i2 = cview.begin();
    xml::const_nodes_view::const_iterator ci2 = cview.begin();

    BOOST_CHECK( i == i );
    BOOST_CHECK( !(i != i) );
    BOOST_CHECK( ci == ci );
    BOOST_CHECK( !(ci != ci) );
    BOOST_CHECK( i == ci );
    BOOST_CHECK( !(i != ci) );
    BOOST_CHECK( ci == i );
    BOOST_CHECK( !(ci != i) );

    BOOST_CHECK( i2 == i2 );
    BOOST_CHECK( !(i2 != i2) );
    BOOST_CHECK( ci2 == ci2 );
    BOOST_CHECK( !(ci2 != ci2) );
    BOOST_CHECK( i2 == ci2 );
    BOOST_CHECK( !(i2 != ci2) );
    BOOST_CHECK( ci2 == i2 );
    BOOST_CHECK( !(ci2 != i2) );

    BOOST_CHECK( i2 == i );
    BOOST_CHECK( !(i2 != i) );
    BOOST_CHECK( ci2 == ci );
    BOOST_CHECK( !(ci2 != ci) );
    BOOST_CHECK( i2 == ci );
    BOOST_CHECK( !(i2 != ci) );
    BOOST_CHECK( ci2 == i );
    BOOST_CHECK( !(ci2 != i) );

    BOOST_CHECK( i == i2 );
    BOOST_CHECK( !(i != i2) );
    BOOST_CHECK( ci == ci2 );
    BOOST_CHECK( !(ci != ci2) );
    BOOST_CHECK( i == ci2 );
    BOOST_CHECK( !(i != ci2) );
    BOOST_CHECK( ci == i2 );
    BOOST_CHECK( !(ci != i2) );
}


BOOST_AUTO_TEST_CASE( get_namespace )
{
    xml::tree_parser parser(test_file_path("node/data/namespace.xml").c_str());

    BOOST_CHECK_EQUAL( parser.get_document().get_root_node().get_namespace(),
                       "http://pmade.org/namespace/test" );
}

BOOST_AUTO_TEST_CASE( set_namespace )
{
    xml::tree_parser parser(test_file_path("node/data/namespace.xml").c_str());
    xml::document doc(parser.get_document());

    doc.get_root_node().set_namespace("http://pmade.org/namespace/newOne");

    BOOST_CHECK( is_same_as_file(doc, "node/data/namespace.out") );
}


BOOST_AUTO_TEST_SUITE_END()
