#pragma once

#include <Platform.hpp>

namespace Engine
{

class SoundsManager
{
public:
    static SoundsManager &GetInstance();

    // Getters
    const std::filesystem::path &GetSound(const std::string &name);

    // Setters
    void AddSound(const std::string &name, const std::filesystem::path &filepath);

private:
    std::unordered_map<std::string, std::filesystem::path> m_SoundsMap;

private:
    SoundsManager();
    ~SoundsManager() = default;

    bool Exists(const std::string &name) const { return m_SoundsMap.find(name) != m_SoundsMap.end(); };
};

} // namespace Engine
