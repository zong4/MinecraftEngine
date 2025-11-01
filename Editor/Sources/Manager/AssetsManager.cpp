#include "AssetsManager.hpp"

#include "Manager/ConfigManager.hpp"

MCEditor::AssetsManager &MCEditor::AssetsManager::GetInstance()
{
    static AssetsManager instance;
    return instance;
}

void MCEditor::AssetsManager::AddAsset(const std::filesystem::path &filepath)
{
    ENGINE_PROFILE_FUNCTION();

    std::string relativePath =
        std::filesystem::relative(filepath, ConfigManager::GetInstance().GetAssetsPath()).string();

    if (ConfigManager::IsTexture(filepath))
    {
        MCEngine::TextureLibrary::GetInstance().AddTexture(relativePath,
                                                           std::make_shared<MCEngine::Texture2D>(filepath.string()));
    }
}

MCEditor::AssetsManager::AssetsManager()
{
    ENGINE_PROFILE_FUNCTION();

    std::filesystem::path path(ConfigManager::GetInstance().GetAssetsPath());
    if (!std::filesystem::exists(path))
    {
        LOG_ENGINE_ASSERT("Font directory does not exist: " + path.string());
        return;
    }

    for (const auto &entry : std::filesystem::recursive_directory_iterator(path))
    {
        if (entry.is_regular_file())
        {
            AddAsset(entry.path());
        }
    }
}
