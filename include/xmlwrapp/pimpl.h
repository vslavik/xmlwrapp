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

#ifndef XMLWRAPP_PIMPL_H_20030714T1820
#define XMLWRAPP_PIMPL_H_20030714T1820

/** @file
 * Functions and classes related to the Pimpl idiom.
 * @see http://www.gotw.ca/gotw/024.htm
 **/

#include <xmlwrapp/boost/smart_ptr.hpp>

namespace xmlwrapp {
  //####################################################################
  //! Code related to the Pimpl idiom.
  //! @see http://www.gotw.ca/gotw/024.htm
  namespace pimpl {
    //####################################################################
    //! @struct impl<T>
    //! Declared but undefined private implementation type.
    template <typename T> struct impl;
    //####################################################################
    //! Specifies how a particular type T is deleted.
    //! By default, passed as second parameter to xmlwrapp::pimpl::local_ptr.
    //! May be specialized for a particular type, or another class with
    //! a similar interface may be passed instead.
    template <typename T>
    struct local_ptr_delete {
      //####################################################################
      //! delete p.
      void do_delete (T *p) { delete p; }
    };

    //####################################################################
    //! Template for a smart pointer which is uncopyable and unassignable
    //! like boost::scoped_ptr, which allows a user-defined deletion
    //! function like boost::shared_ptr, and which allows release of ownership
    //! like std::auto_ptr.
    //! @see http://boost.org/libs/smart_ptr/scoped_ptr.htm
    //! @see http://boost.org/libs/smart_ptr/shared_ptr.htm
    //! @see http://dinkumware.com/htm_cpl/memory.html#auto_ptr
    template <typename T, typename Deleter = local_ptr_delete<T> >
    class local_ptr {
    public:
      //####################################################################
      //! Take ownership of the given object.
      //! @author Shane Beasley
      explicit local_ptr (T *obj = 0, Deleter d = Deleter())
	: object_(obj), deleter_(d) { }
      //####################################################################
      //! Get a reference to the owned object.
      //! @author Shane Beasley
      T &operator* () const { return *object_; }
      //####################################################################
      //! Get a pointer to the owned object.
      //! @author Shane Beasley
      T *operator-> () const { return object_; }
      //####################################################################
      //! Get a pointer to the owned object.
      //! @author Shane Beasley
      T *get () const { return object_; }
      //####################################################################
      //! Transfer ownership of the owned object to the caller.
      //! @author Shane Beasley
      T *release () { T *p = object_; object_ = 0; return p; }
      //####################################################################
      //! Take ownership of the given object, deleting the previously-owned
      //! object.
      //! @author Shane Beasley
      void reset (T *obj = 0) {
	if (object_) deleter_.do_delete(object_);
	object_ = obj;
      }
      //####################################################################
      //! Swap with another local_ptr.
      //! @author Shane Beasley
      void swap (local_ptr &rhs) { std::swap(impl_, rhs.impl_); }
      //####################################################################
      //! Delete the owned object.
      //! @author Shane Beasley
      ~local_ptr () { reset(); }

    private:
      local_ptr (const local_ptr &);
      local_ptr &operator= (const local_ptr &);
      T *object_;
      Deleter deleter_;
    };
  }
}
//####################################################################
//! A smart pointer (ptr = shared_ptr or scoped_ptr) to T.
//! @see http://boost.org/libs/smart_ptr/smart_ptr.htm
//! @author Shane Beasley
#define XMLWRAPP_PIMPL(ptr, T) ::xmlwrapp_boost::ptr<T>
//####################################################################
//! The private implementation type for T.
//! @author Shane Beasley
#define XMLWRAPP_IMPL(T) ::xmlwrapp::pimpl::impl<T>
//####################################################################
//! The private implementation type for
//! T<\link #XMLWRAPP_RW_ACCESS XMLWRAPP_RW_ACCESS\endlink>.
//! @author Shane Beasley
#define XMLWRAPP_IMPL_T(T) \
  ::xmlwrapp::pimpl::impl< T<XMLWRAPP_RW_ACCESS> >
//####################################################################

#endif
