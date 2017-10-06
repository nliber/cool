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

#include <iostream>


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
        struct is_tuple_like<T, std::void_t<decltype(std::tuple_size<std::remove_reference_t<T>>::value)>>
        : std::true_type {};

    }

    template<typename T, bool SkipOstreamInsert = false>
    class Out
    {

        template<typename... Ss>
        void Elements(cool::Spacer<Ss...> const&) const
        {
        }

        template<typename... Ss, typename U, typename... Us>
        void Elements(cool::Spacer<Ss...> const& spacer, U&& u, Us&&... us) const
        {
            *m_os << spacer << cool::Out{std::forward<U>(u)};
            Elements(spacer, std::forward<Us>(us)...);
        }

        void TupleLike() const
        {
            *m_os << '{';
            std::apply([this](auto&&... args)
            {
                Elements(cool::Spacer(','), std::forward<decltype(args)>(args)...);
            }, m_value);
            *m_os << '}';
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
            size_t extent = std::extent_v<std::remove_reference_t<T>>;
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
            using CVV = std::remove_reference_t<T>;
            using V   = std::remove_cv_t<CVV>;

            if constexpr(std::is_same_v<V, char>)
                Char();
            else if constexpr(std::is_same_v<V, signed char>)
                IntegralPromotion();
            else if constexpr(std::is_same_v<V, unsigned char>)
                IntegralPromotion();
            else if constexpr(std::is_same_v<V, std::string_view>)
                StringView();
            else if constexpr(std::is_same_v<V, std::string>)
                StringView();
            else if constexpr(1 == std::rank_v<CVV> && std::is_same_v<std::remove_extent_t<CVV>, const char>)
                CStringLiteral();
            else if constexpr(std::is_pointer_v<V> && std::is_same_v<std::remove_const_t<std::remove_pointer_t<V>>, char>)
                CharStar();
            else
                OStreamInsert();
        }

        void Enum() const
        {
            PrettyName();
            *m_os << '(' << cool::Out{static_cast<std::underlying_type_t<T>>(m_value)} << ')';
        }

        void Range() const
        {
            *m_os << +std::size(m_value) << '[';

            cool::Spacer comma(',');
            for (auto&& v : m_value)
                *m_os << comma << cool::Out{v};

            *m_os << ']';
        }

        void PrettyName() const
        { *m_os << cool::pretty_name(m_value); }

    public:
        using value_type = T;
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
            if constexpr(!SkipOstreamInsert && boost::has_left_shift<std::ostream&, T, std::ostream&>())
                that.HasOstreamInsert();
            else if constexpr(std::is_enum_v<T>)
                that.Enum();
            else if constexpr(type_traits::is_range<T>{})
                that.Range();
            else if constexpr(type_traits::is_tuple_like<T>{})
                that.TupleLike();
            else
                that.PrettyName();

            return os;
        }

    private:
        mutable std::ostream* m_os;
        T                     m_value;
    };

    template<typename T>
    explicit Out(T&&) -> Out<T>;

    template<typename T>
    Out<T, true> OutOp(T&& t)
    { return Out<T, true>{std::forward<T>(t)}; }

} // cool namespace

#endif /* COOL_OUT_H_ */

