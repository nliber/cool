#ifndef COOL_OUT_H_
#define COOL_OUT_H_

#include <cool/pretty_name.h>
#include <cool/CChar.h>
#include <cool/Spacer.h>
#include <boost/type_traits/has_left_shift.hpp>
#include <iterator>
#include <ostream>
#include <string>
#include <string_view>
#include <type_traits>
#include <utility>

namespace cool
{
    namespace type_traits
    {
        template<typename T, typename = void>
        struct is_range
        : std::false_type {};

        template<typename T>
        struct is_range<T, std::void_t<decltype(std::begin(std::declval<T>()))>>
        : std::true_type {};

        template<typename T, typename = void>
        struct is_tuple_like
        : std::false_type {};

        template<typename T>
        struct is_tuple_like<T, std::void_t<decltype(std::tuple_size<T>::value)>>
        : std::true_type {};

        template<typename E>
        using is_scoped_enum = std::bool_constant<std::is_enum_v<E> && !std::is_convertible_v<E, int>>;

        template<typename E, typename = void>
        struct has_ostream_inserter
        : std::false_type {};

        template<typename T>
        struct has_ostream_inserter<T, std::void_t<decltype(operator<<(std::declval<std::ostream&>(), std::declval<T>()))>>
        : std::true_type {};

    }

    template<typename T, bool SkipOstreamInsert = false>
    class Out
    {
        using deduced_type = T;
        using value_type = std::remove_reference_t<deduced_type>;
        using noncv_type = std::remove_cv_t<value_type>;

        void TupleLike() const
        {
            std::apply([&os = *m_os](auto&&... args)
            {
                os << '{';
                cool::Spacer comma{','};

                // fold expression over a comma (not to be confused with the Spacer comma)
                ((os << comma << cool::Out{std::forward<decltype(args)>(args)}), ...);

                os << '}';
            }, m_value);
        }

        void OStreamInsert() const
        { *m_os << m_value; }

        void Char() const
        { *m_os << '\'' << cool::CChar{m_value} << '\''; }

        void IntegralPromotion() const
        { *m_os << +m_value; }

        void StringView() const
        {
            *m_os << '\"';
            for (char c : m_value)
                *m_os << cool::CChar{c};
            *m_os << '\"';
        }

        void CStringLiteral() const
        {
            size_t extent = std::extent_v<value_type>;
            if (m_value[extent-1])
                Range();
            else
                *m_os << cool::Out<std::string_view>{m_value, extent-1};
        }

        void CharStar() const
        {
            if (m_value)
                *m_os << cool::Out<std::string_view>{m_value};
            else
                *m_os << "nullptr";
        }

        void HasOstreamInsert() const
        {
            if constexpr(std::is_same_v<noncv_type, char>)
                Char();
            else if constexpr(std::is_same_v<noncv_type, signed char>)
                IntegralPromotion();
            else if constexpr(std::is_same_v<noncv_type, unsigned char>)
                IntegralPromotion();
            else if constexpr(std::is_same_v<noncv_type, std::string_view>)
                StringView();
            else if constexpr(std::is_same_v<noncv_type, std::string>)
                StringView();
            else if constexpr(1 == std::rank_v<value_type> && std::is_same_v<std::remove_extent_t<value_type>, const char>)
                CStringLiteral();
            else if constexpr(std::is_pointer_v<noncv_type> && std::is_same_v<std::remove_const_t<std::remove_pointer_t<noncv_type>>, char>)
                CharStar();
            else if constexpr(std::is_enum_v<noncv_type> && !type_traits::is_scoped_enum<noncv_type>{})
                UnscopedEnum();
            else
                OStreamInsert();
        }

        void Enum() const
        {
            *m_os
                << cool::pretty_name(m_value)
                << '('
                << cool::Out{static_cast<std::underlying_type_t<value_type>>(m_value)}
                << ')'
                ;
        }

        void UnscopedEnum() const
        {
            if constexpr(type_traits::has_ostream_inserter<deduced_type>{})
                OStreamInsert();
            else
                Enum();
        }

        void Range() const
        {
            *m_os << +std::size(m_value) << '[';

            cool::Spacer comma{','};
            for (auto&& v : m_value)
                *m_os << comma << cool::Out{v};

            *m_os << ']';
        }

        void PrettyName() const
        { *m_os << cool::pretty_name(m_value); }

        void NeedsOStreamInsert() const
        {
            if constexpr(std::is_enum_v<value_type>)
                Enum();
            else if constexpr(type_traits::is_range<deduced_type>{})
                Range();
            else if constexpr(type_traits::is_tuple_like<value_type>{})
                TupleLike();
            else
                PrettyName();
        }

    public:
        static constexpr bool skip_ostream_insert = SkipOstreamInsert;

        template<typename... Ts>
        explicit Out(Ts&&... ts)
        : m_value{std::forward<Ts>(ts)...}
        { /* std::cerr << cool::pretty_name(*this) << std::endl; */ }

        Out(Out const&)            = delete;
        Out& operator=(Out const&) = delete;
        Out& operator=(Out&&)      = delete;
        Out(Out&&)                 = delete;

        friend std::ostream& operator<<(std::ostream& os, Out const&& that)
        {
            that.m_os = &os;

            if constexpr(!SkipOstreamInsert && boost::has_left_shift<std::ostream&, deduced_type, std::ostream&>())
                that.HasOstreamInsert();
            else
                that.NeedsOStreamInsert();

            return os;
        }

    private:
        mutable std::ostream* m_os;
        deduced_type          m_value;
    };

    template<typename T>
    explicit Out(T&&) -> Out<T>;

    template<typename T>
    Out<T, true> OutOp(T&& t)
    { return Out<T, true>{std::forward<T>(t)}; }

} // cool namespace

#endif /* COOL_OUT_H_ */

