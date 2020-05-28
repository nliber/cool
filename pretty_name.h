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
    //  qualifiers are removed.
    //
    // make_pretty_name
    //
    //  template<typename T> make_pretty_name()
    //  template<typename T> make_pretty_name(T&&)
    //
    //  make_pretty_name are function templates that creates a pretty_name for
    //  the type T specified or deduced.  When specified, useful for typedefs
    //  inside classes or other cases where no object is available.
    //
    //  Note:  unlike pretty_name, make_pretty_name will not strip
    //  const/volatile when the type is *specified*.  It will, however,
    //  still strip references.
    //
    // pretty_type
    //
    //  pretty_type is a class that encapsulates the human readable name of the
    //  deduced (static) type passed to it, keeping the top level
    //  const/volatile qualifier and an "&" at the end for l-value references.
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
    { return pretty_name(t); }

    class pretty_type : public std::string_view
    {
        template<typename T>
        static auto
        constexpr sizeof_ptpf(T&&) noexcept
        {
            return sizeof __PRETTY_FUNCTION__ - sizeof "static auto sizeof_ptpf" + sizeof "pretty_type";
        }

        static auto
        constexpr offset_name() noexcept
        {
            return sizeof_ptpf(0) - sizeof "int]";
        }

    public:
        template<typename T>
        constexpr pretty_type(T&&) noexcept
        : std::string_view{       __PRETTY_FUNCTION__ + offset_name(),
                           sizeof __PRETTY_FUNCTION__ - offset_name() - sizeof "]"}
        {}
    };

    template<typename T>
    constexpr pretty_type make_pretty_type() noexcept
    {
        pretty_type pt{static_cast<T*>(nullptr)};
        pt.remove_suffix((' ' == pt[pt.size() - 2]) + sizeof '*');
        return pt;
    }

} // cool namespace


#endif /* COOL_PRETTY_NAME_H_ */

