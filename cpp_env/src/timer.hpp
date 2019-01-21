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
#include <iostream>
#include <iomanip>


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
     ~Timer()
    {
        in_file.open(file_name);
        long old_time;
        std::cout << std::setw(27)<<" "<< "current    "<<"previous " << '\n';
        for (const auto &[session, new_time] : _time_by_session)
        {
            in_file >> old_time;
            std::cout << std::setw(29) << std::setiosflags(std::ios::left) << session << std::setw(5) << new_time << "\t\t" << std::setw(6) << old_time << "       " << (metric == Metric::second ? "seconds" : "ms") << '\n';
        }
        in_file.close();
        out_file.open(file_name, std::ofstream::trunc | std::ofstream::out);
        for (const auto &pair : _time_by_session)
        {
            out_file << pair.second << ' ';
        }
        out_file.close();
    }

    void stop(const std::string &session_name){
        _tp_current = Clock::now();
        long time_elapsed = metric == Metric::second ? std::chrono::duration_cast<std::chrono::seconds>(_tp_current - _tp_start).count() : std::chrono::duration_cast<std::chrono::milliseconds>(_tp_current - _tp_start).count();
        _time_by_session["[" + session_name + "]"] = time_elapsed;
        _tp_start = _tp_current;
    }
    void start(){ _tp_start = Clock::now(); }


private:
    timePoint _tp_start, _tp_current;
    std::map<std::string, long> _time_by_session;
    std::ofstream out_file;
    std::ifstream in_file;
    const std::string file_name;
    const Metric metric;
};

#endif //HELLO_TIMER_H
