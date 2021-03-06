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
    //  wrong or questionable if the type is not C++17 trivially copyable.
    //
    //  Questionable things are deleted copy/move
    //  constructors/assignment operators unless all four are deleted
    //
    //  There appears to be a bug in std::is_*constructible<T> such that
    //  if the destructor is deleted it always returns false.  There also
    //  appears to be a bug in std::is_trivially_*_constructible<T> such that
    //  if the destructor exists but is not trivial it always returns false.
    //  The static_asserts will indicate that by saying that the corresponding
    //  constructors are "possibly" non-trivial/deleted.
    //
    //  Works with C++14 or later.
    //
    ///////////////////////////////////////////////////////////////////////////
    template<typename T>
    constexpr void is_trivially_copyable() noexcept
    {
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

        // has at least one eligible copy constructor, move constructor,
        // copy assignment operator, or move assignment operator
        constexpr bool cmca = cc || mc || ca || ma;

        constexpr bool tc = std::is_trivially_copyable<T>::value;

        // has at least one eligible copy constructor, move constructor,
        // copy assignment operator, or move assignment operator
        //
        // each eligible copy constructor, move constructor,
        // copy assignment operator, and move assignment is trivial
        //
        // has a trivial, non-deleted destructor
        constexpr bool itc = cmca &&
                             (tcc || !cc) &&
                             (tmc || !mc) &&
                             (tca || !ca) &&
                             (tma || !ma) &&
                             td;

        static_assert(tc,
                      "not trivially copyable");

        // Workaround for clang/gcc is_trivially_copyable bugs
        static_assert(itc,
                      "not C++17 trivially copyable");

        // has at least one eligible copy constructor, move constructor,
        // copy assignment operator, or move assignment operator

        static_assert(cmca,
                      "deleted copy/move constructors/assignment operators");

        //each eligible copy constructor, move constructor,
        //copy assignment operator, and move assignment is trivial

        // Note:  std::is_*constructible<T> returns false if the destructor
        // is deleted, and std::is_trivially_*_constructible returns false
        // if the destructor is non-trivial.  This is indicated in the
        // static_assert with "possibly"

        static_assert(tcc || !td || !cc,
                      "non-trivial copy constructor");

        static_assert(tcc || td || !cc,
                      "possibly non-trivial copy constructor");

        static_assert(cc || !d || itc || !cmca,
                      "deleted copy constructor");

        static_assert(cc || d || !cmca,
                      "possibly deleted copy constructor");

        static_assert(tmc || !td || !mc,
                      "non-trivial move constructor");

        static_assert(tmc || td || !mc,
                      "possibly non-trivial move constructor");

        static_assert(mc || !d || itc || !cmca,
                      "deleted move constructor");

        static_assert(mc || d || !cmca,
                      "possibly deleted move constructor");

        static_assert(tca || !ca,
                      "non-trivial copy assignment operator");

        static_assert(ca || itc || !cmca,
                      "deleted copy assignment operator");

        static_assert(tma || !ma,
                      "non-trivial move assignment operator");

        static_assert(ma || itc || !cmca,
                      "deleted move assignment operator");


        // has a trivial, non-deleted destructor
       
        static_assert(td || !d,
                      "non-trivial destructor");

        static_assert(d,
                      "deleted destructor");
    }

} // cool namespace

#endif /* COOL_IS_TRIVIALLY_COPYABLE_H_ */

