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


/*
 * This test should make sure that we can construct a tree using nodes and
 * output the nodes using stream insertion.
 */
TEST_CASE_METHOD( SrcdirConfig, "node/create_node", "[node]" )
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

    CHECK( is_same_as_file(root_node, "node/data/01.out") );
}


/*
 * This test checks xml::node::iterator xml::node::find (const char *name);
 */

TEST_CASE_METHOD( SrcdirConfig, "node/find", "[node]" )
{
    xml::tree_parser parser(test_file_path("node/data/02.xml").c_str());

    xml::node &root = parser.get_document().get_root_node();
    xml::node::iterator i = root.find("person");

    REQUIRE( i != root.end() );
    CHECK( is_same_as_file(*i, "node/data/02a.out") );
}


/*
 * This test checks xml::node::const_iterator xml::node::find (const char *name) const;
 */

TEST_CASE_METHOD( SrcdirConfig, "node/find_consts", "[node]" )
{
    xml::tree_parser parser(test_file_path("node/data/02.xml").c_str());

    const xml::node& root = parser.get_document().get_root_node();
    xml::node::const_iterator i = root.find("person");

    REQUIRE( i != root.end() );
    CHECK( is_same_as_file(*i, "node/data/02a.out") );
}


/*
 * This test checks xml::node::iterator xml::node::find (const char *name, * iterator start);
 */

TEST_CASE_METHOD( SrcdirConfig, "node/find_with_start", "[node]" )
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

    CHECK( is_same_as_file( ostr, "node/data/02c.out") );
}


/*
 * This test checks xml::node::const_iterator xml::node::find (const char *name, * iterator start) const;
 */

TEST_CASE_METHOD( SrcdirConfig, "node/find_with_start_const", "[node]" )
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

    CHECK( is_same_as_file( ostr, "node/data/02c.out") );
}


/*
 * This test checks xml::node::elements (const char *name);
 */

TEST_CASE_METHOD( SrcdirConfig, "node/elements", "[node]" )
{
    xml::tree_parser parser(test_file_path("node/data/02.xml").c_str());

    xml::node &root = parser.get_document().get_root_node();
    xml::nodes_view persons(root.elements("person"));

    CHECK( persons.size() == 3u );

    std::ostringstream ostr;

    for (xml::nodes_view::const_iterator i = persons.begin(); i != persons.end(); ++i)
    {
        ostr << *i;
    }

    CHECK( is_same_as_file( ostr, "node/data/02c.out") );
}


/*
 * This test checks xml::node::elements (const char *name) const;
 */

TEST_CASE_METHOD( SrcdirConfig, "node/elements_const", "[node]" )
{
    xml::tree_parser parser(test_file_path("node/data/02.xml").c_str());

    const xml::node &root = parser.get_document().get_root_node();
    xml::const_nodes_view persons(root.elements("person"));

    CHECK( persons.size() == 3u );

    std::ostringstream ostr;

    for (xml::const_nodes_view::const_iterator i = persons.begin(); i != persons.end(); ++i)
    {
        ostr << *i;
    }

    CHECK( is_same_as_file( ostr, "node/data/02c.out") );
}


/*
 * This test checks xml::node::elements();
 */

TEST_CASE_METHOD( SrcdirConfig, "node/elements_all", "[node]" )
{
    xml::tree_parser parser(test_file_path("node/data/02.xml").c_str());

    xml::node &root = parser.get_document().get_root_node();
    xml::nodes_view all(root.elements());

    std::ostringstream ostr;

    for (xml::nodes_view::const_iterator i = all.begin(); i != all.end(); ++i)
    {
        ostr << *i;
    }

    CHECK( is_same_as_file( ostr, "node/data/02g.out") );
}


/*
 * This test checks xml::node::elements() const;
 */

TEST_CASE_METHOD( SrcdirConfig, "node/elements_all_const", "[node]" )
{
    xml::tree_parser parser(test_file_path("node/data/02.xml").c_str());

    const xml::node &root = parser.get_document().get_root_node();
    xml::const_nodes_view all(root.elements());

    std::ostringstream ostr;

    for (xml::const_nodes_view::const_iterator i = all.begin(); i != all.end(); ++i)
    {
        ostr << *i;
    }

    CHECK( is_same_as_file( ostr, "node/data/02g.out") );
}


/*
 * Tests that elements() returns empty set when it should.
 */

TEST_CASE_METHOD( SrcdirConfig, "node/elements_empty", "[node]" )
{
    xml::tree_parser parser(test_file_path("node/data/02.xml").c_str());
    const xml::node &root = parser.get_document().get_root_node();

    xml::const_nodes_view view(root.elements("nonexistent"));
    CHECK( view.empty() );
    CHECK( view.begin() == view.end() );
}


