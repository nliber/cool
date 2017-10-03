#ifndef COOL_CHRONO_H_
#define COOL_CHRONO_H_

#include <cool/ratio.h>
#include <chrono>
#include <ctime>
#include <ostream>

///////////////////////////////////////////////////////////////////////////////
// A stream inserter operator for
//  std::chrono::duration
//
// Usage:
//  using cool::chrono::duration::operator<<;
//  std::cout << std::chrono::hours{2} << std::endl;  // outputs "2 hours"
///////////////////////////////////////////////////////////////////////////////
namespace cool { namespace chrono {
namespace duration {
    template<typename Rep, intmax_t N, intmax_t D>
    std::ostream& operator<<(std::ostream& os, std::chrono::duration<Rep, std::ratio<N, D>> const& that)
    {
        os << that.count() << ' ';

        if (N == 3600 && D == 1)
            os << "hours";
        else if (N == 60 && D == 1)
            os << "minutes";
        else if (N == 1 && D == 1)
            os << "seconds";
        else
        {
            using cool::ratio::operator<<;
            os << std::ratio<N, D>{} << "seconds";
        }

        return os;
    }
} // duration namespace

namespace system_clock
{
    inline
    std::ostream& operator<<(std::ostream& os, std::chrono::system_clock)
    {
        std::chrono::system_clock::time_point tp{std::chrono::system_clock::now()};
        std::time_t                           timeT{std::chrono::system_clock::to_time_t(tp)};

        struct tm                             tm;
        localtime_r(&timeT, &tm);

        return os << asctime(&tm);
    }
} // system_clock namespace

} /* chrono namespace */ } /* cool namespace */

#endif /* COOL_CHRONO_H_ */

