// bslstl_forwarditerator.h                                           -*-C++-*-
#ifndef INCLUDED_BSLSTL_FORWARDITERATOR
#define INCLUDED_BSLSTL_FORWARDITERATOR

#ifndef INCLUDED_BSLS_IDENT
#include <bsls_ident.h>
#endif
BSLS_IDENT("$Id: $")

//@PURPOSE: Provide a template to create STL-compliant forward iterators.
//
//@CLASSES:
//  bslstl::ForwardIterator: forward iterator template
//
//@SEE_ALSO: bslstl_iterator, bslstl_bidirectionaliterator,
//           bslstl_randomaccessiterator
//
//@DESCRIPTION: This component provides an iterator adaptor that, given an
// implementation class defining a core set of iterator functionality, adapts
// it to provide an STL-compliant forward iterator interface.  The set of
// requirements for a forward iterator is found in "Table 106: Forward iterator
// requirements", under the tag "[forward.iterators]".  (Note that this
// reference is sourced in N3092, a C++0x working paper; the actual table
// number may vary in the actual standard.)  Include bsl_iterator.h to use this
// component.
//
///Usage
///-----
// Given the following "iterator-like" implementation class:
//..
//  template <class T>
//  class my_IteratorImp {
//    public:
//      // CREATORS
//      my_IteratorImp();
//      my_IteratorImp(const my_IteratorImp&);
//      ~my_IteratorImp();
//
//      // An additional value-constructor should be supplied that can be
//      // called by the unspecified container type, providing access to the
//      // container's internal data structure that is to be iterated over.
//      // This would typically be called by 'begin' and 'end'.
//
//      // MANIPULATORS
//      my_IteratorImp& operator=(const my_IteratorImp&);
//
//      void operator++();
//
//      // ACCESSORS
//      T& operator*() const;
//  };
//
//  template <class T>
//  bool operator==(const my_IteratorImp<T>& lhs,
//                  const my_IteratorImp<T>& rhs);
//..
// simply add the following two 'typedef's to any container class that provides
// 'my_IteratorImp<T>' access, and the container will have STL-compliant
// forward iterators:
//..
//  typedef bslstl::ForwardIterator<T, my_IteratorImp<T> > iterator;
//  typedef bslstl::ForwardIterator<const T, my_IteratorImp<T> >
//                                                              const_iterator;
//..
// Note that the implementation for 'const_iterator' is 'my_IteratorImp<T>' and
// *not* 'my_IteratorImp<const T>', rather the 'const' is added to the return
// value of 'operator*' by way of conversion to the first template argument.

// DOCUMENTATION IS INCOMPLETE: Note that we need to say something about 'T'
// and iterator stability.  Incrementing two copies of 'T' must produce two
// iterators that compare equal, and refer to exactly the same object.  Without
// this guarantee, we cannot build the multi-pass property of Forward Iterator,
// and merely have an Input Iterator.  This is not something we can detect at
// compile time, and is prohibitively expensive to validate at runtime.  It is
// the sort of thing that should be validated by a Forward Iterator test suite,
// it could be what we really need, in addition to the adapters, is a
// generalized test case to validate that a type conforms to the iterator
// requirements.

// Prevent 'bslstl' headers from being included directly in 'BSL_OVERRIDES_STD'
// mode.  Doing so is unsupported, and is likely to cause compilation errors.
#if defined(BSL_OVERRIDES_STD) && !defined(BSL_STDHDRS_PROLOGUE_IN_EFFECT)
#error "<bslstl_forwarditerator.h> header can't be included directly in \
BSL_OVERRIDES_STD mode"
#endif

#ifndef INCLUDED_BSLSCM_VERSION
#include <bslscm_version.h>
#endif

#ifndef INCLUDED_BSLSTL_ITERATOR
#include <bslstl_iterator.h>
#endif

#ifndef INCLUDED_BSLMF_REMOVECVQ
#include <bslmf_removecvq.h>
#endif

#ifndef INCLUDED_BSLS_UTIL
#include <bsls_util.h>
#endif

#ifndef INCLUDED_ITERATOR
#include <iterator>
#define INCLUDED_ITERATOR
#endif

#ifndef INCLUDED_CSTDDEF
#include <cstddef>  // 'ptrdiff_t'
#define INCLUDED_CSTDDEF
#endif

namespace BloombergLP {

namespace bslstl {