/*
 * Tests that elements() returns empty set when it should.
 */

TEST_CASE_METHOD( SrcdirConfig, "node/elements_empty_all", "[node]" )
{
    xml::tree_parser parser(test_file_path("node/data/empty.xml").c_str());
    const xml::node &root = parser.get_document().get_root_node();

    xml::const_nodes_view view(root.elements());
    CHECK( view.empty() );
    CHECK( view.begin() == view.end() );
}


/*
 * These tests check xml::node::replace()
 */

TEST_CASE_METHOD( SrcdirConfig, "node/replace", "[node]" )
{
    xml::tree_parser parser(test_file_path("node/data/03.xml").c_str());

    xml::node &root = parser.get_document().get_root_node();
    xml::node::iterator i = root.find("to_replace", root.begin());

    REQUIRE( i != root.end() );

    xml::node replacement("replace");
    replacement.push_back(xml::node("replace_child"));
    root.replace(i, replacement);

    CHECK( is_same_as_file(root, "node/data/03a.out") );
}


TEST_CASE_METHOD( SrcdirConfig, "node/replace_returned_iter", "[node]" )
{
    xml::tree_parser parser(test_file_path("node/data/03.xml").c_str());

    xml::node &root = parser.get_document().get_root_node();
    xml::node::iterator i = root.find("to_replace", root.begin());

    REQUIRE( i != root.end() );

    xml::node replacement("replace");
    replacement.push_back(xml::node("replace_child"));
    i = root.replace(i, replacement);

    REQUIRE( i != root.end() );
    CHECK( is_same_as_file(*i, "node/data/03b.out") );
}


/*
 * These tests check xml::node::erase()
 */

TEST_CASE_METHOD( SrcdirConfig, "node/erase", "[node]" )
{
    xml::tree_parser parser(test_file_path("node/data/04.xml").c_str());

    xml::node &root = parser.get_document().get_root_node();
    xml::node::iterator i = root.find("to_remove", root.begin());

    REQUIRE( i != root.end() );

    root.erase(i);

    CHECK( is_same_as_file(root, "node/data/04a.out") );
}


TEST_CASE_METHOD( SrcdirConfig, "node/erase_returned_iter", "[node]" )
{
    xml::tree_parser parser(test_file_path("node/data/04.xml").c_str());

    xml::node &root = parser.get_document().get_root_node();
    xml::node::iterator i = root.find("to_remove", root.begin());

    REQUIRE( i != root.end() );

    i = root.erase(i);

    REQUIRE( i != root.end() );
    CHECK( is_same_as_file(*i, "node/data/04b.out") );
}


TEST_CASE_METHOD( SrcdirConfig, "node/clear", "[node]" )
{
    xml::tree_parser parser(test_file_path("node/data/04.xml").c_str());

    xml::node &root = parser.get_document().get_root_node();

    REQUIRE( root.size() > 0 );

    root.clear();

    CHECK( root.empty() );
    CHECK( is_same_as_file(root, "node/data/04c.out") );
}


/*
 * These tests check xml::node::insert()
 */

TEST_CASE_METHOD( SrcdirConfig, "node/insert", "[node]" )
{
    xml::tree_parser parser(test_file_path("node/data/05.xml").c_str());

    xml::node &root = parser.get_document().get_root_node();
    xml::node::iterator i = root.find("insert_before", root.begin());

    REQUIRE( i != root.end() );

    root.insert(i, xml::node("inserted"));

    CHECK( is_same_as_file(root, "node/data/05a.out") );
}


TEST_CASE_METHOD( SrcdirConfig, "node/insert_returned_iter", "[node]" )
{
    xml::tree_parser parser(test_file_path("node/data/05.xml").c_str());

    xml::node &root = parser.get_document().get_root_node();
    xml::node::iterator i = root.find("insert_before", root.begin());

    REQUIRE( i != root.end() );

    i = root.insert(i, xml::node("inserted"));

    REQUIRE( i != root.end() );
    CHECK( is_same_as_file(*i, "node/data/05b.out") );
}


TEST_CASE_METHOD( SrcdirConfig, "node/insert_in_front", "[node]" )
{
    xml::tree_parser parser(test_file_path("node/data/05.xml").c_str());

    xml::node &root = parser.get_document().get_root_node();

    root.insert(root.begin(), xml::node("inserted"));

    CHECK( is_same_as_file(root, "node/data/05c.out") );
}


TEST_CASE_METHOD( SrcdirConfig, "node/insert_in_front_returned_iter", "[node]" )
{
    xml::tree_parser parser(test_file_path("node/data/05.xml").c_str());

    xml::node &root = parser.get_document().get_root_node();

    xml::node::iterator i = root.insert(root.begin(), xml::node("inserted"));

    REQUIRE( i != root.end() );
    CHECK( is_same_as_file(*i, "node/data/05d.out") );
}


