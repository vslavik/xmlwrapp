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

/** @file
 * This file contains the implementation of the xml::event_parser class for
 * the libxml XML parser.
**/

// make MSVC6 shutup about long template names
#if defined(_MSC_VER)
#  pragma warning(disable : 4786)
#endif

// xmlwrapp includes
#include "xmlwrapp/event_parser.h"
#include "xmlwrapp/node.h"
#include "utility.h"

// libxml includes
#include <libxml/parser.h>
#include <libxml/xmlversion.h>

// standard includes
#include <new>
#include <cstring>
#include <iostream>
#include <fstream>

//####################################################################
/*
 * This is a hack to fix a problem with a change in the libxml2 API for
 * versions starting at 2.6.0
 */
#if LIBXML_VERSION >= 20600
#   define initxmlDefaultSAXHandler xmlSAX2InitDefaultSAXHandler
#   include <libxml/SAX2.h>
#endif
//####################################################################
namespace {
    const std::size_t const_buffer_size = 4096;

    extern "C" xmlEntityPtr cb_get_entity (void *, const xmlChar *name);
    extern "C" void cb_start_element (void *parser, const xmlChar *tag, const xmlChar **props);
    extern "C" void cb_end_element (void *parser, const xmlChar *tag);
    extern "C" void cb_text (void *parser, const xmlChar *text, int length);
    extern "C" void cb_pi (void *parser, const xmlChar *target, const xmlChar *data);
    extern "C" void cb_comment (void *parser, const xmlChar *text);
    extern "C" void cb_cdata (void *parser, const xmlChar *text, int length);
    extern "C" void cb_warning (void *parser, const char *message, ...);
    extern "C" void cb_error (void *parser, const char *message, ...);
    extern "C" void cb_ignore (void*, const xmlChar*, int);
} // end anonymous namespace
//####################################################################
struct xml::epimpl {
public:
    epimpl (event_parser &parent);
    ~epimpl (void);

    xmlSAXHandler sax_handler_;
    xmlParserCtxt *parser_context_;
    bool parser_status_;
    std::string last_error_message_;

    void event_start_element (const xmlChar *tag, const xmlChar **props);
    void event_end_element (const xmlChar *tag);
    void event_text (const xmlChar *text, int length);
    void event_pi (const xmlChar *target, const xmlChar *data);
    void event_comment (const xmlChar *text);
    void event_cdata (const xmlChar *text, int length);
    void event_warning (const std::string &message);
    void event_error (const std::string &message);
private:
    event_parser &parent_;

