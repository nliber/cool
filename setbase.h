#ifndef COOL_SETBASE_H_
#define COOL_SETBASE_H_

#include <cassert>
#include <ios>
#include <istream>
#include <optional>
#include <ostream>

namespace cool
{
    class setbase
    {
        using fmtflags = std::ios_base::fmtflags;
    public:
        using base_type = int;

        setbase() = default;

        explicit setbase(int base)
        : m_base{base}
        {}

        explicit setbase(int base, std::ios_base& ios)
        : m_base{base}
        { save(ios); }

        explicit setbase(std::ios_base& ios, int base)
        : m_base{base}
        { save(ios); }

        setbase(setbase const&)            = delete;
        setbase& operator=(setbase const&) = delete;
        setbase& operator=(setbase&&)      = delete;
        setbase(setbase&&)                 = delete;

        ~setbase()
        {
            if (m_ios)
                m_ios->setf(m_flags, std::ios_base::basefield);
        }

        template<typename charT, typename traits>
        friend auto& operator>>(std::basic_istream<charT, traits>& is, setbase const& that)
        {
            that.save(is);
            return is;
        }

        template<typename charT, typename traits>
        friend auto& operator<<(std::basic_ostream<charT, traits>& os, setbase const& that)
        {
            that.save(os);
            return os;
        }

    private:
        void save(std::ios_base& ios) const
        {
            assert(!m_ios);

            m_flags = m_base ? ios.setf(m_base ==  8 ? std::ios_base::oct :
                                        m_base == 10 ? std::ios_base::dec :
                                        m_base == 16 ? std::ios_base::hex :
                                        std::ios_base::fmtflags{}         , std::ios_base::basefield) :
                               ios.flags();

            m_ios = &ios;
        }

        mutable std::ios_base* m_ios = nullptr;
        std::optional<int>     m_base;
        mutable fmtflags       m_flags;
    };
} // cool namespace

#endif /* COOL_SETBASE_H_ */

