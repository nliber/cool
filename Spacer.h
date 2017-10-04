#ifndef COOL_SPACER_H_
#define COOL_SPACER_H_

#include <cassert>
#include <ostream>
#include <string>
#include <tuple>
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
        operator>>(std::basic_istream<charT, traits>& is, NullIOStream)
        { return is; }
    };

    template<typename Beginning,
             typename Middle,
             typename Ending = NullIOStream,
             typename charT  = char,
             typename traits = std::char_traits<charT>>
    class Spacer
    {
    public:
        using beginning_type = Beginning;
        using middle_type    = Middle;
        using ending_type    = Ending;
        using value_type     = charT;
        using traits_type    = traits;
        using ostream_type   = std::basic_ostream<charT, traits>;

        template<typename M>
        explicit Spacer(M&& m)
        : m_separators{std::forward<M>(m), Beginning{}, Ending{}}
        {}

        template<typename B, typename M>
        explicit Spacer(B&& b, M&& m)
        : m_separators{std::forward<M>(m), std::forward<B>(b), Ending{}}
        {}

        template<typename B, typename M, typename E>
        explicit Spacer(B&& b, M&& m, E&& e)
        : m_separators{std::forward<M>(m), std::forward<B>(b), std::forward<E>(e)}
        {}

        Spacer(Spacer const&)            = delete;
        Spacer& operator=(Spacer const&) = delete;
        Spacer& operator=(Spacer&&)      = delete;
        Spacer(Spacer&&)                 = delete;

        const auto& beginning() const noexcept { return std::get<1>(m_separators); }
        const auto& middle()    const noexcept { return std::get<0>(m_separators); }
        const auto& ending()    const noexcept { return std::get<2>(m_separators); }

        friend ostream_type& operator<<(ostream_type& os, Spacer const& that)
        {
            if (that.m_os)
            {
                assert(&os == that.m_os);
                os << that.middle();
            } else {
                os << that.beginning();
                that.m_os = &os;
            }

            return os;
        }

        ~Spacer()
        {
            if (m_os)
                *m_os << ending();
        }

    private:
        // Using a tuple because some implementations (such as libstdc++)
        //  use EBO to reduce size.
        using tuple_type = std::tuple<Middle, Beginning, Ending>;

        mutable ostream_type* m_os = nullptr;
        tuple_type            m_separators;
    };

    template<typename M>
    explicit Spacer(M&&)           -> Spacer<NullIOStream, M>;

    template<typename B, typename M>
    explicit Spacer(B&&, M&&)      -> Spacer<B, M>;

    template<typename B, typename M, typename E>
    explicit Spacer(B&&, M&&, E&&) -> Spacer<B, M, E>;

} // cool namespace

#endif /* COOL_SPACER_H_ */

