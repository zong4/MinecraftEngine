#include "Timer.hpp"

float MCEngine::Timer::GetElapsedTime()
{
    float elapsedTime = std::chrono::duration<float, std::chrono::seconds::period>(
                            std::chrono::high_resolution_clock::now() - m_StartTime)
                            .count();

    // Restart the timer for the next frame
    Start();

    return elapsedTime;
}