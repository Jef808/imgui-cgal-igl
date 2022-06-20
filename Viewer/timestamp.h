#ifndef UTILS_H_
#define UTILS_H_

#include <cassert>
#include <chrono>
#include <iostream>
#include <vector>

namespace TimeUtil {


class TimeStamper {
public:

    using ms = std::chrono::milliseconds;
    using ns = std::chrono::nanoseconds;
    using clock = std::chrono::steady_clock;
    using time_point = clock::time_point;
    using period = clock::period;
    using duration = clock::duration;
    struct time_interval { time_point start; time_point end; };

    TimeStamper() = default;

    enum class Status { Stopped, Running, Idle };

    void begin();

    void toggle_suspend();

    void end();

    time_point get();

private:
    Status m_status { Status::Stopped };
    time_point time_started { time_point::max() };
    time_point time_ended { time_point::min() };
    std::vector<time_interval> idle_time_intervals;
    size_t n_stamps{ 0 };
};



inline void TimeStamper::begin()
{
    switch (m_status)
    {
        case Status::Idle:
        {
            idle_time_intervals.back().end = clock::now();
            m_status = Status::Running;
            return;
        }
        case Status::Running:
        {
            std::cerr << "WARNING: TimeStamper::begin() "
                         "called on running clock. "
                         "Resetting data"
                      << std::endl;
            break;
        }
        case Status::Stopped:
        {
            if (not idle_time_intervals.empty() || n_stamps > 0) {
                std::cerr << "WARNING: TimeStamper::begin() "
                             "called on stopped but clock was "
                             "already initialized. Resetting data."
                          << std::endl;
                idle_time_intervals.clear();
                n_stamps = 0;
                m_status = Status::Running;
                break;
            }
        }
        default:
            assert(false && "ERROR: unimplemented timestamper status");
    }
    time_started = clock::now();
}


inline void TimeStamper::toggle_suspend()
{
    const time_point tp = clock::now();

    switch (m_status)
    {
        case Status::Idle:
        {
            idle_time_intervals.back().end = tp;
            m_status = Status::Running;
            break;
        }
        case Status::Running:
        {
            idle_time_intervals.emplace_back().start = tp;
            m_status = Status::Idle;
            break;
        }
        case Status::Stopped:
        {
            std::cerr << "WARNING: TimeStamper::toggle_suspend() "
                         "called on stopped clock"
                      << std::endl;
            if (not idle_time_intervals.empty() || n_stamps > 0) {
                std::cerr << "--- Moreover, clock was initialized. "
                             "Resetting data."
                          << std::endl;
                idle_time_intervals.clear();
                n_stamps = 0;
            }
            m_status = Status::Running;
            time_started = clock::now();
            break;
        }
        default:
            assert(false && "ERROR: unimplemented timestamper status.");
    }
}

inline void TimeStamper::end()
{
    time_point tp = clock::now();
    switch (m_status)
    {
        case Status::Running:
        {
            time_ended = tp;
            break;
        }
        case Status::Idle:
        {
            idle_time_intervals.back().end = time_ended = tp;
            break;
        }
        case Status::Stopped:
        {
            std::cerr << "WARNING: TimeStamper::end() "
                         "called on stopped clock"
                      << std::endl;
            break;
        }
        default:
            assert(false && "ERROR: unimplemented timestamper status.");
    }
    m_status = Status::Stopped;
}


inline TimeStamper::time_point TimeStamper::get() {
    return clock::now();
    ++n_stamps;
}


} // namespace TimeUtil

#endif // UTILS_H_
