// (C) Copyright David Abrahams 2002. Permission to copy, use, modify,
// sell and distribute this software is granted provided this
// copyright notice appears in all copies. This software is provided
// "as is" without express or implied warranty, and with no claim as
// to its suitability for any purpose.

// Boost versions of
//
//    std::iterator_traits<>::iterator_category
//    std::iterator_traits<>::difference_type
//    std::distance()
//
// ...for all compilers and iterators
//
// Additionally, if X is a pointer
//    std::iterator_traits<X>::pointer

// Otherwise, if partial specialization is supported or X is not a pointer
//    std::iterator_traits<X>::value_type
//    std::iterator_traits<X>::pointer
//    std::iterator_traits<X>::reference
//
// See http://www.boost.org for most recent version including documentation.

// Revision History
// 04 Mar 2001 - More attempted fixes for Intel C++ (David Abrahams)
// 03 Mar 2001 - Put all implementation into namespace
//               xmlwrapp_boost::detail::iterator_traits_. Some progress made on fixes
//               for Intel compiler. (David Abrahams)
// 02 Mar 2001 - Changed XMLWRAPP_BOOST_MSVC to XMLWRAPP_BOOST_MSVC_STD_ITERATOR in a few
//               places. (Jeremy Siek)
// 19 Feb 2001 - Improved workarounds for stock MSVC6; use yes_type and
//               no_type from type_traits.hpp; stopped trying to remove_cv
//               before detecting is_pointer, in honor of the new type_traits
//               semantics. (David Abrahams)
// 13 Feb 2001 - Make it work with nearly all standard-conforming iterators
//               under raw VC6. The one category remaining which will fail is
//               that of iterators derived from std::iterator but not
//               xmlwrapp_boost::iterator and which redefine difference_type.
// 11 Feb 2001 - Clean away code which can never be used (David Abrahams)
// 09 Feb 2001 - Always have a definition for each traits member, even if it
//               can't be properly deduced. These will be incomplete types in
//               some cases (undefined<void>), but it helps suppress MSVC errors
//               elsewhere (David Abrahams)
// 07 Feb 2001 - Support for more of the traits members where possible, making
//               this useful as a replacement for std::iterator_traits<T> when
//               used as a default template parameter.
// 06 Feb 2001 - Removed useless #includes of standard library headers
//               (David Abrahams)

#ifndef ITERATOR_DWA122600_HPP_
# define ITERATOR_DWA122600_HPP_

# include <xmlwrapp/boost/config.hpp>
# include <xmlwrapp/boost/type_traits/remove_const.hpp>
# include <xmlwrapp/boost/type_traits/detail/yes_no_type.hpp>
# include <xmlwrapp/boost/type_traits/is_pointer.hpp>
# include <xmlwrapp/boost/type_traits/is_base_and_derived.hpp>
# include <xmlwrapp/boost/mpl/if.hpp>
# include <xmlwrapp/boost/mpl/aux_/has_xxx.hpp>
# include <iterator>
# include <cstddef>

// should be the last #include
#include "xmlwrapp/boost/type_traits/detail/bool_trait_def.hpp"

// STLPort 4.0 and betas have a bug when debugging is enabled and there is no
// partial specialization: instead of an iterator_category typedef, the standard
// container iterators have _Iterator_category.
//
// Also, whether debugging is enabled or not, there is a broken specialization
// of std::iterator<output_iterator_tag,void,void,void,void> which has no
// typedefs but iterator_category.
# if defined(__SGI_STL_PORT)

#  if (__SGI_STL_PORT <= 0x410) && !defined(__STL_CLASS_PARTIAL_SPECIALIZATION) && defined(__STL_DEBUG)
#   define XMLWRAPP_BOOST_BAD_CONTAINER_ITERATOR_CATEGORY_TYPEDEF
#  endif

#  define XMLWRAPP_BOOST_BAD_OUTPUT_ITERATOR_SPECIALIZATION

# endif // STLPort <= 4.1b4 && no partial specialization

