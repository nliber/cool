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
    void is_trivially_copyable()
    {
        static_assert(std::is_trivially_copyable<T>::value,
                     "not trivially copyable");

        static_assert(std::is_copy_constructible<T>::value ||
                      std::is_move_constructible<T>::value ||
                      std::is_copy_assignable<T>::value    ||
                      std::is_move_assignable<T>::value    ,
                     "copy/move constructors/assignment operators deleted");

        static_assert(std::is_trivially_copy_constructible<T>::value ||
                     !std::is_copy_constructible<T>::value           ,
                     "not trivially copy constructible");

        static_assert(std::is_trivially_move_constructible<T>::value ||
                     !std::is_move_constructible<T>::value           ,
                     "not trivially move constructible");

        static_assert(std::is_trivially_copy_assignable<T>::value ||
                     !std::is_copy_assignable<T>::value           ,
                     "not trivially copy assignable");

        static_assert(std::is_trivially_move_assignable<T>::value ||
                     !std::is_move_assignable<T>::value           ,
                     "not trivially move assignable");

        static_assert(std::is_trivially_destructible<T>::value,
                     "not trivially destructible");

    }

} // cool namespace

#endif /* COOL_IS_TRIVIALLY_COPYABLE_H_ */

