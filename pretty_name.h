#ifndef COOL_PRETTY_NAME_H_
#define COOL_PRETTY_NAME_H_

#include <string_view>

namespace cool
{
    ///////////////////////////////////////////////////////////////////////////
    // pretty_name
    //
    //  pretty_name is a class that encapsulates the human readable name of the
    //  (static) type passed into it, after the top level const/volatile
    //  qualifiers are removed.
    //
    // pretty_type
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
            int i{};
            return sizeof_ptpf(i) - sizeof "int&]";
        }

    public:
        template<typename T>
        constexpr pretty_type(T&&) noexcept
        : std::string_view{       __PRETTY_FUNCTION__ + offset_name(),
                           sizeof __PRETTY_FUNCTION__ - offset_name() - sizeof "]"}
        {}
    };

} // cool namespace


#endif /* COOL_PRETTY_NAME_H_ */

