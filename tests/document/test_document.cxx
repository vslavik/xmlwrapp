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

#include <stdlib.h>

/*
 * This test checks xml::document iteration.
 */

TEST_CASE_METHOD( SrcdirConfig, "document/dump_type", "[document]" )
{
    xml::init::change_flag change(&xml::init::substitute_entities, false);

    xml::tree_parser parser(test_file_path("document/data/01.xml").c_str());

    std::ostringstream ostr;
    xml::node::iterator i = parser.get_document().begin(),
                        end = parser.get_document().end();
    for (; i!=end; ++i)
        dump_node_type(ostr, *i);

    CHECK( is_same_as_file(ostr, "document/data/01.out") );
}


/*
 * This test checks xml::document default constructor.
 */

TEST_CASE_METHOD( SrcdirConfig, "document/default_ctor", "[document]" )
{
    xml::document doc;
    CHECK( is_same_as_file( doc, "document/data/02.out") );
}


/*
 * This test checks xml::document constructor that takes the name of the root
 * node.
 */

TEST_CASE_METHOD( SrcdirConfig, "document/ctor_root_name", "[document]" )
{
    xml::document doc(xml::node("root"));
    CHECK( is_same_as_file( doc, "document/data/03.out") );
}


/*
 * This test checks xml::document constructor that takes a node.
 */

TEST_CASE_METHOD( SrcdirConfig, "document/ctor_root_node", "[document]" )
{
    xml::node n("root", "pcdata");
    xml::document doc(n);
    CHECK( is_same_as_file( doc, "document/data/04.out") );
}


/*
 * This test checks xml::document copy constructor.
 */

TEST_CASE_METHOD( SrcdirConfig, "document/copy_ctor", "[document]" )
{
    xml::node n("root", "pcdata");
    xml::document doc(n);

    xml::document doc_copy(doc);

    CHECK( is_same_as_file( doc_copy, "document/data/04.out") );
}


/*
 * This test checks xml::document assignment operator.
 */

TEST_CASE_METHOD( SrcdirConfig, "document/assignment_operator", "[document]" )
{
    xml::node n("root", "pcdata");
    xml::document doc(n);

    xml::document doc_copy;
    doc_copy = doc;

    CHECK( is_same_as_file( doc_copy, "document/data/04.out") );
}


/*
 * This test checks xml::document::get_root_node.
 */

TEST_CASE_METHOD( SrcdirConfig, "document/get_root_node", "[document]" )
{
    xml::node n("root", "pcdata");
    xml::document doc(n);

    CHECK( is_same_as_file( doc.get_root_node(), "document/data/04.out") );
}


/*
 * This test checks xml::document::set_root_node().
 */

TEST_CASE_METHOD( SrcdirConfig, "document/set_root_node", "[document]" )
{
    std::ostringstream ostr;

    xml::node n("root", "pcdata");

    xml::document doc;
    ostr << doc; // blank document

    doc.set_root_node(n);
    ostr << doc;

    CHECK( is_same_as_file( ostr, "document/data/08.out") );
}


/*
 * This test checks xml::document::get_version().
 */

TEST_CASE_METHOD( SrcdirConfig, "document/get_version", "[document]" )
{
    xml::tree_parser parser(test_file_path("document/data/09.xml").c_str());

    CHECK_THAT
    (
        parser.get_document().get_version(),
        Catch::Matchers::Equals("1.1")
    );
}


/*
 * This test checks xml::document::set_version().
 */

TEST_CASE_METHOD( SrcdirConfig, "document/set_version", "[document]" )
{
    xml::document doc(xml::node("root"));
    doc.set_version("1.1");

    CHECK( is_same_as_file( doc, "document/data/10.out") );
}


/*
 * This test checks xml::document::get_encoding().
 */

TEST_CASE_METHOD( SrcdirConfig, "document/get_encoding", "[document]" )
{
    xml::tree_parser parser(test_file_path("document/data/11.xml").c_str());

    CHECK_THAT
    (
        parser.get_document().get_encoding(),
        Catch::Matchers::Equals("UTF-8")
    );
}


/*
 * This test checks xml::document::set_encoding().
 */

TEST_CASE_METHOD( SrcdirConfig, "document/set_encoding", "[document]" )
{
    xml::document doc(xml::node("root"));
    doc.set_encoding("UTF-8");

    CHECK( is_same_as_file( doc, "document/data/12.out") );
}


/*
 * This test checks xml::document::get_is_standalone().
 */

TEST_CASE_METHOD( SrcdirConfig, "document/get_is_standalone", "[document]" )
{
    xml::tree_parser parser1(test_file_path("document/data/13a.xml").c_str());
    CHECK( parser1.get_document().get_is_standalone() == false );

    xml::tree_parser parser2(test_file_path("document/data/13b.xml").c_str());
    CHECK( parser2.get_document().get_is_standalone() == true );
}


/*
 * This test checks xml::document::set_is_standalone().
 */

TEST_CASE_METHOD( SrcdirConfig, "document/set_is_standalone", "[document]" )
{
    xml::document doc1(xml::node("root"));
    doc1.set_is_standalone(true);
    CHECK( is_same_as_file( doc1, "document/data/13a.out") );

    xml::document doc2(xml::node("root"));
    doc2.set_is_standalone(false);
    CHECK( is_same_as_file( doc2, "document/data/13b.out") );
}


/*
 * This test checks xml::document::process_xinclude()
 */
TEST_CASE_METHOD( SrcdirConfig, "document/process_xinclude", "[document]" )
{
    xml::tree_parser parser(test_file_path("document/data/14.xml").c_str());

    CHECK( parser.get_document().process_xinclude() );
    CHECK( is_same_as_file( parser.get_document(), "document/data/14.out") );
}


