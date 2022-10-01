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

/**
    @file

    This file contains the definition of the xml::init class.
 */

#ifndef _xmlwrapp_init_h_
#define _xmlwrapp_init_h_

// xmlwrapp includes
#include "xmlwrapp/export.h"

/// XML library namespace
namespace xml
{

/**
    The xml::init class is used to configure the XML parser.

    If you want to use and of the xml::init member functions, do so before
    you start any threads or use any other part of xmlwrapp. The member
    functions may alter global and/or static variables and affect the behavior
    of subsequently created classes (and the parser in particular).
    In other words, this class is not thread safe.

    @note In xmlwrapp versions prior to 0.6.0, this class was used to initialize
          the library and exactly one instance had to be created before first
          use. This is no longer true: user code doesn't have to create any
          instances, but it @em can create as many instances as it wants.
 */
class XMLWRAPP_API init
{
public:
    init();
    ~init();

    /**
        RAII helper changing some global XML library flag only for the duration
        of its lifetime.

        Example use:
        @code
        void some_function()
        {
            if ( whatever )
            {
                xml::init::change_flag change(&xml::init::substitute_entities, false);

                // entities are not substituted here
            }

            // entities substitution flag reverted to its original value here
        }
        @endcode

        @since 0.9.1
     */
    class change_flag
    {
    public:
        typedef bool (*change_func_t)(bool);

        /**
            Constructor changes the flag using the specified function.

            The same function will be called from this object destructor to
            restore the flag value.

            @param change_func one of xml::init static methods, such as
                indent_output or remove_whitespace
            @param flag the value of the flag to use
         */
        change_flag(change_func_t change_func, bool flag)
            : change_func_(change_func),
              flag_orig_((*change_func)(flag))
        {
        }

        /// Destructor restores the original flag value.
        ~change_flag()
        {
            (*change_func_)(flag_orig_);
        }

    private:
        change_func_t const change_func_;
        bool const flag_orig_;
    };

    /**
        This member function controls whether or not the XML parser should
        add text nodes for indenting when generating XML text output from a
        node tree. The default is true.

        @param flag True to turn on indenting, false to turn it off.
        @return previous state of the flag.
     */
    static bool indent_output(bool flag);

    /**
        This member function controls whether or not the XML parser should
        remove ignorable whitespace around XML elements. The default
        is false.

        @param flag True to remove whitespace, false to leave alone.
        @return previous state of the flag.
     */
    static bool remove_whitespace(bool flag);

    /**
        This member function controls whether or not the XML parser should
        substitute entities while parsing. The default is true.

        @param flag True to turn on substitution, false to turn off.
        @return previous state of the flag.
     */
    static bool substitute_entities(bool flag);

    /**
        This member function controls whether or not the XML parser should
        load external (DTD) subsets while parsing. This will only affect the
        loading of the subsets, it does not cause files to be validated. The
        default is true.

        @param flag True to turn on loading, false to turn it off.
        @return previous state of the flag.
     */
    static bool load_external_subsets(bool flag);

    /**
        This member function controls whether or not the XML parser should
        validate every XML document that is parses with its DTD. The default
        is false.

        @return flag True to turn on validation, false to turn it off.
        @return previous state of the flag.
     */
    static bool validate_xml(bool flag);

private:
    init(const init&) = delete;
    init& operator=(const init&) = delete;

    void init_library();
    void shutdown_library();

    static int ms_counter;
};

} // namespace xml

// use a "nifty counter" to ensure that any source file that uses xmlwrapp
// will initialize the library prior to its first use
namespace
{
    xml::init g_xmlwrapp_initializer;
}

#endif // _xmlwrapp_init_h_
