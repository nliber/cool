#ifndef COOL_SPACER_H_
#define COOL_SPACER_H_

#include <cassert>
#include <ostream>
#include <string>
#include <tuple>
#include <type_traits>
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
//  All these constructors take an optional std::basic_ostream<charT, traits>&
//  parameter at the end which is used for deduction guides only to deduce the
//  ostream type (nothing is streamed to it on construction, nor is it stored).
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
             typename Ending = void,
             typename charT  = char,
             typename traits = std::char_traits<charT>>
    class Spacer
    {
        // Using a tuple because some implementations (such as libstdc++)
        //  use EBO to reduce size.

        // Helpers to convert void into an empty type capital Void that can be stored in a tuple
        struct Void { friend auto& operator<<(std::basic_ostream<charT, traits>& os, Void) { return os; } };

        template<typename T>
        using void_to_Void_t = std::conditional_t<std::is_void<T>{}, Void, T>;

        using Separators = std::tuple<void_to_Void_t<Middle>, void_to_Void_t<Beginning>, void_to_Void_t<Ending>>;

        constexpr decltype(auto) beginning() const noexcept { return std::get<1>(m_separators); }
        constexpr decltype(auto) middle()    const noexcept { return std::get<0>(m_separators); }
        constexpr decltype(auto) ending()    const noexcept { return std::get<2>(m_separators); }

    public:
        using beginning_type = Beginning;
        using middle_type    = Middle;
        using ending_type    = Ending;
        using value_type     = charT;
        using traits_type    = traits;
        using ostream_type   = std::basic_ostream<charT, traits>;

        template<typename M>
        explicit Spacer(M&& m)
        : m_separators{std::forward<M>(m), std::tuple_element_t<1, Separators>{}, std::tuple_element_t<2, Separators>{}}
        {}

        template<typename M>
        explicit Spacer(M&& m, ostream_type&)
        : Spacer{std::forward<M>(m)}
        {}

        template<typename B, typename M>
        explicit Spacer(B&& b, M&& m)
        : m_separators{std::forward<M>(m), std::forward<B>(b), std::tuple_element_t<2, Separators>{}}
        {}

        template<typename B, typename M>
        explicit Spacer(B&& b, M&& m, ostream_type&)
        : Spacer{std::forward<B>(b), std::forward<M>(m)}
        {}

        template<typename B, typename M, typename E>
        explicit Spacer(B&& b, M&& m, E&& e)
        : m_separators{std::forward<M>(m), std::forward<B>(b), std::forward<E>(e)}
        {}

        template<typename B, typename M, typename E>
        explicit Spacer(B&& b, M&& m, E&& e, ostream_type&)
        : Spacer{std::forward<B>(b), std::forward<M>(m), std::forward<E>(e)}
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
        mutable ostream_type* m_os = nullptr;
        Separators            m_separators;
    };

    template<typename M>
    explicit Spacer(M&&)
    -> Spacer<void, M>;

    template<typename M, typename charT, typename traits>
    explicit Spacer(M&&, std::basic_ostream<charT, traits>&)
    -> Spacer<void, M, void, charT, traits>;

    template<typename B, typename M>
    explicit Spacer(B&&, M&&)
    -> Spacer<B, M>;

    template<typename B, typename M, typename charT, typename traits>
    explicit Spacer(B&&, M&&, std::basic_ostream<charT, traits>&)
    -> Spacer<B, M, void, charT, traits>;

    template<typename B, typename M, typename E>
    explicit Spacer(B&&, M&&, E&&)
    -> Spacer<B, M, E>;

    template<typename B, typename M, typename E, typename charT, typename traits>
    explicit Spacer(B&&, M&&, E&&, std::basic_ostream<charT, traits>&)
    -> Spacer<B, M, E, charT, traits>;

} // cool namespace

#endif /* COOL_SPACER_H_ */

