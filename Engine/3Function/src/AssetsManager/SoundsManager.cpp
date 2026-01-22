#include "SoundsManager.hpp"

Engine::SoundsManager &Engine::SoundsManager::GetInstance()
{
    static SoundsManager instance;
    return instance;
}

const std::filesystem::path &Engine::SoundsManager::GetSound(const std::string &name)
{
    if (!Exists(name))
    {
        LOG_ENGINE_ERROR("Sound not found: " + name);
        static std::filesystem::path emptyPath;
        return emptyPath;
    }
    return m_SoundsMap[name];
}

void Engine::SoundsManager::AddSound(const std::string &name, const std::filesystem::path &filepath)
{
    if (filepath.empty())
    {
        LOG_ENGINE_ERROR("Cannot add null sound: " + name);
        return;
    }

    if (Exists(name))
    {
        LOG_ENGINE_WARN("Sound already exists: " + name + ", overwriting");
    }

    // Add sound to map
    m_SoundsMap[name] = filepath;
    LOG_ENGINE_TRACE("Sound added: " + name);
}

Engine::SoundsManager::SoundsManager()
{
    PROFILE_FUNCTION();

    std::filesystem::path path(std::string(FUNCTION_ROOT) + "/resources/Sounds/");
    if (!std::filesystem::exists(path))
    {
        LOG_ENGINE_ERROR("Sound directory does not exist: " + path.string());
        return;
    }

    // Load all sounds
    for (auto &&entry : std::filesystem::recursive_directory_iterator(path))
    {
        if (entry.path().extension() == ".wav" || entry.path().extension() == ".mp3")
        {
            std::string soundName = entry.path().stem().string();
            AddSound(soundName, entry.path());
        }
    }

    LOG_ENGINE_INFO("SoundsManager initialized");
}