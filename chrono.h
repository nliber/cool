#ifndef COOL_CHRONO_H_
#define COOL_CHRONO_H_

#include <cool/ratio.h>
#include <array>
#include <chrono>
#include <ctime>
#include <ostream>

///////////////////////////////////////////////////////////////////////////////
// Stream inserters operator for things in std::chrono
//
// std::chrono::duration
//
// Usage:
//  using cool::chrono::duration::operator<<;
//  std::cout << std::chrono::hours{2} << std::endl;  // outputs "2 hours"
//
//
// std::chrono::system_clock
//
// Usage:
//  using cool::chrono::system_clock::operator<<;
//  std::cout << std::chrono::system_clock{} << std::endl; // outputs "Thu, 12 Aug 1965 12:59:00 -0500"
//
//  now() - returns a '\0'-terminated array containing the formatted system_clock
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
    inline auto now()
    {
        std::chrono::system_clock::time_point tp{std::chrono::system_clock::now()};
        std::time_t                           timeT{std::chrono::system_clock::to_time_t(tp)};

        struct tm                             tm;
        localtime_r(&timeT, &tm);

        std::array<char, sizeof "Thu, 12 Aug 1965 12:59:00 -0500"> formatted;
        strftime(formatted.data(), formatted.size(), "%a, %d %b %Y %T %z", &tm);

        return formatted;
    }

    inline
    std::ostream& operator<<(std::ostream& os, std::chrono::system_clock)
    { return os << cool::chrono::system_clock::now().data(); }
} // system_clock namespace

} /* chrono namespace */ } /* cool namespace */

#endif /* COOL_CHRONO_H_ */

