#pragma once

#include "../src/Renderer/Texture/TextureLibrary.hpp"

namespace Engine
{

struct SkyboxComponent
{
    std::string TextureCubeName;

public:
    SkyboxComponent(const std::string textureCubeName = "DefaultCubeMap") : TextureCubeName(textureCubeName) {}

    // Getters
    std::shared_ptr<TextureCube> GetTextureCube() const
    {
        return Engine::TextureLibrary::GetInstance().GetTextureCube(TextureCubeName);
    }
};

} // namespace Engine