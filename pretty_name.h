#ifndef COOL_PRETTY_NAME_H_
#define COOL_PRETTY_NAME_H_

#include <string_view>
#include <utility>

namespace cool
{
    ///////////////////////////////////////////////////////////////////////////
    // pretty_name
    //
    //  template<typename T> pretty_name(T&&)
    //  pretty_name is a function template that encapsulates the human readable
    //  name of the (static) type deduced, after the top level const/volatile
    //  and reference qualifiers are removed.
    //
    // pretty_type
    //
    //  template<typename T> pretty_type()
    //  pretty_type is a function template that encapsulates the human readable
    //  name of the type specified.
    //
    //  In conjunction with the pretty_type_t constructor, pretty_type does the
    //  work for all the rest of the function templates.
    //
    // pretty_ref
    //
    //  template<typename T> pretty_ref(T&&)
    //  pretty_lref is a function template that encapsulates the human readable
    //  name of the (static) type deduced.  The result is always
    //  reference-qualified.
    //
    //  This shows the reference type that forward<T> returns.
    //
    // pretty_lref
    //
    //  template<typename T> pretty_lref(T&&)
    //  pretty_lref is a function template that encapsulates the human readable
    //  name of the (static) type deduced.  The result is reference-qualified
    //  if and only if an l-value reference is passed in.
    //
    //  This is primarily used to show how forwarding references are deduced.
    //
    // pretty_type_t
    // 
    //  pretty_type_t is a concrete class publicly derived from string_view
    //  which the pretty_* function templates return.
    //
    //  It is necessary because starting with gcc10 alias templates such as
    //  string_view are also expanded as part of __PRETTY_FUNCTION__ and
    //  would make parsing __PRETTY_FUNCTION__ more complicated and the
    //  __PRETTY_FUNCTION__ strings stored in the binary even bigger.
    //
    //  Unlike string_view in general, pretty_type_t never can hold a dangling
    //  reference, as the __PRETTY_FUNCTION__ strings have program lifetime.
    ///////////////////////////////////////////////////////////////////////////
    
    class pretty_type_t : public std::string_view
    {
        explicit constexpr pretty_type_t(const char* pf) noexcept
        : std::string_view{pf}
        { 
            constexpr std::string_view pattern{"T = "};
            remove_prefix(find(pattern) + pattern.size());
            remove_suffix(sizeof ']');
        }

        template<typename T>
        friend constexpr pretty_type_t pretty_type() noexcept;
    };

    template<typename T>
    constexpr pretty_type_t pretty_type() noexcept
    { return pretty_type_t(__PRETTY_FUNCTION__); }

    template<typename T>
    constexpr pretty_type_t pretty_lref(T&&) noexcept
    { return pretty_type<T>(); }

    template<typename T>
    constexpr pretty_type_t pretty_ref(T&& t) noexcept
    { return pretty_type<decltype(std::forward<T>(t))>(); }

    template<typename T>
    constexpr pretty_type_t pretty_name(const volatile T&) noexcept
    { return pretty_type<T>(); }

    template<typename T>
    constexpr pretty_type_t pretty_name(const volatile T&&) noexcept
    { return pretty_type<T>(); }

} // cool namespace


#endif /* COOL_PRETTY_NAME_H_ */

