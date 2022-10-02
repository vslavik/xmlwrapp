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

    This file contains the definition of the xml::schema class.
 */

#ifndef _xmlwrapp_schema_h_
#define _xmlwrapp_schema_h_

// xmlwrapp includes
#include "xmlwrapp/init.h"
#include "xmlwrapp/export.h"
#include "xmlwrapp/errors.h"

#include <memory>

XMLWRAPP_MSVC_SUPPRESS_DLL_MEMBER_WARN

namespace xml
{

// forward declarations
class document;
class error_messages;

namespace impl
{
struct schema_impl;
}

/**
    XML Schema.

    This class is used to validate documents against XML Schema.

    @since 0.7.0
 */
class XMLWRAPP_API schema
{
public:
    /**
        Parses XML Schema document and creates schema instance from it.

        Errors are handled by @a on_error handler; by default, xml::exception
        is thrown on errors. If there's a fatal error that prevents the schema
        from being loaded and the error handler doesn't throw an exception, the
        constructor will throw xml::exception anyway.
     */
    explicit schema(const document& doc, error_handler& on_error = throw_on_error);

    /// Destructor
    ~schema();

    /**
        Validates the document @a doc against the schema.

        Errors are handled by @a on_error handler; by default, xml::exception
        is thrown on errors.

        @return `true` if the document is valid with regard to the schema,
                `false` otherwise.
     */
    bool validate(const document& doc, error_handler& on_error = throw_on_error) const;

private:
    std::unique_ptr<impl::schema_impl> pimpl_;

    // Schema class is not copyable
    schema(const schema&) = delete;
    schema& operator=(const schema&) = delete;
};

} // namespace xml

XMLWRAPP_MSVC_RESTORE_DLL_MEMBER_WARN

#endif // _xmlwrapp_schema_h_
