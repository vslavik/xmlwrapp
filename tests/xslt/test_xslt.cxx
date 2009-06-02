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

#include <xsltwrapp/xsltwrapp.h>

BOOST_AUTO_TEST_SUITE( xslt )


/*
 * Test stylesheet creation
 */

BOOST_AUTO_TEST_CASE( creation_fail1 )
{
    BOOST_CHECK_THROW
    (
        xslt::stylesheet style1(test_file_path("xslt/data/01a.xsl").c_str()),
        std::exception
    );
}

BOOST_AUTO_TEST_CASE( creation_ok )
{
    xslt::stylesheet style1(test_file_path("xslt/data/01b.xsl").c_str());
}

BOOST_AUTO_TEST_CASE( creation_fail2 )
{
    BOOST_CHECK_THROW
    (
        xslt::stylesheet style1(test_file_path("xslt/data/01c.xsl").c_str()),
        std::exception
    );
}


/*
 * Test the first form of apply()
 */

BOOST_AUTO_TEST_CASE( apply1 )
{
    xslt::stylesheet style(test_file_path("xslt/data/02a.xsl").c_str());
    xml::tree_parser parser(test_file_path("xslt/data/input.xml").c_str());

    xml::document result;
    BOOST_CHECK( style.apply(parser.get_document(), result) );

    BOOST_CHECK( is_same_as_file(result, "xslt/data/02a.out") );
}


/*
 * Test the third form of apply
 */

BOOST_AUTO_TEST_CASE( apply3 )
{
    xslt::stylesheet style(test_file_path("xslt/data/02a.xsl").c_str());
    xml::tree_parser parser(test_file_path("xslt/data/input.xml").c_str());

    xml::document& result = style.apply(parser.get_document());
    BOOST_CHECK( is_same_as_file(result, "xslt/data/02a.out") );
}


/*
 * Test the second form of apply
 */

BOOST_AUTO_TEST_CASE( apply2 )
{
    xslt::stylesheet style(test_file_path("xslt/data/03a.xsl").c_str());
    xml::tree_parser parser(test_file_path("xslt/data/input.xml").c_str());

    xslt::stylesheet::param_type params;
    params["foo"] = "'bar'";

    xml::document result;
    BOOST_CHECK( style.apply(parser.get_document(), result, params) );
    BOOST_CHECK( is_same_as_file(result, "xslt/data/03a.out") );
}


/*
 * Test the fourth form of apply
 */

BOOST_AUTO_TEST_CASE( apply4 )
{
    xslt::stylesheet style(test_file_path("xslt/data/03a.xsl").c_str());
    xml::tree_parser parser(test_file_path("xslt/data/input.xml").c_str());

    xslt::stylesheet::param_type params;
    params["foo"] = "'bar'";

    xml::document& result = style.apply(parser.get_document(), params);
    BOOST_CHECK( is_same_as_file(result, "xslt/data/03a.out") );
}


/*
 * Tests libxslt errors reporting
 */

BOOST_AUTO_TEST_CASE( xsl_with_errors )
{
    xslt::stylesheet style(test_file_path("xslt/data/with_errors.xsl").c_str());
    xml::tree_parser parser(test_file_path("xslt/data/input.xml").c_str());

    xml::document result;
    BOOST_CHECK( !style.apply(parser.get_document(), result) );
}

BOOST_AUTO_TEST_CASE( xsl_with_errors_throw )
{
    xslt::stylesheet style(test_file_path("xslt/data/with_errors.xsl").c_str());
    xml::tree_parser parser(test_file_path("xslt/data/input.xml").c_str());

    BOOST_CHECK_THROW
    (
        style.apply(parser.get_document()),
        std::exception
    );
}


BOOST_AUTO_TEST_SUITE_END()
