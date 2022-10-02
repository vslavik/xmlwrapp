/*
 * Copyright (C) 2011 Jonas Weber (mail@jonasw.de)
 *               2013 Vaclav Slavik <vslavik@gmail.com>
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

  This files contains definitions for XPath-related classes.
 */

#ifndef _xmlwrapp_xpath_h_
#define _xmlwrapp_xpath_h_


// xmlwrapp includes
#include "xmlwrapp/init.h"
#include "xmlwrapp/errors.h"
#include "xmlwrapp/export.h"
#include "xmlwrapp/nodes_view.h"

#include <memory>
#include <string>

namespace xml
{

class node;
class document;

namespace impl
{
struct xpath_context_impl;
}

/**
    Context in which XPath expressions can be evaluated.

    The context object can (and indeed, should) be reused for multiple XPath
    queries on the same document.

    @since 0.8.0
 */
class XMLWRAPP_API xpath_context
{
public:
    /**
        Create XPath context for the given document.

        @param doc  Document for the context to work with. The lifetime
                    of the document must be greater than the context object's.
     */
    xpath_context(const xml::document& doc);

    ~xpath_context();

    /**
        Register a namespace with prefix.

        This function has to be called in order to be able to evaluate XPath
        expressions that match nodes in a non-default namespace. Must be
        called before evaluate().

        @param prefix  The prefix used in XPath expressions for the namespace.
                       (Notice that it doesn't have to be the same prefix as
                       used in the XML document.)
        @param href    URI of the namespace used in the document.
     */
    void register_namespace(const std::string& prefix, const std::string& href);

    /**
        Execute an XPath query in the document scope.

        Calling this function is exactly equivalent to using the overload
        taking an xml::node argument with xml::document::get_root_node().

        In particular, this implies that if you need to modify nodes in the
        returned set, you can simple use the non-const overload taking
        xml::node& argument and pass xml::document::get_root_node() result to
        it.
     */
    const_nodes_view evaluate(const std::string& expr,
                              error_handler& on_error = throw_on_error);

    /**
        Execute an XPath query in the scope of XML node @a n.

        @param  expr  XPath expression.
        @param  n     The context node for the expression.
        @param  on_error Error handler throwing an exception if an error occurs
            during the expression evaluation. Notice that absence of matches
            isn't considered to be an error, in this case the function just
            returns an empty set.

        @return Const set of matching nodes.
     */
    const_nodes_view evaluate(const std::string& expr,
                              const xml::node& n,
                              error_handler& on_error = throw_on_error);

    /**
        Execute an XPath query in the scope of XML node @a n.

        This overload is identical to the one taking const @a n argument,
        except that it returns a set of nodes that can be modified.
     */
    nodes_view evaluate(const std::string& expr,
                        xml::node& n,
                        error_handler& on_error = throw_on_error);

private:
    // no copying
    xpath_context(const xpath_context&) = delete;
    xpath_context& operator=(const xpath_context&) = delete;

    std::unique_ptr<impl::xpath_context_impl> pimpl_;
};

} // namespace xml

#endif // _xmlwrapp_xpath_h_
