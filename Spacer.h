#ifndef COOL_SPACER_H_
#define COOL_SPACER_H_

#include <cool/NullInserterExtractor.h>
#include <cassert>
#include <ostream>
#include <string>
#include <tuple>
#include <utility>

///////////////////////////////////////////////////////////////////////////////
//
// Spacer
//  Utility class for "spacing" through elements when looping
//
//  Note:  Because of deduction guides, it is possible for Spacer to have
//  reference semantics.
//
// Template parameters (note: the order of parameters is funky):
//  Middle - the type of the separator between elements
//  Beginning - the type of the first separator streamed
//  End - the type to be streamed on destruction
//        (if at least one separator was streamed)
//  charT - the value type of the ostream
//  traits - the traits type of the ostream
//
// Constructors:
//  Spacer(Middle m)
//  Spacer(Beginning b, Middle m)
//  Spacer(Beginning b, Middle m, End e)
//
// Usage:
//  {
//      cool::Spacer comma('[', ", ", std::string("]\n"));
//      std::cout << comma << 1 << comma << 2 << comma << 3;
//  } // outputs: [1, 2, 3]
//
///////////////////////////////////////////////////////////////////////////////

namespace cool
{

    template<typename Beginning,
             typename Middle,
             typename Ending = NullInserterExtractor,
             typename charT  = char,
             typename traits = std::char_traits<charT>>
    class Spacer
    {
        // Using a tuple because some implementations (such as libstdc++)
        //  use EBO to reduce size.
        using Separators = std::tuple<Middle, Beginning, Ending>;

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

        constexpr const auto& beginning() const noexcept { return std::get<1>(m_separators); }
        constexpr const auto& middle()    const noexcept { return std::get<0>(m_separators); }
        constexpr const auto& ending()    const noexcept { return std::get<2>(m_separators); }

        // No need to pass Spacer by const reference,
        //  since it is never intended to be a temporary
        friend ostream_type& operator<<(ostream_type& os, Spacer& that)
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
        ostream_type*    m_os = nullptr;
        const Separators m_separators;
    };

    template<typename M>
    explicit Spacer(M&&)           -> Spacer<NullInserterExtractor, M>;

    template<typename B, typename M>
    explicit Spacer(B&&, M&&)      -> Spacer<B, M>;

    template<typename B, typename M, typename E>
    explicit Spacer(B&&, M&&, E&&) -> Spacer<B, M, E>;

} // cool namespace

#endif /* COOL_SPACER_H_ */

