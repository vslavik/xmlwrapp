#ifndef XMLWRAPP_BOOST_SCOPED_ARRAY_HPP_INCLUDED
#define XMLWRAPP_BOOST_SCOPED_ARRAY_HPP_INCLUDED

//  (C) Copyright Greg Colvin and Beman Dawes 1998, 1999.
//  Copyright (c) 2001, 2002 Peter Dimov
//
//  Permission to copy, use, modify, sell and distribute this software
//  is granted provided this copyright notice appears in all copies.
//  This software is provided "as is" without express or implied
//  warranty, and with no claim as to its suitability for any purpose.
//
//  http://www.boost.org/libs/smart_ptr/scoped_array.htm
//

#include <xmlwrapp/boost/assert.hpp>
#include <xmlwrapp/boost/checked_delete.hpp>
#include <xmlwrapp/boost/config.hpp>   // in case ptrdiff_t not in std
#include <cstddef>            // for std::ptrdiff_t

namespace xmlwrapp_boost
{

// Debug hooks

#if defined(XMLWRAPP_BOOST_SP_ENABLE_DEBUG_HOOKS)

void sp_array_constructor_hook(void * p);
void sp_array_destructor_hook(void * p);

#endif

//  scoped_array extends scoped_ptr to arrays. Deletion of the array pointed to
//  is guaranteed, either on destruction of the scoped_array or via an explicit
//  reset(). Use shared_array or std::vector if your needs are more complex.

template<class T> class scoped_array // noncopyable
{
private:

    T * ptr;

    scoped_array(scoped_array const &);
    scoped_array & operator=(scoped_array const &);

    typedef scoped_array<T> this_type;

public:

    typedef T element_type;

    explicit scoped_array(T * p = 0) : ptr(p) // never throws
    {
#if defined(XMLWRAPP_BOOST_SP_ENABLE_DEBUG_HOOKS)
        xmlwrapp_boost::sp_array_constructor_hook(ptr);
#endif
    }

    ~scoped_array() // never throws
    {
#if defined(XMLWRAPP_BOOST_SP_ENABLE_DEBUG_HOOKS)
        xmlwrapp_boost::sp_array_destructor_hook(ptr);
#endif
        xmlwrapp_boost::checked_array_delete(ptr);
    }

    void reset(T * p = 0) // never throws
    {
        XMLWRAPP_BOOST_ASSERT(p == 0 || p != ptr); // catch self-reset errors
        this_type(p).swap(*this);
    }

    T & operator[](std::ptrdiff_t i) const // never throws
    {
        XMLWRAPP_BOOST_ASSERT(ptr != 0);
        XMLWRAPP_BOOST_ASSERT(i >= 0);
        return ptr[i];
    }

    T * get() const // never throws
    {
        return ptr;
    }

    // implicit conversion to "bool"

    typedef T * (this_type::*unspecified_bool_type)() const;

    operator unspecified_bool_type() const // never throws
    {
        return ptr == 0? 0: &this_type::get;
    }

    bool operator! () const // never throws
    {
        return ptr == 0;
    }

    void swap(scoped_array & b) // never throws
    {
        T * tmp = b.ptr;
        b.ptr = ptr;
        ptr = tmp;
    }

};

template<class T> inline void swap(scoped_array<T> & a, scoped_array<T> & b) // never throws
{
    a.swap(b);
}

} // namespace xmlwrapp_boost

#endif  // #ifndef XMLWRAPP_BOOST_SCOPED_ARRAY_HPP_INCLUDED
