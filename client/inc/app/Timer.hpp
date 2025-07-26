#pragma once

#include "Logger.hpp"

#include <chrono>

class Timer
{
public:
    Timer();
    ~Timer() = default;
    bool hasTimePassedMS(double milliseconds);
    void refresh();

private:
    std::chrono::time_point<std::chrono::system_clock> m_startTime;
    std::chrono::time_point<std::chrono::system_clock> m_endTime;
};