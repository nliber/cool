#ifndef COOL_CCHAR_H_
#define COOL_CCHAR_H_

#include <array>
#include <cctype>
#include <ostream>

///////////////////////////////////////////////////////////////////////////////
// CChar
//
//  Convert a char to a printable string, escaping it if necessary.
//
///////////////////////////////////////////////////////////////////////////////

namespace cool
{
    class CChar
    {
    public:
        using value_type    = char;
        using string_type   = std::array<char, 5>;
        using const_pointer = value_type const*;

    private:
        static constexpr string_type SimpleEscapeSequence(char c) noexcept
        { return string_type{'\\', c, '\0'}; }

        static constexpr string_type Char(char c) noexcept
        { return string_type{c, '\0'}; }

        static constexpr string_type OctalEscapeSequence(unsigned char uc) noexcept
        { return string_type{'\\',
                             "01234567"[uc / 64],
                             "01234567"[uc % 64 / 8],
                             "01234567"[uc      % 8],
                             '\0'}; }

        static constexpr string_type EscapeSequence(char c) noexcept
        {
            switch (c)
            {
            case '\'':
            case '\"':
            case '\?':
            case '\\': return SimpleEscapeSequence(c);
            case '\a': return SimpleEscapeSequence('a');
            case '\b': return SimpleEscapeSequence('b');
            case '\f': return SimpleEscapeSequence('f');
            case '\n': return SimpleEscapeSequence('n');
            case '\r': return SimpleEscapeSequence('r');
            case '\t': return SimpleEscapeSequence('t');
            case '\v': return SimpleEscapeSequence('v');
            default  : return isprint(c) ? Char(c) : OctalEscapeSequence(c);
            };
        }

    public:
        explicit constexpr CChar(value_type c) noexcept
        : m_cchar{EscapeSequence(c)}
        , m_c{c}
        {}

        constexpr value_type    get()   const noexcept { return m_c;            }
        constexpr string_type   str()   const noexcept { return m_cchar;        }
        constexpr const_pointer data()  const noexcept { return m_cchar.data(); }
        constexpr const_pointer c_str() const noexcept { return m_cchar.data(); }

        friend std::ostream& operator<<(std::ostream& os, CChar const& that)
        { return os << that.c_str(); }

    private:
        string_type m_cchar;
        value_type  m_c;
    };

} // cool namespace

#endif /* COOL_CCHAR_H_ */