                    //======================
                    // class ForwardIterator
                    //======================

template <typename T, typename ITER_IMP, typename TAG_TYPE =
                                                     std::forward_iterator_tag>
class ForwardIterator
                   : public std::iterator<TAG_TYPE, T, std::ptrdiff_t, T *, T&>
{
    // Given an 'ITER_IMP' type that implements a minimal subset of an iterator
    // interface, this template generates a complete iterator that meets all of
    // the requirements of a "forward iterator" in the C++ standard.  If 'T' is
    // const-qualified, then the resulting type is a const iterator.  'T' shall
    // not be a function or reference type.  'ITER_IMP' must provide public
    // operations so that, for objects 'i' and 'j' of type 'ITER_IMP', the
    // following operations are supported:
    //..
    //     ITER_IMP i;                          Default construction
    //     ITER_IMP j(i);                       Copy construction
    //     i = j                                Assignment
    //     ++i                                  Increment to next element
    //     i == j     // convertible to bool    Equality comparison
    //     *i         // convertible to T&      Element access (dereference)
    //..

    // PRIVATE TYPES
    typedef typename bsl::remove_cv<T>::type UnCvqT;   // value type without
                                                       // 'const' and
                                                       // 'volatile'
                                                       // qualifications

  private:
    // DATA
    ITER_IMP d_imp;  // externally-supplied implementation of iterator
                     // functionality

  public:
    // TYPES
    typedef T                          value_type;
    typedef std::ptrdiff_t             difference_type;
    typedef T                         *pointer;
    typedef T&                         reference;
    typedef std::forward_iterator_tag  iterator_category;

    // CREATORS
    ForwardIterator();
        // Construct the default value for this iterator type.  All default-
        // constructed 'ForwardIterator' objects represent non-dereferenceable
        // iterators into the same empty range.  They do not have a singular
        // value unless an object of the type specified by the template
        // parameter 'ITER_IMP' has a singular value after
        // value-initialization.

    ForwardIterator(const ITER_IMP& implementation);
        // Construct a forward iterator having the specified 'implementation'
        // of the parameterized 'ITER_IMP' type.

    //! ForwardIterator(const ForwardIterator& original);
        // Create a 'ForwardIterator' having the same value as the specified
        // 'original' iterator.  Note that this method's definition is compiler
        // generated.

    ForwardIterator(const ForwardIterator<UnCvqT,ITER_IMP,TAG_TYPE>& other);
        // Construct a forward iterator from another (compatible)
        // 'ForwardIterator' type, e.g., a mutable iterator of the same type.
        // Note that since this is a template, it has a lower priority in
        // overload resolution than the other constructors.

    //! ~ForwardIterator();
        // Destroy this iterator.  Note that this method's definition is
        // compiler generated.

    // MANIPULATORS
    //! ForwardIterator& operator=(const ForwardIterator& rhs);
        // Copy the value of the specified 'rhs' to this iterator.  Return a
        // reference to this modifiable object.  Note that this method's
        // definition is compiler generated.

    ForwardIterator& operator++();
        // Increment to the next element.  Return a reference to this
        // modifiable iterator.  The behavior is undefined if, on entry, this
        // iterator has the past-the-end value for an iterator over the
        // underlying sequence.

    ITER_IMP& imp();
        // Return a modifiable reference to the implementation object.

    // ACCESSORS
    T& operator*() const;
        // Return a reference to the current, modifiable element.  The behavior
        // is undefined if this iterator has the past-the-end value for an
        // iterator over the underlying sequence.

    T *operator->() const;
        // Return a pointer to the current, modifiable element.  The behavior
        // is undefined if this iterator has the past-the-end value for an
        // iterator over the underlying sequence.

    const ITER_IMP& imp() const;
        // Return a non-modifiable reference to the implementation object.
};

// FREE OPERATORS
template <typename T1, typename T2, typename ITER_IMP, typename TAG_TYPE>
bool operator==(const ForwardIterator<T1,ITER_IMP,TAG_TYPE>& lhs,
                const ForwardIterator<T2,ITER_IMP,TAG_TYPE>& rhs);
    // Return 'true' if the specified 'lhs' iterator has the same value as the
    // specified 'rhs' iterator, and 'false' otherwise.  Two iterators have the
    // same value if they refer to the same element, or both have the past-the-
    // end value for the underlying sequence.  The behavior is undefined unless
    // both iterators refer to the same underlying sequence.

template <typename T1, typename T2, typename ITER_IMP, typename TAG_TYPE>
bool operator!=(const ForwardIterator<T1,ITER_IMP,TAG_TYPE>& lhs,
                const ForwardIterator<T2,ITER_IMP,TAG_TYPE>& rhs);
    // Return 'true' if the specified 'lhs' iterator does not have the same
    // value as the specified 'rhs' iterator, and 'false' otherwise.  Two
    // iterators do not have the same value if (1) they do not refer to the
    // same element and (2) both do not have the past-the-end iterator value
    // for the underlying sequence.  The behavior is undefined unless both
    // iterators refer to the same underlying sequence.

template <typename T, typename ITER_IMP, typename TAG_TYPE>
ForwardIterator<T,ITER_IMP,TAG_TYPE>
operator++(ForwardIterator<T,ITER_IMP,TAG_TYPE>& iter, int);
    // Increment the specified 'iter' to the next element.  Return the previous
    // value of 'iter'.  The behavior is undefined if, on entry, 'iter' has the
    // past-the-end value for an iterator of the underlying sequence.

// ===========================================================================
//                      INLINE FUNCTION DEFINITIONS
// ===========================================================================

