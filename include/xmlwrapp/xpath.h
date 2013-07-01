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

  This files contains definitions for the xml::xpath namespace.
 */

#ifndef _xmlwrapp_xpath_h__
#define _xmlwrapp_xpath_h_


// xmlwrapp includes
#include "xmlwrapp/init.h"
#include "xmlwrapp/export.h"
#include "xmlwrapp/nodes_view.h"

namespace xml
{
    namespace impl
    {
        struct xpitimpl;
    }

    class node;
    class document;

    /**
      Contains classes to use XPath
     */
    namespace xpath
    {

        class node_set;

        /**
          A context in which XPath-Expressions can be evaluated.

          This class is neccessary because every namespace that is used
          in the query has to be registered (with prefix) in a context-class.
         */
        class XMLWRAPP_API context
        {
            public:
                context(const xml::document& doc);

                ~context();

                /**
                  Registers a namespace with prefix.
                  @param prefix The prefix used in the expression, not per se the same as in the document.
                  @param href The href of the namespace used in the document.
                 */
                void register_namespace(const char* prefix, const char* href);

                /**
                  Executes a query, namely <tt>expr</tt>.
                  @return A Node-Set which can be iterated over
                 */
                const_nodes_view evaluate(const char* expr);

            private:
                void* ctxtptr;
                context(const context&); context operator=(context&);
        };

    }
}

#endif // _xmlwrapp_xpath_h_
