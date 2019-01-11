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

    CHECK( is_same_as_file(ostr, prefix + ".out") );
}

TEST_CASE_METHOD( SrcdirConfig, "attributes/read", "[attributes]" )
{
    do_attr_read("attributes/data/01a");
    do_attr_read("attributes/data/01b");
    do_attr_read("attributes/data/01c");
}


/*
 * Test to see if the xml::attributes::insert function works.
 */

TEST_CASE_METHOD( SrcdirConfig, "attributes/insert", "[attributes]" )
{
    xml::tree_parser parser(test_file_path("attributes/data/02.xml").c_str());

    xml::attributes &attrs = parser.get_document().get_root_node().get_attributes();
    attrs.insert("b", "b");

    CHECK( is_same_as_file(parser.get_document(), "attributes/data/02.out") );
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

    CHECK_THAT( i->get_name(), Catch::Matchers::Equals(to_find) );
    CHECK_THAT( i->get_value(), Catch::Matchers::Equals(expected_value) );

    return true;
}

TEST_CASE_METHOD( SrcdirConfig, "attributes/find", "[attributes]" )
{
    xml::tree_parser parser(test_file_path("attributes/data/03.xml").c_str());
    const xml::document& doc = parser.get_document();

    CHECK( do_attr_find(doc, "one", "1") == true );
    CHECK( do_attr_find(doc, "two", "2") == true );
    CHECK( do_attr_find(doc, "three", "3") == true );
    CHECK( do_attr_find(doc, "missing", NULL) == false );
    CHECK( do_attr_find(doc, "also_missing", NULL) == false );
}


/*
 * Test to see if the xml::attributes::remove(iterator) works.
 */

static void do_remove_by_iter(const char *name, const char *outfile)
{
    xml::tree_parser parser(test_file_path("attributes/data/04.xml").c_str());

    xml::attributes &attrs = parser.get_document().get_root_node().get_attributes();
    xml::attributes::iterator i = attrs.find(name);

    REQUIRE( i != attrs.end() );
    attrs.erase(i);

    CHECK( is_same_as_file(parser.get_document(), outfile) );
}

TEST_CASE_METHOD( SrcdirConfig, "attributes/remove_by_iter", "[attributes]" )
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

    CHECK( is_same_as_file(parser.get_document(), outfile) );
}

TEST_CASE_METHOD( SrcdirConfig, "attributes/remove_by_name", "[attributes]" )
{
    do_remove_by_name("attr_one", "attributes/data/04a.out");
    do_remove_by_name("attr_two", "attributes/data/04b.out");
    do_remove_by_name("attr_three", "attributes/data/04c.out");
    do_remove_by_name("attr_four", "attributes/data/04d.out");
}


/*
 * Test to see if  xml::attributes::find() can see DTD default attributes
 */

TEST_CASE_METHOD( SrcdirConfig, "attributes/find_dtd_default_attr", "[attributes]" )
{
    xml::tree_parser parser(test_file_path("attributes/data/09.xml").c_str());

    CHECK( parser.get_document().has_internal_subset() );
    CHECK( parser.get_document().validate() );

    const xml::attributes &attrs =
        parser.get_document().get_root_node().get_attributes();

    {
        xml::attributes::const_iterator i = attrs.find("one");
        REQUIRE( i != attrs.end() );
        CHECK_THAT( i->get_value(), Catch::Matchers::Equals("1") );
        ++i;
        CHECK( i == attrs.end() );
    }

    {
        xml::attributes::const_iterator i = attrs.find("two");
        REQUIRE( i != attrs.end() );
        CHECK_THAT( i->get_value(), Catch::Matchers::Equals("two") );
        ++i;
        CHECK( i == attrs.end() );
    }

    {
        xml::attributes::const_iterator i = attrs.find("three");
        REQUIRE( i != attrs.end() );
        CHECK_THAT( i->get_value(), Catch::Matchers::Equals("three") );
        ++i;
        CHECK( i == attrs.end() );
    }
}


/*
 * Test to see if xml::attributes::find() will die when DTD default
 * attributes are really implied.
 */

TEST_CASE_METHOD( SrcdirConfig, "attributes/dtd_implied", "[attributes]" )
{
    xml::tree_parser parser(test_file_path("attributes/data/10.xml").c_str());

    CHECK( parser.get_document().has_internal_subset() );
    CHECK( parser.get_document().validate() );

    const xml::attributes &attrs =
        parser.get_document().get_root_node().get_attributes();

    CHECK( attrs.find("optional") == attrs.end() );
}


/*
 * Test to see if the xml::attributes copy constructor works.
 */

TEST_CASE_METHOD( SrcdirConfig, "attributes/attr_copy_ctor", "[attributes]" )
{
    std::ostringstream ostr;
    xml::tree_parser parser(test_file_path("attributes/data/08.xml").c_str());

    // make a copy
    xml::attributes attrs = parser.get_document().get_root_node().get_attributes();
    xml::attributes::const_iterator i = attrs.begin(), end = attrs.end();

    for ( ; i != end; ++i )
        ostr << i->get_name() << "=" << i->get_value() << "\n";

    CHECK( is_same_as_file(ostr, "attributes/data/08.out") );
}


/*
 * Test to see if the xml::attributes::empty() works.
 */

TEST_CASE_METHOD( SrcdirConfig, "attributes/attr_empty_a", "[attributes]" )
{
    xml::tree_parser parser(test_file_path("attributes/data/06a.xml").c_str());

    xml::attributes &attrs = parser.get_document().get_root_node().get_attributes();

    CHECK( attrs.empty() );
}

TEST_CASE_METHOD( SrcdirConfig, "attributes/attr_empty_b", "[attributes]" )
{
    xml::tree_parser parser(test_file_path("attributes/data/06b.xml").c_str());

    xml::attributes &attrs = parser.get_document().get_root_node().get_attributes();

    CHECK( !attrs.empty() );
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

TEST_CASE_METHOD( SrcdirConfig, "attributes/attr_size", "[attributes]" )
{
    CHECK( do_get_attr_size("attributes/data/07a.xml") == 0 );
    CHECK( do_get_attr_size("attributes/data/07b.xml") == 1 );
    CHECK( do_get_attr_size("attributes/data/07c.xml") == 2 );
    CHECK( do_get_attr_size("attributes/data/07d.xml") == 3 );
}


TEST_CASE_METHOD( SrcdirConfig, "attributes/compare_attr_iterators", "[attributes]" )
{
    xml::node n("root");
    xml::attributes& attrs = n.get_attributes();

    xml::attributes::iterator i = attrs.begin();
    xml::attributes::const_iterator ci = attrs.begin();

    CHECK( i == i );
    CHECK( !(i != i) );
    CHECK( ci == ci );
    CHECK( !(ci != ci) );
    CHECK( i == ci );
    CHECK( !(i != ci) );
    CHECK( ci == i );
    CHECK( !(ci != i) );
}
