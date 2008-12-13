/*
 * Copyright (C) 2001-2003 Peter J Jones (pjones@pmade.org)
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

#include <xmlwrapp/xmlwrapp.h>
#include <iostream>

namespace {
    class test_parser : public xml::event_parser {
    public:
	test_parser (void)
	{ std::cout << "-- CTOR:\n"; }

	~test_parser (void)
	{ std::cout << "-- DTOR:\n"; }
    private:
	bool start_element (const std::string &name, const xml::event_parser::attrs_type&) {
	    std::cout << "-- START_ELEMENT: " << name << "\n";
	    return true;
	}

	bool end_element (const std::string &name) {
	    std::cout << "-- END_ELEMENT: " << name << "\n";
	    return true;
	}

	bool text (const std::string &contents) {
	    std::cout << "-- TEXT: " << contents << "\n";
	    return true;
	}

	bool processing_instruction (const std::string &target, const std::string &data) {
	    std::cout << "-- PI: TARGET=" << target << " DATA=" << data << "\n";
	    return true;
	}

	bool comment (const std::string &contents) {
	    std::cout << "-- COMMENT: " << contents << "\n";
	    return true;
	}

	bool cdata (const std::string &contents) {
	    std::cout << "-- CDATA: " << contents << "\n";
	    return true;
	}
    };
}

int main (int argc, char *argv[]) {
    if (argc != 2) {
	std::cerr << argv[0] << ": missing XML file\n";
	return 1;
    }

    test_parser parser;

    if (!parser.parse_file(argv[1])) {
	std::cerr << argv[0] << ": error parsing file: " << argv[1] << ": " << parser.get_error_message() << "\n";
	return 1;
    }

    return 0;
}
