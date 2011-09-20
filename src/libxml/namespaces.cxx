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
#include <string.h>

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

            xml::namespaces::iterator begin()
            {
                if (node_ == NULL)
                    return xml::namespaces::iterator(NULL);
                else
                    return xml::namespaces::iterator(node_->nsDef);
            }
            xml::namespaces::iterator end()
            {
                return xml::namespaces::iterator(NULL);
            }
        };

        struct nsdef_it_impl : public pimpl_base<nsdef_it_impl>
        {
            xmlNsPtr data;
            xml::namespaces::ns myval;

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
    namespaces::iterator::iterator()
    { impl = NULL; }

    namespaces::iterator::iterator (void *ns)
    {
        impl = new impl::nsdef_it_impl(ns);
    }
    namespaces::iterator::~iterator()
    {
        delete impl;
    }

    namespaces::iterator::value_type& namespaces::iterator::operator*()
    { 
        if (impl == NULL) throw xml::exception("iterator is at the end");
        return impl->myval;
    }
    namespaces::iterator::value_type* namespaces::iterator::operator->()
    { 
        if (impl == NULL) throw xml::exception("iterator is at the end");
        return &(impl->myval); 
    }
    namespaces::iterator& namespaces::iterator::operator++()
    {
        if (impl == NULL) return *this;
        impl->go_next();
        return *this;
    }

    namespaces::iterator namespaces::iterator::operator++(int)
    {
        iterator tmp = *this;
        if (impl != NULL) impl->go_next();
        return tmp;
    }

    namespaces::iterator::iterator(const namespaces::iterator& other)
    {
        impl = new impl::nsdef_it_impl (other.get_ns());
    }

    namespaces::iterator& namespaces::iterator::operator= (const namespaces::iterator& other)
    {
        delete impl;
        impl = new impl::nsdef_it_impl (other.get_ns());
        return *this;
    }

    bool XMLWRAPP_API operator== (const xml::namespaces::iterator& lhs, const xml::namespaces::iterator& rhs)
    {
        return (lhs.get_ns() == rhs.get_ns());
    }

    bool XMLWRAPP_API operator!=(const xml::namespaces::iterator& lhs, const xml::namespaces::iterator& rhs)
    {
        return (lhs.get_ns() != rhs.get_ns());
    }
    void* namespaces::iterator::get_ns() const 
    { 
        if (impl == NULL)
            return NULL;
        else
            return impl->get_data(); 
    }

    namespaces::namespaces(int) : data(NULL)
    {
    }

    namespaces::iterator namespaces::find(const char* href)
    {
        xmlNodePtr n = reinterpret_cast<xmlNodePtr>(data);
        xmlNsPtr p = xmlSearchNsByHref(n->doc, n, reinterpret_cast<const xmlChar*>(href));
        
        return namespaces::iterator(p);
    }

    namespaces::iterator namespaces::find_prefix(const char* prefix)
    {
        xmlNodePtr n = reinterpret_cast<xmlNodePtr>(data);
        xmlNsPtr p = xmlSearchNs(n->doc, n, reinterpret_cast<const xmlChar*>(prefix));
 
        return namespaces::iterator(p);
    }

    void namespaces::set_data(void* d)
    {
        data = d;
    }

    namespaces::iterator namespaces::end()
    {
        return namespaces::iterator(NULL);
    }
    /////
    // nsdef
    /////

    namespaces::ns::ns(void* data)
    {
        set_data(data);
    }
    namespaces::ns::ns(const char* href, const char* prefix)
    {
        if (prefix != NULL && href != NULL)
        {
            this->href = href;
            this->prefix = prefix;
        }
    }
    namespaces::ns::~ns()
    {
    }
    void namespaces::ns::set_data(void* data)
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
    const char* namespaces::ns::get_href() const
    {
        return href.c_str();
    }

    const char* namespaces::ns::get_prefix() const
    {
        return prefix.c_str();
    }



    namespaces::definitions::definitions(int) : impl(NULL)
    {
    };

    namespaces::ns namespaces::get_default()
    {
        return namespaces::ns(NULL, NULL);
    }

    namespaces::definitions::~definitions()
    {
        if (impl != NULL)
            delete impl;
    }

    void namespaces::definitions::set_data(void* data)
    {
        if (impl == NULL)
            impl = new impl::nsdefs_impl();
        impl->set_data(data);
    }

    namespaces::iterator namespaces::definitions::begin()
    {
        return impl->begin();
    }

    namespaces::iterator namespaces::definitions::end()
    {
        return impl->end();
    }

    bool namespaces::definitions::empty() const
    {
        return (impl->node_->nsDef == NULL);
    }

    void namespaces::definitions::push_back(const ns& ns)
    {
        xmlNsPtr newns = xmlNewNs (impl->node_, reinterpret_cast<const xmlChar*> (ns.get_href()), reinterpret_cast<const xmlChar*> (ns.get_prefix()));
	if (newns == NULL) throw xml::exception("creation of namespace failed");
    }

    namespaces::iterator namespaces::definitions::find (const char* prefix)
    {
        for (xmlNsPtr r = impl->node_->nsDef; r != NULL; r = r->next)
        {
            if (strcmp(reinterpret_cast<const char*> (r->prefix), prefix) == 0)
                return iterator(r);
        }
        return iterator();
    }
    namespaces::iterator namespaces::definitions::find_href (const char* href)
    {
        for (xmlNsPtr r = impl->node_->nsDef; r != NULL; r = r->next)
        {
            if (strcmp(reinterpret_cast<const char*> (r->href), href) == 0)
                return iterator(r);
        }
        return iterator();
//        xmlNsPtr ns = xmlSearchNsByHref (impl->node_->doc, impl->node_, reinterpret_cast<const xmlChar*> (href));
  //      return iterator(ns);
    }
    void namespaces::definitions::erase (namespaces::iterator it)
    {
        xmlNsPtr toeraseptr = reinterpret_cast<xmlNsPtr> (it.get_ns());

        // Is it the first definition ?
        if (impl->node_->nsDef == toeraseptr)
        {
            impl->node_->nsDef = toeraseptr->next;
            xmlFreeNs(toeraseptr);
            return;
        }

        // Nope, it isn't. Iterate
        for (xmlNsPtr runner = impl->node_->nsDef; runner != NULL; runner = runner->next)
        {
            if (runner->next == toeraseptr)
            {
               runner->next = toeraseptr->next;
               xmlFreeNs(toeraseptr);
               return;
            }
        }
        throw xml::exception("namespace not defined here");
    }
} // namespace xml