                        //-----------------------------
                        // class ForwardIterator
                        //-----------------------------

// CREATORS
template <typename T, typename ITER_IMP, typename TAG_TYPE>
inline
ForwardIterator<T,ITER_IMP,TAG_TYPE>::ForwardIterator()
: d_imp()
{
}

template <typename T, typename ITER_IMP, typename TAG_TYPE>
inline
ForwardIterator<T,ITER_IMP,TAG_TYPE>::
ForwardIterator(const ITER_IMP& implementation)
: d_imp(implementation)
{
}

template <typename T, typename ITER_IMP, typename TAG_TYPE>
inline
ForwardIterator<T,ITER_IMP,TAG_TYPE>::
ForwardIterator(const ForwardIterator<UnCvqT,ITER_IMP,TAG_TYPE>& other)
: d_imp(other.imp())
{
}

// MANIPULATORS
template <typename T, typename ITER_IMP, typename TAG_TYPE>
inline
ForwardIterator<T,ITER_IMP,TAG_TYPE>&
ForwardIterator<T,ITER_IMP,TAG_TYPE>::operator++()
{
    ++this->d_imp;
    return *this;
}

template <typename T, typename ITER_IMP, typename TAG_TYPE>
inline
ITER_IMP& ForwardIterator<T,ITER_IMP,TAG_TYPE>::imp()
{
    return d_imp;
}

// ACCESSORS
template <typename T, typename ITER_IMP, typename TAG_TYPE>
inline
T& ForwardIterator<T,ITER_IMP,TAG_TYPE>::operator*() const
{
    return *d_imp;
}

template <typename T, typename ITER_IMP, typename TAG_TYPE>
inline
T *ForwardIterator<T,ITER_IMP,TAG_TYPE>::operator->() const
{
    return BSLS_UTIL_ADDRESSOF(*d_imp);
}

template <typename T, typename ITER_IMP, typename TAG_TYPE>
inline
const ITER_IMP&
ForwardIterator<T,ITER_IMP,TAG_TYPE>::imp() const
{
    return d_imp;
}

}  // close package namespace

// FREE OPERATORS
template <typename T, typename ITER_IMP, typename TAG_TYPE>
inline
bslstl::ForwardIterator<T,ITER_IMP,TAG_TYPE>
bslstl::operator++(ForwardIterator<T,ITER_IMP,TAG_TYPE>& iter, int)
{
    ForwardIterator<T,ITER_IMP,TAG_TYPE> tmp(iter);
    ++iter;
    return tmp;
}

template <typename T1, typename T2, typename ITER_IMP, typename TAG_TYPE>
inline
bool bslstl::operator==(const ForwardIterator<T1,ITER_IMP,TAG_TYPE>& lhs,
                        const ForwardIterator<T2,ITER_IMP,TAG_TYPE>& rhs)
{
    return lhs.imp() == rhs.imp();
}

template <typename T1,  typename T2, typename ITER_IMP, typename TAG_TYPE>
inline
bool bslstl::operator!=(const ForwardIterator<T1,ITER_IMP,TAG_TYPE>& lhs,
                        const ForwardIterator<T2,ITER_IMP,TAG_TYPE>& rhs)
{
    return !(lhs == rhs);
}


}  // close enterprise namespace

#endif

// ----------------------------------------------------------------------------
// Copyright (C) 2013 Bloomberg Finance L.P.
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to
// deal in the Software without restriction, including without limitation the
// rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
// sell copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
// FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
// IN THE SOFTWARE.
// ----------------------------- END-OF-FILE ----------------------------------
