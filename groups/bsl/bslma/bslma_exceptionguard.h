// bslma_exceptionguard.h                                             -*-C++-*-
#ifndef INCLUDED_BSLMA_EXCEPTIONGUARD
#define INCLUDED_BSLMA_EXCEPTIONGUARD

#ifndef INCLUDED_BSLS_IDENT
#include <bsls_ident.h>
#endif
BSLS_IDENT("$Id: $")

//@PURPOSE: Provide a check that objects throwing exceptions do not change.
//
//@CLASSES:
//  bslma::ExceptionGuard: guard to check an object state has not changed
//
//@SEE_ALSO: bslma_testallocator
//
//@DESCRIPTION: This component provides a class that can be used to ASSERT if
// an object changes state when a method fails by throwing an exception.  This
// is often used to validate the strong exception safety guarantee in a test
// driver, usually with the test support macros provided by the component
// 'bslma_testallocator', such as 'BSLMA_TESTALLOCATOR_EXCEPTION_TEST_BEGIN'.
// The object under test must be CopyConstructible, and support the extended
// copy constructor taking an allocator.  Note that this may be a generalised
// STL allocator, conforming to the Allocator requirements of the C++ standard,
// rather than just a 'bslma::Allocator'.  This allows for testing standard
// library components such as those in 'bsl'.
//
// As the constructor must make a copy of the object under test, this class
// should not be used in a test driver until after the extended copy
// constructor has been proven tested.  Similarly, the destructor asserts that
// the value has not changed using 'operator==', which should also be confirmed
// as correct before relying on this class in a test driver.  Finally, the
// 'resetvalue' method should not be used prior to validating the copy-
// assignment operator.
//
///Usage
///-----
// TBD ...

#ifndef INCLUDED_BSLSCM_VERSION
#include <bslscm_version.h>
#endif

#ifndef INCLUDED_BSLS_ASSERT
#include <bsls_assert.h>
#endif

namespace BloombergLP {

namespace bslma {

class Allocator;

                            // ====================
                            // class ExceptionGuard
                            // ====================

template <class OBJECT>
class ExceptionGuard {
    // This class provide a mechanism to verify the strong exception guarantee
    // in exception-throwing code.  On construction, this class stores the
    // a copy of an object of the parameterized type 'OBJECT' and the address
    // of that object.  On destruction, if 'release' was not invoked, it will
    // verify the value of the object is the same as the value of the copy
    // create on construction.  This class requires the copy constructor and
    // 'operator ==' to be tested before use.

    // DATA
    int           d_line;      // the line number at construction
    OBJECT        d_copy;      // copy of the object being tested
    const OBJECT *d_object_p;  // address of the original object

  public:
    // CREATORS
    ExceptionGuard(const OBJECT *object,
                   int           line,
                   Allocator    *basicAllocator = 0);
        // Create the exception guard for the specified 'object' at the
        // specified 'line' number.  Optionally, specify 'basicAllocator' used
        // to supply memory.

    template <class ALLOCATOR>
    ExceptionGuard(const OBJECT     *object,
                   int               line,
                   const ALLOCATOR&  basicAllocator);
        // Create the exception guard for the specified 'object' at the
        // specified 'line' number.  Optionally, specify 'basicAllocator' used
        // to supply memory.

    ~ExceptionGuard();
        // Destroy the exception guard.  If the guard was not released, verify
        // that the state of the object supplied at construction has not
        // change.

    // MANIPULATORS
    void release();
        // Release the guard from verifying the state of the object.

    void resetValue(const OBJECT& value, int line);
        // Reset the expected state of the guarded object, if an exception 
        // should propagate past this guard, to the specified 'value', which is
        // set from the specified 'line'.
};

// ============================================================================
//                      TEMPLATE FUNCTION DEFINITIONS
// ============================================================================


                       // --------------------
                       // class ExceptionGuard
                       // --------------------

// CREATORS
template <class OBJECT>
inline
ExceptionGuard<OBJECT>::ExceptionGuard(const OBJECT *object,
                                       int           line,
                                       Allocator    *basicAllocator)
: d_line(line)
, d_copy(*object, basicAllocator)
, d_object_p(object)
{
}

template <class OBJECT>
template <class ALLOCATOR>
inline
ExceptionGuard<OBJECT>::ExceptionGuard(const OBJECT     *object,
                                       int               line,
                                       const ALLOCATOR&  basicAllocator)
: d_line(line)
, d_copy(*object, basicAllocator)
, d_object_p(object)
{
}

template <class OBJECT>
ExceptionGuard<OBJECT>::~ExceptionGuard()
{
    if (d_object_p) {
        // This test is tricky, as it is typically triggered while an exception
        // is active, and so should not have an assert handler that in turn
        // throws an exception.  Note that as this assertion is the whole
        // purpose of the class, we use 'BSLS_ASSERT_OPT' so that it is active
        // in most build modes.
        BSLS_ASSERT_OPT(d_copy == *d_object_p);
    }
}

// MANIPULATORS
template <class OBJECT>
inline
void ExceptionGuard<OBJECT>::release()
{
    d_object_p = 0;
}

template <class OBJECT>
inline
void ExceptionGuard<OBJECT>::resetValue(const OBJECT& value, int line)
{
    d_copy = value;
    d_line = line;
}

}  // close package namespace

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
