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
 * The following code demonstrates the usage of the xml::event_parser class.
 * This class must be derived from and the new subclass must implement the
 * pure virtual member functions from xml::event_parser. These member
 * functions are called when parsing events occur.
 */

// xmlwrapp include
#include <xmlwrapp/xmlwrapp.h>

// standard includes
#include <iostream>
#include <exception>

/*
 * Here we create a class that will receive the parsing events.
 */
class myparser : public xml::event_parser {
public:
    myparser (void)
    { std::cout << "myparser constructor\n"; }

    ~myparser (void) override
    { std::cout << "myparser destructor\n"; }
private:
    bool start_element (const std::string &name, const attrs_type&) override
    { std::cout << "begin tag '" << name << "'\n"; return true; }

    bool end_element (const std::string &name) override
    { std::cout << "end tag '" << name << "'\n"; return true;   }

    bool text (const std::string&) override
    { return true; }
};

/*
 * And here is good ol' main.
 */
int main (int argc, char *argv[]) {
    if (argc != 2) {
	std::cerr << argv[0] << ": you must give one and only one XML file name\n";
	return 1;
    }

    myparser parser;

    if (!parser.parse_file(argv[1])) {
	std::cerr << argv[0] << ": error parsing XML file\n";
	return 1;
    }

    return 0;
}
