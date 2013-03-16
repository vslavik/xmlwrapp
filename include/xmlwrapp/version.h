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

#define XMLWRAPP_VERSION_MAJOR   0
#define XMLWRAPP_VERSION_MINOR   7
#define XMLWRAPP_VERSION_MICRO   0

/**
    Checks if xmlwrapp version is at least @a major.@a minor.@a micro.
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
             XMLWRAPP_VERSION_MICRO >= (micro))               \
        )

#endif // _xmlwrapp_version_h_
