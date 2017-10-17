#ifndef COOL_EBO_WRAPPER_H_
#define COOL_EBO_WRAPPER_H_

#include <type_traits>
#include <utility>

///////////////////////////////////////////////////////////////////////////////
// ebo_wrapper
//
//  ebo_wrapper is a wrapper which uses private inheritance where it can and
//  aggregation where it must.  It's key benefit is to provide a consistent
//  interface regardless of which implementation is used.
//
//  The intended use is to privately inherit from ebo_wrapper<T> and call
//  ref() to get a reference to the held T.
//
//  cool::detail::ebo_wrapper takes two template parameters, the second of
//  which should always be void and is used to SFINAE private inheritance vs.
//  aggregation implementations.
//
//  cool::ebo_wrapper only takes one template parameter.
//
///////////////////////////////////////////////////////////////////////////////

namespace cool
{
    namespace detail
    {
        template<typename T, typename = void>
        struct ebo_wrapper
        : private T
        {
            using value_type      = T;
            using reference       = T      &;
            using const_reference = T const&;

            constexpr ebo_wrapper() = default;

            template<typename U, typename = std::enable_if_t<!std::is_convertible_v<U, ebo_wrapper>>>
            constexpr ebo_wrapper(U&& u)
            noexcept(noexcept(T(std::forward<U>(u))))
            : T(std::forward<U>(u))
            {}

            template<typename U0, typename U1, typename... Us>
            constexpr ebo_wrapper(U0&& u0, U1&& u1, Us&&... us)
            noexcept(noexcept(T(std::forward<U0>, std::forward<U1>, std::forward<Us>(us)...)))
            : T(std::forward<U0>(u0), std::forward<U1>(u1), std::forward<Us>(us)...)
            {}

            constexpr reference       ref()       noexcept { return *this; }
            constexpr const_reference ref() const noexcept { return *this; }

            constexpr operator       reference()       noexcept { return *this; }
            constexpr operator const_reference() const noexcept { return *this; }
        };
        
        template<typename T>
        struct ebo_wrapper<T, std::enable_if_t<!std::is_class_v<T> || std::is_final_v<T>>>
        {
            using value_type      = T;
            using reference       = T      &;
            using const_reference = T const&;

            constexpr ebo_wrapper() = default;

            template<typename U, typename = std::enable_if_t<!std::is_convertible_v<U, ebo_wrapper>>>
            constexpr ebo_wrapper(U&& u)
            noexcept(noexcept(T(std::forward<U>(u))))
            : m_t(std::forward<U>(u))
            {}

            template<typename U0, typename U1, typename... Us>
            constexpr ebo_wrapper(U0&& u0, U1&& u1, Us&&... us)
            noexcept(noexcept(T(std::forward<U0>, std::forward<U1>, std::forward<Us>(us)...)))
            : m_t(std::forward<U0>(u0), std::forward<U1>(u1), std::forward<Us>(us)...)
            {}

            constexpr reference       ref()       noexcept { return m_t; }
            constexpr const_reference ref() const noexcept { return m_t; }

            constexpr operator       reference()       noexcept { return m_t; }
            constexpr operator const_reference() const noexcept { return m_t; }

        private:
            T m_t;
        };

    } // detail namespace

    template<typename T>
    using ebo_wrapper = detail::ebo_wrapper<T>;

} // cool namespace

#endif /* COOL_EBO_WRAPPER_H_ */