/*
 * This test checks xml::document::size()
 */

TEST_CASE_METHOD( SrcdirConfig, "document/size", "[document]" )
{
    xml::document doc_01(xml::node("root"));
    CHECK( doc_01.size() == 1u );

    doc_01.push_back(xml::node(xml::node::comment("This is a comment")));
    CHECK( doc_01.size() == 2u );

    xml::document doc_02(doc_01);
    CHECK( doc_02.size() == 2u );

    xml::document doc_03;
    CHECK( doc_03.size() == 1u );

    xml::node n("root");
    xml::document doc_04(n);
    CHECK( doc_04.size() == 1u );
}


/*
 * This test checks xml::document::push_back and insert
 */

TEST_CASE_METHOD( SrcdirConfig, "document/push_back_and_insert", "[document]" )
{
    xml::document doc(xml::node("root"));

    doc.push_back(xml::node(xml::node::comment(" Comment From push_back ")));

    xml::node::iterator n(doc.insert(xml::node(xml::node::comment("This Will Be Changed"))));
    n->set_content(" Comment From insert ");

    n = doc.insert(doc.begin(), xml::node(xml::node::pi("test")));
    n->set_content("one=\"1\"");

    CHECK( is_same_as_file( doc, "document/data/17.out") );
}


/*
 * This test checks xml::document::push_back and insert to make sure they
 * throw exceptions
 */

TEST_CASE_METHOD( SrcdirConfig, "document/push_back_and_insert_throw", "[document]" )
{
    xml::document doc(xml::node("root"));

    REQUIRE_THROWS_AS
    (
        doc.push_back(xml::node("noway")),
        xml::exception&
    );

    REQUIRE_THROWS_AS
    (
        doc.insert(xml::node("noway")),
        xml::exception&
    );

    REQUIRE_THROWS_AS
    (
        doc.insert(doc.end(), xml::node("noway")),
        xml::exception&
    );
}


/*
 * This test checks xml::document::replace()
 */

TEST_CASE_METHOD( SrcdirConfig, "document/replace", "[document]" )
{
    xml::document doc(xml::node("root"));

    xml::node::iterator n(doc.insert(xml::node(xml::node::comment(" To Be Replaced "))));
    doc.replace(n, xml::node(xml::node::comment(" This is the replacement comment ")));

    CHECK( is_same_as_file( doc, "document/data/19.out") );
}


/*
 * This test checks xml::document::replace() to make sure it throws exceptions
 */

TEST_CASE_METHOD( SrcdirConfig, "document/replace_throw", "[document]" )
{
    xml::document doc(xml::node("root"));
    xml::node::iterator n(doc.insert(xml::node(xml::node::comment(" To Be Replaced "))));

    REQUIRE_THROWS_AS
    (
        doc.replace(n, xml::node("noway")),
        xml::exception&
    );

    REQUIRE_THROWS_AS
    (
        doc.replace(doc.begin(), xml::node(xml::node::comment(" no way "))),
        xml::exception&
    );
}


/*
 * This test checks xml::document::erase().
 */

TEST_CASE_METHOD( SrcdirConfig, "document/erase", "[document]" )
{
    xml::document doc(xml::node("root"));
    doc.push_back(xml::node(xml::node::comment(" Comment from push_back ")));

    xml::node::iterator n(doc.insert(xml::node(xml::node::comment(" You should not see me "))));
    doc.erase(n);

    CHECK( is_same_as_file(doc, "document/data/21.out") );
}

/*
 * This test checks xml::document::erase() to make sure it throws an
 * exception.
 */

TEST_CASE_METHOD( SrcdirConfig, "document/cant_erase_root", "[document]" )
{
    xml::document doc(xml::node("root"));
    doc.push_back(xml::node(xml::node::comment(" Comment from push_back ")));

    REQUIRE_THROWS_AS
    (
        doc.erase(doc.begin(), doc.end()),
        xml::exception&
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

TEST_CASE_METHOD( SrcdirConfig, "document/save_to_file", "[document]" )
{
    xml::document doc(xml::node("root"));
    doc.get_root_node().push_back(xml::node("child"));

    temp_test_file test_file;
    doc.save_to_file(test_file.get_name());

    std::ifstream stream(test_file.get_name());
    CHECK( is_same_as_file(read_file_into_string(stream), "document/data/15.out") );
}


TEST_CASE_METHOD( SrcdirConfig, "document/save_throws_on_failure", "[document]" )
{
    xml::document doc(xml::node("root"));
    doc.get_root_node().push_back(xml::node(xml::node::text("invalid character: \x7")));

    std::string s;
    REQUIRE_THROWS_AS
    (
        doc.save_to_string(s),
        xml::exception&
    );

    temp_test_file test_file;
    REQUIRE_THROWS_AS
    (
        doc.save_to_file(test_file.get_name()),
        xml::exception&
    );
}


TEST_CASE_METHOD( SrcdirConfig, "document/save_to_file_gzip", "[document]" )
{
    xml::document doc(xml::node("root"));
    doc.get_root_node().push_back(xml::node("child"));

    temp_test_file test_file;
    std::string gzfilename = test_file.get_name();
    gzfilename.append(".gz");

    doc.save_to_file(gzfilename.c_str(), 9);

    // verify that the file was can be read back as compressed

    // by libxml2 directly
    xml::tree_parser parser(gzfilename.c_str());

    // unzip and read back the uncompressed file
    std::string command("gunzip " + gzfilename);

    if ( system(command.c_str()) != 0 ) {
        WARN("Skipping test as gunzip is not available");
        return;
    }

    std::ifstream stream(test_file.get_name());
    CHECK( is_same_as_file(read_file_into_string(stream), "document/data/15.out") );
}
