/*
 * Copyright (C) 2009 Vaclav Slavik <vslavik@gmail.com>
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
#include "xmlwrapp/nodes_view.h"

// definition include
#include "node_iterator.h"
#include "pimpl_base.h"
#include "utility.h"

using namespace xml::impl;

namespace xml
{

// ------------------------------------------------------------------------
// xml::const_nodes_view
// ------------------------------------------------------------------------

const_nodes_view::const_nodes_view(const const_nodes_view& other)
    : data_begin_(other.data_begin_),
      advance_func_(other.advance_func_)
{
    if ( advance_func_ )
        advance_func_->inc_ref();
}


const_nodes_view::const_nodes_view(const nodes_view& other)
    : data_begin_(other.data_begin_),
      advance_func_(other.advance_func_)
{
    if ( advance_func_ )
        advance_func_->inc_ref();
}


const_nodes_view::~const_nodes_view()
{
    if ( advance_func_ )
        advance_func_->dec_ref();
}


const_nodes_view& const_nodes_view::operator=(const const_nodes_view& other)
{
    if ( advance_func_ )
        advance_func_->dec_ref();

    data_begin_ = other.data_begin_;
    advance_func_ = other.advance_func_;

    if ( advance_func_ )
        advance_func_->inc_ref();

    return *this;
}


const_nodes_view& const_nodes_view::operator=(const nodes_view& other)
{
    if ( advance_func_ )
        advance_func_->dec_ref();

    data_begin_ = other.data_begin_;
    advance_func_ = other.advance_func_;

    if ( advance_func_ )
        advance_func_->inc_ref();

    return *this;
}


const_nodes_view::size_type const_nodes_view::size() const
{
    using namespace std;
    return distance(begin(), end());
}


// ------------------------------------------------------------------------
// xml::nodes_view
// ------------------------------------------------------------------------

nodes_view::nodes_view(const nodes_view& other)
    : data_begin_(other.data_begin_),
      advance_func_(other.advance_func_)
{
    if ( advance_func_ )
        advance_func_->inc_ref();
}


nodes_view::~nodes_view()
{
    if ( advance_func_ )
        advance_func_->dec_ref();
}


nodes_view& nodes_view::operator=(const nodes_view& other)
{
    if ( advance_func_ )
        advance_func_->dec_ref();

    data_begin_ = other.data_begin_;
    advance_func_ = other.advance_func_;

    if ( advance_func_ )
        advance_func_->inc_ref();

    return *this;
}


nodes_view::size_type nodes_view::size() const
{
    using namespace std;
    return distance(begin(), end());
}

} // namespace xml
