#pragma once

#include <Core.hpp>

namespace Engine
{

class WindowUtility
{
public:
    static float GetDPIScale();
    static std::pair<int, int> GetScreenResolution();
    static int GetScreenResolutionScale();
};

} // namespace Engine
