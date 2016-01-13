/*
 * Copyright (C) 2016 Vadim Zeitlin (vz-xmlwrapp@zeitlins.org)
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

    This file attempts to hide the differences between C++03 and C++11.
 */

#ifndef _xsltwrapp_cpp11_h_
#define _xsltwrapp_cpp11_h_

#include <memory>

// This symbol can be predefined to prevent the use of C++11 features even if
// they are deemed to be available.
#ifndef XMLWRAPP_NO_CPP11

// This symbol, on the contrary, can be predefined to force the use of C++11
// features even if they're not automatically detected.
#ifndef XMLWRAPP_CPP11

// We consider any compiler defining __cplusplus appropriately to be a C++11
// compiler and make a special exception for MSVS 2015 which doesn't define it
// but is close enough to be considered one.
#if __cplusplus >= 201103L || \
    (defined(_MSC_VER) && _MSC_VER >= 1900)
    #define XMLWRAPP_CPP11
#endif

#endif // XMLWRAPP_CPP11

// Additionally, some compilers support some C++11 features even though they
// don't have the full C++11 support, notably earlier versions of MSVS.
#if defined(XMLWRAPP_CPP11) || \
    (defined(_MSC_VER) && _MSC_VER >= 1600)
    #define XMLWRAPP_CPP11_MEMORY
#endif

#endif // !XMLWRAPP_NO_CPP11


// Define auto_ptr<> as either std::auto_ptr<> or std::unique_ptr<> to allow
// compiling the code in both C++ dialects without getting the warnings about
// auto_ptr<> being deprecated in C++11 code.
//
// Notice that our xml::impl::auto_ptr<> is more like scoped_ptr<>, but we
// can't call it this as we don't have type aliases in C++03 and so we can only
// bring std::auto_ptr<> in scope but not rename it to scoped_ptr<> there.
namespace xml
{

namespace impl
{

#ifdef XMLWRAPP_CPP11_MEMORY
    template <typename T>
    using auto_ptr = std::unique_ptr<T>;
#else // C++03
    using std::auto_ptr;
#endif // C++03/11

} // namespace impl

} // namespace xml

#endif // _xsltwrapp_cpp11_h_
