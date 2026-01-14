#include "InstrumentationTimer.hpp"

#include "Instrumentor.hpp"

Engine::InstrumentationTimer::InstrumentationTimer(const std::string &name)
    : m_Name(name), m_StartTime(std::chrono::high_resolution_clock::now())
{
}

Engine::InstrumentationTimer::~InstrumentationTimer()
{
    // Capture end timepoint
    auto &&endTimepoint = std::chrono::high_resolution_clock::now();
    long long startTime =
        std::chrono::time_point_cast<std::chrono::microseconds>(m_StartTime).time_since_epoch().count();
    long long endTime =
        std::chrono::time_point_cast<std::chrono::microseconds>(endTimepoint).time_since_epoch().count();

    // Get thread ID
    uint32_t threadID = std::hash<std::thread::id>{}(std::this_thread::get_id());

    // Write profile result to Instrumentor
    Instrumentor::GetInstance().WriteProfile({m_Name, startTime, endTime, threadID});
}