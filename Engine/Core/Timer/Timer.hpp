#pragma once

#include "pch.hpp"

namespace MCEngine
{

class Timer
{
public:
    Timer() { Start(); }

    float GetElapsedSeconds() { return GetElapsedTime(); }
    float GetElapsedMilliseconds() { return GetElapsedTime() * 1000.0f; }

protected:
    std::chrono::time_point<std::chrono::high_resolution_clock> m_StartTime;

private:
    void Start() { m_StartTime = std::chrono::high_resolution_clock::now(); }

    float GetElapsedTime();
};

} // namespace MCEngine
