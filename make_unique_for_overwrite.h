#ifndef COOL_MAKE_UNIQUE_FOR_OVERWRITE_H_
#define COOL_MAKE_UNIQUE_FOR_OVERWRITE_H_

///////////////////////////////////////////////////////////////////////////////
// make_unique_for_overwrite<T>
//
//  make_unique_for_overwrite is a replacement for make_unique which
//  default initializes (as opposed to value initializes) both single objects
//  (when no args are provided) and arrays of unknown bounds.
//
//  std::unique_ptr<T> make_unique_for_overwrite<T>()
//      default-initialized T
//
//  std::unique_ptr<T> make_unique_for_overwrite<T>(Arg&& arg, Args&&... args)
//      same as make_unique<T>(Arg&&, Args&&...)
//
//  std::unique_ptr<T[]> make_unique_for_overwrite<T[]>(size_t n)
//      default-initialized array of T of size n
//
//  make_unique_for_overwrite<T[n]>(Args&&...)
//      deleted (same as make_unique<T[n]>(Args&&...))
//
///////////////////////////////////////////////////////////////////////////////

#include <memory>
#include <type_traits>

namespace cool
{
    namespace detail
    {
        // make_unique_if are helpers for make_unique_for_overwrite
        template<typename T>
        struct make_unique_if
        { using single_object = std::unique_ptr<T>; };

        template<typename T>
        struct make_unique_if<T[]>
        { using array = std::unique_ptr<T[]>; };

        template<typename T, size_t N>
        struct make_unique_if<T[N]>
        { using bound = void; };

    } // detail namespace

    // std::unique_ptr<T> make_unique_for_overwrite<T>()
    template<typename T>
    typename detail::make_unique_if<T>::single_object
    make_unique_for_overwrite()
    { return std::unique_ptr<T>(new T); }

    // std::unique_ptr<T> make_unique_for_overwrite<T>(Arg&& arg, Args&&... args)
    template<typename T, typename... Args>
    typename detail::make_unique_if<T>::single_object
    make_unique_for_overwrite(Args&&... args)
    { return std::unique_ptr<T>(new T(std::forward<Args>(args)...)); }

    // std::unique_ptr<T[]> make_unique_for_overwrite<T[]>(size_t n)
    template<typename T>
    typename detail::make_unique_if<T>::array
    make_unique_for_overwrite(size_t n)
    { return std::unique_ptr<T>(new std::remove_extent_t<T>[n]); }

    // make_unique_for_overwrite<T[n]>(Args&&...)
    template<typename T, typename... Args>
    typename detail::make_unique_if<T>::bound
    make_unique_for_overwrite(Args&&...) = delete;

} // cool namespace

#endif /* COOL_MAKE_UNIQUE_FOR_OVERWRITE_H_ */

