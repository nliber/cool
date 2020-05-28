#ifndef COOL_PRETTY_NAME_H_
#define COOL_PRETTY_NAME_H_

#include <string_view>
#include <type_traits>

namespace cool
{
    ///////////////////////////////////////////////////////////////////////////
    // pretty_name
    //
    //  pretty_name is a class that encapsulates the human readable name of the
    //  (static) type passed into it, after the top level const/volatile
    //  and refrence qualifiers are removed.
    //
    // make_pretty_name
    //
    //  template<typename T> make_pretty_name()
    //  template<typename T> make_pretty_name(T&&)
    //
    //  make_pretty_name is a function template that creates a pretty_name for
    //  the type T specified or deduced.  When specified, useful for typedefs
    //  inside classes or other cases where no object is available.
    //
    //  Note:  unlike pretty_name, make_pretty_name will not strip
    //  top-level const/volatile.  It will still strip reference qualifiers.
    //
    // pretty_type
    //
    //  pretty_type is a function template which returns a pretty_type_t
    //  that encapsulates the human readable name of the
    //  specified or deduced type passed to it, keeping the top level
    //  const/volatile qualifiers and reference qualifieers.
    //
    //  Note:  when the type is deduced, r-values have no reference specifiers.
    //
    ///////////////////////////////////////////////////////////////////////////
    
    class pretty_name : public std::string_view
    {
        template<typename T>
        static auto
        constexpr sizeof_pnpf(const volatile T&) noexcept
        {
            return sizeof __PRETTY_FUNCTION__ - sizeof "static auto sizeof_pnpf" + sizeof "pretty_name";
        }

        static auto
        constexpr offset_name() noexcept
        {
            int i{};
            return sizeof_pnpf(i) - sizeof "int]";
        }

    public:
        template<typename T>
        constexpr pretty_name(const volatile T&) noexcept
        : std::string_view{       __PRETTY_FUNCTION__ + offset_name(),
                           sizeof __PRETTY_FUNCTION__ - offset_name() - sizeof "]"}
        {}

        template<typename T>
        constexpr pretty_name(const volatile T&& t) noexcept
        : pretty_name{t}
        {}
    };

    template<typename T>
    constexpr pretty_name make_pretty_name() noexcept
    {
        pretty_name pn{static_cast<std::remove_reference_t<T>*>(nullptr)};
        pn.remove_suffix((' ' == pn[pn.size() - 2]) + sizeof '*');
        return pn;
    }

    template<typename T>
    constexpr pretty_name make_pretty_name(T&& t) noexcept
    { return make_pretty_name<T>(); }


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
    constexpr pretty_type_t pretty_type(T&&) noexcept
    { return pretty_type<T>(); }

} // cool namespace


#endif /* COOL_PRETTY_NAME_H_ */

