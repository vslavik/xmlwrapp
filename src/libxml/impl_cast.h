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

#ifndef XMLWRAPP_IMPL_CAST_H_20030714T1820
#define XMLWRAPP_IMPL_CAST_H_20030714T1820

#include "xmlwrapp/pimpl.h"
/** @file
 * Framework for a "type-safe void pointer" idiom, or a "shared Pimpl" idiom.
 **/

namespace xmlwrapp {
  //####################################################################
  /**
   * Implementation of a "type-safe void pointer" idiom, a modification
   * of the Pimpl idiom for wrapping dynamically-allocated objects.
   *
   * The scenario:
   * We write a wrapper class to manage a dynamically-allocated object
   * of some underlying representation type, Rep. Also, we want to hide
   * the actual type of Rep from users of our wrapper.
   *
   * In the interface, we declare (but do not define) a private
   * implementation type, Impl, and store a Pimpl pointer in each instance
   * of our wrapper. In our implementation, we specialize the template
   * impl_cast::get_rep for our Rep type (e.g., by using #XMLWRAPP_ASSOCIATE),
   * then freely convert between the two using impl_cast::as_rep and
   * impl_cast::as_impl.
   *
   * Since Impl was declared but not defined, it is an incomplete type like
   * void, and thus is difficult to misuse. However, unlike void *, this is
   * type-safe, since the only casting is encapsulated and strictly limited
   * to conversion from Rep to Impl and back.
   *
   * Here is an example:
   *
   * First, we define a simple class interface using the Pimpl idiom:
   * @include impl_cast/Integer.h
   *
   * Next is an example of user code which would use that class:
   * @include impl_cast/main.cxx
   *
   * Finally, this is how we would implement the class interface
   * we defined:
   * @include impl_cast/Integer.cxx
   *
   * @see http://www.gotw.ca/gotw/024.htm The Pimpl idiom proper.
   **/
  namespace impl_cast {
    //####################################################################
    //! Define the underlying representation (Rep) for the given private
    //! implementation (Impl). User must specialize for each Impl type.
    //! @author Shane Beasley
    template <typename Impl> struct get_rep {
      template <typename> struct specialize_get_rep_for;
      typedef struct specialize_get_rep_for<Impl> type;
    };
    //####################################################################
    //! Convert a pointer to private implementation (Impl) from the
    //! underlying representation (Rep) associated with it.
    //! @author Shane Beasley
    template <typename Impl>
    Impl *as_impl (typename get_rep<Impl>::type *p) {
      return reinterpret_cast<Impl *>(p);
    }
    //####################################################################
    //! Convert a pointer from private implementation (Impl) to the
    //! underlying representation (Rep) associated with it.
    //! @author Shane Beasley
    template <typename Impl>
    typename get_rep<Impl>::type *
    as_rep (Impl *p) {
      return reinterpret_cast<typename get_rep<Impl>::type *>(p);
    }
    //####################################################################
    //! Convert a pointer from private implementation (Impl) to the
    //! underlying representation (Rep) associated with it.
    //! @author Shane Beasley
    template <typename Impl>
    typename get_rep<Impl>::type *
    as_rep (const XMLWRAPP_PIMPL(shared_ptr, Impl) &p) {
      return reinterpret_cast<typename get_rep<Impl>::type *>(p.get());
    }
    //####################################################################
    //! Utility struct containing useful typedefs.
    template <typename Impl> struct factory_base {
      //####################################################################
      typedef Impl impl, *impl_ptr;
      //####################################################################
      typedef typename get_rep<impl>::type *rep_ptr;
      //####################################################################
      //! @see xmlwrapp::impl_cast::as_impl
      //! @author Shane Beasley
      static impl_ptr as_impl (rep_ptr rep) {
	return impl_cast::as_impl<impl>(rep);
      }
    };
    //####################################################################
    //! Define the private implementation (Impl) for the given interface
    //! class (Iface). User must specialize for each Iface whose Impl is
    //! something other than Iface::impl.
    //! @author Shane Beasley
    template <typename Iface> struct get_impl {
      typedef typename Iface::impl type;
    };
    //####################################################################
    //! Given an interface class (Iface), convert a pointer to its private
    //! implementation (Impl) from the underlying representation (Rep)
    //! associated with it.
    //! @author Shane Beasley
    template <typename Iface>
    typename get_impl<Iface>::type *
    as_my_impl (typename get_rep<typename get_impl<Iface>::type>::type *p) {
      return reinterpret_cast<typename get_impl<Iface>::type *>(p);
    }
  }
}

//####################################################################
//! Associate the private implementation (Impl) with its underlying
//! representation (Rep).
//! @note Call this macro only from global scope!
//! @author Shane Beasley
#define XMLWRAPP_ASSOCIATE(Impl, Rep)                       \
namespace xmlwrapp { namespace impl_cast {                  \
    template <> struct get_rep<Impl> { typedef Rep type; }; \
} }

#endif
