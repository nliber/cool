#ifndef COOL_EBO_WRAPPER_H_
#define COOL_EBO_WRAPPER_H_

#include <functional>
#include <type_traits>
#include <utility>

///////////////////////////////////////////////////////////////////////////////
// ebo_wrapper
//
//  ebo_wrapper is a wrapper which uses private inheritance to take advantage
//  of EBO for non-final classes and aggregation everywhere else.  The 
//  key benefit is to provide a consistent interface regardless of which
//  implementation is used.
//
//  The intended use is to privately inherit from ebo_wrapper<T> and call
//  ref() to get a reference to the held T.
//
//  Like reference_wrapper, there is also an implicit conversion to
//  (const_)reference and a Callable interface.
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
        {
            using value_type      = T;
            using reference       = T      &;
            using const_reference = T const&;

            template<typename... Us, typename = std::enable_if_t<std::is_constructible_v<T, Us...>>>
            constexpr ebo_wrapper(Us&&... us)
            noexcept(noexcept(T(std::forward<Us>(us)...)))
            : m_t(std::forward<Us>(us)...)
            {}

            constexpr reference       ref()       noexcept { return m_t; }
            constexpr const_reference ref() const noexcept { return m_t; }

            constexpr operator       reference()       noexcept { return m_t; }
            constexpr operator const_reference() const noexcept { return m_t; }

            template<typename... ArgTypes>
            auto // std::invoke_result_t<T&, ArgTypes...>
            operator()(ArgTypes&&... args)
            { return std::invoke(ref(), std::forward<ArgTypes>(args)...); }

            template<typename... ArgTypes>
            auto // std::invoke_result_t<T const&, ArgTypes...>
            operator()(ArgTypes&&... args) const
            { return std::invoke(ref(), std::forward<ArgTypes>(args)...); }

        private:
            T m_t;
        };

        template<typename T>
        struct ebo_wrapper<T, std::enable_if_t<std::is_empty_v<T> && !std::is_final_v<T>>>
        : private T
        {
            using value_type      = T;
            using reference       = T      &;
            using const_reference = T const&;

            template<typename... Us, typename = std::enable_if_t<std::is_constructible_v<T, Us...>>>
            constexpr ebo_wrapper(Us&&... us)
            noexcept(noexcept(T(std::forward<Us>(us)...)))
            : T(std::forward<Us>(us)...)
            {}

            constexpr reference       ref()       noexcept { return *this; }
            constexpr const_reference ref() const noexcept { return *this; }

            constexpr operator       reference()       noexcept { return *this; }
            constexpr operator const_reference() const noexcept { return *this; }

            template<typename... ArgTypes>
            auto // std::invoke_result_t<T&, ArgTypes...>
            operator()(ArgTypes&&... args)
            { return std::invoke(ref(), std::forward<ArgTypes>(args)...); }

            template<typename... ArgTypes>
            auto // std::invoke_result_t<T const&, ArgTypes...>
            operator()(ArgTypes&&... args) const
            { return std::invoke(ref(), std::forward<ArgTypes>(args)...); }

        };

    } // detail namespace

    template<typename T>
    using ebo_wrapper = detail::ebo_wrapper<T>;

} // cool namespace

#endif /* COOL_EBO_WRAPPER_H_ */

