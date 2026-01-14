#include "Timer.hpp"

float Engine::Timer::GetElapsedTime()
{
    float elapsedTime = std::chrono::duration<float, std::chrono::seconds::period>(
                            std::chrono::high_resolution_clock::now() - m_StartTime)
                            .count();
    Start(); // Restart the timer for the next measurement
    return elapsedTime;
}