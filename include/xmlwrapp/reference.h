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

#ifndef XMLWRAPP_REFERENCE_H_20030809T064856
#define XMLWRAPP_REFERENCE_H_20030809T064856

namespace xmlwrapp {
  //####################################################################
  //! @internal Various implementation details.
  namespace detail {
    //####################################################################
    //! A reference to T, restricted as per the given access specifier.
    //! 
    //! Useful when the preprocessor or a template parameter might create
    //! an ambiguity:
    //!
    //! \include reference.cxx
    template <typename T>
    class reference {
    public:
      //####################################################################
      //! Reference to T.
      typedef T &ref_type;
      //####################################################################
      //! Construct a new reference to T.
      //! @author Shane Beasley
      reference (ref_type ref) : myRef(ref) { }
      //####################################################################
      //! Act as a reference to T.
      //! @author Shane Beasley
      operator ref_type () const { return myRef; }
      //####################################################################
      //! Get the reference to T represented by this object.
      //! @author Shane Beasley
      ref_type ref () const { return myRef; }
      //####################################################################
    private:
      ref_type myRef;
    };
  }
}

#endif
