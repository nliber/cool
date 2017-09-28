#ifndef COOL_RESETIOSFLAGS_H_
#define COOL_RESETIOSFLAGS_H_

#include <cassert>
#include <ios>
#include <istream>
#include <ostream>

namespace cool
{
    class resetiosflags
    {
    public:
        using fmtflags = std::ios_base::fmtflags;

        explicit resetiosflags(fmtflags mask)
        : m_mask{mask}
        {}

        explicit resetiosflags(fmtflags mask, std::ios_base& ios)
        : m_mask{mask}
        { reset(ios); }

        explicit resetiosflags(std::ios_base& ios, fmtflags mask)
        : m_mask{mask}
        { reset(ios); }

        resetiosflags(resetiosflags const&)            = delete;
        resetiosflags& operator=(resetiosflags const&) = delete;
        resetiosflags& operator=(resetiosflags&&)      = delete;
        resetiosflags(resetiosflags&&)                 = delete;

        ~resetiosflags()
        {
            if (m_ios)
                m_ios->setf(m_flags, m_mask);
        }

        template<typename charT, typename traits>
        friend auto& operator>>(std::basic_istream<charT, traits>& is, resetiosflags const& that)
        {
            that.reset(is);
            return is;
        }

        template<typename charT, typename traits>
        friend auto& operator<<(std::basic_ostream<charT, traits>& os, resetiosflags const& that)
        {
            that.reset(os);
            return os;
        }

    private:
        void reset(std::ios_base& ios) const
        {
            assert(!m_ios);

            m_flags = ios.setf(fmtflags{}, m_mask);
            m_ios   = &ios;
        }

        mutable std::ios_base* m_ios = nullptr;
        fmtflags               m_mask;
        mutable fmtflags       m_flags;
    };
} // cool namespace

#endif /* COOL_RESETIOSFLAGS_H_ */

