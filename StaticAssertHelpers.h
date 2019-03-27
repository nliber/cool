#ifndef COOL_STATICASSERTHELPERS_H_
#define COOL_STATICASSERTHELPERS_H_

// These are some helper template classes to give better error messages than
//  static_assert; namely, ones with the values or class name
//
// Example usage:
//
//  static_assert(AssertSizeof<char, 2>(), "");
//
// produces the following error messages of the form:
//
//  StaticAssertHelpers.h: error: invalid use of incomplete type ‘struct AssertEqual<Expected<long unsigned int, 2ul>, Actual<long unsigned int, 1ul> >’
//  StaticAssertHelpers.h: error: declaration of ‘struct AssertEqual<Expected<long unsigned int, 2ul>, Actual<long unsigned int, 1ul> >’
//
//  YourFile.cpp: error: ‘struct AssertSizeof<char, 2ul>’ used where a ‘bool’ was expected
//
// Expected<...> and Actual<...> derive from integral_constant so the names
//  "Expected" and "Actual" show up in the error message
//
// Assert<T> is always an incomplete type
//
// AssertTrue<C> derives from true_type iff the type is C<T, 0>
//
// AssertFalse<C> derives from true_type iff the type is C<T, v> where v != 0
//
// AssertEqual<E, A> derives from true_type iff E and A represent the same
//  integral constant; otherwise, it is an incomplete type
//
// AssertSizeof<T, e> derives from true_type iff sizeof(T) == e; otherwise, it
//  is an incomplete type
//
// Note: no need to put a message in the static_assert(AssertHelper<>(), ""),
//  since it will never be displayed.  Use it as a comment if you like

#include <type_traits>

namespace cool {

    template<typename T, T v>
    struct Expected : std::integral_constant<T, v> {};

    template<typename T, T v>
    struct Actual : std::integral_constant<T, v> {};


    template<typename...>
    struct Assert;


    template<typename C>
    struct AssertTrue;

    template<template<typename U, U> class C, typename T, T v>
    struct AssertTrue<C<T, v>> : std::true_type {};

    template<template<typename U, U> class C, typename T>
    struct AssertTrue<C<T, 0>>;


    template<typename C>
    struct AssertFalse;

    template<template<typename U, U> class C, typename T, T v>
    struct AssertFalse<C<T, v>>;

    template<template<typename U, U> class C, typename T>
    struct AssertFalse<C<T, 0>> : std::true_type {};


    template<typename E, typename A>
    struct AssertEqual;

    template<template<typename LL, LL> class L, template<typename RR, RR> class R, typename T, T v>
    struct AssertEqual<L<T, v>, R<T, v>> : std::true_type {};


    // Use decltype(sizeof(int)) instead of size_t so we don't have to include another header

    template<typename T, decltype(sizeof(int)) e>
    struct AssertSizeof : AssertEqual<Expected<decltype(sizeof(int)), e>, Actual<decltype(sizeof(int)), sizeof(T)>>::type {};

} // cool namespace

#endif /* COOL_STATICASSERTHELPERS_H_ */

