/*
 * Copyright (C) 2001-2003 Peter J Jones (pjones@pmade.org)
 * Copyright (C) 2009      Vaclav Slavik (vslavik@fastmail.fm)
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


BOOST_AUTO_TEST_SUITE( attributes )

/*
 * Test to see if the xml::attributes function can see all the attributes of
 * a node.
 */

static void do_attr_read(const std::string& prefix)
{
    std::ostringstream ostr;

    xml::tree_parser parser(test_file_path(prefix + ".xml").c_str());

    const xml::attributes &attrs = parser.get_document().get_root_node().get_attributes();
    xml::attributes::const_iterator i=attrs.begin(), end=attrs.end();
    for (; i!=end; ++i)
    {
        ostr << i->get_name() << "=" << i->get_value() << "\n";
    }

    BOOST_CHECK( is_same_as_file(ostr, prefix + ".out") );
}

BOOST_AUTO_TEST_CASE( read )
{
    do_attr_read("attributes/data/01a");
    do_attr_read("attributes/data/01b");
    do_attr_read("attributes/data/01c");
}


/*
 * Test to see if the xml::attributes::insert function works.
 */

BOOST_AUTO_TEST_CASE( insert )
{
    xml::tree_parser parser(test_file_path("attributes/data/02.xml").c_str());

    xml::attributes &attrs = parser.get_document().get_root_node().get_attributes();
    attrs.insert("b", "b");

    BOOST_CHECK( is_same_as_file(parser.get_document(), "attributes/data/02.out") );
}


/*
 * Test to see if the xml::attributes::find works.
 */

static bool do_attr_find(const xml::document& doc,
                         const char *to_find,
                         const char *expected_value)
{
    const xml::attributes &attrs = doc.get_root_node().get_attributes();
    xml::attributes::const_iterator i = attrs.find(to_find);

    if ( i == attrs.end() )
        return false;

    BOOST_CHECK_EQUAL( i->get_name(), to_find );
    BOOST_CHECK_EQUAL( i->get_value(), expected_value );

    return true;
}

BOOST_AUTO_TEST_CASE( find )
{
    xml::tree_parser parser(test_file_path("attributes/data/03.xml").c_str());
    const xml::document& doc = parser.get_document();

    BOOST_CHECK( do_attr_find(doc, "one", "1") == true );
    BOOST_CHECK( do_attr_find(doc, "two", "2") == true );
    BOOST_CHECK( do_attr_find(doc, "three", "3") == true );
    BOOST_CHECK( do_attr_find(doc, "missing", NULL) == false );
    BOOST_CHECK( do_attr_find(doc, "also_missing", NULL) == false );
}


/*
 * Test to see if the xml::attributes::remove(iterator) works.
 */

static void do_remove_by_iter(const char *name, const char *outfile)
{
    xml::tree_parser parser(test_file_path("attributes/data/04.xml").c_str());

    xml::attributes &attrs = parser.get_document().get_root_node().get_attributes();
    xml::attributes::iterator i = attrs.find(name);

    BOOST_REQUIRE( i != attrs.end() );
    attrs.erase(i);

    BOOST_CHECK( is_same_as_file(parser.get_document(), outfile) );
}

BOOST_AUTO_TEST_CASE( remove_by_iter )
{
    do_remove_by_iter("attr_one", "attributes/data/04a.out");
    do_remove_by_iter("attr_two", "attributes/data/04b.out");
    do_remove_by_iter("attr_three", "attributes/data/04c.out");
    do_remove_by_iter("attr_four", "attributes/data/04d.out");
}


/*
 * Test to see if the xml::attributes::remove(const char*) works.
 */

static void do_remove_by_name(const char *name, const char *outfile)
{
    xml::tree_parser parser(test_file_path("attributes/data/04.xml").c_str());

    xml::attributes &attrs = parser.get_document().get_root_node().get_attributes();

    attrs.erase(name);

    BOOST_CHECK( is_same_as_file(parser.get_document(), outfile) );
}

BOOST_AUTO_TEST_CASE( remove_by_name )
{
    do_remove_by_name("attr_one", "attributes/data/04a.out");
    do_remove_by_name("attr_two", "attributes/data/04b.out");
    do_remove_by_name("attr_three", "attributes/data/04c.out");
    do_remove_by_name("attr_four", "attributes/data/04d.out");
}