namespace xmlwrapp_boost { namespace detail {

XMLWRAPP_BOOST_MPL_HAS_XXX_TRAIT_DEF(value_type)
XMLWRAPP_BOOST_MPL_HAS_XXX_TRAIT_DEF(reference)
XMLWRAPP_BOOST_MPL_HAS_XXX_TRAIT_DEF(pointer)
XMLWRAPP_BOOST_MPL_HAS_XXX_TRAIT_DEF(difference_type)
XMLWRAPP_BOOST_MPL_HAS_XXX_TRAIT_DEF(iterator_category)

# if !defined(XMLWRAPP_BOOST_NO_STD_ITERATOR_TRAITS)             \
  && !defined(XMLWRAPP_BOOST_NO_TEMPLATE_PARTIAL_SPECIALIZATION) \
  && !defined(XMLWRAPP_BOOST_MSVC_STD_ITERATOR)
// Define a new template so it can be specialized
template <class Iterator>
struct iterator_traits
    : std::iterator_traits<Iterator>
{};
using std::distance;
# elif  !defined(XMLWRAPP_BOOST_NO_TEMPLATE_PARTIAL_SPECIALIZATION) \
     && !defined(XMLWRAPP_BOOST_MSVC_STD_ITERATOR)

// Rogue Wave Standard Library fools itself into thinking partial
// specialization is missing on some platforms (e.g. Sun), so fails to
// supply iterator_traits!
template <class Iterator>
struct iterator_traits
{
    typedef typename Iterator::value_type value_type;
    typedef typename Iterator::reference reference;
    typedef typename Iterator::pointer pointer;
    typedef typename Iterator::difference_type difference_type;
    typedef typename Iterator::iterator_category iterator_category;
};

template <class T>
struct iterator_traits<T*>
{
    typedef T value_type;
    typedef T& reference;
    typedef T* pointer;
    typedef std::ptrdiff_t difference_type;
    typedef std::random_access_iterator_tag iterator_category;
};

template <class T>
struct iterator_traits<T const*>
{
    typedef T value_type;
    typedef T const& reference;
    typedef T const* pointer;
    typedef std::ptrdiff_t difference_type;
    typedef std::random_access_iterator_tag iterator_category;
};

# else

// is_mutable_iterator --
//
//   A metafunction returning true iff T is a mutable iterator type
//   with a nested value_type. Will only work portably with iterators
//   whose operator* returns a reference, but that seems to be OK for
//   the iterators supplied by Dinkumware. Some input iterators may
//   compile-time if they arrive here, and if the compiler is strict
//   about not taking the address of an rvalue.

// This one detects ordinary mutable iterators - the result of
// operator* is convertible to the value_type.
template <class T>
type_traits::yes_type is_mutable_iterator_helper(T const*, XMLWRAPP_BOOST_DEDUCED_TYPENAME T::value_type*);

// This one detects output iterators such as ostream_iterator which
// return references to themselves.
template <class T>
type_traits::yes_type is_mutable_iterator_helper(T const*, T const*);

type_traits::no_type is_mutable_iterator_helper(...);

template <class T>
struct is_mutable_iterator_impl
{
    static T t;
    
