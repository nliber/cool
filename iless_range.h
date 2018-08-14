#ifndef COOL_ILESS_RANGE_H_
#define COOL_ILESS_RANGE_H_

#include <cool/iless_char.h>
#include <algorithm>
#include <iterator>

namespace cool
{
    ///////////////////////////////////////////////////////////////////////////
    // iless_range is a comparison class which lexicographicall compares two
    //  container-like objects without regards to case
    ///////////////////////////////////////////////////////////////////////////
    struct iless_range : private iless_char
    {
        iless_range() = default;
        explicit iless_range(iless_char const& ilc) : iless_char{ilc} {}
        explicit iless_range(std::locale const& loc) : iless_char{loc} {}

        using iless_char::get_locale;
        iless_char const& get_iless_char() const noexcept { return *this; }

        template<typename L, typename R>
        bool operator()(L const& l, R const& r) const
        {
            return std::lexicographical_compare(std::begin(l), std::end(l),
                                                std::begin(r), std::end(r),
                                                get_iless_char());
        }

        using is_transparent = void;
    };

} // cool namespace

#endif /* COOL_ILESS_RANGE_H_ */

