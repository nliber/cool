#ifndef COOL_PRETTY_NAME_HPP_
#define COOL_PRETTY_NAME_HPP_

#include <boost/utility/string_ref.hpp>

namespace cool
{
    ///////////////////////////////////////////////////////////////////////////
    // pretty_name
    //
    //  pretty_name is a class that encapsulates the human readable name of the
    //  (static) type passed into it, after the top level const/volatile
    //  qualifiers are removed.
    //
    //  It is implemented as a type derived from string_ref and it has whole
    //  program lifetime.
    ///////////////////////////////////////////////////////////////////////////
    struct pretty_name : boost::string_ref
    {
#ifdef __clang__
        template<typename T>
        constexpr explicit pretty_name(const T &) noexcept
        : boost::string_ref(       __PRETTY_FUNCTION__ + sizeof "cool::pretty_name::pretty_name(const T &) [T = " - 1,
                            sizeof __PRETTY_FUNCTION__ - sizeof "cool::pretty_name::pretty_name(const T &) [T = " - sizeof "]" + 1)
        {}

        template<typename T>
        constexpr explicit pretty_name(const volatile T &) noexcept
        : boost::string_ref(       __PRETTY_FUNCTION__ + sizeof "cool::pretty_name::pretty_name(const volatile T &) [T = " - 1,
                            sizeof __PRETTY_FUNCTION__ - sizeof "cool::pretty_name::pretty_name(const volatile T &) [T = " - sizeof "]" + 1)
        {}
#else /* !defined(__clang__) */
        template<typename T>
        constexpr explicit pretty_name(const T&) noexcept
        : boost::string_ref(       __PRETTY_FUNCTION__ + sizeof "constexpr cool::pretty_name::pretty_name(const T&) [with T = " - 1,
                            sizeof __PRETTY_FUNCTION__ - sizeof "constexpr cool::pretty_name::pretty_name(const T&) [with T = " - sizeof "]" + 1)
        {}

        template<typename T>
        constexpr explicit pretty_name(const volatile T&) noexcept
        : boost::string_ref(       __PRETTY_FUNCTION__ + sizeof "constexpr cool::pretty_name::pretty_name(const volatile T&) [with T = " - 1,
                            sizeof __PRETTY_FUNCTION__ - sizeof "constexpr cool::pretty_name::pretty_name(const volatile T&) [with T = " - sizeof "]" + 1)
        {}
#endif  /* !defined(__clang__) */
    };
} // cool namespace

#endif /* COOL_PRETTY_NAME_HPP_ */

