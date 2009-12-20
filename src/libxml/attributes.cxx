/*
 * Copyright (C) 2001-2003 Peter J Jones (pjones@pmade.org)
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

// xmlwrapp includes
#include "xmlwrapp/attributes.h"
#include "ait_impl.h"
#include "pimpl_base.h"

// standard includes
#include <new>
#include <algorithm>

// libxml2 includes
#include <libxml/tree.h>

namespace xml
{

using namespace xml::impl;

// ------------------------------------------------------------------------
// xml::attributes::pimpl
// ------------------------------------------------------------------------

struct attributes::pimpl : public pimpl_base<attributes::pimpl>
{
    pimpl() : owner_(true)
    {
        xmlnode_ = xmlNewNode(0, reinterpret_cast<const xmlChar*>("blank"));
        if (!xmlnode_)
            throw std::bad_alloc();
    }

    pimpl(xmlNodePtr node) : xmlnode_(node), owner_(false) {}

    pimpl(const pimpl& other) : owner_(true)
    {
        xmlnode_ = xmlNewNode(0, reinterpret_cast<const xmlChar*>("blank"));
        if (!xmlnode_)
            throw std::bad_alloc();

        xmlAttrPtr i=other.xmlnode_->properties;
        xmlAttrPtr copy;

        // work around bug in libxml
        for ( ; i != 0; i = i->next )
        {
            if ( (copy = xmlCopyProp(0, i)) == 0)
            {
                xmlFreeNode(xmlnode_);
                throw std::bad_alloc();
            }

            copy->prev = 0;
            copy->next = 0;
            xmlAddChild(xmlnode_, reinterpret_cast<xmlNodePtr>(copy));
        }
    }

    ~pimpl()
    {
        release();
    }

    void release()
    {
        if (owner_ && xmlnode_)
            xmlFreeNode(xmlnode_);
    }

    xmlNodePtr xmlnode_;
    bool owner_;
};


// ------------------------------------------------------------------------
// xml::attributes
// ------------------------------------------------------------------------

attributes::attributes()
{
    pimpl_ = new pimpl;
}


attributes::attributes(int)
{
    pimpl_ = new pimpl(0);
}


attributes::attributes(const attributes& other)
{
    pimpl_ = new pimpl(*other.pimpl_);
}


attributes& attributes::operator=(const attributes& other)
{
    attributes tmp(other);
    swap(tmp);
    return *this;
}


void attributes::swap(attributes& other)
{
    std::swap(pimpl_, other.pimpl_);
}


attributes::~attributes()
{
    delete pimpl_;
}


void* attributes::get_data()
{
    return pimpl_->xmlnode_;
}


void attributes::set_data(void *node)
{
    xmlNodePtr x = static_cast<xmlNodePtr>(node);

    pimpl_->release();
    pimpl_->owner_ = false;
    pimpl_->xmlnode_ = x;
}


attributes::iterator attributes::begin()
{
    return iterator(pimpl_->xmlnode_, pimpl_->xmlnode_->properties);
}


attributes::const_iterator attributes::begin() const
{
    return const_iterator(pimpl_->xmlnode_, pimpl_->xmlnode_->properties);
}


attributes::iterator attributes::end()
{
    return iterator();
}


attributes::const_iterator attributes::end() const
{
    return const_iterator();
}


void attributes::insert(const char *name, const char *value)
{
    xmlSetProp(pimpl_->xmlnode_,
               reinterpret_cast<const xmlChar*>(name),
               reinterpret_cast<const xmlChar*>(value));
}


attributes::iterator attributes::find(const char *name)
{
    xmlAttrPtr prop = find_prop(pimpl_->xmlnode_, name);
    if ( prop != 0 )
        return iterator(pimpl_->xmlnode_, prop);

    xmlAttributePtr dtd_prop = find_default_prop(pimpl_->xmlnode_, name);
    if ( dtd_prop != 0 )
        return iterator(name, reinterpret_cast<const char*>(dtd_prop->defaultValue), true);

    return iterator();
}


attributes::const_iterator attributes::find(const char *name) const
{
    xmlAttrPtr prop = find_prop(pimpl_->xmlnode_, name);
    if (prop != 0)
        return const_iterator(pimpl_->xmlnode_, prop);

    xmlAttributePtr dtd_prop = find_default_prop(pimpl_->xmlnode_, name);

    if (dtd_prop != 0)
    {
        return const_iterator(name, reinterpret_cast<const char*>(dtd_prop->defaultValue), true);
    }

    return const_iterator();
}


attributes::iterator attributes::erase (iterator to_erase)
{
    xmlNodePtr prop = static_cast<xmlNodePtr>(to_erase.get_raw_attr());
    if (prop == 0)
        return iterator(); // handle fake and bad iterators
    ++to_erase;

    xmlUnlinkNode(prop);
    xmlFreeNode(prop);

    return to_erase;
}


void attributes::erase(const char *name)
{
    xmlUnsetProp(pimpl_->xmlnode_, reinterpret_cast<const xmlChar*>(name));
}


bool attributes::empty() const
{
    return pimpl_->xmlnode_->properties == 0;
}


attributes::size_type attributes::size() const
{
    size_type count = 0;

    xmlAttrPtr prop = pimpl_->xmlnode_->properties;
    while (prop != 0)
    {
        ++count;
        prop = prop->next;
    }

    return count;
}

} // namespace xml
