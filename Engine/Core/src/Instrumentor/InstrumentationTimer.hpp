#pragma once

#include "pch.hpp"

namespace MCEngine
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

#define ENGINE_PROFILE_SCOPE(name) MCEngine::InstrumentationTimer timer##__LINE__(name);

#ifdef _MSC_VER
#define FUNCTION_SIGNATURE __FUNCSIG__
#else
#define FUNCTION_SIGNATURE __PRETTY_FUNCTION__
#endif
#define ENGINE_PROFILE_FUNCTION() ENGINE_PROFILE_SCOPE(FUNCTION_SIGNATURE)

} // namespace MCEngine