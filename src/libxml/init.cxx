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
#include "xmlwrapp/init.h"

// libxml includes
#include <libxml/globals.h>
#include <libxml/xmlerror.h>
#include <libxml/parser.h>

namespace
{

bool change_flag_and_return_old_value(int* flag, bool new_value)
{
    const bool old_value = *flag != 0;
    *flag = new_value ? 1 : 0;
    return old_value;
}

} // anonymous namespace

namespace xml
{

int init::ms_counter = 0;

init::init()
{
    if ( ms_counter++ == 0 )
        init_library();
}


init::~init()
{
    if ( --ms_counter == 0 )
        shutdown_library();
}


void init::init_library()
{
    // init the parser (keeps libxml2 thread safe)
    xmlInitParser();

    // set some libxml global variables
    indent_output(true);
    remove_whitespace(false);
    substitute_entities(true);
    load_external_subsets(true);
    validate_xml(false);
}


void init::shutdown_library()
{
    xmlCleanupParser();
}


bool init::indent_output(bool flag)
{
    return change_flag_and_return_old_value(&xmlIndentTreeOutput, flag);
}


bool init::remove_whitespace(bool flag)
{
    return !change_flag_and_return_old_value(&xmlKeepBlanksDefaultValue, !flag);
}


bool init::substitute_entities(bool flag)
{
    return change_flag_and_return_old_value(&xmlSubstituteEntitiesDefaultValue, flag);
}


bool init::load_external_subsets(bool flag)
{
    return change_flag_and_return_old_value(&xmlLoadExtDtdDefaultValue, flag);
}


bool init::validate_xml(bool flag)
{
  return change_flag_and_return_old_value(&xmlDoValidityCheckingDefaultValue, flag);
}

} // namespace xml
