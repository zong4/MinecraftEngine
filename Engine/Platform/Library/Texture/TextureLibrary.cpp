#include "TextureLibrary.hpp"

#include <glad/glad.h>

MCEngine::TextureLibrary &MCEngine::TextureLibrary::GetInstance()
{
    static TextureLibrary instance;
    return instance;
}

int MCEngine::TextureLibrary::GetTextureSlot(const std::shared_ptr<Texture> &texture)
{
    for (size_t i = 0; i < m_TextureSlots.size(); ++i)
    {
        if (m_TextureSlots[i] == GetName(texture))
        {
            return static_cast<int>(i);
        }
    }

    // Find an empty slot
    for (size_t i = 0; i < m_TextureSlots.size(); ++i)
    {
        if (m_TextureSlots[i].empty())
        {
            m_TextureSlots[i] = GetName(texture);
            return static_cast<int>(i);
        }
    }

    LOG_ENGINE_WARN("No available texture slots");
    return -1;
}

std::string MCEngine::TextureLibrary::GetName(const std::shared_ptr<Texture> &texture) const
{
    for (const auto &[name, ptr] : m_Textures)
    {
        if (ptr == texture)
            return name;
    }
    LOG_ENGINE_WARN("Texture not found in library");
    return "";
}

std::shared_ptr<MCEngine::Texture2D> MCEngine::TextureLibrary::GetTexture2D(const std::string &name)
{
    ENGINE_PROFILE_FUNCTION();

    if (!Exists(name))
    {
        LOG_ENGINE_ERROR("Texture not found: " + name);
        return nullptr;
    }
    return std::dynamic_pointer_cast<Texture2D>(m_Textures[name]);
}

std::shared_ptr<MCEngine::TextureCube> MCEngine::TextureLibrary::GetTextureCube(const std::string &name)
{
    ENGINE_PROFILE_FUNCTION();

    if (!Exists(name))
    {
        LOG_ENGINE_ERROR("Texture not found: " + name);
        return nullptr;
    }
    return std::dynamic_pointer_cast<TextureCube>(m_Textures[name]);
}

void MCEngine::TextureLibrary::AddTexture(const std::string &name, const std::shared_ptr<Texture> &texture)
{
    ENGINE_PROFILE_FUNCTION();

    if (Exists(name))
    {
        LOG_ENGINE_ERROR("Texture already exists: " + name);
        return;
    }
    m_Textures[name] = texture;
    LOG_ENGINE_TRACE("Texture added: " + name);
}

void MCEngine::TextureLibrary::ClearTextureSlots() { m_TextureSlots.fill(""); }

MCEngine::TextureLibrary::TextureLibrary()
{
    ENGINE_PROFILE_FUNCTION();

    AddTexture("White", std::make_shared<Texture2D>(1, 1, GL_RGBA8, GL_RGBA, GL_UNSIGNED_BYTE,
                                                    new unsigned char[4]{255, 255, 255, 255}));

    std::filesystem::path path(std::string(PROJECT_ROOT) + "/Engine/Resources/Images/");
    if (!std::filesystem::exists(path))
    {
        LOG_ENGINE_ERROR("Texture directory does not exist: " + path.string());
        return;
    }

    for (auto &&entry : std::filesystem::directory_iterator(path))
    {
        if (entry.is_directory())
        {
            auto findFile = [&](const std::filesystem::path &dir, const std::string &baseName) -> std::string {
                static const std::vector<std::string> exts = {".png", ".jpg"};
                for (auto &ext : exts)
                {
                    auto filePath = dir / (baseName + ext);
                    if (std::filesystem::exists(filePath))
                        return filePath.string();
                }
                return "";
            };

            auto &&right = findFile(entry.path(), "right");
            if (right != "")
            {
                auto &&left = findFile(entry.path(), "left");
                auto &&top = findFile(entry.path(), "top");
                auto &&bottom = findFile(entry.path(), "bottom");
                auto &&front = findFile(entry.path(), "front");
                auto &&back = findFile(entry.path(), "back");

                AddTexture(entry.path().stem().string(), std::make_shared<TextureCube>(std::array<std::string, 6>{
                                                             right, left, top, bottom, front, back}));
            }
        }
        else if (entry.is_regular_file())
        {
            if (entry.path().extension() == ".png" || entry.path().extension() == ".jpg")
            {
                AddTexture(entry.path().stem().string(), std::make_shared<Texture2D>(entry.path().string()));
            }
        }
    }

    LOG_ENGINE_INFO("Texture Library initialized");
}

bool MCEngine::TextureLibrary::Exists(const std::string &name) const
{
    return m_Textures.find(name) != m_Textures.end();
}
