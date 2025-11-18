#pragma once

#include <Function.hpp>

namespace MCEditor
{

class ConfigManager
{
public:
    static ConfigManager &GetInstance();

    static bool IsScene(std::filesystem::path path);
    static bool IsTexture(std::filesystem::path path);

    const MCEngine::WindowProperty &GetWindowProperty() const { return s_WindowProperty; }
    static std::filesystem::path GetConfigsPath() { return s_ConfigsPath; }
    static std::filesystem::path GetAssetsPath() { return s_AssetsPath; }
    static std::filesystem::path GetScenesPath() { return s_ScenesPath; }
    static std::filesystem::path GetIconsPath() { return s_IconsPath; }

private:
    static MCEngine::WindowProperty s_WindowProperty;

    static std::filesystem::path s_ConfigsPath;
    static std::filesystem::path s_AssetsPath;
    static std::filesystem::path s_ScenesPath;
    static std::filesystem::path s_IconsPath;

    static std::string s_SceneExtension;
    static std::set<std::string> s_TextureExtensions;

private:
    ConfigManager();

    static bool IsExtensionInSet(const std::string &extension, const std::set<std::string> &extensionSet);
};

} // namespace MCEditor