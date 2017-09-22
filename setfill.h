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
        setfill() = default;

        explicit setfill(charT f)
        : m_fill{f}
        {}

        explicit setfill(charT f, std::basic_ios<charT, traits>& ios)
        : m_fill{ios.fill(f)}
        , m_ios{&ios}
        {}

        explicit setfill(std::basic_ios<charT, traits>& ios)
        : m_fill{ios.fill()}
        , m_ios{&ios}
        {}

        explicit setfill(std::basic_ios<charT, traits>& ios, charT f)
        : setfill{f, ios}
        {}

        setfill(setfill const&)            = delete;
        setfill& operator=(setfill const&) = delete;
        setfill& operator=(setfill&&)      = delete;
        setfill(setfill&&)                 = delete;

        ~setfill()
        {
            if (m_ios)
                m_ios->fill(*m_fill);
        }

        friend std::istream& operator>>(std::istream& is, setfill& that)
        {
            that.fill(is);
            return is;
        }

        friend std::ostream& operator<<(std::ostream& os, setfill const& that)
        {
            that.fill(os);
            return os;
        }

    private:
        void fill(std::basic_ios<charT, traits>& ios) const
        {
            assert(!m_ios);

            if (m_fill)
                m_fill = ios.fill(*m_fill);
            else
                m_fill = ios.fill();

            m_ios = &ios;
        }

        mutable std::optional<charT>           m_fill;
        mutable std::basic_ios<charT, traits>* m_ios = nullptr;
    };

    setfill()                  -> setfill<char>;

    explicit setfill(char)     -> setfill<char>;
    explicit setfill(wchar_t)  -> setfill<wchar_t>;
    explicit setfill(char16_t) -> setfill<char16_t>;
    explicit setfill(char32_t) -> setfill<char32_t>;

} // cool namespace

#endif /* COOL_SETFILL_H_ */

