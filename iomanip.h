#ifndef COOL_IOMANIP_H_
#define COOL_IOMANIP_H_

#include <cassert>
#include <ios>
#include <istream>
#include <optional>
#include <ostream>
#include <string>

///////////////////////////////////////////////////////////////////////////////
//
// These io manipulators save off the old state when streamed and restore that
// state when destructed.
//
// Those which take a parameter set the new state after saving off the old
// state.
//
// These correspond to the manipulators in <iomanip>:
//
//  resetiosflags(std::ios_base::fmtflags)
//
//  setiosflags(std::ios_base::fmtflags)
//
//  setbase()
//  setbase(int)
//
//  template<typename charT, typename traits> setfill()
//  template<typename charT, typename traits> setfill(charT)
//
//  setprecision()
//  setprecision(std::streamsize)
//
//
// These are new io manipulators:
//
//  setflags saves and restores all flags:
//
//  setflags()
//  setflags(std::ios_base::fmtflags)
//
//  setiomanip saves and restores precision, flags and fill
//  If you pass in true, then the following are set after save
//  (values from <http://eel.is/c++draft/basic.ios.cons>):
//
//      precision = 6
//      flags     = skipws | dec
//      fill      = widen(' ')
//
//  template<typename charT, typename traits> setiomanip()
//  template<typename charT, typename traits> setiomanip(bool)
//
//
// While they are intended to be used as one-liners, you can declare them for
// multiline streaming operations.  They all take an optional stream parameter
// if you wish to set them up when you declare them.
//
// There are deduction guides to make the syntax more regular for the templated
// classes, but deduction guides are a fairly new C++17 feature and compilers
// (gcc7 and clang5) do not yet agree on their usage.
//
//
// Example:
//
//  std::cout << cool::setiomanip(true) << std::boolalpha << false << ' ' << true << '\n';
//  std::cout << false << ' ' << true << '\n';
//
//  Outputs:
//  false true
//  0 1
//
///////////////////////////////////////////////////////////////////////////////

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
        { save(ios); }

        explicit resetiosflags(std::ios_base& ios, fmtflags mask)
        : m_mask{mask}
        { save(ios); }

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
            that.save(is);
            return is;
        }

        template<typename charT, typename traits>
        friend auto& operator<<(std::basic_ostream<charT, traits>& os, resetiosflags const& that)
        {
            that.save(os);
            return os;
        }

    private:
        void save(std::ios_base& ios) const
        {
            assert(!m_ios);

            m_flags = ios.setf(fmtflags{}, m_mask);
            m_ios   = &ios;
        }

        mutable std::ios_base* m_ios = nullptr;
        fmtflags               m_mask;
        mutable fmtflags       m_flags;
    };


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
        { save(ios); }

        explicit setfill(std::basic_ios<charT, traits>& ios)
        { save(ios); }

        explicit setfill(std::basic_ios<charT, traits>& ios, charT f)
        : m_newFill{f}
        { save(ios); }

        setfill(setfill const&)            = delete;
        setfill& operator=(setfill const&) = delete;
        setfill& operator=(setfill&&)      = delete;
        setfill(setfill&&)                 = delete;

        ~setfill()
        {
            if (m_ios)
                m_ios->fill(m_oldFill);
        }

        friend auto& operator>>(std::basic_istream<charT, traits>& is, setfill const& that)
        {
            that.save(is);
            return is;
        }

        friend auto& operator<<(std::basic_ostream<charT, traits>& os, setfill const& that)
        {
            that.save(os);
            return os;
        }

    private:
        void save(std::basic_ios<charT, traits>& ios) const
        {
            assert(!m_ios);

            m_oldFill = m_newFill ? ios.fill(*m_newFill) : ios.fill();
            m_ios = &ios;
        }

        mutable std::basic_ios<charT, traits>* m_ios = nullptr;
        std::optional<charT>                   m_newFill;
        mutable charT                          m_oldFill;

    };

    setfill()                                               -> setfill<char>;

    template<typename charT>
    explicit setfill(charT)                                 -> setfill<charT>;

    template<typename charT, typename traits>
    explicit setfill(charT, std::basic_ios<charT, traits>&) -> setfill<charT, traits>;

    template<typename charT, typename traits>
    explicit setfill(std::basic_ios<charT, traits>&)        -> setfill<charT, traits>;

    template<typename charT, typename traits>
    explicit setfill(std::basic_ios<charT, traits>&, charT) -> setfill<charT, traits>;


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
        { save(ios); }

        explicit setprecision(std::ios_base& ios)
        { save(ios); }

        explicit setprecision(std::ios_base& ios, std::streamsize p)
        : m_newPrecision{p}
        { save(ios); }

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
        friend auto& operator>>(std::basic_istream<charT, traits>& is, setprecision const& that)
        {
            that.save(is);
            return is;
        }

        template<typename charT, typename traits>
        friend auto& operator<<(std::basic_ostream<charT, traits>& os, setprecision const& that)
        {
            that.save(os);
            return os;
        }

    private:
        void save(std::ios_base& ios) const
        {
            assert(!m_ios);

            m_oldPrecision = m_newPrecision ? ios.precision(*m_newPrecision) : ios.precision();
            m_ios = &ios;
        }

        mutable std::ios_base*    m_ios = nullptr;
        std::optional<streamsize> m_newPrecision;
        mutable streamsize        m_oldPrecision;

    };


    class setflags
    {
    public:
        using fmtflags = std::ios::fmtflags;

        setflags() = default;

        explicit setflags(fmtflags f)
        : m_newFlags{f}
        {}

        explicit setflags(fmtflags f, std::ios_base& ios)
        : m_newFlags{f}
        { save(ios); }

        explicit setflags(std::ios_base& ios)
        { save(ios); }

        explicit setflags(std::ios_base& ios, fmtflags f)
        : m_newFlags{f}
        { save(ios); }

        setflags(setflags const&)            = delete;
        setflags& operator=(setflags const&) = delete;
        setflags& operator=(setflags&&)      = delete;
        setflags(setflags&&)                 = delete;

        ~setflags()
        {
            if (m_ios)
                m_ios->flags(m_oldFlags);
        }

        template<typename charT, typename traits>
        friend auto& operator>>(std::basic_istream<charT, traits>& is, setflags const& that)
        {
            that.save(is);
            return is;
        }

        template<typename charT, typename traits>
        friend auto& operator<<(std::basic_ostream<charT, traits>& os, setflags const& that)
        {
            that.save(os);
            return os;
        }

    private:
        void save(std::ios_base& ios) const
        {
            assert(!m_ios);

            m_oldFlags = m_newFlags ? ios.flags(*m_newFlags) : ios.flags();
            m_ios = &ios;
        }

        mutable std::ios_base*  m_ios = nullptr;
        std::optional<fmtflags> m_newFlags;
        mutable fmtflags        m_oldFlags;
    };


    template<typename charT, typename traits = std::char_traits<charT>>
    class setiomanip
    {
    public:
        using char_type   = charT;
        using traits_type = traits;

        setiomanip() = default;

        explicit setiomanip(bool init)
        : m_init{init}
        {}

        explicit setiomanip(bool init, std::basic_ios<charT, traits>& ios)
        : m_init{init}
        { save(ios); }

        explicit setiomanip(std::basic_ios<charT, traits>& ios)
        { save(ios); }

        explicit setiomanip(std::basic_ios<charT, traits>& ios, bool init)
        : m_init{init}
        { save(ios); }

        setiomanip(setiomanip const&)            = delete;
        setiomanip& operator=(setiomanip const&) = delete;
        setiomanip& operator=(setiomanip&&)      = delete;
        setiomanip(setiomanip&&)                 = delete;

        ~setiomanip()
        {
            if (m_ios)
            {
                m_ios->fill(m_fill);
                m_ios->flags(m_flags);
                m_ios->precision(m_precision);
            }
        }

        friend auto& operator>>(std::basic_istream<charT, traits>& is, setiomanip const& that)
        {
            that.save(is);
            return is;
        }

        friend auto& operator<<(std::basic_ostream<charT, traits>& os, setiomanip const& that)
        {
            that.save(os);
            return os;
        }

    private:
        void save(std::basic_ios<charT, traits>& ios) const
        {
            assert(!m_ios);

            m_precision = m_init ? ios.precision(6)                                      : ios.precision();
            m_flags     = m_init ? ios.flags(std::ios_base::skipws | std::ios_base::dec) : ios.flags();
            m_fill      = m_init ? ios.fill(ios.widen(' '))                              : ios.fill();

            m_ios       = &ios;
        }

        mutable std::basic_ios<charT, traits>* m_ios = nullptr;
        mutable std::streamsize                m_precision;
        mutable std::ios_base::fmtflags        m_flags;
        mutable charT                          m_fill;
        bool                                   m_init = false;
    };

    setiomanip()                                              -> setiomanip<char>;
    explicit setiomanip(bool)                                 -> setiomanip<char>;

    template<typename charT, typename traits>
    explicit setiomanip(bool, std::basic_ios<charT, traits>&) -> setiomanip<charT, traits>;

    template<typename charT, typename traits>
    explicit setiomanip(std::basic_ios<charT, traits>&)       -> setiomanip<charT, traits>;

    template<typename charT, typename traits>
    explicit setiomanip(std::basic_ios<charT, traits>&, bool) -> setiomanip<charT, traits>;
} // cool namespace

#endif /* COOL_IOMANIP_H_ */

