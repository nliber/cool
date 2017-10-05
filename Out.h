#ifndef COOL_OUT_H_
#define COOL_OUT_H_

#include <cool/pretty_name.h>
#include <cool/Spacer.h>
#include <iterator>
#include <ostream>
#include <string>
#include <type_traits>
#include <utility>

namespace cool
{
    namespace type_traits
    {
        template<typename T, typename charT = char, typename traits = std::char_traits<charT>, typename = void>
        struct is_ostream_insertable
        : std::false_type {};

        template<typename T, typename charT, typename traits>
        struct is_ostream_insertable<T, charT, traits,
                                     std::void_t<decltype(std::declval<std::basic_ostream<charT, traits>&>() << std::declval<T>())>>
        : std::true_type {};

        template<typename T, typename = void>
        struct is_range
        : std::false_type {};

        template<typename T>
        struct is_range<T, std::void_t<decltype(std::begin(std::declval<T>()))>>
        : std::true_type {};
    }

    template<typename T, bool SkipOstreamInsert = false>
    class Out
    {
        void OstreamInsert(std::ostream& os) const
        { os << m_ref; }

        void Enum(std::ostream& os) const
        {
            PrettyName(os);
            os << '(' << +static_cast<std::underlying_type_t<T>>(m_ref) << ')';
        }

        void Range(std::ostream& os) const
        {
            os << +std::size(m_ref) << '[';

            cool::Spacer comma(',');
            for (auto&& v : m_ref)
                os << comma << cool::Out(v);

            os << ']';
        }

        void PrettyName(std::ostream& os) const
        { os << cool::pretty_name(m_ref); }

    public:
        using value_type = T;
        static constexpr bool skip_ostream_insert = SkipOstreamInsert;

        explicit Out(T const& t)
        : m_ref{t}
        {}

        Out(Out const&)            = delete;
        Out& operator=(Out const&) = delete;
        Out& operator=(Out&&)      = delete;
        Out(Out&&)                 = delete;

        friend std::ostream& operator<<(std::ostream& os, Out const&& that)
        {
            if constexpr(!SkipOstreamInsert && type_traits::is_ostream_insertable<T>{})
                that.OstreamInsert(os);
            else if constexpr(std::is_enum_v<T>)
                that.Enum(os);
            else if constexpr(type_traits::is_range<T>{})
                that.Range(os);
            else
                that.PrettyName(os);

            return os;
        }

    private:
        T const& m_ref;
    };

    template<typename T>
    explicit Out(T const&) -> Out<T>;

    template<typename T>
    Out<T, true> OutOp(T const& t)
    { return Out<T, true>{t}; }

} // cool namespace

#endif /* COOL_OUT_H_ */