    XMLWRAPP_BOOST_STATIC_CONSTANT(bool, value = sizeof(
               detail::is_mutable_iterator_helper((T*)0, &*t))
           == sizeof(type_traits::yes_type)
        );
};

XMLWRAPP_BOOST_TT_AUX_BOOL_TRAIT_DEF1(
    is_mutable_iterator,T,::xmlwrapp_boost::detail::is_mutable_iterator_impl<T>::value)


// is_full_iterator_traits --
//
//   A metafunction returning true iff T has all the requisite nested
//   types to satisfy the requirements for a fully-conforming
//   iterator_traits implementation.
template <class T>
struct is_full_iterator_traits_impl
{
    enum { value = 
           has_value_type<T>::value 
           & has_reference<T>::value 
           & has_pointer<T>::value 
           & has_difference_type<T>::value
           & has_iterator_category<T>::value
    };
};

XMLWRAPP_BOOST_TT_AUX_BOOL_TRAIT_DEF1(
    is_full_iterator_traits,T,::xmlwrapp_boost::detail::is_full_iterator_traits_impl<T>::value)


#  ifdef XMLWRAPP_BOOST_BAD_CONTAINER_ITERATOR_CATEGORY_TYPEDEF
XMLWRAPP_BOOST_MPL_HAS_XXX_TRAIT_DEF(_Iterator_category)
    
// is_stlport_40_debug_iterator --
//
//   A metafunction returning true iff T has all the requisite nested
//   types to satisfy the requirements of an STLPort 4.0 debug iterator
//   iterator_traits implementation.
template <class T>
struct is_stlport_40_debug_iterator_impl
{
    enum { value = 
           has_value_type<T>::value 
           & has_reference<T>::value 
           & has_pointer<T>::value 
           & has_difference_type<T>::value
           & has__Iterator_category<T>::value
    };
};

XMLWRAPP_BOOST_TT_AUX_BOOL_TRAIT_DEF1(
    is_stlport_40_debug_iterator,T,::xmlwrapp_boost::detail::is_stlport_40_debug_iterator_impl<T>::value)

template <class T>
struct stlport_40_debug_iterator_traits
{
    typedef typename T::value_type value_type;
    typedef typename T::reference reference;
    typedef typename T::pointer pointer;
    typedef typename T::difference_type difference_type;
    typedef typename T::_Iterator_category iterator_category;
};
# endif // XMLWRAPP_BOOST_BAD_CONTAINER_ITERATOR_CATEGORY_TYPEDEF 

template <class T> struct pointer_iterator_traits;

# ifndef XMLWRAPP_BOOST_NO_TEMPLATE_PARTIAL_SPECIALIZATION
template <class T>
struct pointer_iterator_traits<T*>
{
    typedef remove_const<T>::type value_type;
    typedef T* pointer;
    typedef T& reference;
    typedef std::random_access_iterator_tag iterator_category;
    typedef std::ptrdiff_t difference_type;
};
# else 
template <class Ptr>
struct must_manually_specialize_xmlwrapp_boost_detail_iterator_traits;

template <class T>
struct pointer_iterator_traits
{
    typedef T pointer;
    typedef std::random_access_iterator_tag iterator_category;
    typedef std::ptrdiff_t difference_type;

