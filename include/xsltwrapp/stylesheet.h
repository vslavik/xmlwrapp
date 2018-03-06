/*
 * Copyright (C) 2001-2003 Peter J Jones (pjones@pmade.org)
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

   This file contains the definition of the xslt::stylesheet class.
 */

#ifndef _xsltwrapp_stylesheet_h_
#define _xsltwrapp_stylesheet_h_

// xmlwrapp includes
#include "xsltwrapp/init.h"
#include "xmlwrapp/document.h"
#include "xmlwrapp/export.h"
#include "xmlwrapp/errors.h"

// standard includes
#include <map>
#include <string>

namespace xslt
{

/**
    The xslt::stylesheet class is used to hold information about an XSLT
    stylesheet. You can use it to load in a stylesheet and then use that
    stylesheet to transform an XML document to something else.
 */
class XSLTWRAPP_API stylesheet
{
public:
    struct pimpl;

    /// Type for passing parameters to the stylesheet
    typedef std::map<std::string, std::string> param_type;

    /**
        Create a new xslt::stylesheet object and load and parse the
        stylesheet in the given filename.

        Errors are handled by @a on_error handler; by default, xml::exception
        is thrown on errors. If there's a fatal error that prevents the
        stylesheet from being loaded and the error handler doesn't throw an
        exception, the constructor will throw xml::exception anyway.

        @param filename The name of the file that contains the stylesheet.
        @param on_error Handler called to process errors and warnings (since 0.7.0).
     */
    explicit stylesheet(const char *filename,
                        xml::error_handler& on_error = xml::throw_on_error);

    /**
        Create a new xslt::stylesheet object from an xml::document object
        that contains the parsed stylesheet. The given xml::document is
        passed by value. This is needed because the stylesheet will own the
        document and free it.

        Errors are handled by @a on_error handler; by default, xml::exception
        is thrown on errors. If there's a fatal error that prevents the
        stylesheet from being loaded and the error handler doesn't throw an
        exception, the constructor will throw xml::exception anyway.

        @param doc The parsed stylesheet.
        @param on_error Handler called to process errors and warnings (since 0.7.0).
     */
    explicit stylesheet(xml::document doc,
                        xml::error_handler& on_error = xml::throw_on_error);

    /**
        Clean up after an xslt::stylesheet.
     */
    ~stylesheet();

    /**
        Apply this stylesheet to the given XML document. The result document
        is placed in the second document parameter.

        @param doc The XML document to transform.
        @param result The result tree after applying this stylesheet.

        @return True if the transformation was successful and the results placed in result.
        @return False if there was an error, result is not modified.

        @deprecated Use the form that takes xml::error_handler argument.
     */
    XMLWRAPP_DEPRECATED("use the form that takes error_handler argument")
    bool apply(const xml::document& doc, xml::document& result);

    /**
        Apply this stylesheet to the given XML document. The result document
        is placed in the second document parameter.

        @param doc The XML document to transform.
        @param result The result tree after applying this stylesheet.
        @param with_params Override xsl:param elements using the given key/value map

        @return True if the transformation was successful and the results placed in result.
        @return False if there was an error, result is not modified.

        @deprecated Use the form that takes xml::error_handler argument.
     */
    XMLWRAPP_DEPRECATED("use the form that takes error_handler argument")
    bool apply(const xml::document& doc, xml::document& result, const param_type& with_params);

    /**
        Apply this stylesheet to the given XML document. The result document
        is placed in the second document parameter.

        @param doc The XML document to transform.
        @param result The result tree after applying this stylesheet.
        @param on_error Handler called to process errors and warnings (since 0.7.0).

        @return True if the transformation was successful and the results placed in result.
        @return False if there was an error, result is not modified.
     */
    bool apply(const xml::document& doc,
               xml::document& result,
               xml::error_handler& on_error);

    /**
        Apply this stylesheet to the given XML document. The result document
        is placed in the second document parameter.

        @param doc The XML document to transform.
        @param result The result tree after applying this stylesheet.
        @param with_params Override xsl:param elements using the given key/value map
        @param on_error Handler called to process errors and warnings (since 0.7.0).

        @return True if the transformation was successful and the results placed in result.
        @return False if there was an error, result is not modified.
     */
    bool apply(const xml::document& doc,
               xml::document& result,
               const param_type& with_params,
               xml::error_handler& on_error);

    /**
        Apply this stylesheet to the given XML document. The results document
        is returned. If there is an error during transformation, this
        function will throw a xml::exception exception.

        Each time you call this member function, the xml::document object
        that was returned from the last call becomes invalid. That is, of
        course, unless you copied it first.

        @param doc The XML document to transform.
        @param on_error Handler called to process errors and warnings (since 0.7.0).

        @return A reference to the result tree.
     */
    xml::document& apply(const xml::document& doc,
                         xml::error_handler& on_error = xml::throw_on_error);

    /**
        Apply this stylesheet to the given XML document. The results document
        is returned. If there is an error during transformation, this
        function will throw a xml::exception exception.

        Each time you call this member function, the xml::document object
        that was returned from the last call becomes invalid. That is, of
        course, unless you copied it first.

        @param doc The XML document to transform.
        @param with_params Override xsl:param elements using the given key/value map
        @param on_error Handler called to process errors and warnings (since 0.7.0).

        @return A reference to the result tree.
     */
    xml::document& apply(const xml::document& doc,
                         const param_type& with_params,
                         xml::error_handler& on_error = xml::throw_on_error);

    /**
        If you used one of the xslt::stylesheet::apply member functions that
        return a bool, you can use this function to get the text message for
        the transformation error.

        If you are using one of the apply member functions that throws
        exceptions, this function should not be used. The text message for
        the transformation error will be given to the xml::exception
        constructor.

        @return The last error message.

        @deprecated Use apply() variants that take xml::error_handler argument.
     */
    XMLWRAPP_DEPRECATED("use apply() variants that take error_handler argument")
    const std::string& get_error_message() const;

private:
    void init(xml::document& doc, xml::error_handler& on_error);

private:
    pimpl *pimpl_;

    // an xslt::stylesheet cannot yet be copied or assigned to.
    stylesheet(const stylesheet&);
    stylesheet& operator=(const stylesheet&);
}; // end xslt::stylesheet class

} // end xslt namespace

#endif // _xsltwrapp_stylesheet_h_
