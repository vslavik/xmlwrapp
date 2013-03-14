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

  This files contains definitions for the xml::namespaces class.
*/

#ifndef _xmlwrapp_namespaces_h_
#define _xmlwrapp_namespaces_h_


// xmlwrapp includes
#include "xmlwrapp/init.h"
#include "xmlwrapp/export.h"


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
    class node;
    class attributes;

    /**
        The xml::namespaces class and its subclasses are used to access XML-Namespaces.
    */
    class XMLWRAPP_API namespaces
    {
        public:

            class definitions;

            /** 
              Holds information about a specific namespace
             */
            class XMLWRAPP_API ns
            {

                public:
                    /**
                      Gets the Href of this namespace,
                      like http://www.w3.org/1999/xhtml .
                      @return Temporary const char*.
                     */
                    const char* get_href() const;

                    /**
                      Gets the prefix of this namespace.
                      @return Temporary const char*.
                     */
                    const char* get_prefix() const;

                    /**
                      Constructs new namespace-object,
                      for example to define on an element.
                     */
                    ns(const char* href, const char* prefix);

                    ns (const ns&);
                    ns& operator= (const ns& o);

                    ~ns();

                    /**
                      For compatibility: auto-convert to const char*, returning href
                    */
                    operator const char* () const;
                private:
                    // handle to impl
                    impl::nsdef_impl* impl;

                    ns(void* data);
                    void set_data(void* data);

                    std::string href; std::string prefix;


                    friend class impl::nsdef_it_impl;
                    friend class xml::node;
                    friend class xml::attributes;
                    friend class xml::namespaces;
            };
            /**
              Allows to iterate over namespace-definitions
             */
            class XMLWRAPP_API iterator
            { 
                public:
                    typedef xml::namespaces::ns value_type;

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
                    friend class xml::namespaces::definitions;
                    friend class xml::node;
                    friend class xml::namespaces;

                    impl::nsdef_it_impl* impl;

                    void* get_ns() const;
            };

            /**
              Holds information about namespace-definitions on this element

              @code
              <a xmlns:b="http://example.org"><b:c /></a>
              @endcode
             */
            class XMLWRAPP_API definitions
            {
                public:
                    /*
                       Constructs a collection by node

                       \param nod The parent node
                     */
                    //definitions (const node& nod);


                    /**
                      Deconstructs the whole thing
                     */
                    ~definitions();



                    /**
                      Gets an iterator pointing to the start of namespace definitions.
                     */
                    iterator begin();

                    /**
                      Gets an iterator pointing past the end of namespace definitions.
                     */
                    iterator end();

                    /**
                      Puts a new namespace definition in the element.
                     */
                    void push_back(const xml::namespaces::ns& ns);

                    /**
                        Finds a namespace-definition based on prefix.
                        @return an iterator to the namespace
                        @see xml::namespaces::find_prefix
                    */
                    iterator find (const char* prefix);

                    /**
                        Finds a namespace-definition based on href.
                        @return an iterator to the namespace
                        @see xml::namespaces::find
                    */
                    iterator find_href (const char* href);

                    /**
                        Erases a namespace-definition on this element.
                    */
                    void erase(iterator it);

                    /**
                      Find out if there are namespace definitions in this element.

                      @return True there are no definitions
                      @return False there are definitions
                     */
                    bool empty() const;

                private:
                    impl::nsdefs_impl* impl;
                    friend class xml::impl::node_impl;
                    friend class xml::node;

                    // empty init
                    explicit definitions(int);

                    void set_data (void* data);
            };

            /** 
              Find a namespace available on this element by prefix.
              @param href The Href specified for this namespace.
              @return An object containing information about this namespace.
              @throws xml::exception If namespace is not found.
             */
            iterator find(const char* href);

            /**
              Find a namespace available on this element by href.
              @param prefix The prefix of this namespace.
              @return An object containing information about this namespace.
              @throws xml::exception If namespace is not found.
             */
            iterator find_prefix(const char* prefix);


            /**
              Returns an iterator to the end of the list.
              Its occurrence signals a missing/not defined namespace.
            */
            iterator end();


            /**
              Returns a definition for the empty namespace (no prefix, no url)
            */
            static ns get_default();

        private:
            explicit namespaces(const node& n);
            namespaces(const namespaces&);
            namespaces& operator=(const namespaces&);
            explicit namespaces(int); // empty
            void* data;
            void set_data(void* d);


            friend class xml::node;
            friend class xml::impl::node_impl;
    };
} // namespace xml

#endif // _xmlwrapp_namespaces_h_
