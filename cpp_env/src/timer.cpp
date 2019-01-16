//
// Created by afterburner on 19-1-11.
//

#include "timer.h"
#include <iomanip>  // for operator<<, setw
#include <iostream> // for cout

Timer::~Timer()
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

void Timer::stop(std::string session_name)
{
    _tp_current = Clock::now();
    long time_elapsed = metric == Metric::second ? std::chrono::duration_cast<std::chrono::seconds>(_tp_current - _tp_start).count() : std::chrono::duration_cast<std::chrono::milliseconds>(_tp_current - _tp_start).count();
    _time_by_session["[" + session_name + "]"] = time_elapsed;
    _tp_start = _tp_current;
}

void Timer::start() { _tp_start = Clock::now(); }
