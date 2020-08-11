#ifndef COOL_IS_TRIVIALLY_COPYABLE_H_
#define COOL_IS_TRIVIALLY_COPYABLE_H_

#include <type_traits>

namespace cool
{
    ///////////////////////////////////////////////////////////////////////////
    // is_trivially_copyable
    //
    //  is_trivially_copyable() is a helper function when trying to debug
    //  things that aren't trivially copyable but are supposed to be.  With
    //  modern compilers, each static_assert will fire for each thing that is
    //  wrong.
    //
    //  Works with C++11 or later.
    //
    ///////////////////////////////////////////////////////////////////////////
    template<typename T>
    constexpr void is_trivially_copyable() noexcept
    {
        // Both clang and gcc have a bug where they don't take into account
        // the trivial, non-deleted destructor
        constexpr bool tc = std::is_trivially_copyable<T>::value &&
                            std::is_trivially_destructible<T>::value;

        constexpr bool cc = std::is_copy_constructible<T>::value;
        constexpr bool mc = std::is_move_constructible<T>::value;
        constexpr bool ca = std::is_copy_assignable<T>::value;
        constexpr bool ma = std::is_move_assignable<T>::value;
        constexpr bool d  = std::is_destructible<T>::value;

        constexpr bool tcc = std::is_trivially_copy_constructible<T>::value;
        constexpr bool tmc = std::is_trivially_move_constructible<T>::value;
        constexpr bool tca = std::is_trivially_copy_assignable<T>::value;
        constexpr bool tma = std::is_trivially_move_assignable<T>::value;
        constexpr bool td  = std::is_trivially_destructible<T>::value;

        constexpr bool cmca = cc || mc || ca || ma;

        static_assert(std::is_trivially_copyable<T>::value,
                      "not trivially copyable");

        // has at least one eligible copy constructor, move constructor,
        // copy assignment operator, or move assignment operator

        static_assert(cmca,
                      "deleted copy/move constructors/assignment operators");

        //each eligible copy constructor, move constructor,
        //copy assignment operator, and move assignment

        static_assert(tcc || !cc,
                      "non-trivial copy constructor");

        static_assert(cc || !cmca || tc,
                      "deleted copy constructor");

        static_assert(tmc || !mc,
                      "non-trivial move constructor");

        static_assert(mc || !cmca || tc,
                      "deleted move constructor");

        static_assert(tca || !ca,
                      "non-trivial copy assignment operator");

        static_assert(ca || !cmca || tc,
                      "deleted copy assignment operator");

        static_assert(tma || !ma,
                      "non-trivial move assignment operator");

        static_assert(ma || !cmca || tc,
                      "deleted move assignment operator");


        // has a trivial, non-deleted destructor
       
        static_assert(td || !d,
                      "non-trivial destructor");

        static_assert(d,
                      "deleted destructor");
    }

} // cool namespace

#endif /* COOL_IS_TRIVIALLY_COPYABLE_H_ */