/*
 * This test checks xml::node::move_under()
 */
TEST_CASE_METHOD( SrcdirConfig, "node/node_move_under", "[node]" )
{
    xml::tree_parser parser(test_file_path("node/data/02.xml").c_str());
    xml::document doc(parser.get_document());

    xml::node& root = doc.get_root_node();

    xml::nodes_view::iterator i = root.elements().begin();

    // Check that moving the node under itself doesn't work.
    CHECK_THROWS_AS
    (
        i->move_under(*i->find("name")),
        xml::exception&
    );

    i->move_under(*root.find("unrelated_element"));

    CHECK( is_same_as_file(doc, "node/data/02_after_move.out") );
}


/*
 * This test checks xml::node::empty()
 */

TEST_CASE_METHOD( SrcdirConfig, "node/node_empty", "[node]" )
{
    xml::node n("root");
    CHECK( n.empty() );

    n.push_back(xml::node("one"));
    CHECK( !n.empty() );

    n.push_back(xml::node("two"));
    CHECK( !n.empty() );
}


/*
 * This test checks xml::node::size()
 */

TEST_CASE_METHOD( SrcdirConfig, "node/node_size", "[node]" )
{
    xml::node n("root");
    CHECK( n.size() == 0u );

    n.push_back(xml::node("one"));
    CHECK( n.size() == 1u );

    n.push_back(xml::node("two"));
    CHECK( n.size() == 2u );
}

/*
 * This test checks xml::node::get_type()
 */

namespace
{

void do_dump_node_types(const char *xmlfile, const char *outfile)
{
    xml::init::change_flag change(&xml::init::substitute_entities, false);

    xml::tree_parser parser(test_file_path(xmlfile).c_str());
    xml::node &root = parser.get_document().get_root_node();

    std::ostringstream ostr;
    dump_node_type(ostr, root);

    CHECK( is_same_as_file(ostr, outfile) );
}

}

TEST_CASE_METHOD( SrcdirConfig, "node/dump_node_types", "[node]" )
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
    xml::init::change_flag change(&xml::init::remove_whitespace, true);

    xml::tree_parser parser(test_file_path(xmlfile).c_str());

    xml::node &root = parser.get_document().get_root_node();
    root.sort("child", "order");

    CHECK( is_same_as_file(parser.get_document(), outfile) );
}

TEST_CASE_METHOD( SrcdirConfig, "node/sort_by_attr", "[node]" )
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

TEST_CASE_METHOD( SrcdirConfig, "node/sort_with_predicate", "[node]" )
{
    xml::init::change_flag change(&xml::init::remove_whitespace, true);


    xml::tree_parser parser(test_file_path("node/data/08a.xml").c_str());

    xml::node &root = parser.get_document().get_root_node();
    root.sort(node_sort_cmp());

    CHECK( is_same_as_file(parser.get_document(), "node/data/08a.out") );
}


/*
 * This test checks xml::node::node(text)
 */

TEST_CASE_METHOD( SrcdirConfig, "node/text_node", "[node]" )
{
    xml::node root("root");
    xml::node n(xml::node::text("some text"));
    root.push_back(n);
    CHECK( is_same_as_file(root, "node/data/14.out") );
}


/*
 * This test checks xml::node::node(cdata)
 */

TEST_CASE_METHOD( SrcdirConfig, "node/cdata_node", "[node]" )
{
    xml::node n(xml::node::cdata("This is a CDATA section"));
    CHECK( is_same_as_file(n, "node/data/09.out") );
}


/*
 * This test checks xml::node::node(comment)
 */

TEST_CASE_METHOD( SrcdirConfig, "node/comment_node", "[node]" )
{
    xml::node n(xml::node::comment("This is an XML comment"));
    CHECK( is_same_as_file(n, "node/data/10.out") );
}


/*
 * This test checks xml::node::node(pi)
 */

TEST_CASE_METHOD( SrcdirConfig, "node/processing_instruction", "[node]" )
{
    xml::node n(xml::node::pi("xslt", "stylesheet=\"test.xsl\""));
    CHECK( is_same_as_file(n, "node/data/11.out") );
}


/*
 * Test correct (expected) handling of text escaping.
 */

TEST_CASE_METHOD( SrcdirConfig, "node/text_substitute_entities", "[node]" )
{
    xml::tree_parser parser(test_file_path("node/data/special_chars.xml").c_str());
    xml::node &root = parser.get_document().get_root_node();

    const std::string content = root.get_content();

    CHECK_THAT( content, Catch::Matchers::Equals("Marlow & Sons") );
}


