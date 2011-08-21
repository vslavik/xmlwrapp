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

    This files containts definitions for the xml::namespacedefinitions class.
*/

#ifndef _xmlwrapp_namespaces_h_
#define _xmlwrapp_namespaces_h_

#include "xmlwrapp/node.h"

#include <string>

namespace xml
{

    // Forward declarations
    namespace impl
    {
        class nsdefs_impl;
        class nsdef_impl;
        struct nsdef_it_impl;
        struct node_impl;
    }


    /**
      Holds information about namespace-definitions on this element

      @code
      <a xmlns:b="http://example.org"><b:c /></a>
      @endcode
     */
    class namespacedefinitions
    {
        public:
            /**
              Constructs a collection by node

              \param nod The parent node
             */
            namespacedefinitions (const node& nod);


            /**
              Deconstructs the whole thing
             */
            ~namespacedefinitions();

            /** 
              Holds information about a specific namespace

              @code
              <c:a />
              @endcode
             */
            class nsdef
            {
                public:
                    const char* get_href() const;
                    const char* get_prefix() const;
                private:
                    // handle to impl
                    impl::nsdef_impl* impl;

                    nsdef(void* data);
                    ~nsdef();
                    void set_data(void* data);

                    std::string href; std::string prefix;

                    // do not copy / assign
                    nsdef (const nsdef&);
                    nsdef& operator= (const nsdef& o);

                    friend class impl::nsdef_it_impl;
            };


            /**
              Allows to iterate over namespace-definitions
             */
            class iterator
            { 
                public:
                    typedef nsdef value_type;

                    value_type& operator*();
                    value_type* operator->();

                    iterator(); // empty
                    iterator(const iterator& other);
                    iterator& operator= (const iterator& other);
                    ~iterator();

                    // prefix increment
                    iterator& operator++();

                    // postfix increment (avoid!)
                    iterator operator++(int);

                    friend bool XMLWRAPP_API operator== (const iterator& lhs, const iterator& rhs);
                    friend bool XMLWRAPP_API operator!= (const iterator& lhs, const iterator& rhs);

                private:
                    iterator(void* ns);
                    friend class impl::nsdefs_impl;

                    impl::nsdef_it_impl* impl;

                    void* get_ns() const;
            };

            /**
              Gets an iterator pointing to the start of namespace definitions.
             */
            iterator begin();

            /**
              Gets an iterator pointing past the end of namespace definitions.
             */
            iterator end();

            /**
              Find out if there are namespace definitions in this element.

              @return True there are no definitions
              @return False there are definitions
             */
            bool empty() const;

        private:
            impl::nsdefs_impl* impl;
            friend class impl::node_impl;
            friend class xml::node;

            // empty init
            explicit namespacedefinitions(int);

            void set_data (void* data);
    };


} // namespace xml

#endif // _xmlwrapp_namespaces_h_
