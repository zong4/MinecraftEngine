#pragma once

#include <Core.hpp>

namespace Engine
{

std::filesystem::path GetExeDirectory();
std::filesystem::path GetLogsDirectory();
std::filesystem::path GetConfigsDirectory();
std::filesystem::path GetResourcesDirectory();
std::filesystem::path GetAssetsDirectory();

} // namespace Engine