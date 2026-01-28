#include "AudiosManager.hpp"

Engine::AudiosManager &Engine::AudiosManager::GetInstance()
{
    static AudiosManager instance;
    return instance;
}

const std::filesystem::path &Engine::AudiosManager::GetAudio(const std::string &name)
{
    if (!Exists(name))
    {
        LOG_ENGINE_ERROR("Audio not found: " + name);
        static std::filesystem::path emptyPath;
        return emptyPath;
    }
    return m_AudiosMap[name];
}

void Engine::AudiosManager::AddAudio(const std::string &name, const std::filesystem::path &filepath)
{
    // Validate filepath
    if (filepath.empty())
    {
        LOG_ENGINE_ERROR("Cannot add null Audio: " + name);
        return;
    }

    // Check if Audio already exists
    if (Exists(name))
    {
        LOG_ENGINE_WARN("Audio already exists: " + name + ", overwriting");
    }

    // Add Audio to map
    m_AudiosMap[name] = filepath;
    LOG_ENGINE_TRACE("Audio added: " + name);
}

Engine::AudiosManager::AudiosManager()
{
    PROFILE_FUNCTION();

    // Check Audios directory path
    std::filesystem::path path(std::string(EDITOR_ROOT) + "/resources/Audios/");
    if (!std::filesystem::exists(path))
    {
        LOG_ENGINE_ERROR("Audio directory does not exist: " + path.string());
        return;
    }

    // Load all Audios
    for (auto &&entry : std::filesystem::recursive_directory_iterator(path))
    {
        if (entry.path().extension() == ".wav" || entry.path().extension() == ".mp3")
        {
            std::string AudioName = entry.path().stem().string();
            AddAudio(AudioName, entry.path());
        }
    }
    LOG_ENGINE_INFO("AudiosManager initialized");
}