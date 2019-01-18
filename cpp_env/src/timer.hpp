//
// Created by afterburner on 19-1-11.
//

#ifndef HELLO_TIMER_H
#define HELLO_TIMER_H

#include <chrono>  // for high_resolution_clock, time_point
#include <fstream> // for ifstream, ofstream
#include <map>     // for map
#include <string>  // for string
#include <utility> // for move

enum class Metric
{
    second,
    milisecond
};

struct Timer
{
    typedef std::chrono::high_resolution_clock Clock;
    typedef std::chrono::time_point<Clock> timePoint;

    Timer(Metric m = Metric::milisecond, std::string file_name = "benchmark.log") : _tp_start(Clock::now()), file_name(std::move(file_name)), metric(m) {}
    Timer(const Timer &other) = delete;
    Timer(Timer &&other) = default;
    Timer &operator=(const Timer &other) = delete;
    Timer &operator=(Timer &&other) = default;
    virtual ~Timer();

    void stop(std::string session_name);
    void start();

private:
    timePoint _tp_start, _tp_current;
    std::map<std::string, long> _time_by_session;
    std::ofstream out_file;
    std::ifstream in_file;
    const std::string file_name;
    const Metric metric;
};

#endif //HELLO_TIMER_H
