#include "TexturesManager.hpp"

Engine::TexturesManager &Engine::TexturesManager::GetInstance()
{
    static TexturesManager instance;
    return instance;
}

int Engine::TexturesManager::GetTextureSlot(const std::shared_ptr<Texture> &texture)
{
    // Check if texture is already bound to a slot
    for (size_t i = 0; i < m_TexturesSlot.size(); i++)
    {
        if (m_TexturesSlot[i] == GetName(texture))
            return static_cast<int>(i);
    }

    // Find an empty slot
    for (size_t i = 0; i < m_TexturesSlot.size(); i++)
    {
        if (m_TexturesSlot[i].empty())
        {
            m_TexturesSlot[i] = GetName(texture);
            return static_cast<int>(i);
        }
    }

    // No available slots
    LOG_ENGINE_ERROR("No available texture slots");
    return -1;
}

std::string Engine::TexturesManager::GetName(const std::shared_ptr<Texture> &texture) const
{
    for (const auto &[name, ptr] : m_TexturesMap)
    {
        if (ptr == texture)
            return name;
    }
    LOG_ENGINE_ERROR("Texture not found in library");
    return "";
}

std::shared_ptr<Engine::Texture2D> Engine::TexturesManager::GetTexture2D(const std::string &name)
{
    if (!Exists(name))
    {
        LOG_ENGINE_ERROR("Texture not found: " + name);
        return nullptr;
    }
    return std::dynamic_pointer_cast<Texture2D>(m_TexturesMap[name]);
}

std::shared_ptr<Engine::TextureCube> Engine::TexturesManager::GetTextureCube(const std::string &name)
{
    if (!Exists(name))
    {
        LOG_ENGINE_ERROR("Texture not found: " + name);
        return nullptr;
    }
    return std::dynamic_pointer_cast<TextureCube>(m_TexturesMap[name]);
}

void Engine::TexturesManager::AddTexture(const std::string &name, const std::shared_ptr<Texture> &texture)
{
    // Validate texture
    if (!texture)
    {
        LOG_ENGINE_ERROR("Cannot add null texture: " + name);
        return;
    }

    // Check if texture already exists
    if (Exists(name))
    {
        LOG_ENGINE_WARN("Texture already exists: " + name + ", overwriting");
    }

    // Add texture to map
    m_TexturesMap[name] = texture;
    LOG_ENGINE_TRACE("Texture added: " + name);
}

Engine::TexturesManager::TexturesManager()
{
    PROFILE_FUNCTION();

    // Load default textures
    AddTexture("DefaultTexture", Texture2D::WhiteTexture());
    AddTexture("DefaultCubeMap", TextureCube::WhiteTexture());

    // Chick Textures directory path
    std::filesystem::path path = GetResourcesDirectory() / "Textures/";
    if (!std::filesystem::exists(path))
    {
        LOG_ENGINE_ERROR("Texture directory does not exist: " + path.string());
        return;
    }

    // Load all textures
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
    LOG_ENGINE_INFO("TexturesManager initialized");
}
