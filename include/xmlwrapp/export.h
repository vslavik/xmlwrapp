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

#ifdef HAVE_VISIBILITY
    #define XMLWRAPP_API   __attribute__ ((visibility("default")))
    #define XSLTWRAPP_API  __attribute__ ((visibility("default")))
#else
    #define XMLWRAPP_API
    #define XSLTWRAPP_API
#endif

#if defined(__clang__)
    #if defined(__has_extension) && __has_extension(attribute_deprecated_with_message)
        #define XMLWRAPP_DEPRECATED(msg) __attribute__((deprecated(msg)))
    #else
        #define XMLWRAPP_DEPRECATED(msg) __attribute__((deprecated))
    #endif
#elif defined(__GNUC__)
    #define XMLWRAPP_DEPRECATED(msg) __attribute__((deprecated))
#elif defined(__VISUALC__) && (__VISUALC__ >= 1300)
    #define XMLWRAPP_DEPRECATED(msg) __declspec(deprecated)
#else
    #define XMLWRAPP_DEPRECATED(msg)
#endif

#endif // _xmlwrapp_export_h_