TEST_CASE_METHOD( SrcdirConfig, "node/escape_node_with_content", "[node]" )
{
    xml::node n("root", "Marlow & Sons");

    CHECK_THAT( n.get_content(), Catch::Matchers::Equals("Marlow & Sons") );
    CHECK( is_same_as_file(n, "node/data/special_chars.xml") );
}


TEST_CASE_METHOD( SrcdirConfig, "node/escape_text_node", "[node]" )
{
    xml::node n("root");
    n.push_back(xml::node(xml::node::text("Marlow & Sons")));

    CHECK_THAT( n.get_content(), Catch::Matchers::Equals("Marlow & Sons") );
    CHECK( is_same_as_file(n, "node/data/special_chars.xml") );
}


TEST_CASE_METHOD( SrcdirConfig, "node/escape_set_content", "[node]" )
{
    xml::node n("root");
    // Note that set_content() takes CDATA content as argument, so "&"
    // has to be written as "&amp;".
    n.set_content("Marlow &amp; Sons");

    CHECK_THAT( n.get_content(), Catch::Matchers::Equals("Marlow & Sons") );
    CHECK( is_same_as_file(n, "node/data/special_chars.xml") );
}


TEST_CASE_METHOD( SrcdirConfig, "node/escape_set_text_content", "[node]" )
{
    xml::node n("root");
    n.set_text_content("Marlow & Sons");

    CHECK_THAT( n.get_content(), Catch::Matchers::Equals("Marlow & Sons") );
    CHECK( is_same_as_file(n, "node/data/special_chars.xml") );
}


TEST_CASE_METHOD( SrcdirConfig, "node/compare_node_iterators", "[node]" )
{
    xml::node n("root");
    xml::node::iterator i = n.begin();
    xml::node::const_iterator ci = n.begin();

    CHECK( i == i );
    CHECK( !(i != i) );
    CHECK( ci == ci );
    CHECK( !(ci != ci) );
    CHECK( i == ci );
    CHECK( !(i != ci) );
    CHECK( ci == i );
    CHECK( !(ci != i) );
}


TEST_CASE_METHOD( SrcdirConfig, "node/compare_nodes_view_iterators", "[node]" )
{
    xml::node n("root");
    xml::nodes_view view(n.elements("foo"));
    xml::const_nodes_view cview(n.elements("foo"));

    xml::nodes_view::iterator i = view.begin();
    xml::nodes_view::const_iterator ci = view.begin();

    xml::const_nodes_view::iterator i2 = cview.begin();
    xml::const_nodes_view::const_iterator ci2 = cview.begin();

    CHECK( i == i );
    CHECK( !(i != i) );
    CHECK( ci == ci );
    CHECK( !(ci != ci) );
    CHECK( i == ci );
    CHECK( !(i != ci) );
    CHECK( ci == i );
    CHECK( !(ci != i) );

    CHECK( i2 == i2 );
    CHECK( !(i2 != i2) );
    CHECK( ci2 == ci2 );
    CHECK( !(ci2 != ci2) );
    CHECK( i2 == ci2 );
    CHECK( !(i2 != ci2) );
    CHECK( ci2 == i2 );
    CHECK( !(ci2 != i2) );

    CHECK( i2 == i );
    CHECK( !(i2 != i) );
    CHECK( ci2 == ci );
    CHECK( !(ci2 != ci) );
    CHECK( i2 == ci );
    CHECK( !(i2 != ci) );
    CHECK( ci2 == i );
    CHECK( !(ci2 != i) );

    CHECK( i == i2 );
    CHECK( !(i != i2) );
    CHECK( ci == ci2 );
    CHECK( !(ci != ci2) );
    CHECK( i == ci2 );
    CHECK( !(i != ci2) );
    CHECK( ci == i2 );
    CHECK( !(ci != i2) );
}

// Test fixture providing "doc" preloaded with the contents of namespace.xml.
class NamespaceTest : private SrcdirConfig
{
public:
    NamespaceTest()
        : parser(test_file_path("node/data/namespace.xml").c_str()),
          doc(parser.get_document())
    {
    }

protected:
    xml::tree_parser parser;
    xml::document doc;
};

TEST_CASE_METHOD( NamespaceTest, "node/get_namespace", "[node][ns]" )
{
    CHECK_THAT
    (
        doc.get_root_node().get_namespace(),
        Catch::Matchers::Equals("http://pmade.org/namespace/test")
    );
}

TEST_CASE_METHOD( NamespaceTest, "node/set_namespace", "[node][ns]" )
{
    doc.get_root_node().set_namespace("http://pmade.org/namespace/newOne");

    CHECK( is_same_as_file(doc, "node/data/namespace.out") );
}
