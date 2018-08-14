#ifndef COOL_ILESS_CHAR_H_
#define COOL_ILESS_CHAR_H_

#include <locale>

namespace cool
{
    ///////////////////////////////////////////////////////////////////////////
    // iless_char is a comparison class which less than compares two
    // characters without regards to case
    ///////////////////////////////////////////////////////////////////////////
    struct iless_char : private std::locale
    {
        iless_char() = default;
        explicit iless_char(std::locale const& loc) : std::locale{loc} {}

        std::locale const& get_locale() const noexcept
        { return *this; }

        // Transform a character for comparison
        template<typename C>
        C operator()(C c) const
        { return std::toupper(c, get_locale()); }

        template<typename L, typename R>
        bool operator()(L l, R r) const
        { return (*this)(l) < (*this)(r); }

        using is_transparent = void;
    };

} // cool namespace

#endif /* COOL_ILESS_CHAR_H_ */

