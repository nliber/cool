#ifndef COOL_SETPRECISION_H_
#define COOL_SETPRECISION_H_

#include <cassert>
#include <ios>
#include <istream>
#include <optional>
#include <ostream>

namespace cool
{
    class setprecision
    {
    public:
        using streamsize = std::streamsize;

        setprecision() = default;

        explicit setprecision(streamsize p)
        : m_newPrecision{p}
        {}

        explicit setprecision(streamsize p, std::ios_base& ios)
        : m_newPrecision{p}
        { precision(ios); }

        explicit setprecision(std::ios_base& ios)
        { precision(ios); }

        explicit setprecision(std::ios_base& ios, std::streamsize p)
        : m_newPrecision{p}
        { precision(ios); }

        setprecision(setprecision const&)            = delete;
        setprecision& operator=(setprecision const&) = delete;
        setprecision& operator=(setprecision&&)      = delete;
        setprecision(setprecision&&)                 = delete;

        ~setprecision()
        {
            if (m_ios)
                m_ios->precision(m_oldPrecision);
        }

        template<typename charT, typename traits>
        friend auto& operator>>(std::basic_istream<charT, traits>& is, setprecision& that)
        {
            that.precision(is);
            return is;
        }

        template<typename charT, typename traits>
        friend auto& operator<<(std::basic_ostream<charT, traits>& os, setprecision const& that)
        {
            that.precision(os);
            return os;
        }

    private:
        void precision(std::ios_base& ios) const
        {
            assert(!m_ios);

            m_oldPrecision = m_newPrecision ? ios.precision(*m_newPrecision) : ios.precision();
            m_ios = &ios;
        }

        mutable std::ios_base*    m_ios = nullptr;
        std::optional<streamsize> m_newPrecision;
        mutable streamsize        m_oldPrecision;

    };
} // cool namespace

#endif /* COOL_SETPRECISION_H_ */

