#pragma once

#include "Renderer/Texture/TextureLibrary.hpp"

namespace MCEngine
{

struct SkyboxComponent
{
    std::string TextureCubeName;

public:
    SkyboxComponent(const std::string textureCubeName = "DefaultCubeMap") : TextureCubeName(textureCubeName) {}

    // Getters
    std::shared_ptr<TextureCube> GetTextureCube() const
    {
        return MCEngine::TextureLibrary::GetInstance().GetTextureCube(TextureCubeName);
    }
};

} // namespace MCEngine