    // Makes MSVC6 happy under some circumstances
    typedef must_manually_specialize_xmlwrapp_boost_detail_iterator_traits<T> value_type;
    typedef must_manually_specialize_xmlwrapp_boost_detail_iterator_traits<T> reference;
};

// Use this as a base class in manual iterator_traits specializations
// for pointer types. T should be the value_type. CV should be the
// cv-qualified value_type to which */& is added in order to produce
// pointer/reference.
template <class T, class CV = T>
struct ptr_iter_traits
{
    typedef T value_type;
    typedef CV* pointer;
    typedef CV& reference;
    typedef std::random_access_iterator_tag iterator_category;
    typedef std::ptrdiff_t difference_type;
};
# endif // XMLWRAPP_BOOST_NO_TEMPLATE_PARTIAL_SPECIALIZATION

// We'll sort iterator types into one of these classifications, from which we
// can determine the difference_type, pointer, reference, and value_type
template <class Iterator>
struct standard_iterator_traits
{
    typedef typename Iterator::difference_type difference_type;
    typedef typename Iterator::value_type value_type;
    typedef typename Iterator::pointer pointer;
    typedef typename Iterator::reference reference;
    typedef typename Iterator::iterator_category iterator_category;
};

template <class Iterator>
struct msvc_stdlib_mutable_traits
    : std::iterator_traits<Iterator>
{
    typedef typename std::iterator_traits<Iterator>::distance_type difference_type;
    typedef value_type* pointer;
    typedef value_type& reference;
};

template <class Iterator>
struct msvc_stdlib_const_traits
    : std::iterator_traits<Iterator>
{
    typedef typename std::iterator_traits<Iterator>::distance_type difference_type;
    typedef const value_type* pointer;
    typedef const value_type& reference;
};

# ifdef XMLWRAPP_BOOST_BAD_OUTPUT_ITERATOR_SPECIALIZATION
template <class Iterator>
struct is_bad_output_iterator
    : is_base_and_derived<
        std::iterator<std::output_iterator_tag,void,void,void,void>
        , Iterator>
{
};

struct bad_output_iterator_traits
{
    typedef void value_type;
    typedef void difference_type;
    typedef std::output_iterator_tag iterator_category;
    typedef void pointer;
    typedef void reference;
};
# endif

// If we're looking at an MSVC6 (old Dinkumware) ``standard''
// iterator, this will generate an appropriate traits class. 
template <class Iterator>
struct msvc_stdlib_iterator_traits
    : mpl::if_<
       is_mutable_iterator<Iterator>
       , msvc_stdlib_mutable_traits<Iterator>
       , msvc_stdlib_const_traits<Iterator>
      >::type
{};

template <class Iterator>
struct non_pointer_iterator_traits
    : mpl::if_<
        // if the iterator contains all the right nested types...
        is_full_iterator_traits<Iterator>
        // Use a standard iterator_traits implementation
        , standard_iterator_traits<Iterator>
# ifdef XMLWRAPP_BOOST_BAD_CONTAINER_ITERATOR_CATEGORY_TYPEDEF
        // Check for STLPort 4.0 broken _Iterator_category type
        , mpl::if_<
             is_stlport_40_debug_iterator<Iterator>
             , stlport_40_debug_iterator_traits<Iterator>
# endif
        // Otherwise, assume it's a Dinkum iterator
        , msvc_stdlib_iterator_traits<Iterator>
# ifdef XMLWRAPP_BOOST_BAD_CONTAINER_ITERATOR_CATEGORY_TYPEDEF
        >::type
# endif 
    >::type
{
};

template <class Iterator>
struct iterator_traits_aux
    : mpl::if_<
        is_pointer<Iterator>
        , pointer_iterator_traits<Iterator>
        , non_pointer_iterator_traits<Iterator>
    >::type
{
};

template <class Iterator>
struct iterator_traits
{
    // Explicit forwarding from base class needed to keep MSVC6 happy
    // under some circumstances.
 private:
# ifdef XMLWRAPP_BOOST_BAD_OUTPUT_ITERATOR_SPECIALIZATION
    typedef 
    typename mpl::if_<
        is_bad_output_iterator<Iterator>
        , bad_output_iterator_traits
        , iterator_traits_aux<Iterator>
    >::type base;
# else
    typedef iterator_traits_aux<Iterator> base;
# endif
 public:
    typedef typename base::value_type value_type;
    typedef typename base::pointer pointer;
    typedef typename base::reference reference;
    typedef typename base::difference_type difference_type;
    typedef typename base::iterator_category iterator_category;
};

// This specialization cuts off ETI (Early Template Instantiation) for MSVC.
template <> struct iterator_traits<int>{};

namespace iterator_traits_
{
  template <class Iterator, class Difference>
  struct distance_select
  {
      static Difference execute(Iterator i1, const Iterator i2, ...)
      {
          typename Difference result = 0;
          while (i1 != i2)
          {
              ++i1;
              ++result;
          }
          return result;
      }

      static Difference execute(Iterator i1, const Iterator i2, std::random_access_iterator_tag*)
      {
          return i2 - i1;
      }
  };
} // namespace xmlwrapp_boost_boost::detail::iterator_traits_

template <class Iterator>
inline typename iterator_traits<Iterator>::difference_type
distance(Iterator first, Iterator last)
{
    typedef typename iterator_traits<Iterator>::difference_type diff_t;
    typedef typename ::xmlwrapp_boost::detail::iterator_traits<Iterator>::iterator_category iterator_category;
    
    return iterator_traits_::distance_select<Iterator,diff_t>::execute(
        first, last, (iterator_category*)0);
}
# endif // workarounds

}} // namespace xmlwrapp_boost_boost::detail

# undef XMLWRAPP_BOOST_BAD_CONTAINER_ITERATOR_CATEGORY_TYPEDEF
# undef XMLWRAPP_BOOST_BAD_OUTPUT_ITERATOR_SPECIALIZATION

#endif // ITERATOR_DWA122600_HPP_
