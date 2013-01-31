/*
 * Copyright (C) 2013      Vaclav Slavik (vslavik@gmail.com)
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

BOOST_AUTO_TEST_SUITE( schema )


BOOST_AUTO_TEST_CASE( load_non_schema_file )
{
    xml::document sch_doc =
            xml::tree_parser(test_file_path("schema/data/valid.xml").c_str()).get_document();

    BOOST_CHECK_THROW
    (
        xml::schema sch(sch_doc),
        xml::exception
    );

    xml::error_messages log;
    BOOST_CHECK_THROW
    (
        // throw even when collecting errors into log
        xml::schema sch(sch_doc, log),
        xml::exception
    );
    BOOST_CHECK( log.has_errors() );
}


BOOST_AUTO_TEST_CASE( validate_ok )
{
    xml::document sch_doc =
            xml::tree_parser(test_file_path("schema/data/schema.xsd").c_str()).get_document();
    xml::schema sch(sch_doc);

    xml::document doc =
            xml::tree_parser(test_file_path("schema/data/valid.xml").c_str()).get_document();

    BOOST_CHECK( sch.validate(doc) );

    // And the same with logging:
    xml::error_messages log;
    BOOST_CHECK( sch.validate(doc) );
    BOOST_CHECK( !log.has_errors() );
    BOOST_CHECK( !log.has_warnings() );
}


BOOST_AUTO_TEST_CASE( validate_fail )
{
    xml::document sch_doc =
            xml::tree_parser(test_file_path("schema/data/schema.xsd").c_str()).get_document();
    xml::schema sch(sch_doc);

    xml::document doc =
            xml::tree_parser(test_file_path("schema/data/invalid.xml").c_str()).get_document();

    BOOST_CHECK_THROW
    (
        sch.validate(doc),
        xml::exception
    );

    // And the same with logging:
    xml::error_messages log;
    BOOST_CHECK( !sch.validate(doc, log) );
    BOOST_CHECK( log.has_errors() );
}


BOOST_AUTO_TEST_SUITE_END()
