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

#include <xsltwrapp/xsltwrapp.h>

/*
 * Test stylesheet creation
 */

TEST_CASE_METHOD( SrcdirConfig, "xslt/creation_fail1", "[xslt]" )
{
    CHECK_THROWS_AS
    (
        xslt::stylesheet(test_file_path("xslt/data/01a.xsl").c_str()),
        xml::exception&
    );
}

TEST_CASE_METHOD( SrcdirConfig, "xslt/creation_ok", "[xslt]" )
{
    xslt::stylesheet style1(test_file_path("xslt/data/01b.xsl").c_str());
}

TEST_CASE_METHOD( SrcdirConfig, "xslt/creation_fail2", "[xslt]" )
{
    CHECK_THROWS_AS
    (
        xslt::stylesheet(test_file_path("xslt/data/01c.xsl").c_str()),
        xml::exception&
    );
}


/*
 * Test the first form of apply()
 */

TEST_CASE_METHOD( SrcdirConfig, "xslt/apply1", "[xslt]" )
{
    xslt::stylesheet style(test_file_path("xslt/data/02a.xsl").c_str());
    xml::tree_parser parser(test_file_path("xslt/data/input.xml").c_str());

    xml::document result;
    xml::error_messages errors;
    CHECK( style.apply(parser.get_document(), result, errors) );

    CHECK( is_same_as_file(result, "xslt/data/02a.out") );
}


/*
 * Test the third form of apply
 */

TEST_CASE_METHOD( SrcdirConfig, "xslt/apply3", "[xslt]" )
{
    xslt::stylesheet style(test_file_path("xslt/data/02a.xsl").c_str());
    xml::tree_parser parser(test_file_path("xslt/data/input.xml").c_str());

    xml::document& result = style.apply(parser.get_document());
    CHECK( is_same_as_file(result, "xslt/data/02a.out") );
}


/*
 * Test the second form of apply
 */

TEST_CASE_METHOD( SrcdirConfig, "xslt/apply2", "[xslt]" )
{
    xslt::stylesheet style(test_file_path("xslt/data/03a.xsl").c_str());
    xml::tree_parser parser(test_file_path("xslt/data/input.xml").c_str());

    xslt::stylesheet::param_type params;
    params["foo"] = "'bar'";

    xml::document result;
    xml::error_messages errors;
    CHECK( style.apply(parser.get_document(), result, params, errors) );
    CHECK( is_same_as_file(result, "xslt/data/03a.out") );
}


/*
 * Test the fourth form of apply
 */

TEST_CASE_METHOD( SrcdirConfig, "xslt/apply4", "[xslt]" )
{
    xslt::stylesheet style(test_file_path("xslt/data/03a.xsl").c_str());
    xml::tree_parser parser(test_file_path("xslt/data/input.xml").c_str());

    xslt::stylesheet::param_type params;
    params["foo"] = "'bar'";

    xml::document& result = style.apply(parser.get_document(), params);
    CHECK( is_same_as_file(result, "xslt/data/03a.out") );
}


/*
 * Tests libxslt errors reporting
 */

TEST_CASE_METHOD( SrcdirConfig, "xslt/xsl_with_errors", "[xslt]" )
{
    xslt::stylesheet style(test_file_path("xslt/data/with_errors.xsl").c_str());
    xml::tree_parser parser(test_file_path("xslt/data/input.xml").c_str());

    xml::document result;
    xml::error_messages errors;
    CHECK( !style.apply(parser.get_document(), result, errors) );
}

TEST_CASE_METHOD( SrcdirConfig, "xslt/xsl_with_errors_throw", "[xslt]" )
{
    xslt::stylesheet style(test_file_path("xslt/data/with_errors.xsl").c_str());
    xml::tree_parser parser(test_file_path("xslt/data/input.xml").c_str());

    CHECK_THROWS_AS
    (
        style.apply(parser.get_document()),
        xml::exception&
    );
}
