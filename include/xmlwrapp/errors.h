/*
 * Copyright (C) 2010-2013 Vaclav Slavik <vslavik@gmail.com>
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

/**
    @file

    This file contains errors-handling classes: xml::exception and
    xml::error_handler and derived classes.
 */

#ifndef _xmlwrapp_errors_h_
#define _xmlwrapp_errors_h_

// xmlwrapp includes
#include "xmlwrapp/export.h"

#include <stdexcept>
#include <string>
#include <list>

/// XML library namespace
namespace xml
{

class error_messages;

/**
    This exception class is thrown by xmlwrapp for all runtime XML-related
    errors.

    @note C++ runtime may still thrown other errors when used from xmlwrapp.
          Also, std::bad_alloc() is thrown in out-of-memory situations.

    @since 0.7.0
 */
class XMLWRAPP_API exception : public std::runtime_error
{
public:
    explicit exception(const std::string& what);
    explicit exception(const error_messages& what);
};


/**
    The xml::error_handler class is used to handle libxml2 errors and warnings
    emitted during parsing, validation etc.

    Although you can derive a custom handler from it, the specializations
    included in xmlwrapp should suffice for most uses: throw_on_error and
    throw_on_error_or_warning throw on issues, error_messages collects errors
    and warnings without throwing.

    @since 0.7.0
 */
class XMLWRAPP_API error_handler
{
public:
    virtual ~error_handler() {}

    /// Called by xmlwrapp to report an error.
    virtual void on_error(const std::string& msg) = 0;

    /// Called by xmlwrapp to report a warning.
    virtual void on_warning(const std::string& msg) = 0;
};


/**
    An error handler that ignores both errors and warnings.

    @see ignore_errors
 */
class error_handler_ignore_errors : public error_handler
{
public:
    void on_error(const std::string&) {}
    void on_warning(const std::string&) {}
};

/**
    Specialization of error_handler that throws on any error.

    @see throw_on_error
 */
class error_handler_throw_on_error : public error_handler
{
public:
    void on_error(const std::string& msg) { throw exception(msg); }
    void on_warning(const std::string&) {}
};

/**
    Specialization of error_handler that throws on any error or warning.

    @see throw_on_error_or_warning
 */
class error_handler_throw_on_error_or_warning : public error_handler_throw_on_error
{
public:
    void on_warning(const std::string& msg) { on_error(msg); }
};

/// Error handler ignoring all errors, its use is strongly discouraged.
extern XMLWRAPP_API error_handler_ignore_errors              ignore_errors;

/// Error handler object that throws on any error.
extern XMLWRAPP_API error_handler_throw_on_error             throw_on_error;

/// Error handler object that throws on any error or warning.
extern XMLWRAPP_API error_handler_throw_on_error_or_warning  throw_on_error_or_warning;


/**
    Single message in error_messages.

    @since 0.7.0
 */
class XMLWRAPP_API error_message
{
public:
    /// A type for different type of errors
    enum message_type
    {
        type_error,         ///< error
        type_warning        ///< warning
    };

    /**
        Create a new xml::error_message object.

        @param err_msg  The error message.
        @param msg_type The error type.
     */
    error_message(const std::string& err_msg, message_type msg_type)
        : message_(err_msg), type_(msg_type)
    {}

    /// Get the error message type.
    message_type type() const { return type_; }

    /// Get the error message.
    const std::string& message() const { return message_; }

private:
    std::string  message_;
    message_type type_;
};


/**
    The xml::error_messages class is used to store all the error messages
    which are collected while parsing or validating an XML document.

    @since 0.7.0
 */
class XMLWRAPP_API error_messages : public error_handler
{
public:
    /// A type to store multiple messages
    typedef std::list<error_message> messages_type;

    error_messages() : has_errors_(false), has_warnings_(false) {}

    /// Get the error messages.
    const messages_type& messages() const { return messages_; }

    /**
        Convenience function to find if there are any messages at all.
     */
    bool empty() const
    {
        return messages_.empty();
    }

    /**
        Check if there are warnings in the error messages.

        @return true if there is at least one warning in the error messages.
                It does not consider errors.
     */
    bool has_warnings() const { return has_warnings_; }

    /**
        Check if there are any errors.
     */
    bool has_errors() const { return has_errors_; }


    /**
        Convert error messages into a single printable string.

        The returned string is typically multiline, with the messages
        separated with newlines ('\n').
     */
    std::string print() const;



    // Implementation of error_handler methods:
    void on_error(const std::string& msg);
    void on_warning(const std::string& msg);

protected:
    /// Called by print() to format a single message.
    virtual std::string format_for_print(const error_message& msg) const;

private:
    messages_type messages_;
    bool          has_errors_;
    bool          has_warnings_;
};


} // namespace xml

#endif // _xmlwrapp_errors_h_
