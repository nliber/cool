#ifndef COOL_PRETTY_NAME_H_
#define COOL_PRETTY_NAME_H_

#include <string_view>

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
    ///////////////////////////////////////////////////////////////////////////
    
    namespace detail
    {
        ///////////////////////////////////////////////////////////////////////
        // pretty_fn<T>
        //
        //  Helper function to return __PRETTY_FUNCTION__ containing
        //  "T = type".  This is necessary because starting with gcc10 template
        //  aliases are also expanded in __PRETTY_FUNCTION__, so it would
        //  be more complicated to return a string_view (which is really
        //  basic_string_view<char, char_traits<char>>) than a const char*
        ///////////////////////////////////////////////////////////////////////
        template<typename T>
        constexpr const char* pretty_fn() noexcept
        { return __PRETTY_FUNCTION__; }

        ///////////////////////////////////////////////////////////////////////
        // pretty_type_sv
        //
        //  Helper function to parse the result of pretty_fn<T>() for the human
        //  readable name for T.
        ///////////////////////////////////////////////////////////////////////
        inline constexpr std::string_view pretty_type_sv(const char*&& pf) noexcept
        {
            std::string_view pt(pf);

            constexpr std::string_view pattern{"T = "};
            pt.remove_prefix(pt.find(pattern) + pattern.size());
            pt.remove_suffix(sizeof ']');

            return pt;
        }
    } // detail namespace


    template<typename T>
    constexpr std::string_view pretty_type() noexcept
    { return detail::pretty_type_sv(detail::pretty_fn<T>()); }

    template<typename T>
    constexpr std::string_view pretty_lref(T&&) noexcept
    { return detail::pretty_type_sv(detail::pretty_fn<T>()); }

    template<typename T>
    constexpr std::string_view pretty_ref(T&&) noexcept
    { return detail::pretty_type_sv(detail::pretty_fn<T&&>()); }

    template<typename T>
    constexpr std::string_view pretty_name(const volatile T&) noexcept
    { return detail::pretty_type_sv(detail::pretty_fn<T>()); }

    template<typename T>
    constexpr std::string_view pretty_name(const volatile T&&) noexcept
    { return detail::pretty_type_sv(detail::pretty_fn<T>()); }

} // cool namespace


#endif /* COOL_PRETTY_NAME_H_ */

