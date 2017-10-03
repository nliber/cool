#ifndef COOL_RATIO_H_
#define COOL_RATIO_H_

#include <cstdint>
#include <ostream>
#include <ratio>

///////////////////////////////////////////////////////////////////////////////
// A stream inserter operator for std::ratio
//
// Usage:
//  using cool::ratio::operator<<;
//  std::cout << std::ratio<1000,1>{} << std::endl;  // outputs "kilo"
///////////////////////////////////////////////////////////////////////////////

namespace cool { namespace ratio {
    template<intmax_t N, intmax_t D>
    std::ostream& operator<<(std::ostream& os, std::ratio<N, D>)
    {
        if (N == 1)
        {
            switch (D)
            {
#if 0
            case 1'000'000'000'000'000'000'000'000:
                return os << "yocto";
            case 1'000'000'000'000'000'000'000:
                return os << "zepto";
#endif
            case 1'000'000'000'000'000'000:
                return os << "atto";
            case 1'000'000'000'000'000:
                return os << "femto";
            case 1'000'000'000'000:
                return os << "pico";
            case 1'000'000'000:
                return os << "nano";
            case 1'000'000:
                return os << "micro";
            case 1'000:
                return os << "milli";
            case 100:
                return os << "centi";
            case 10:
                return os << "deci";
            default:
                break;
            }
        }

        if (D == 1)
        {
            switch (N)
            {
            case 10:
                return os << "deca";
            case 100:
                return os << "hecto";
            case 1'000:
                return os << "kilo";
            case 1'000'000:
                return os << "mega";
            case 1'000'000'000:
                return os << "giga";
            case 1'000'000'000'000:
                return os << "tera";
            case 1'000'000'000'000'000:
                return os << "peta";
            case 1'000'000'000'000'000'000:
                return os << "exa";
#if 0
            case 1'000'000'000'000'000'000'000:
                return os << "zetta";
            case 1'000'000'000'000'000'000'000'000:
                return os << "yotta";
#endif
            default:
                break;
            }
        }

        return os << "ratio<" << N << ',' << D << '>';
    }

} /* ratio namespace */ } /* cool namespace */

#endif /* COOL_RATIO_H_ */

