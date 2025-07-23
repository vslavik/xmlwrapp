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
#include "ait_impl.h"
#include "utility.h"
#include "xmlwrapp/attributes.h"
#include "xmlwrapp/errors.h"

// standard includes
#include <algorithm>

// libxml2 includes
#include <libxml/tree.h>

namespace xml
{

using namespace impl;

// ------------------------------------------------------------------------
// xml::impl::ait_impl
// ------------------------------------------------------------------------

namespace impl
{

ait_impl::ait_impl(xmlNodePtr node, xmlAttrPtr prop)
    : xmlnode_(node), xmlattr_(prop), fake_(false)
{
    attr_.set_data(xmlnode_, xmlattr_);
}


ait_impl::ait_impl(const char *name, const char *value, bool)
    : xmlnode_(nullptr), xmlattr_(nullptr), fake_(true)
{
    // in this constructor and in the functions to follow, the last
    // parameter, the bool, is only used to create a unique signature
    attr_.set_data(name, value, true);
}


ait_impl::ait_impl(const ait_impl& other)
    : xmlnode_(other.xmlnode_), xmlattr_(other.xmlattr_), fake_(other.fake_)
{
    if (fake_)
        attr_.set_data(other.attr_.get_name(), other.attr_.get_value(), true);
    else
        attr_.set_data(xmlnode_, xmlattr_);
}


ait_impl& ait_impl::operator=(const ait_impl& other)
{
    ait_impl tmp(other);

    std::swap(xmlnode_, tmp.xmlnode_);
    std::swap(xmlattr_, tmp.xmlattr_);
    std::swap(fake_, tmp.fake_);
    attr_.swap(tmp.attr_);

    return *this;
}


attributes::attr* ait_impl::get()
{
    return &attr_;
}


xmlAttrPtr ait_impl::get_raw_attr()
{
    return xmlattr_;
}


ait_impl& ait_impl::operator++()
{
    if (xmlattr_)
        xmlattr_ = xmlattr_->next;
    else
        fake_ = false;

    attr_.set_data(xmlnode_, xmlattr_);
    return *this;
}


ait_impl ait_impl::operator++(int)
{
    ait_impl tmp(xmlnode_, xmlattr_);
    ++(*this);
    return tmp;
}

} // namespace impl


// ------------------------------------------------------------------------
// xml::attributes::iterator
// ------------------------------------------------------------------------

attributes::iterator::iterator()
{
}


attributes::iterator::iterator(void *node, void *prop)
    : pimpl_{new ait_impl(static_cast<xmlNodePtr>(node), static_cast<xmlAttrPtr>(prop))}
{
}


attributes::iterator::iterator(const char *name, const char *value, bool)
    : pimpl_{new ait_impl(name, value, true)}
{
}


attributes::iterator::iterator (const iterator &other)
    : pimpl_{new ait_impl(*other.pimpl_)}
{
}


attributes::iterator& attributes::iterator::operator=(const iterator& other)
{
    iterator tmp(other);
    swap(tmp);
    return *this;
}


attributes::iterator::iterator (iterator&&) = default;
attributes::iterator& attributes::iterator::operator=(iterator&&) = default;


void attributes::iterator::swap(iterator& other)
{
    std::swap(pimpl_, other.pimpl_);
}


attributes::iterator::~iterator() = default;


void* attributes::iterator::get_raw_attr()
{
    return pimpl_->get_raw_attr();
}


attributes::iterator::reference attributes::iterator::operator*() const
{
    return *(pimpl_->get());
}


attributes::iterator::pointer attributes::iterator::operator->() const
{
    return pimpl_->get();
}


attributes::iterator& attributes::iterator::operator++()
{
    ++(*pimpl_);
    return *this;
}


attributes::iterator attributes::iterator::operator++(int)
{
    iterator tmp(*this);
    ++(*this);
    return tmp;
}


// ------------------------------------------------------------------------
// xml::attributes::const_iterator
// ------------------------------------------------------------------------

attributes::const_iterator::const_iterator()
{
}


attributes::const_iterator::const_iterator(void *node, void *prop)
    : pimpl_{new ait_impl(static_cast<xmlNodePtr>(node), static_cast<xmlAttrPtr>(prop))}
{
}


attributes::const_iterator::const_iterator(const char *name, const char *value, bool)
    : pimpl_{new ait_impl(name, value, true)}
{
}


attributes::const_iterator::const_iterator(const const_iterator& other)
    : pimpl_{new ait_impl(*other.pimpl_)}
{
}


attributes::const_iterator::const_iterator(const iterator& other)
    : pimpl_{new ait_impl(*other.pimpl_)}
{
}


attributes::const_iterator& attributes::const_iterator::operator=(const const_iterator& other)
{
    const_iterator tmp(other);
    swap(tmp);
    return *this;
}


attributes::const_iterator::const_iterator(const_iterator&&) = default;
attributes::const_iterator& attributes::const_iterator::operator=(const_iterator&&) = default;


void attributes::const_iterator::swap(const_iterator& other)
{
    std::swap(pimpl_, other.pimpl_);
}


attributes::const_iterator::~const_iterator() = default;


void* attributes::const_iterator::get_raw_attr()
{
    return pimpl_->get_raw_attr();
}


attributes::const_iterator::reference attributes::const_iterator::operator*() const
{
    return *(pimpl_->get());
}


attributes::const_iterator::pointer attributes::const_iterator::operator->() const
{
    return pimpl_->get();
}


attributes::const_iterator& attributes::const_iterator::operator++()
{
    ++(*pimpl_);
    return *this;
}


attributes::const_iterator attributes::const_iterator::operator++(int)
{
    const_iterator tmp(*this);
    ++(*this);
    return tmp;
}


// ------------------------------------------------------------------------
// xml::attributes::attr
// ------------------------------------------------------------------------

void attributes::attr::swap(attr& other)
{
    std::swap(node_, other.node_);
    std::swap(prop_, other.prop_);
    name_.swap(other.name_);
    value_.swap(other.value_);
}


void attributes::attr::set_data(void *node, void *prop)
{
    node_ = node;
    prop_ = prop;
    name_.erase();
    value_.erase();
}


void attributes::attr::set_data(const char *name, const char *value, bool)
{
    node_ = nullptr;
    prop_ = nullptr;
    name_ = name;
    value_ = value;
}


const char* attributes::attr::get_name() const
{
    if (!name_.empty())
        return name_.c_str(); // we were given a name not a node

    if (!node_ || !prop_)
        throw xml::exception("access to invalid attributes::attr object!");

    return reinterpret_cast<const char*>(static_cast<xmlAttrPtr>(prop_)->name);
}


const char* attributes::attr::get_value() const
{
    if (!value_.empty())
        return value_.c_str(); // we were given a value, not a node

    if (!node_ || !prop_)
        throw xml::exception("access to invalid attributes::attr object!");

    xmlChar *tmpstr = xmlNodeListGetString(reinterpret_cast<xmlNodePtr>(node_)->doc, reinterpret_cast<xmlAttrPtr>(prop_)->children, 1);
    if (tmpstr == nullptr)
        return "";

    xmlchar_helper helper(tmpstr);
    value_.assign(helper.get());
    return value_.c_str();
}

// ------------------------------------------------------------------------
// helper friend functions and operators
// ------------------------------------------------------------------------

bool operator==(const attributes::iterator& lhs, const attributes::iterator& rhs)
{
    return ait_impl::are_equal(lhs.pimpl_.get(), rhs.pimpl_.get());
}

bool operator!=(const attributes::iterator& lhs, const attributes::iterator& rhs)
{
    return !(lhs == rhs);
}

bool operator==(const attributes::const_iterator& lhs, const attributes::const_iterator& rhs)
{
    return ait_impl::are_equal(lhs.pimpl_.get(), rhs.pimpl_.get());
}

bool operator!=(const attributes::const_iterator& lhs, const attributes::const_iterator& rhs)
{
    return !(lhs == rhs);
}


namespace impl
{

xmlAttrPtr find_prop(xmlNodePtr xmlnode, const char *name)
{
    xmlAttrPtr prop = xmlnode->properties;

    for (; prop; prop = prop->next )
    {
        if (xmlStrEqual(prop->name, reinterpret_cast<const xmlChar*>(name)))
            return prop;
    }

    return nullptr;
}


xmlAttributePtr find_default_prop(xmlNodePtr xmlnode, const char *name)
{
    if (xmlnode->doc != nullptr)
    {
        xmlAttributePtr dtd_attr=nullptr;

        if (xmlnode->doc->intSubset != nullptr)
        {
            dtd_attr = xmlGetDtdAttrDesc(xmlnode->doc->intSubset, xmlnode->name, reinterpret_cast<const xmlChar*>(name));
        }

        if (dtd_attr == nullptr && xmlnode->doc->extSubset != nullptr)
        {
            dtd_attr = xmlGetDtdAttrDesc(xmlnode->doc->extSubset, xmlnode->name, reinterpret_cast<const xmlChar*>(name));
        }

        if (dtd_attr != nullptr && dtd_attr->defaultValue != nullptr)
            return dtd_attr;
    }

    return nullptr;
}

bool ait_impl::are_equal(const ait_impl* lhs, const ait_impl* rhs)
{
    xmlAttrPtr lhsAttr;
    if (lhs)
    {
        if (lhs->fake_)
            return false;

        lhsAttr = lhs->xmlattr_;
    }
    else
    {
        lhsAttr = nullptr;
    }

    xmlAttrPtr rhsAttr;
    if (rhs)
    {
        if (rhs->fake_)
            return false;

        rhsAttr = rhs->xmlattr_;
    }
    else
    {
        rhsAttr = nullptr;
    }

    return lhsAttr == rhsAttr;
}

} // namespace impl

} // namespace xml
