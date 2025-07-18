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

/**
    @file

    This file contains the XMLWRAPP_CHECK_VERSION macro.
 */

#ifndef _xmlwrapp_version_h_
#define _xmlwrapp_version_h_

#include "xmlwrapp/export.h"

/**
    Compile-time major version of the library.

    This is the first version component.

    @see XMLWRAPP_CHECK_VERSION()
 */
#define XMLWRAPP_VERSION_MAJOR   0

/**
    Compile-time minor version of the library.

    This is the second version component.

    @see XMLWRAPP_CHECK_VERSION()
 */
#define XMLWRAPP_VERSION_MINOR  10

/**
    Compile-time micro version of the library.

    This is the third version component.

    @see XMLWRAPP_CHECK_VERSION()
 */
#define XMLWRAPP_VERSION_MICRO   0

#define XMLWRAPP_VERSION_STRING_CONCAT(major, minor, micro) \
  #major "." #minor "." #micro

#define XMLWRAPP_VERSION_STRING_IMPL(major, minor, micro) \
  XMLWRAPP_VERSION_STRING_CONCAT(major, minor, micro)

/**
    Compile-time version of the library as a string.

    This is a string in the form "major.minor.micro".

    @see XMLWRAPP_VERSION_MAJOR, XMLWRAPP_VERSION_MINOR,
         XMLWRAPP_VERSION_MICRO, XMLWRAPP_CHECK_VERSION()

    @since 0.10.0
 */
#define XMLWRAPP_VERSION_STRING \
  XMLWRAPP_VERSION_STRING_IMPL(XMLWRAPP_VERSION_MAJOR, \
                               XMLWRAPP_VERSION_MINOR, \
                               XMLWRAPP_VERSION_MICRO)

/**
    Checks if xmlwrapp version is at least @a major.@a minor.@a micro.

    This is a compile-time check, see check_version() for a runtime check.
 */
#define XMLWRAPP_CHECK_VERSION(major, minor, micro)           \
        (                                                     \
            XMLWRAPP_VERSION_MAJOR > (major)                  \
            ||                                                \
            (XMLWRAPP_VERSION_MAJOR == (major) &&             \
             XMLWRAPP_VERSION_MINOR >= (minor))               \
            ||                                                \
            (XMLWRAPP_VERSION_MAJOR == (major) &&             \
            (XMLWRAPP_VERSION_MINOR == (minor) &&             \
             XMLWRAPP_VERSION_MICRO >= (micro)))              \
        )

namespace xml
{

/**
    Return major runtime version of xmlwrapp library.

    This can be different from XMLWRAPP_VERSION_MAJOR if a different version of
    the library is used at runtime than the one that was used to compile the
    code.

    @see XMLWRAPP_VERSION_MAJOR

    @since 0.10.0
 */
XMLWRAPP_API int get_major_version();

/**
    Return minor runtime version of xmlwrapp library.

    @see get_major_version(), XMLWRAPP_VERSION_MINOR

    @since 0.10.0
 */
XMLWRAPP_API int get_minor_version();

/**
    Return micro runtime version of xmlwrapp library.

    @see get_major_version(), get_minor_version(), XMLWRAPP_VERSION_MICRO

    @since 0.10.0
 */
XMLWRAPP_API int get_micro_version();

/**
    Return the full runtime version of xmlwrapp library.

    This is a string in the form "major.minor.micro".

    @see get_major_version(), get_minor_version(), get_micro_version()

    @since 0.10.0
 */
XMLWRAPP_API const char* get_version_string();

/**
    Check that the library version is at least the given one.

    Returns true if the version is at least @a major.@a minor.@a micro, false
    otherwise,

    @see XMLWRAPP_CHECK_VERSION(), get_version_string()

    @since 0.10.0
 */
XMLWRAPP_API bool check_version(int major, int minor, int micro);

} // namespace xml

#endif // _xmlwrapp_version_h_
