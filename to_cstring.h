#ifndef COOL_TO_CSTRING_H_
#define COOL_TO_CSTRING_H_

#include <algorithm>    // copy
#include <array>
#include <cassert>
#include <climits>      // CHAR_BIT
#include <cstddef>      // size_t, ptrdiff_t
#include <cstdint>      // uintmax_t
#include <iterator>     // reverse_iterator
#include <ostream>
#include <string>       // char_traits
#include <string_view>
#include <type_traits>
#include <utility>      // swap

namespace cool
{
    ///////////////////////////////////////////////////////////////////////////
    // to_cstring
    //
    //  to_cstring is a non-allocating owning string
    //  which holds an integral value converted to a character string.
    //
    //  Requires that base has a value between 2 and 36 (inclusive).
    //
    //  On construction, the value is converted to a string of digits in the
    //  given base (with no redundant leading zeroes).  Digits in the range
    //  10..35 (inclusive) are represented as lowercase characters a..z.  If
    //  the value is less than zero, the representation starts with '-'.
    //
    //  One example of using this is printf("%s\n", to_cstring(value).c_str());
    //  * It doesn't use locales, so it may be faster.
    //  * Like streams, you don't have to know the exact type of value passed
    //    to it, as a templated constructor takes care of that
    //
    ///////////////////////////////////////////////////////////////////////////
    class to_cstring
    {
    public:
        // types
        using value_type                      = char;
        using traits_type                     = std::char_traits<value_type>;
        using pointer                         = value_type*;
        using const_pointer                   = const value_type*;
        using reference                       = value_type&;
        using const_reference                 = const value_type&;
        using const_iterator                  = const_pointer;
        using iterator                        = const_iterator;
        using const_reverse_iterator          = std::reverse_iterator<const_iterator>;
        using reverse_iterator                = const_reverse_iterator;
        using size_type                       = std::size_t;
        using difference_type                 = std::ptrdiff_t;
        static const constexpr size_type npos = static_cast<size_type>(-1);

        // public constructors
        template<typename I, typename = std::enable_if_t<std::is_integral_v<I>>>
        explicit to_cstring(I i, int base = 10) noexcept
        : to_cstring{i < I{}, magnitude_type(i < I{} ? -i : i), magnitude_type(base)}
        {}

        // Copying added for efficiency (compiler-generated copy/move are correct but inefficient)
        to_cstring(to_cstring const& that) noexcept
        : m_pos{that.m_pos}
        {
            // need to copy m_cstring[m_pos..max_size() + sizeof('\0')],
            // since we are not yet '\0'-terminated

            std::copy(&that.m_cstring[m_pos], &that.m_cstring[max_size() + sizeof('\0')], &m_cstring[m_pos]);
        }

        to_cstring& operator=(to_cstring const& that) noexcept
        {
            // need to copy m_cstring[m_pos..max_size()],
            // since we are already '\0'-terminated

            m_pos = that.m_pos;
            std::copy(&that.m_cstring[m_pos], &that.m_cstring[max_size()], &m_cstring[m_pos]);

            return *this;
        }

        // iterator support
        const_iterator         begin()   const noexcept { return m_cstring.data() + m_pos; }
        const_iterator         end()     const noexcept { return m_cstring.data() + max_size(); }
        const_iterator         cbegin()  const noexcept { return begin(); }
        const_iterator         cend()    const noexcept { return end(); }
        const_reverse_iterator rbegin()  const noexcept { return const_reverse_iterator{end()}; }
        const_reverse_iterator rend()    const noexcept { return const_reverse_iterator{begin()}; }
        const_reverse_iterator crbegin() const noexcept { return rbegin(); }
        const_reverse_iterator crend()   const noexcept { return rend(); }

        // capacity
        size_type size()                const noexcept { return max_size() - m_pos; }
        size_type length()              const noexcept { return size(); }
        static constexpr size_type max_size() noexcept { return m_max_size; }
        static constexpr bool      empty()    noexcept { return false; }

        // element access
        const_reference operator[](size_type pos) const          { return m_cstring[m_pos + pos]; }
        const_reference at(size_type pos)         const          { return m_cstring.at(m_pos + pos); }
        const_reference front()                   const noexcept { return *begin(); }
        const_reference back()                    const noexcept { return m_cstring[max_size() - 1]; }

        // string operations
        const_pointer   data()             const noexcept { return begin(); }
        const_pointer   c_str()            const noexcept { return begin(); }
        operator        std::string_view() const noexcept { return std::string_view{data(), size()}; }

        // comparisons
        // Note:  these do string comparisons, not numeric comparisons
        friend bool operator==(to_cstring const& l, to_cstring const& r) noexcept
        { return std::equal(l.begin(), l.end(), r.begin(), r.end()); }

        friend bool operator!=(to_cstring const& l, to_cstring const& r) noexcept
        { return !(l == r); }

        friend bool operator<(to_cstring const& l, to_cstring const& r) noexcept
        { return std::lexicographical_compare(l.begin(), l.end(), r.begin(), r.end()); }

        friend bool operator>(to_cstring const& l, to_cstring const& r) noexcept
        { return r < l; }

        friend bool operator<=(to_cstring const& l, to_cstring const& r) noexcept
        { return !(r < l); }

        friend bool operator>=(to_cstring const& l, to_cstring const& r) noexcept
        { return !(l < r); }

        // swap
        friend void swap(to_cstring& l, to_cstring& r)
        {
            // l.m_cstring[l.m_pos..pos) are the chars to be copied from l to r
            // r.m_cstring[r.m_pos..pos) are the chars to be copied from r to l
            // m_cstring[pos..max_size()) are the chars to be swapped between l and r
            // No need to swap the end '\0'

            size_type pos = std::max(l.m_pos, r.m_pos);

            std::copy(&l.m_cstring[l.m_pos], &l.m_cstring[pos], &r.m_cstring[l.m_pos]);
            std::copy(&r.m_cstring[r.m_pos], &r.m_cstring[pos], &l.m_cstring[r.m_pos]);
            std::swap_ranges(&l.m_cstring[pos], &l.m_cstring[max_size()], &r.m_cstring[pos]);

            using std::swap;
            swap(l.m_pos, r.m_pos);
        }

        void swap(to_cstring& that)
        {
            using std::swap;
            swap(*this, that);
        }

        friend std::ostream& operator<<(std::ostream& os, to_cstring const& that)
        { return os << that.c_str(); }

    private:
        using magnitude_type                        = std::uintmax_t;
        static const constexpr size_type m_max_size = sizeof('-') + CHAR_BIT * sizeof(magnitude_type);
        using cstring_type                          = std::array<char, m_max_size + sizeof('\0')>;

        explicit to_cstring(bool negative, magnitude_type magnitude, magnitude_type base) noexcept
        : m_pos{max_size()}
        {
            assert(2 <= base && base <= 36);

            m_cstring[m_pos] = '\0';
            do
            {
                m_cstring[--m_pos] = "0123456789abcdefghijklmnopqrstuvwxyz"[magnitude % base];
                magnitude /= base;
            }
            while (magnitude);

            if (negative)
                m_cstring[--m_pos] = '-';
        }

        cstring_type m_cstring;
        size_type    m_pos;
    };

}  // cool namespace

#endif /* COOL_TO_CSTRING_H_ */

