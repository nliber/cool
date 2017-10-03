#ifndef COOL_STOPWATCH_H_
#define COOL_STOPWATCH_H_

#include <cool/chrono.h>
#include <chrono>
#include <ostream>

///////////////////////////////////////////////////////////////////////////////
// Stopwatch
//
// Stopwatch measures lap time
// 
//  Clock - defaults to high_resolution_clock
//  lap() - return Clock::duration of how much time has passed
//  operator<< - stream inserted of formatted duration
///////////////////////////////////////////////////////////////////////////////
namespace cool
{

    template<typename Clock = std::chrono::high_resolution_clock>
    class Stopwatch
    {
    public:
        using clock    = Clock;
        using duration = typename clock::duration;

        duration lap() const noexcept
        { return clock::now() - start; }

        friend std::ostream& operator<<(std::ostream& os, Stopwatch const& that)
        {
            using cool::chrono::duration::operator<<;
            return os << that.lap();
        }

    private:
        using time_point = typename clock::time_point;
        time_point start = clock::now();
    };

    Stopwatch() -> Stopwatch<>;

} // cool namespace

#endif /* COOL_STOPWATCH_H_ */

