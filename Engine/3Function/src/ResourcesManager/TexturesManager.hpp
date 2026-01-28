#pragma once

#include "ResourcesRoot.hpp"

namespace Engine
{

class TexturesManager
{
public:
    static TexturesManager &GetInstance();

    // Getters
    int GetTextureSlot(const std::shared_ptr<Texture> &texture);
    std::string GetName(const std::shared_ptr<Texture> &texture) const;
    std::shared_ptr<Texture2D> GetTexture2D(const std::string &name);
    std::shared_ptr<TextureCube> GetTextureCube(const std::string &name);

    // Setters
    void AddTexture(const std::string &name, const std::shared_ptr<Texture> &texture);
    void ClearTextureSlots() { m_TexturesSlot.fill(""); }

private:
    std::array<std::string, 16> m_TexturesSlot = {""};
    std::unordered_map<std::string, std::shared_ptr<Texture>> m_TexturesMap;

private:
    TexturesManager();
    ~TexturesManager() = default;

    bool Exists(const std::string &name) const { return m_TexturesMap.find(name) != m_TexturesMap.end(); }
};

} // namespace Engine