#include "ConfigManager.hpp"

#include <nlohmann/json.hpp>

// Set default window properties
MCEngine::WindowProperty MCEditor::ConfigManager::s_WindowProperty =
    MCEngine::WindowProperty("Minecraft Engine", 1280, 720, true, glm::vec4(0.1f, 0.1f, 0.1f, 1.0f));

// Set default paths
std::filesystem::path MCEditor::ConfigManager::s_ConfigsPath = "Editor/Configs/";
std::filesystem::path MCEditor::ConfigManager::s_AssetsPath = "Editor/Assets/";
std::filesystem::path MCEditor::ConfigManager::s_ScenesPath = "Editor/Assets/Scenes/";
std::filesystem::path MCEditor::ConfigManager::s_IconsPath = "Editor/Resources/Icons/";

// Set default extensions
std::string MCEditor::ConfigManager::s_SceneExtension = ".mcscene";
std::set<std::string> MCEditor::ConfigManager::s_TextureExtensions = {".png", ".jpg"};

MCEditor::ConfigManager &MCEditor::ConfigManager::GetInstance()
{
    static ConfigManager instance;
    return instance;
}

bool MCEditor::ConfigManager::IsScene(std::filesystem::path path) { return path.extension() == s_SceneExtension; }

bool MCEditor::ConfigManager::IsTexture(std::filesystem::path path)
{
    return IsExtensionInSet(path.extension().string(), s_TextureExtensions);
}

MCEditor::ConfigManager::ConfigManager()
{
    ENGINE_PROFILE_FUNCTION();

    // Prepend PROJECT_ROOT to make paths absolute
    s_ConfigsPath = std::filesystem::path(PROJECT_ROOT) / s_ConfigsPath;
    s_AssetsPath = std::filesystem::path(PROJECT_ROOT) / s_AssetsPath;
    s_ScenesPath = std::filesystem::path(PROJECT_ROOT) / s_ScenesPath;
    s_IconsPath = std::filesystem::path(PROJECT_ROOT) / s_IconsPath;
}

bool MCEditor::ConfigManager::IsExtensionInSet(const std::string &extension, const std::set<std::string> &extensionSet)
{
    return extensionSet.find(extension) != extensionSet.end();
}