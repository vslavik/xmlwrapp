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

#ifndef XMLWRAPP_DETAIL_NONCOPYABLE_H_20040801T145201
#define XMLWRAPP_DETAIL_NONCOPYABLE_H_20040801T145201

#ifndef DOXYGEN_HIDE_INTERNAL

namespace xmlwrapp {
  namespace detail {
    //####################################################################
    //! Non-polymorphic base class that prevents copy-construction or
    //! assignment of derived classes by making this functionality private.
    //! Attempting to copy or assign instances will elicit useful
    //! error messages about being "noncopyable".
    class noncopyable {
    protected:
      noncopyable () { }
      ~noncopyable () { }

    private:
      explicit noncopyable (const noncopyable &);
      noncopyable &operator= (const noncopyable &);
    };

    //####################################################################
    //! Non-polymorphic base class that effectively prevents instantiation
    //! of derived classes. Useful primarily as a hack for Visual C++ 6.0
    //! in conjunction with template specialization.
    class uninstantiable : noncopyable {
    private:
      uninstantiable ();
      ~uninstantiable ();
      friend class no_friends; // g++ warns "all private, no friends"
    };
  }
}

#endif

#endif
