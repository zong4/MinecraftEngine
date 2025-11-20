#pragma once

#include "pch.hpp"

namespace MCEngine
{

class WindowUtility
{
public:
    static float GetDPIScale();
    static std::pair<int, int> GetScreenResolution();
    static int GetScreenResolutionScale();
};

} // namespace MCEngine
