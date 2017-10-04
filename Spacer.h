#ifndef COOL_SPACER_H_
#define COOL_SPACER_H_

#include <cassert>
#include <ostream>
#include <string>
#include <string_view>
#include <utility>

///////////////////////////////////////////////////////////////////////////////
//
// Spacer
//  Utility class for "spacing" through elements when looping
//  Note:  Because of deduction guides, this usually has
//  reference semantics for l-values.
//
// Template parameters (note: the order of parameters is funky):
//  charT - the value type of the ostream
//  traits - the traits type of the stream
//  Middle - the type of the separator between elements
//  Beginning - the type of the first separator streamed
//  End - the type to be streamed on destruction
//      (only relevant when at least one separator streamed)
//
// Constructors:
//  Spacer(Middle m)
//  Spacer(Beginning b, Middle m)
//  Spacer(Beginning b, Middle m, End e)
//
// Usage:
//  {
//      cool::Spacer comma('[', ", ", "]\n");
//      std::cout << comma << 1 << comma << 2 << comma << 3;
//  } // outputs: [1, 2, 3]
///////////////////////////////////////////////////////////////////////////////

namespace cool
{
    class NullIOStream
    {
        template<typename charT, typename traits>
        friend std::basic_ostream<charT, traits>&
        operator<<(std::basic_ostream<charT, traits>& os, NullIOStream)
        { return os; }

        template<typename charT, typename traits>
        friend std::basic_istream<charT, traits>&
        operator>>(std::basic_ostream<charT, traits>& is, NullIOStream)
        { return is; }
    };

    template<typename charT     = char,
             typename traits    = std::char_traits<charT>,
             typename Middle    = NullIOStream,
             typename Beginning = NullIOStream,
             typename End       = NullIOStream>
    class Spacer
    {
    public:
        using value_type     = charT;
        using traits_type    = traits;
        using ostream_type   = std::basic_ostream<charT, traits>;
        using beginning_type = Beginning;
        using middle_type    = Middle;
        using end_type       = End;

        template<typename M>
        explicit Spacer(M&& m)
        : m_middle{std::forward<M>(m)}
        {}

        template<typename B, typename M>
        explicit Spacer(B&& b, M&& m)
        : m_middle{std::forward<M>(m)}
        , m_beginning{std::forward<B>(b)}
        {}

        template<typename B, typename M, typename E>
        explicit Spacer(B&& b, M&& m, E&& e)
        : m_middle{std::forward<M>(m)}
        , m_beginning{std::forward<B>(b)}
        , m_end{std::forward<E>(e)}
        {}

        Spacer(Spacer const&)            = delete;
        Spacer& operator=(Spacer const&) = delete;
        Spacer& operator=(Spacer&&)      = delete;
        Spacer(Spacer&&)                 = delete;

        friend ostream_type& operator<<(ostream_type& os, Spacer const& that)
        {
            if (that.m_os)
            {
                assert(&os == that.m_os);
                os << static_cast<Middle>(that.m_middle);
            } else {
                os << static_cast<Beginning>(that.m_beginning);
                that.m_os = &os;
            }

            return os;
        }

        ~Spacer()
        {
            if (m_os)
                *m_os << static_cast<End>(m_end);
        }

    private:
        mutable ostream_type* m_os{};
        Middle                m_middle{};
        Beginning             m_beginning{};
        End                   m_end{};
    };

    template<typename M>
    explicit Spacer(M&&)           -> Spacer<char, std::char_traits<char>, M>;

    template<typename B, typename M>
    explicit Spacer(B&&, M&&)      -> Spacer<char, std::char_traits<char>, M, B>;

    template<typename B, typename M, typename E>
    explicit Spacer(B&&, M&&, E&&) -> Spacer<char, std::char_traits<char>, M, B, E>;

} // cool namespace

#endif /* COOL_SPACER_H_ */

