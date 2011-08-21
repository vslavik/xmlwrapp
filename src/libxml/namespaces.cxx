/*
 * Copyright (C) 2011 Jonas Weber <mail@jonasw.de>
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


//xmlwrapp includes
#include "xmlwrapp/namespaces.h"
#include "xmlwrapp/exception.h"
#include "pimpl_base.h"

// std
#include <new>
#include <algorithm>

// libxml2
#include <libxml/tree.h>

namespace xml
{
    namespace impl
    {


        ///////
        // nsdefs_impl
        ///////
        struct nsdefs_impl : public pimpl_base<nsdefs_impl>
        {
            xmlNodePtr node_;
            nsdefs_impl() : node_(NULL) { };

            void set_data(void* data) { node_ = reinterpret_cast<xmlNodePtr> (data); }

            xml::namespacedefinitions::iterator begin()
            {
                if (node_ == NULL)
                    return xml::namespacedefinitions::iterator(NULL);
                else
                    return xml::namespacedefinitions::iterator(node_->nsDef);
            }
            xml::namespacedefinitions::iterator end()
            {
                return xml::namespacedefinitions::iterator(NULL);
            }
        };

        struct nsdef_it_impl : public pimpl_base<nsdef_it_impl>
        {
            xmlNsPtr data;
            xml::namespacedefinitions::nsdef myval;

            nsdef_it_impl(void* dt) : myval(dt)
            {
                data = reinterpret_cast<xmlNsPtr>(dt);
            };

            void* get_data() const { return reinterpret_cast<void*> (data); };
            void set_data(void* dt) { data = reinterpret_cast<xmlNsPtr> (dt); myval.set_data(data); };

            void go_next()
            {
                data = data->next;
                myval.set_data(data);

            }
        };
    } // namespace impl

    //////
    // iterator
    //////

    namespacedefinitions::iterator::iterator (void *ns)
    {
        impl = new impl::nsdef_it_impl(ns);
    }
    namespacedefinitions::iterator::~iterator()
    {
        delete impl;
    }

    namespacedefinitions::iterator::value_type& namespacedefinitions::iterator::operator*()
    { return impl->myval; }

    namespacedefinitions::iterator::value_type* namespacedefinitions::iterator::operator->()
    { return &(impl->myval); }

    namespacedefinitions::iterator& namespacedefinitions::iterator::operator++()
    {
        impl->go_next();
        return *this;
    }

    namespacedefinitions::iterator namespacedefinitions::iterator::operator++(int)
    {
        iterator tmp = *this;
        impl->go_next();
        return tmp;
    }

    namespacedefinitions::iterator::iterator(const namespacedefinitions::iterator& other)
    {
        impl = new impl::nsdef_it_impl (other.get_ns());
    }

    //TODO: copy & assign

    bool XMLWRAPP_API operator== (const namespacedefinitions::iterator& lhs, const namespacedefinitions::iterator& rhs)
    {
        return (lhs.get_ns() == rhs.get_ns());
    }

    bool XMLWRAPP_API operator!=(const namespacedefinitions::iterator& lhs, const namespacedefinitions::iterator& rhs)
    {
        return (lhs.get_ns() != rhs.get_ns());
    }

    void* namespacedefinitions::iterator::get_ns() const { return impl->get_data(); }



    /////
    // nsdef
    /////

    namespacedefinitions::nsdef::nsdef(void* data)
    {
        set_data(data);
    }
    namespacedefinitions::nsdef::~nsdef()
    {
    }
    void namespacedefinitions::nsdef::set_data(void* data)
    {
        if (data == NULL)
        {
            href = "";
            prefix = "";
            return;
        }
        xmlNsPtr ns = reinterpret_cast<xmlNsPtr> (data);

        href = reinterpret_cast<const char*> (ns->href);
        if (ns->prefix != NULL)
            prefix = reinterpret_cast<const char*> (ns->prefix);
        else
            prefix = "";
    }
    const char* namespacedefinitions::nsdef::get_href() const
    {
        return href.c_str();
    }

    const char* namespacedefinitions::nsdef::get_prefix() const
    {
        return prefix.c_str();
    }



    namespacedefinitions::namespacedefinitions(int) : impl(NULL)
    {
    };



    namespacedefinitions::~namespacedefinitions()
    {
        if (impl != NULL)
            delete impl;
    }

    void namespacedefinitions::set_data(void* data)
    {
        if (impl == NULL)
            impl = new impl::nsdefs_impl();
        impl->set_data(data);
    }

    namespacedefinitions::iterator namespacedefinitions::begin()
    {
        return impl->begin();
    }

    namespacedefinitions::iterator namespacedefinitions::end()
    {
        return impl->end();
    }



} // namespace xml
