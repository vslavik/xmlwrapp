/*
 * Copyright (c) 2003 Shane Beasley
 *
 * Permission is hereby granted, free of charge, to any person
 * obtaining a copy of this software and associated documentation
 * files (the "Software"), to deal in the Software without restriction,
 * including without limitation the rights to use, copy, modify, merge,
 * publish, distribute, sublicense, and/or sell copies of the Software,
 * and to permit persons to whom the Software is furnished to do so,
 * subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 * IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
 * CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
 * TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
 * SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

#ifndef XMLWRAPP_ACCESS_H_20030809T064856
#define XMLWRAPP_ACCESS_H_20030809T064856

/** @file
 * Definition of xmlwrapp::access policies for read-only and read-write views of documents
 */

#include "config.h"

//####################################################################
//! @see xmlwrapp::access::specifier
#define XMLWRAPP_ACCESS_SPECIFIER ::xmlwrapp::access::specifier
//####################################################################
//! @see xmlwrapp::access::read_only
#define XMLWRAPP_RO_ACCESS        ::xmlwrapp::access::read_only
//####################################################################
//! @see xmlwrapp::access::read_write
#define XMLWRAPP_RW_ACCESS        ::xmlwrapp::access::read_write
//####################################################################
//! Typedef for T, restricted according to the given access specifier.
//! @note MSVC++6 and g++2 do this differently, hence the macro.
#ifdef XMLWRAPP_ACCESS_RESTRICT
#  define XMLWRAPP_RESTRICT(Access, T) \
  typename ::xmlwrapp::access::restrict<Access, T>::type
#else
#  define XMLWRAPP_RESTRICT(Access, T) \
  typename ::xmlwrapp::access::input_policy<Access>::template as_input<T>::type
#endif
//####################################################################
//! Gives friendship to both <code>T<#XMLWRAPP_RO_ACCESS></code> and
//! <code>T<#XMLWRAPP_RW_ACCESS></code>.
//! @note MSVC++6 and g++2 do this differently, hence the macro.
#ifdef XMLWRAPP_TEMPLATE_FRIENDS
#  define XMLWRAPP_FRIEND_T(T) \
  template <XMLWRAPP_ACCESS_SPECIFIER> friend class T
#else
#  define XMLWRAPP_FRIEND_T(T) \
  friend class T<XMLWRAPP_RO_ACCESS>; \
  friend class T<XMLWRAPP_RW_ACCESS>
#endif

namespace xmlwrapp {
  //####################################################################
  /**
   * Access policies for const-correct reference types.
   *
   * The principle of "const correctness" dictates that you should use a
   * const reference (or pointer) if you do not intend to change the data
   * to which it refers. In so doing, you convey your intent to both human
   * readers and to the compiler, which can produce an error if you
   * inadvertently attempt to change the data.
   *
   * Although not mandatory -- in fact, most languages don't support it,
   * and C added it only recently -- it makes a design more robust.
   **/
  namespace access {
    //####################################################################
    //! Specifiers for access policies.
    //! Typically passed as an argument through a template parameter of type
    //! #XMLWRAPP_ACCESS_SPECIFIER to define the type of access required
    //! by the template.
    enum specifier {
      //####################################################################
      //! Specifies a read-only access policy for reference types.
      //! A read-only reference may refer to either read-only or read-write
      //! data, but it may only generate read-only references.
      //! (const T * and const T & are read-only reference types.)
      read_only  = true,
      //####################################################################
      //! Specifies a read-write access policy for reference types.
      //! A read-write reference may only refer to read-write data, but
      //! it may generate either read-write or read-only references.
      //! (Non-const T * and T & are read-write reference types.)
      read_write = false
    };
    //####################################################################
    // MSVC++6 needs a definition for this... Why?
    template <XMLWRAPP_ACCESS_SPECIFIER Access> struct input_policy {
      template <typename T> struct as_input { struct type; };
    };
    //####################################################################
    //! Define how read-only references accept input.
    //! @see as_input
    template <> struct input_policy<read_only> {
      //####################################################################
      //! Define how read-only references accept input of type T.
      template <typename T> struct as_input {
	//####################################################################
	//! Read-only references accept const references, which may be bound
	//! to both const and non-const data.
	typedef const T type;
      };
    };
    //####################################################################
    //! Define how read-write references accept input.
    //! @see as_input
    template <> struct input_policy<read_write> {
      //####################################################################
      //! Define how read-write references accept input of type T.
      template <typename T> struct as_input {
	//####################################################################
	//! Read-only references accept non-const references, which may be
	//! bound only to non-const data.
	typedef T type;
      };
    };
    //####################################################################
    //! Define how references with the given access policy accept input
    //! of type T.
    //! @see input_policy<read_only>::as_input
    //! @see input_policy<read_write>::as_input
    //! @author Shane Beasley
    template <XMLWRAPP_ACCESS_SPECIFIER Access, typename T>
    struct restrict {
      typedef typename input_policy<Access>::template as_input<T>::type type;
    };
  }
}

#endif
