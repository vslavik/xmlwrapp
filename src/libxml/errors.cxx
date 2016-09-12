/*
 * Copyright (C) 2013 Vaclav Slavik <vslavik@gmail.com>
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
 *    the schemaation and/or other materials provided with the
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

// xmlwrapp includes
#include "xmlwrapp/errors.h"

#include "utility.h"
#include "errors_impl.h"

namespace xml
{

error_handler_ignore_errors              ignore_errors;
error_handler_throw_on_error             throw_on_error;
error_handler_throw_on_error_or_warning  throw_on_error_or_warning;

// ------------------------------------------------------------------------
// xml::exception
// ------------------------------------------------------------------------

exception::exception(const std::string& what) : std::runtime_error(what)
{
}

exception::exception(const error_messages& what) : std::runtime_error(what.print())
{
}

// ------------------------------------------------------------------------
// xml::error_messages
// ------------------------------------------------------------------------

void error_messages::on_error(const std::string& msg)
{
    messages_.push_back(error_message(msg, error_message::type_error));
    has_errors_ = true;
}

void error_messages::on_warning(const std::string& msg)
{
    messages_.push_back(error_message(msg, error_message::type_warning));
    has_warnings_ = true;
}

std::string error_messages::print() const
{
    std::string buffer;

    messages_type::const_iterator begin(messages_.begin());
    messages_type::const_iterator end(messages_.end());
    for (messages_type::const_iterator k = begin; k != end; ++k)
    {
        if (k != begin)
            buffer += "\n";
        buffer += format_for_print(*k);
    }

    return buffer;
}

std::string error_messages::format_for_print(const error_message& msg) const
{
    switch (msg.type())
    {
        case error_message::type_error:
            return "error: " + msg.message();
        case error_message::type_warning:
            return "warning: " + msg.message();
    }

    return msg.message(); // silence bogus gcc warning
}

// ------------------------------------------------------------------------
// libxml2 callbacks interface
// ------------------------------------------------------------------------

namespace impl
{

// Notice that these callbacks are called from C code, not C++. Although
// throwing exceptions through C code does work in some implementations, it
// is not guaranteed to. We therefore need to make sure no exception
// propagates outside of the callbacks by catching them.
//
// This is also the reason for errors_collector's existence: we cannot safely
// call error_handler's methods, because they may throw (it is in fact often
// desirable). So we collect the errors and "replay" them after returning from
// C code.

extern "C" void cb_messages_warning_v(void *out, const char *message, va_list ap)
{
    try
    {
        error_messages *err = static_cast<error_messages*>(out);

        std::string text;
        printf2string(text, message, ap);
        err->on_warning(text);
    }
    catch (...) {}
}

extern "C" void cb_messages_error_v(void *out, const char *message, va_list ap)
{
    try
    {
        error_messages *err = static_cast<error_messages*>(out);

        std::string text;
        printf2string(text, message, ap);
        err->on_error(text);
    }
    catch (...) {}
}

extern "C" void cb_messages_warning(void *out, const char *message, ...)
{
    CALL_CB_MESSAGES_WARNING(out, message);
}

extern "C" void cb_messages_error(void *out, const char *message, ...)
{
    CALL_CB_MESSAGES_ERROR(out, message);
}


void errors_collector::replay(error_handler& dest)
{
    const messages_type& msg = messages();

    for ( messages_type::const_iterator i = msg.begin();
          i != msg.end();
          ++i )
    {
        switch ( i->type() )
        {
            case error_message::type_error:
                dest.on_error(i->message());
                break;
            case error_message::type_warning:
                dest.on_warning(i->message());
                break;
        }
    }
}

std::string errors_collector::format_for_print(const error_message& msg) const
{
    // The errors_collector class is also used to provide backward
    // compatibility errors reporting in tree_parser and xslt::stylesheet. The
    // "error: " prefix wasn't included there, so don't add it now either so
    // that we don't break user UI output. On the other hand, include "warning:
    // " to make it clear these aren't errors.

    switch (msg.type())
    {
        case error_message::type_error:
            return msg.message();
        case error_message::type_warning:
            return "warning: " + msg.message();
    }

    return msg.message(); // silence bogus gcc warning
}

// ----------------------------------------------------------------------------
// global_errors_installer
// ----------------------------------------------------------------------------

global_errors_installer::global_errors_installer(error_messages& on_error) :
    xml_error_orig_(xmlGenericError),
    xml_error_context_orig_(xmlGenericErrorContext)
{
    xmlSetGenericErrorFunc(&on_error, cb_messages_error);
}

global_errors_installer::~global_errors_installer()
{
    xmlSetGenericErrorFunc(xml_error_context_orig_, xml_error_orig_);
}

} // namespace impl

} // namespace xml
