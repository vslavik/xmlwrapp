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


TEST_CASE_METHOD( SrcdirConfig, "event/callbacks", "[event]" )
{
    class test_parser : public xml::event_parser
    {
    public:
        test_parser(std::ostream& stream) : stream_(stream)
            { stream_ << "-- CTOR:\n"; }

        ~test_parser () override
            { stream_ << "-- DTOR:\n"; }

    private:
        bool start_element (const std::string &name, const xml::event_parser::attrs_type&) override
        {
            stream_ << "-- START_ELEMENT: " << name << "\n";
            return true;
        }

        bool end_element (const std::string &name) override
        {
            stream_ << "-- END_ELEMENT: " << name << "\n";
            return true;
        }

        bool text (const std::string &contents) override
        {
            stream_ << "-- TEXT: " << contents << "\n";
            return true;
        }

        bool processing_instruction (const std::string &target, const std::string &data) override
        {
            stream_ << "-- PI: TARGET=" << target << " DATA=" << data << "\n";
            return true;
        }

        bool comment (const std::string &contents) override
        {
            stream_ << "-- COMMENT: " << contents << "\n";
            return true;
        }

        bool cdata (const std::string &contents) override
        {
            stream_ << "-- CDATA: " << contents << "\n";
            return true;
        }

        std::ostream& stream_;
    };

    std::ostringstream ostr;

    {
        test_parser parser(ostr);
        CHECK( parser.parse_file(test_file_path("event/data/01.xml").c_str()) );
    }

    CHECK( is_same_as_file(ostr, "event/data/01.out") );
}


/*
 * make sure that CDATA text goes to the text callback when there is no
 * cdata callback.
 */

TEST_CASE_METHOD( SrcdirConfig, "event/cdata_to_text_fallback", "[event]" )
{
    struct test_parser : public xml::event_parser
    {
        bool start_element(const std::string&, const xml::event_parser::attrs_type&) override
        {
            return true;
        }

        bool end_element(const std::string&) override
        {
            return true;
        }

        bool text(const std::string &contents) override
        {
            text_ += contents;
            return true;
        }

        std::string text_;
    };

    test_parser parser;
    CHECK( parser.parse_file(test_file_path("event/data/01.xml").c_str()) );

    CHECK_THAT( parser.text_, Catch::Matchers::Contains("inside two_dot_two") );
}


/*
 * make sure that returning false or throwing an exception will cause the
 * parser to stop.
 */

namespace
{

// parser that tests failures (either exceptions or throwing); _calls_count
// should be 0 after an error occured
struct failing_parser : public xml::event_parser
{
    failing_parser(const char *callback_to_test, bool should_throw)
        : callback_to_test_(callback_to_test),
          should_throw_(should_throw)
    {}

    bool start_element(const std::string&, const xml::event_parser::attrs_type&) override
        { return handle("start_element"); }
    bool end_element(const std::string&) override
        { return handle("end_element"); }
    bool text(const std::string&) override
        { return handle("text"); }
    bool processing_instruction(const std::string&, const std::string&) override
        { return handle("processing_instruction"); }
    bool comment(const std::string&) override
        { return handle("comment"); }
    bool cdata(const std::string&) override
        { return handle("cdata"); }

    bool handle(const char *callback)
    {
        calls_count_++;

        if ( callback_to_test_ != callback )
            return true; // nothing else to do

        // watch for further calls to any of the callbacks:
        calls_count_ = 0;

        // simulate error:
        if ( should_throw_ )
            throw std::runtime_error(callback);
        else
            return false;
    }

    bool called_after_error() const { return calls_count_ > 0; }

    int calls_count_{0};
    std::string callback_to_test_;
    bool should_throw_;
};

void do_test_parser(const char *callback, bool should_throw)
{
    failing_parser parser(callback, should_throw);

    CHECK( !parser.parse_file(test_file_path("event/data/01.xml").c_str()) );
    CHECK( !parser.called_after_error() );
}

} // anonymous namespace

TEST_CASE_METHOD( SrcdirConfig, "event/stop_on_error_in_start_element", "[event]" )
{
    do_test_parser("start_element", true);
    do_test_parser("start_element", false);
}

TEST_CASE_METHOD( SrcdirConfig, "event/stop_on_error_in_end_element", "[event]" )
{
    do_test_parser("end_element", true);
    do_test_parser("end_element", false);
}

TEST_CASE_METHOD( SrcdirConfig, "event/stop_on_error_in_text", "[event]" )
{
    do_test_parser("text", true);
    do_test_parser("text", false);
}

TEST_CASE_METHOD( SrcdirConfig, "event/stop_on_error_in_processing_instruction", "[event]" )
{
    do_test_parser("processing_instruction", true);
    do_test_parser("processing_instruction", false);
}

TEST_CASE_METHOD( SrcdirConfig, "event/stop_on_error_in_comment", "[event]" )
{
    do_test_parser("comment", true);
    do_test_parser("comment", false);
}

TEST_CASE_METHOD( SrcdirConfig, "event/stop_on_error_in_cdata", "[event]" )
{
    do_test_parser("cdata", true);
    do_test_parser("cdata", false);
}
