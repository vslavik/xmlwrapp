/*
 * Copyright (C) 2011 Jonas Weber (mail@jonasw.de)
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
                node_set evaluate(const char* expr);

            private:
                void* ctxtptr;
                context(const context&); context operator=(context&);
        };


        /**
          A set of nodes being the result of an xpath-query
         */
        class XMLWRAPP_API node_set
        {
            public:
                /**
                  An iterator to a xml::node in a xml::node_set
                 */
                class XMLWRAPP_API iterator
                {
                    public:
                        xml::node& operator* ();
                        xml::node* operator->();

                        /**
                          increments the iterator
                         */
                        iterator& operator++();

                        /**
                          postifx increment: increments the iterator (avoid if possible)
                         */
                        iterator operator++(int); // postfix -- avoid

                        // TODO: rly?
                        iterator& operator--();
                        iterator operator--(int); // postfix -- avoid

                        friend bool XMLWRAPP_API xml::xpath::operator==(const iterator& l, const iterator& r);
                        friend bool XMLWRAPP_API xml::xpath::operator!=(const iterator& l, const iterator& r);

                        iterator& operator=(const iterator& i);
                        iterator(const iterator& i);
                        ~iterator();
                    private:
                        iterator(void* data, int pos);
                        void* data; int pos;
                        impl::xpitimpl* pimpl_;
                        friend class node_set;
                };

                /**
                  Returns an iterator which points to the first element in the list.
                 */
                iterator begin();

                /**
                  Returns an iterator which points past the last element in the list.
                 */
                iterator end();

                /**
                  Returns whether the set contains elements
                 */
                bool empty() const;

                /**
                  Returns the number of elements in the set.
                  @see empty()
                 */
                int count() const;

                /**
                  Returns whether the result-set contains the node.
                 */
                bool contains(const xml::node& node) const;

                ~node_set();
            private:
                void* data;
                node_set(void* data);
                //node_set(const node_set&); node_set& operator=(node_set&);
                friend class context;
        };

    }

}

#endif // _xmlwrapp_xpath_h_
