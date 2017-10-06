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
#include <tuple>
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

        // Need a wrapper so we can have "mutable" references
        // tuple already does this work
        using wrapper_type = std::tuple<std::ostream*, deduced_type>;

        std::ostream&  os()   const noexcept { return *std::get<0>(m_wrapper); }
        decltype(auto) data() const noexcept { return std::forward<deduced_type>(std::get<1>(m_wrapper)); }

        void TupleLike() const
        {
            std::apply([&os = os()](auto&&... args)
            {
                os << '{';
                cool::Spacer comma{','};

                // fold expression over a comma (not to be confused with the Spacer comma)
                ((os << comma << cool::Out{std::forward<decltype(args)>(args)}), ...);

                os << '}';
            }, data());
        }

        void OStreamInsert() const
        { os() << data(); }

        void Char() const
        { os() << '\'' << cool::CChar{data()} << '\''; }

        void IntegralPromotion() const
        { os() << +data(); }

        void StringView() const
        {
            os() << '\"';
            for (char c : data())
                os() << cool::CChar{c};
            os() << '\"';
        }

        void CStringLiteral() const
        {
            size_t extent = std::extent_v<value_type>;
            if (data()[extent-1])
                Range();
            else
                os() << cool::Out<std::string_view>{std::string_view{data(), extent-1}};
        }

        void CharStar() const
        {
            if (data())
                os() << cool::Out<std::string_view>{std::string_view{data()}};
            else
                os() << "nullptr";
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
            os()
                << cool::pretty_name(data())
                << '('
                << cool::Out{static_cast<std::underlying_type_t<value_type>>(data())}
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
            os() << +std::size(data()) << '[';

            cool::Spacer comma{','};
            for (auto&& v : data())
                os() << comma << cool::Out{v};

            os() << ']';
        }

        void PrettyName() const
        { os() << cool::pretty_name(data()); }

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

        template<typename U>
        explicit Out(U&& u)
        : m_wrapper{nullptr, std::forward<U>(u)}
        { /* std::cerr << cool::pretty_name(*this) << std::endl; */ }

        Out(Out const&)            = delete;
        Out& operator=(Out const&) = delete;
        Out& operator=(Out&&)      = delete;
        Out(Out&&)                 = delete;

        friend std::ostream& operator<<(std::ostream& os, Out&& that)
        {
            std::get<0>(that.m_wrapper) = &os;
            if constexpr(!SkipOstreamInsert && boost::has_left_shift<std::ostream&, deduced_type, std::ostream&>())
                that.HasOstreamInsert();
            else
                that.NeedsOStreamInsert();

            return os;
        }

    private:
        mutable wrapper_type  m_wrapper;

    };

    template<typename T>
    explicit Out(T&&) -> Out<T>;

    template<typename T>
    Out<T, true> OutOp(T&& t)
    { return Out<T, true>{std::forward<T>(t)}; }

} // cool namespace

#endif /* COOL_OUT_H_ */

