#include "Filesystem.hpp"

#if defined(_WIN32)
#include <windows.h>
#elif defined(__APPLE__)
#include <mach-o/dyld.h>
#elif defined(__linux__)
#include <unistd.h>
#endif

std::filesystem::path Engine::GetExeDirectory()
{
    std::filesystem::path exePath;
#if defined(_WIN32)
    char buffer[MAX_PATH];
    DWORD size = GetModuleFileNameA(NULL, buffer, MAX_PATH);
    if (size == 0 || size == MAX_PATH)
    {
        throw std::runtime_error("Failed to get executable path on Windows");
    }
    exePath = std::filesystem::path(buffer);
#elif defined(__APPLE__)
    uint32_t size = 0;
    _NSGetExecutablePath(nullptr, &size); // get required buffer size
    std::string buffer(size, '\0');
    if (_NSGetExecutablePath(buffer.data(), &size) != 0)
    {
        throw std::runtime_error("Failed to get executable path on macOS");
    }
    exePath = std::filesystem::canonical(buffer);
#elif defined(__linux__)
    char buffer[4096] = {0};
    ssize_t count = readlink("/proc/self/exe", buffer, sizeof(buffer) - 1);
    if (count == -1)
    {
        throw std::runtime_error("Failed to get executable path on Linux");
    }
    exePath = std::filesystem::canonical(std::filesystem::path(buffer));
#else
#error "Unsupported platform"
#endif
    return exePath.parent_path();
}

std::filesystem::path Engine::GetLogsDirectory()
{
#ifdef DEBUG
    return std::filesystem::path(std::string(PROJECT_ROOT) + "/logs/");
#else
    return GetExeDirectory() / "logs/";
#endif
}

std::filesystem::path Engine::GetConfigsDirectory()
{
#ifdef DEBUG
    return std::filesystem::path(std::string(PROJECT_ROOT) + "/Editor/configs/");
#else
    return GetExeDirectory() / "Configs/";
#endif
}

std::filesystem::path Engine::GetResourcesDirectory()
{
#ifdef DEBUG
    return std::filesystem::path(std::string(PROJECT_ROOT) + "/Editor/resources/");
#else
    return GetExeDirectory() / "Resources/";
#endif
}

std::filesystem::path Engine::GetAssetsDirectory()
{
#ifdef DEBUG
    return std::filesystem::path(std::string(PROJECT_ROOT) + "/Editor/assets/");
#else
    return GetExeDirectory() / "Assets/";
#endif
}
