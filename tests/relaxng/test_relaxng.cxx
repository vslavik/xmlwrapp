/*
 * Copyright (C) 2018 Vadim Zeitlin <vz-xmlwrapp@zeitlins.org>
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

TEST_CASE_METHOD( SrcdirConfig, "relaxng/load_non_relaxng_file", "[relaxng]" )
{
    xml::document sch_doc =
            xml::tree_parser(test_file_path("relaxng/data/valid.xml").c_str()).get_document();

    CHECK_THROWS_AS
    (
        xml::relaxng(sch_doc),
        xml::exception
    );

    xml::error_messages log;
    // throw even when collecting errors into log
    CHECK_THROWS_AS
    (
        xml::relaxng(sch_doc, log),
        xml::exception
    );
    CHECK( log.has_errors() );
}


TEST_CASE_METHOD( SrcdirConfig, "relaxng/validate_ok", "[relaxng]" )
{
    xml::document sch_doc =
            xml::tree_parser(test_file_path("relaxng/data/schema.rng").c_str()).get_document();
    xml::relaxng sch(sch_doc);

    xml::document doc =
            xml::tree_parser(test_file_path("relaxng/data/valid.xml").c_str()).get_document();

    CHECK( sch.validate(doc) );

    // And the same with logging:
    xml::error_messages log;
    CHECK( sch.validate(doc) );
    CHECK( !log.has_errors() );
    CHECK( !log.has_warnings() );
}


TEST_CASE_METHOD( SrcdirConfig, "relaxng/validate_invalid", "[relaxng]" )
{
    xml::document sch_doc =
            xml::tree_parser(test_file_path("relaxng/data/schema.rng").c_str()).get_document();
    xml::relaxng sch(sch_doc);

    xml::document doc =
            xml::tree_parser(test_file_path("relaxng/data/invalid.xml").c_str()).get_document();

    CHECK_THROWS_AS
    (
        sch.validate(doc),
        xml::exception
    );

    // And the same with logging:
    xml::error_messages log;
    CHECK( !sch.validate(doc, log) );
    CHECK( log.has_errors() );
}


TEST_CASE_METHOD( SrcdirConfig, "relaxng/validate_nonvalid", "[relaxng]" )
{
    xml::document sch_doc =
            xml::tree_parser(test_file_path("relaxng/data/schema.rng").c_str()).get_document();
    xml::relaxng sch(sch_doc);

    xml::document doc =
            xml::tree_parser(test_file_path("relaxng/data/nonvalid.xml").c_str()).get_document();

    CHECK_THROWS_AS
    (
        sch.validate(doc),
        xml::exception
    );

    // And the same with logging:
    xml::error_messages log;
    CHECK( !sch.validate(doc, log) );
    CHECK( log.has_errors() );

    // The error message is "Did not expect element CCC there" because it's not
    // supposed to occur before "BBB".
    CHECK( log.print().find("CCC") != std::string::npos );
}
