#pragma once

#include <Platform.hpp>

namespace Engine
{

static std::filesystem::path GetLogsRoot()
{
#ifdef DEBUG
    return std::filesystem::path(std::string(PROJECT_ROOT) + "/logs/");
#else
    return std::filesystem::current_path() / "../logs/";
#endif
}

static std::filesystem::path GetConfigsRoot()
{
#ifdef DEBUG
    return std::filesystem::path(std::string(PROJECT_ROOT) + "Editor/configs/");
#else
    return GetProjectRoot() / "../Configs/";
#endif
}

static std::filesystem::path GetResourcesRoot()
{
#ifdef DEBUG
    return std::filesystem::path(std::string(PROJECT_ROOT) + "Editor/resources/");
#else
    return GetProjectRoot() / "../Resources/";
#endif
}

static std::filesystem::path GetAssetsRoot()
{
#ifdef DEBUG
    return std::filesystem::path(std::string(PROJECT_ROOT) + "Editor/assets/");
#else
    return GetProjectRoot() / "../Assets/";
#endif
}

} // namespace Engine