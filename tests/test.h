/*
 * Copyright (C) 2009 Vaclav Slavik (vslavik@gmail.com)
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

#ifndef _xmlwrapp_test_h_
#define _xmlwrapp_test_h_

#include "catch.hpp"

#include <xmlwrapp/xmlwrapp.h>

#include <iostream>
#include <fstream>
#include <sstream>
#include <cstring>

class SrcdirConfig
{
public:
    SrcdirConfig();

    static const std::string& get_srcdir()
    {
        return srcdir;
    }

private:
    // path to source directory, where test data files are located
    static std::string srcdir;
};

// helpers for testing if output matches expected output:

inline std::string test_file_path(const std::string& filename)
{
    return SrcdirConfig::get_srcdir() + "/" + filename;
}

inline std::string read_file_into_string(std::istream& stream)
{
    std::stringstream s;
    s << stream.rdbuf();
    return s.str();
}

inline std::string read_file_into_string(const std::string& filename)
{
    std::ifstream f(test_file_path(filename).c_str());
    return read_file_into_string(f);
}

inline bool is_same_as_file(const std::string& data, const std::string& filename)
{
    const std::string filedata = read_file_into_string(filename);
    if ( data == filedata )
        return true;

    FAIL_CHECK("Expected output:\n" << filedata << "\nActual output:\n" << data);

    return false;
}

inline bool is_same_as_file(const std::ostringstream& stream, const std::string& filename)
{
    return is_same_as_file(stream.str(), filename);
}

inline bool is_same_as_file(const xml::document& doc, const std::string& filename)
{
    std::string xml;
    doc.save_to_string(xml);
    return is_same_as_file(xml, filename);
}

inline bool is_same_as_file(const xml::node& node, const std::string& filename)
{
    std::ostringstream ostr;
    ostr << node;
    return is_same_as_file(ostr, filename);
}


// helpers for dumping information about XML tree:

inline void dump_node_type(std::ostream& s, const xml::node &n, int indent = 0)
{
    for ( int j = 0; j < indent; ++j )
        s << " ";

    switch (n.get_type())
    {
        case xml::node::type_element:
            s << "type_element\n";
            break;

        case xml::node::type_text:
            s << "type_text\n";
            break;

        case xml::node::type_cdata:
            s << "type_cdata\n";
            break;

        case xml::node::type_pi:
            s << "type_pi\n";
            break;

        case xml::node::type_comment:
            s << "type_comment\n";
            break;

        case xml::node::type_entity:
            s << "type_entity\n";
            break;

        case xml::node::type_entity_ref:
            s << "type_entity_ref\n";
            break;

        case xml::node::type_xinclude:
            s << "type_xinclude\n";
            break;

        case xml::node::type_document:
            s << "type_document\n";
            break;

        case xml::node::type_document_type:
            s << "type_document_type\n";
            break;

        case xml::node::type_document_frag:
            s << "type_document_frag\n";
            break;

        case xml::node::type_notation:
            s << "type_notation\n";
            break;

        case xml::node::type_dtd:
            s << "type_dtd\n";
            break;

        case xml::node::type_dtd_element:
            s << "type_dtd_element\n";
            break;

        case xml::node::type_dtd_attribute:
            s << "type_dtd_attribute\n";
            break;

        case xml::node::type_dtd_entity:
            s << "type_dtd_entity\n";
            break;

        case xml::node::type_dtd_namespace:
            s << "type_dtd_namespace\n";
            break;
    }

    for ( xml::node::const_iterator i = n.begin(); i != n.end(); ++i )
        dump_node_type(s, *i, indent + 4);
}

#endif // _xmlwrapp_test_h_
