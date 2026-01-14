#include "TextureLibrary.hpp"

Engine::TextureLibrary &Engine::TextureLibrary::GetInstance()
{
    static TextureLibrary instance;
    return instance;
}

int Engine::TextureLibrary::GetTextureSlot(const std::shared_ptr<Texture> &texture)
{
    for (size_t i = 0; i < m_TextureSlots.size(); i++)
    {
        if (m_TextureSlots[i] == GetName(texture))
            return static_cast<int>(i);
    }

    // Find an empty slot
    for (size_t i = 0; i < m_TextureSlots.size(); i++)
    {
        if (m_TextureSlots[i].empty())
        {
            m_TextureSlots[i] = GetName(texture);
            return static_cast<int>(i);
        }
    }

    LOG_ENGINE_ERROR("No available texture slots");
    return -1;
}

std::string Engine::TextureLibrary::GetName(const std::shared_ptr<Texture> &texture) const
{
    for (const auto &[name, ptr] : m_TextureMap)
    {
        if (ptr == texture)
            return name;
    }
    LOG_ENGINE_ERROR("Texture not found in library");
    return "";
}

std::shared_ptr<Engine::Texture2D> Engine::TextureLibrary::GetTexture2D(const std::string &name)
{
    if (!Exists(name))
    {
        LOG_ENGINE_ERROR("Texture not found: " + name);
        return nullptr;
    }
    return std::dynamic_pointer_cast<Texture2D>(m_TextureMap[name]);
}

std::shared_ptr<Engine::TextureCube> Engine::TextureLibrary::GetTextureCube(const std::string &name)
{
    if (!Exists(name))
    {
        LOG_ENGINE_ERROR("Texture not found: " + name);
        return nullptr;
    }
    return std::dynamic_pointer_cast<TextureCube>(m_TextureMap[name]);
}

void Engine::TextureLibrary::AddTexture(const std::string &name, const std::shared_ptr<Texture> &texture)
{
    if (!texture)
    {
        LOG_ENGINE_ASSERT("Cannot add null texture: " + name);
        return;
    }

    if (Exists(name))
    {
        LOG_ENGINE_WARN("Texture already exists: " + name);
        return;
    }
    m_TextureMap[name] = texture;
    LOG_ENGINE_TRACE("Texture added: " + name);
}

Engine::TextureLibrary::TextureLibrary()
{
    PROFILE_FUNCTION();

    AddTexture("DefaultTexture", Texture2D::WhiteTexture());
    AddTexture("DefaultCubeMap", TextureCube::WhiteTexture());

    std::filesystem::path path(std::string(PROJECT_ROOT) + "/Engine/Function/resources/Images/");
    if (!std::filesystem::exists(path))
    {
        LOG_ENGINE_ASSERT("Texture directory does not exist: " + path.string());
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

            // Try loading as a cubemap directory
            auto &&right = findFile(entry.path(), "right");
            if (right != "")
            {
                auto &&left = findFile(entry.path(), "left");
                auto &&top = findFile(entry.path(), "top");
                auto &&bottom = findFile(entry.path(), "bottom");
                auto &&front = findFile(entry.path(), "front");
                auto &&back = findFile(entry.path(), "back");
                AddTexture(entry.path().stem().string(),
                           TextureCube::Create(std::array<std::string, 6>{right, left, top, bottom, front, back}));
            }
        }
        else if (entry.is_regular_file())
        {
            // Support LDR textures (.png, .jpg) and HDR textures (.hdr)
            if (entry.path().extension() == ".png" || entry.path().extension() == ".jpg" ||
                entry.path().extension() == ".hdr")
            {
                AddTexture(entry.path().stem().string(), Texture2D::Create(entry.path().string()));
            }
        }
    }

    LOG_ENGINE_INFO("Texture Library initialized");
}