    epimpl (const epimpl&);
    epimpl& operator= (const epimpl&);
};
//####################################################################
xml::event_parser::event_parser (void) {
    pimpl_ = new epimpl(*this);
}
//####################################################################
xml::event_parser::~event_parser (void) {
    delete pimpl_;
}
//####################################################################
bool xml::event_parser::parse_file (const char *filename) {
    std::ifstream file(filename); if (!file) return false;
    return parse_stream(file);
}
//####################################################################
bool xml::event_parser::parse_stream (std::istream &stream) {
    char buffer[const_buffer_size];

    while (pimpl_->parser_status_ && (stream.read(buffer, const_buffer_size) || stream.gcount()))
	pimpl_->parser_status_ = parse_chunk(buffer, stream.gcount());

    if (!pimpl_->parser_status_) return false;
    if (!stream && !stream.eof()) return false;

    return parse_finish();
}
//####################################################################
bool xml::event_parser::parse_chunk (const char *chunk, size_type length) {
    xmlParseChunk(pimpl_->parser_context_, chunk, length, 0);
    return pimpl_->parser_status_;
}
//####################################################################
bool xml::event_parser::parse_finish (void) {
    xmlParseChunk(pimpl_->parser_context_, 0, 0, 0);
    return pimpl_->parser_status_;
}
//####################################################################
bool xml::event_parser::processing_instruction (const std::string&, const std::string&) {
    return true;
}
//####################################################################
bool xml::event_parser::comment (const std::string&) {
    return true;
}
//####################################################################
bool xml::event_parser::cdata (const std::string &contents) {
    return text(contents);
}
//####################################################################
bool xml::event_parser::warning (const std::string&) {
    return true;
}
//####################################################################
const std::string& xml::event_parser::get_error_message (void) const {
    if (pimpl_->last_error_message_.empty()) {
	pimpl_->last_error_message_ = "Unknown Error";
    }

    return pimpl_->last_error_message_;
}
//####################################################################
void xml::event_parser::set_error_message (const char *message) {
    pimpl_->last_error_message_ = message;
}
//####################################################################
xml::epimpl::epimpl (event_parser &parent)
    : parser_status_(true), parent_(parent)
{
    std::memset(&sax_handler_, 0, sizeof(sax_handler_));

    sax_handler_.startElement		= cb_start_element;
    sax_handler_.endElement		= cb_end_element;
    sax_handler_.characters		= cb_text;
    sax_handler_.processingInstruction	= cb_pi;
    sax_handler_.comment		= cb_comment;
    sax_handler_.cdataBlock		= cb_cdata;
    sax_handler_.warning		= cb_warning;
    sax_handler_.error			= cb_error;
    sax_handler_.fatalError		= cb_error;

    if (xmlKeepBlanksDefaultValue == 0) sax_handler_.ignorableWhitespace = cb_ignore;
    else sax_handler_.ignorableWhitespace = cb_text;

    if ( (parser_context_ = xmlCreatePushParserCtxt(&sax_handler_, this, 0, 0, 0)) == 0) {
	throw std::bad_alloc();
    }
}
//####################################################################
xml::epimpl::~epimpl (void) {
    xmlFreeParserCtxt(parser_context_);
}
//####################################################################
void xml::epimpl::event_start_element (const xmlChar *tag, const xmlChar **props) {
    if (!parser_status_) return;

    try {

	event_parser::attrs_type attrs;
	const xmlChar **attrp;

	for (attrp = props; attrp && *attrp; attrp += 2) {       
	    attrs[reinterpret_cast<const char*>(*attrp)] = reinterpret_cast<const char*>(*(attrp+1));
	}       

	std::string name = reinterpret_cast<const char*>(tag);
	parser_status_ = parent_.start_element(name, attrs);

    } catch ( ... ) { parser_status_ = false; }
    if (!parser_status_) xmlStopParser(parser_context_);
}
//####################################################################
void xml::epimpl::event_end_element (const xmlChar *tag) {
    if (!parser_status_) return;

    try {

	std::string name = reinterpret_cast<const char*>(tag);
	parser_status_ = parent_.end_element(name);

    } catch ( ... ) { parser_status_ = false; }
    if (!parser_status_) xmlStopParser(parser_context_);
}
//####################################################################
void xml::epimpl::event_text (const xmlChar *text, int length) {
    if (!parser_status_) return;

    try {

	std::string contents(reinterpret_cast<const char*>(text), static_cast<std::string::size_type>(length));
	parser_status_ = parent_.text(contents);

    } catch ( ... ) { parser_status_ = false; }
    if (!parser_status_) xmlStopParser(parser_context_);
}
//####################################################################
void xml::epimpl::event_pi (const xmlChar *target, const xmlChar *data) {
    if (!parser_status_) return;

    try {

	std::string s_target = reinterpret_cast<const char*>(target);
	std::string s_data = reinterpret_cast<const char*>(data);
	parser_status_ = parent_.processing_instruction(s_target, s_data);

    } catch ( ... ) { parser_status_ = false; }
    if (!parser_status_) xmlStopParser(parser_context_);
}
//####################################################################
void xml::epimpl::event_comment (const xmlChar *text) {
    if (!parser_status_) return;

    try {

	std::string contents = reinterpret_cast<const char*>(text);
	parser_status_ = parent_.comment(contents);

    } catch ( ... ) { parser_status_ = false; }
    if (!parser_status_) xmlStopParser(parser_context_);
}
//####################################################################
void xml::epimpl::event_cdata (const xmlChar *text, int length) {
    if (!parser_status_) return;

    try {

	std::string contents(reinterpret_cast<const char*>(text), static_cast<std::string::size_type>(length));
	parser_status_ = parent_.cdata(contents);

    } catch ( ... ) { parser_status_ = false; }
    if (!parser_status_) xmlStopParser(parser_context_);
}
//####################################################################
void xml::epimpl::event_warning (const std::string &message) {
    if (!parser_status_) return;

    try {
    
	parser_status_ = parent_.warning(message);
	if (!parser_status_) last_error_message_ = message;

    } catch ( ... ) { parser_status_ = false; }
    if (!parser_status_) xmlStopParser(parser_context_);
}
//####################################################################
void xml::epimpl::event_error (const std::string &message) {
    try { last_error_message_ = message; } catch ( ... ) { }
    parser_status_ = false;
    xmlStopParser(parser_context_);
}
//####################################################################
namespace {
    //####################################################################
    extern "C" xmlEntityPtr cb_get_entity (void *, const xmlChar *name)
    { return xmlGetPredefinedEntity(name); }
    //####################################################################
    extern "C" void cb_start_element (void *parser, const xmlChar *tag, const xmlChar **props)
    { static_cast<xml::epimpl*>(parser)->event_start_element(tag, props); }
    //####################################################################
    extern "C" void cb_end_element (void *parser, const xmlChar *tag)
    { static_cast<xml::epimpl*>(parser)->event_end_element(tag); }
    //####################################################################
    extern "C" void cb_text (void *parser, const xmlChar *text, int length)
    { static_cast<xml::epimpl*>(parser)->event_text(text, length); }
    //####################################################################
    extern "C" void cb_pi (void *parser, const xmlChar *target, const xmlChar *data)
    { static_cast<xml::epimpl*>(parser)->event_pi(target, data); }
    //####################################################################
    extern "C" void cb_comment (void *parser, const xmlChar *text)
    { static_cast<xml::epimpl*>(parser)->event_comment(text); }
    //####################################################################
    extern "C" void cb_cdata (void *parser, const xmlChar *text, int length)
    { static_cast<xml::epimpl*>(parser)->event_cdata(text, length); }
    //####################################################################
    extern "C" void cb_warning (void *parser, const char *message, ...) {
	std::string complete_message;

	va_list ap;
	va_start(ap, message);
	xml::printf2string(complete_message, message, ap);
	va_end(ap);

	static_cast<xml::epimpl*>(parser)->event_warning(complete_message);
    }
    //####################################################################
    extern "C" void cb_error (void *parser, const char *message, ...) { 
	std::string complete_message;

	va_list ap;
	va_start(ap, message);
	xml::printf2string(complete_message, message, ap);
	va_end(ap);

	static_cast<xml::epimpl*>(parser)->event_error(complete_message);
    }
    //####################################################################
    extern "C" void cb_ignore (void*, const xmlChar*, int) {
	return;
    }
} // end anonymous namespace
