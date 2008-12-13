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

/*
 * make sure that returning false or throwing an exception will cause the
 * parser to stop.
 */

#include <xmlwrapp/xmlwrapp.h>
#include <iostream>
#include <string>

namespace {
    class test_parser : public xml::event_parser {
    public:
	test_parser (const char *cbname, bool use_throw) : cbname_(cbname), throw_(use_throw)
	{ }

    private:
	bool start_element (const std::string&, const xml::event_parser::attrs_type&) {
	    if (cbname_ == "start_element") {
		std::cout << cbname_;
		if (throw_) {std::cout << " throw"; throw cbname_;}
		return false;
	    }

	    return true;
	}

	bool end_element (const std::string&) {
	    if (cbname_ == "end_element") {
		std::cout << cbname_;
		if (throw_) {std::cout << " throw"; throw cbname_;}
		return false;
	    }

	    return true;
	}

	bool text (const std::string&) {
	    if (cbname_ == "text") {
		std::cout << cbname_;
		if (throw_) {std::cout << " throw"; throw cbname_;}
		return false;
	    }

	    return true;
	}

	bool processing_instruction (const std::string&, const std::string&) {
	    if (cbname_ == "processing_instruction") {
		std::cout << cbname_;
		if (throw_) {std::cout << " throw"; throw cbname_;}
		return false;
	    }

	    return true;
	}

	bool comment (const std::string&) {
	    if (cbname_ == "comment") {
		std::cout << cbname_;
		if (throw_) {std::cout << " throw"; throw cbname_;}
		return false;
	    }

	    return true;
	}

	bool cdata (const std::string&) {
	    if (cbname_ == "cdata") {
		std::cout << cbname_;
		if (throw_) {std::cout << " throw"; throw cbname_;}
		return false;
	    }

	    return true;
	}

	std::string cbname_;
	bool throw_;
    };

}

int main (int argc, char *argv[]) {
    if (argc != 3 && argc != 4) {
	std::cerr << "Usage: " << argv[0] << " xmlfile cbname [throw]\n";
	return 1;
    }

    test_parser parser(argv[2],argc == 4);
    bool status = parser.parse_file(argv[1]);
    std::cout << "\n";
    return status ? 1 : 0;
}
