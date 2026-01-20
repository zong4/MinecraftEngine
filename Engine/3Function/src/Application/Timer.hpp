#pragma once

#include <Platform.hpp>

namespace Engine
{

class Timer
{
public:
    Timer() { Start(); }

    // Getters
    float GetElapsedSeconds() { return GetElapsedTime(); }
    float GetElapsedMilliseconds() { return GetElapsedTime() * 1000.0f; }

protected:
    std::chrono::time_point<std::chrono::high_resolution_clock> m_StartTime;

private:
    float GetElapsedTime() // Called once per frame
    {
        float elapsedTime = std::chrono::duration<float, std::chrono::seconds::period>(
                                std::chrono::high_resolution_clock::now() - m_StartTime)
                                .count();
        Start(); // Restart the timer for the next measurement
        return elapsedTime;
    }
    void Start() { m_StartTime = std::chrono::high_resolution_clock::now(); }
};

} // namespace Engine
