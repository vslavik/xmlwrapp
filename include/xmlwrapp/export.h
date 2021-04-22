/*
 * Copyright (C) 2010 Vaclav Slavik <vslavik@gmail.com>
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

#ifndef _xmlwrapp_export_h_
#define _xmlwrapp_export_h_

#if defined(_WIN32)
    // Note that DLL_EXPORT is predefined by libtool when building the DLLs,
    // but when using them, XMLWRAPP_USE_DLL or XSLTWRAPP_USE_DLL must be
    // predefined by the application.
    #if defined(DLL_EXPORT)
        #if defined(XMLWRAPP_BUILD)
            #define XMLWRAPP_API __declspec(dllexport)
        #endif

        #if defined(XSLTWRAPP_BUILD)
            #define XSLTWRAPP_API __declspec(dllexport)
        #endif
    #endif

    #if defined(XMLWRAPP_USE_DLL)
        #define XMLWRAPP_API __declspec(dllimport)
    #endif

    #if defined(XSLTWRAPP_USE_DLL)
        #define XSLTWRAPP_API __declspec(dllimport)
    #endif
#elif defined(HAVE_VISIBILITY)
    #define XMLWRAPP_API   __attribute__ ((visibility("default")))
    #define XSLTWRAPP_API  __attribute__ ((visibility("default")))
#endif

#ifndef XMLWRAPP_API
    #define XMLWRAPP_API
#endif

#ifndef XSLTWRAPP_API
    #define XSLTWRAPP_API
#endif

#if defined(__clang__)
    #if defined(__has_extension) && __has_extension(attribute_deprecated_with_message)
        #define XMLWRAPP_DEPRECATED(msg) __attribute__((deprecated(msg)))
    #else
        #define XMLWRAPP_DEPRECATED(msg) __attribute__((deprecated))
    #endif
#elif defined(__GNUC__)
    #if __GNUC__ > 4  || (__GNUC__ == 4 && __GNUC_MINOR__ >= 5)
        #define XMLWRAPP_DEPRECATED(msg) __attribute__((deprecated(msg)))
    #else
        #define XMLWRAPP_DEPRECATED(msg) __attribute__((deprecated))
    #endif
#elif defined(_MSC_VER)
    #if _MSC_VER >= 1400
        #define XMLWRAPP_DEPRECATED(msg) __declspec(deprecated("deprecated: " msg))
    #elif _MSC_VER >= 1300
        #define XMLWRAPP_DEPRECATED(msg) __declspec(deprecated)
    #else
        #define XMLWRAPP_DEPRECATED(msg)
    #endif
#else
    #define XMLWRAPP_DEPRECATED(msg)
#endif

// Define a macro allowing to disable MSVC warning about using non-DLL-exported
// classes (typically from the standard library itself) as members of the
// DLL-exported classes because this not a problem as long as both the main
// application and the DLL use the same (or at least ABI-compatible) CRT
// version, which should always be the case in practice.
//
// The first macro should be used before declaring such members in the classes
// using XMLWRAPP_API declaration and the second one after declaring them to
// restore the default warning level.
//
// Both macros do nothing for non-MSVC compilers.
#if defined(_MSC_VER)
    #define XMLWRAPP_MSVC_SUPPRESS_DLL_MEMBER_WARN \
        __pragma( warning(push) ) \
        __pragma( warning(disable:4251) ) /* Class needs to have dll-interface to be used by clients of class */
    #define XMLWRAPP_MSVC_RESTORE_DLL_MEMBER_WARN \
        __pragma( warning(pop) )
#else
    #define XMLWRAPP_MSVC_SUPPRESS_DLL_MEMBER_WARN
    #define XMLWRAPP_MSVC_RESTORE_DLL_MEMBER_WARN
#endif

#endif // _xmlwrapp_export_h_
