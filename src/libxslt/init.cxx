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

/** @file
 * This file contains the implementation of the xslt::init class.
**/

// defintion include
#include "xsltwrapp/init.h"

// libxslt includes
#include <libxslt/xslt.h>
#include <libxslt/xsltInternals.h>
#include <libxslt/transform.h>
#include <libxslt/xsltutils.h>
#include <libexslt/exslt.h>

//####################################################################
namespace {
    extern "C" void xslt_error (void *, const char*, ...);
}
//####################################################################
xslt::init::init (void) {
    /*
     * this really sucks, but it is the only way to call xsltInit() right
     * now. This is necessary to make libxslt thread safe.
     */
    xsltStylesheetPtr ss = xsltNewStylesheet();
    xsltFreeStylesheet(ss);

    // set some defautls
    process_xincludes(true);

    // keep libxslt silent
    xsltSetGenericErrorFunc(0, xslt_error);
    xsltSetGenericDebugFunc(0, xslt_error);

    // load EXSLT
    exsltRegisterAll();
}
//####################################################################
xslt::init::~init (void) {
    xsltCleanupGlobals();
}
//####################################################################
void xslt::init::process_xincludes (bool flag) {
    xsltSetXIncludeDefault(flag ? 1 : 0);
}
//####################################################################
namespace {
    extern "C" void xslt_error (void*, const char*, ...)
    { /* don't do anything */ }
}
//####################################################################
