#pragma once

#include <chrono>
#include <string>

namespace Engine
{

class InstrumentationTimer
{
public:
    InstrumentationTimer(const std::string &name);
    ~InstrumentationTimer();

private:
    std::string m_Name;
    std::chrono::time_point<std::chrono::high_resolution_clock> m_StartTime;
};

#define PROFILE_SCOPE(name) Engine::InstrumentationTimer timer##__LINE__(name);

#ifdef _MSC_VER
#define FUNCTION_SIGNATURE __FUNCSIG__
#else
#define FUNCTION_SIGNATURE __PRETTY_FUNCTION__
#endif
#define PROFILE_FUNCTION() PROFILE_SCOPE(FUNCTION_SIGNATURE)

} // namespace Engine