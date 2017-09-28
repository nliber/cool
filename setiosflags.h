#ifndef COOL_SETIOSFLAGS_H_
#define COOL_SETIOSFLAGS_H_

#include <cassert>
#include <ios>
#include <istream>
#include <ostream>

namespace cool
{
    class setiosflags
    {
    public:
        using fmtflags = std::ios_base::fmtflags;

        explicit setiosflags(fmtflags mask)
        : m_mask{mask}
        {}

        explicit setiosflags(fmtflags mask, std::ios_base& ios)
        : m_mask{mask}
        { save(ios); }

        explicit setiosflags(std::ios_base& ios, fmtflags mask)
        : m_mask{mask}
        { save(ios); }

        setiosflags(setiosflags const&)            = delete;
        setiosflags& operator=(setiosflags const&) = delete;
        setiosflags& operator=(setiosflags&&)      = delete;
        setiosflags(setiosflags&&)                 = delete;

        ~setiosflags()
        {
            if (m_ios)
                m_ios->setf(m_flags, m_mask);
        }

        template<typename charT, typename traits>
        friend auto& operator>>(std::basic_istream<charT, traits>& is, setiosflags const& that)
        {
            that.save(is);
            return is;
        }

        template<typename charT, typename traits>
        friend auto& operator<<(std::basic_ostream<charT, traits>& os, setiosflags const& that)
        {
            that.save(os);
            return os;
        }

    private:
        void save(std::ios_base& ios) const
        {
            assert(!m_ios);

            m_flags = ios.setf(m_mask);
            m_ios   = &ios;
        }

        mutable std::ios_base* m_ios = nullptr;
        fmtflags               m_mask;
        mutable fmtflags       m_flags;
    };
} // cool namespace

#endif /* COOL_SETIOSFLAGS_H_ */

