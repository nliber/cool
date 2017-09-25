#ifndef COOL_SETFILL_H_
#define COOL_SETFILL_H_

#include <cassert>
#include <ios>
#include <istream>
#include <optional>
#include <ostream>
#include <string>


namespace cool
{
    template<typename charT, typename traits = std::char_traits<charT>>
    class setfill
    {
    public:
        using char_type = charT;
        using traits_type = traits;

        setfill() = default;

        explicit setfill(charT f)
        : m_newFill{f}
        {}

        explicit setfill(charT f, std::basic_ios<charT, traits>& ios)
        : m_newFill{f}
        { fill(ios); }

        explicit setfill(std::basic_ios<charT, traits>& ios)
        { fill(ios); }

        explicit setfill(std::basic_ios<charT, traits>& ios, charT f)
        : m_newFill{f}
        { fill(ios); }

        setfill(setfill const&)            = delete;
        setfill& operator=(setfill const&) = delete;
        setfill& operator=(setfill&&)      = delete;
        setfill(setfill&&)                 = delete;

        ~setfill()
        {
            if (m_ios)
                m_ios->fill(m_oldFill);
        }

        friend auto& operator>>(std::basic_istream<charT, traits>& is, setfill& that)
        {
            that.fill(is);
            return is;
        }

        friend auto& operator<<(std::basic_ostream<charT, traits>& os, setfill const& that)
        {
            that.fill(os);
            return os;
        }

    private:
        void fill(std::basic_ios<charT, traits>& ios) const
        {
            assert(!m_ios);

            m_oldFill = m_newFill ? ios.fill(*m_newFill) : ios.fill();
            m_ios = &ios;
        }

        mutable std::basic_ios<charT, traits>* m_ios = nullptr;
        std::optional<charT>                   m_newFill;
        mutable charT                          m_oldFill;

    };

    setfill()                  -> setfill<char>;

    explicit setfill(char)     -> setfill<char>;
    explicit setfill(wchar_t)  -> setfill<wchar_t>;
    explicit setfill(char16_t) -> setfill<char16_t>;
    explicit setfill(char32_t) -> setfill<char32_t>;

} // cool namespace

#endif /* COOL_SETFILL_H_ */

