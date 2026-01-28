#pragma once

#include "ResourcesRoot.hpp"

namespace Engine
{

class AudiosManager
{
public:
    static AudiosManager &GetInstance();

    // Getters
    const std::filesystem::path &GetAudio(const std::string &name);

    // Setters
    void AddAudio(const std::string &name, const std::filesystem::path &filepath);

private:
    std::unordered_map<std::string, std::filesystem::path> m_AudiosMap;

private:
    AudiosManager();
    ~AudiosManager() = default;

    bool Exists(const std::string &name) const { return m_AudiosMap.find(name) != m_AudiosMap.end(); };
};

} // namespace Engine