/*
 * Test to see if  xml::attributes::find() can see DTD default attributes
 */

BOOST_AUTO_TEST_CASE( find_dtd_default_attr )
{
    xml::tree_parser parser(test_file_path("attributes/data/09.xml").c_str());

    BOOST_CHECK( parser.get_document().has_internal_subset() );
    BOOST_CHECK( parser.get_document().validate() );

    const xml::attributes &attrs =
        parser.get_document().get_root_node().get_attributes();

    {
        xml::attributes::const_iterator i = attrs.find("one");
        BOOST_REQUIRE( i != attrs.end() );
        BOOST_CHECK_EQUAL( i->get_value(), "1" );
        ++i;
        BOOST_CHECK( i == attrs.end() );
    }

    {
        xml::attributes::const_iterator i = attrs.find("two");
        BOOST_REQUIRE( i != attrs.end() );
        BOOST_CHECK_EQUAL( i->get_value(), "two" );
        ++i;
        BOOST_CHECK( i == attrs.end() );
    }

    {
        xml::attributes::const_iterator i = attrs.find("three");
        BOOST_REQUIRE( i != attrs.end() );
        BOOST_CHECK_EQUAL( i->get_value(), "three" );
        ++i;
        BOOST_CHECK( i == attrs.end() );
    }
}


/*
 * Test to see if xml::attributes::find() will die when DTD default
 * attributes are really implied.
 */

BOOST_AUTO_TEST_CASE( dtd_implied )
{
    xml::tree_parser parser(test_file_path("attributes/data/10.xml").c_str());

    BOOST_CHECK( parser.get_document().has_internal_subset() );
    BOOST_CHECK( parser.get_document().validate() );

    const xml::attributes &attrs =
        parser.get_document().get_root_node().get_attributes();

    BOOST_CHECK( attrs.find("optional") == attrs.end() );
}


/*
 * Test to see if the xml::attributes copy constructor works.
 */

BOOST_AUTO_TEST_CASE( attr_copy_ctor )
{
    std::ostringstream ostr;
    xml::tree_parser parser(test_file_path("attributes/data/08.xml").c_str());

    // make a copy
    xml::attributes attrs = parser.get_document().get_root_node().get_attributes();
    xml::attributes::const_iterator i = attrs.begin(), end = attrs.end();

    for ( ; i != end; ++i )
        ostr << i->get_name() << "=" << i->get_value() << "\n";

    BOOST_CHECK( is_same_as_file(ostr, "attributes/data/08.out") );
}

BOOST_AUTO_TEST_SUITE_END()


/*
 * Test to see if the xml::attributes::empty() works.
 */

BOOST_AUTO_TEST_CASE( attr_empty_a )
{
    xml::tree_parser parser(test_file_path("attributes/data/06a.xml").c_str());

    xml::attributes &attrs = parser.get_document().get_root_node().get_attributes();

    BOOST_CHECK( attrs.empty() );
}

BOOST_AUTO_TEST_CASE( attr_empty_b )
{
    xml::tree_parser parser(test_file_path("attributes/data/06b.xml").c_str());

    xml::attributes &attrs = parser.get_document().get_root_node().get_attributes();

    BOOST_CHECK( !attrs.empty() );
}


/*
 * Test to see if xml::attributes::size() works.
 */

static int do_get_attr_size(const char *testfile)
{
    xml::tree_parser parser(test_file_path(testfile).c_str());

    xml::attributes &attrs = parser.get_document().get_root_node().get_attributes();
    return attrs.size();
}

BOOST_AUTO_TEST_CASE( attr_size )
{
    BOOST_CHECK_EQUAL( do_get_attr_size("attributes/data/07a.xml"), 0 );
    BOOST_CHECK_EQUAL( do_get_attr_size("attributes/data/07b.xml"), 1 );
    BOOST_CHECK_EQUAL( do_get_attr_size("attributes/data/07c.xml"), 2 );
    BOOST_CHECK_EQUAL( do_get_attr_size("attributes/data/07d.xml"), 3 );
}
