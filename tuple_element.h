#ifndef COOL_TUPLE_ELEMENT_H_
#define COOL_TUPLE_ELEMENT_H_

#include <array>
#include <utility>
#include <type_traits>

namespace cool
{
    ///////////////////////////////////////////////////////////////////////////
    // tuple_element
    //
    // tuple_element is a generalization of std::tuple_element which finds the
    // Ith element in an arbitrary typelist T
    //
    // TODO: Use a specialization of std::tuple_element if it exists
    ///////////////////////////////////////////////////////////////////////////
    template<size_t I, typename T>
    struct tuple_element;

    template<size_t I, typename T>
    using tuple_element_t = typename tuple_element<I, T>::type;

    template<size_t I, typename T>
    struct tuple_element<I, const T>
    { using type = std::add_const_t<tuple_element_t<I, T>>; };

    template<size_t I, typename T>
    struct tuple_element<I, volatile T>
    { using type = std::add_volatile_t<tuple_element_t<I, T>>; };

    template<size_t I, typename T>
    struct tuple_element<I, const volatile T>
    { using type = std::add_cv_t<tuple_element_t<I, T>>; };

    template<template<typename...> class L, typename U, typename... Us>
    struct tuple_element<0, L<U, Us...>>
    { using type = U; };

    template<size_t I, template<typename...> class L, typename U, typename... Us>
    struct tuple_element<I, L<U, Us...>>
    { using type = tuple_element_t<I-1, L<Us...>>; };

    template<size_t I, typename U, size_t N>
    struct tuple_element<I, std::array<U, N>>
    { using type = U; static_assert(I<N); };

    template<typename T1, typename T2>
    struct tuple_element<0, std::pair<T1, T2>>
    { using type = T1; };

    template<typename T1, typename T2>
    struct tuple_element<1, std::pair<T1, T2>>
    { using type = T2; };
}

#endif /* COOL_TUPLE_ELEMENT_H_ */

