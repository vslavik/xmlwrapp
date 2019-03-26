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

#ifndef _xmlwrapp_utility_h_
#define _xmlwrapp_utility_h_

#include <xmlwrapp/node.h>

// standard includes
#include <stdexcept>
#include <string>
#include <cstdarg>

// libxml2 includes
#include <libxml/tree.h>

namespace xml
{

namespace impl
{

// Helper for holding libxml objects with guaranteed freeing
template<typename TPtr, void (*FreeFunc)(TPtr)>
class xml_scoped_ptr
{
public:
    explicit xml_scoped_ptr(TPtr ptr) : ptr_(ptr) {}

    ~xml_scoped_ptr()
    {
        if (ptr_)
            FreeFunc(ptr_);
    }

    TPtr operator->() const { return ptr_; }
    TPtr get() const { return ptr_; }
    operator TPtr() const { return ptr_; }

private:
    TPtr ptr_;
};

// exception safe wrapper around xmlChar*s that are returned from some
// of the libxml functions that the user must free.
class xmlchar_helper
{
public:
    xmlchar_helper(xmlChar *ptr) : ptr_(ptr) {}

    ~xmlchar_helper()
        { if (ptr_) xmlFree(ptr_); }

    const char* get() const
        { return reinterpret_cast<const char*>(ptr_); }

private:
    xmlChar *ptr_;
};

inline const xmlChar* xml_string(const std::string& s)
{
    return reinterpret_cast<const xmlChar*>(s.c_str());
}

// Formats given message with arguments into a std::string
void printf2string(std::string& s, const char *message, va_list ap);

// Sun CC uses ancient C++ standard library that doesn't have standard
// std::distance(). Work around it here
#if defined(__SUNPRO_CC) && !defined(_STLPORT_VERSION)
template<typename T>
inline size_t distance(T a, const T&b)
{
    size_t n = 0;
    for ( ; a != b; ++a )
        ++n;
    return n;
}
#endif // defined(__SUNPRO_CC) && !defined(_STLPORT_VERSION)

// Cast size_t to int safely, i.e. throw an exception in case of an overflow.
inline int checked_int_cast(std::size_t len)
{
    if ( len > INT_MAX )
        throw std::invalid_argument("data too long");

    return static_cast<int>(len);
}

// Cast int to size_t safely, checking that it's non-negative (we assume that
// size_t is always big enough to contain INT_MAX, which is true for all
// currently existing architectures).
inline std::size_t checked_size_t_cast(int len)
{
    if ( len < 0 )
        throw std::runtime_error("length value unexpectedly negative");

    return static_cast<std::size_t>(len);
}

} // namespace impl

} // namespace xml


#endif // _xmlwrapp_utility_h_
