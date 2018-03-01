#ifndef COOL_C_STR_T_H_
#define COOL_C_STR_T_H_

#include <array>
#include <cassert>
#include <climits>      // CHAR_BIT
#include <cstddef>      // size_t
#include <limits>       // numeric_limits
#include <string_view>
#include <type_traits>  // make_unsigned

namespace cool
{
    ///////////////////////////////////////////////////////////////////////////
    // c_str_t
    //
    //  c_str_t is a non-allocating owning string (with no throwing operations)
    //  which holds an integral value converted to a character string.
    //
    //  Requires that base has a value between 2 and 36 (inclusive).
    //
    //  On construction, the value is converted to a string of digits in the
    //  given base (with no redundant leading zeroes).  Digits in the range
    //  10..35 (inclusive) are represented as lowercase characters a..z.  If
    //  the value is less than zero, the representation starts with '-'.
    //
    //  One example of using this is printf("%s\n", c_str_t(value).c_str());
    //  * It doesn't use locales, so it may be faster.
    //  * Like streams, you don't have to know the exact type of value passed
    //    to it, as templates take care of that.
    //
    ///////////////////////////////////////////////////////////////////////////
    template<typename I>
    class c_str_t
    {
    public:
        using size_type = std::size_t;
    
        explicit constexpr c_str_t(I i, int base = 10) noexcept
        : m_pos{max_size()}
        , m_base{static_cast<unsigned_t>(base)}
        , m_i{i}
        {
            assert(2 <= base && base <= 36);

            bool       negative{i < I{}};
            unsigned_t u{negative ? -i : i};

            m_c_str_array[m_pos] = '\0';
            do
            {
                m_c_str_array[--m_pos] = "0123456789abcdefghijklmnopqrstuvwxyz"[u % m_base];
                u /= m_base;
            } while (u);

            if (negative)
                m_c_str_array[--m_pos] = '-';
        }

        // capacity
        constexpr size_type size() const noexcept
        { return max_size() - m_pos; }

        constexpr size_type length() const noexcept
        { return size(); }

        static constexpr size_type max_size() noexcept
        { return std::numeric_limits<I>::is_signed + CHAR_BIT * sizeof(I); }

        // string operations
        constexpr const char* c_str() const noexcept
        { return m_c_str_array.data() + m_pos; }

        constexpr const char* data() const noexcept
        { return c_str(); }

        constexpr operator std::string_view() const noexcept
        { return std::string_view(data(), size()); }

    private:
        using c_str_array_t = std::array<char, max_size() + sizeof('\0')>;
        using unsigned_t = std::make_unsigned_t<I>;

        c_str_array_t m_c_str_array;
        size_type     m_pos;
        unsigned_t    m_base;
        I             m_i;
    };

    template<typename I>
    c_str_t(I, int) -> c_str_t<I>;

}  // cool namespace

#endif /* COOL_C_STR_T_H */

