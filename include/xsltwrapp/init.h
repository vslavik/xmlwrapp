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
 * This file contains the definition of the xslt::init class.
**/

#ifndef _xsltwrapp_init_h_
#define _xsltwrapp_init_h_

// xmlwrapp includes
#include <xmlwrapp/init.h>

namespace xslt {

/**
 * The xslt::init class is used to initilize the XSLT engine. For thread
 * safety it should be instantiated one time in the main thread before any
 * other threads use xsltwrapp. Non-threaded programs should instantiante a
 * xslt::init class before using xsltwrapp as well, at least for
 * consistanticy.
 *
 * If you want to use any of the xslt::init member functions, do so before
 * you start any threads or use any other part of xsltwrapp. The member
 * functions may alter global and/or static variables. In other words, this
 * class is not thread safe.
 *
 * Since this class is derived from the xml::init it is not neccessary to
 * use both classes. If you are going to be using xsltwrapp, you should only
 * use this class to initilize both xmlwrapp and xsltwrapp.
**/
class init : public xml::init {
public:
    //####################################################################
    /** 
     * Create a new xslt::init object. This constructor will prepare the
     * XSLT engine parser and set some default values for the engine's
     * global variables.
     *
     * @author Peter Jones
    **/
    //####################################################################
    init (void);

    //####################################################################
    /** 
     * Clean up the XSLT engine. Don't let the xslt::init object go out of
     * scope before you are done using the xsltwrapp or xmlwrapp libraries!
     *
     * @author Peter Jones
    **/
    //####################################################################
    virtual ~init (void);

    //####################################################################
    /** 
     * This function controls whether or not the XSLT engine will process
     * XInclusions by default while parsing the stylesheet. The default, set
     * in the xslt::init constructor, is true.
     *
     * @param flag True to enable XInclusing processing; False otherwise.
     * @author Peter Jones
    **/
    //####################################################################
    void process_xincludes (bool flag);

private:
    init (const init&);
    init& operator= (const init&);
}; // end xslt::init class
    
} // end xslt namespace
#endif
