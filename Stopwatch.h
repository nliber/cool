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

        explicit Stopwatch(bool run = false) noexcept
        : m_start{run ? clock::now() : time_point{}}
        {}

        bool isRunning() const noexcept
        { return m_start != time_point{}; }

        duration lap() const noexcept
        { return isRunning() ? clock::now() - m_start : m_since; }

        operator duration() const noexcept
        { return lap(); }

        void reset(bool run = false) noexcept
        { *this = Stopwatch{run}; }

        void start() noexcept
        {
            if (!isRunning())
            {
                m_start = clock::now() - m_since;
                m_since = duration::zero();
            }
        }

        void stop() noexcept
        {
            // No need to check isRunning(), because if we aren't running,
            // these assignments effectively don't change their value
            m_since = lap();
            m_start = time_point{};
        }

        friend std::ostream& operator<<(std::ostream& os, Stopwatch const& that)
        {
            using cool::chrono::duration::operator<<;
            return os << that.lap();
        }

    private:
        using time_point = typename clock::time_point;
        time_point m_start{};
        duration   m_since = duration::zero();
    };

#if __cplusplus >= 201703L
    Stopwatch(bool) -> Stopwatch<>;
#endif  // C++17

} // cool namespace

#endif /* COOL_STOPWATCH_H_ */

