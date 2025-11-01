#pragma once

#include "pch.hpp"

#include "Renderer/Material/Material.hpp"

namespace MCEngine
{

struct SpriteRendererComponent
{
    std::shared_ptr<Texture2D> TextureInstance;
    glm::vec4 Color;

public:
    SpriteRendererComponent(
        const std::shared_ptr<Texture2D> &texture = TextureLibrary::GetInstance().GetTexture2D("White"),
        const glm::vec4 &color = glm::vec4(1.0f))
        : TextureInstance(texture), Color(color)
    {
    }
};

struct MeshRendererComponent
{
    Material MaterialInstance;

public:
    MeshRendererComponent(const Material &material = Material(glm::vec4(1.0f), 0.3f, 1.0f, 0.5f, 32.0f))
        : MaterialInstance(material)
    {
    }
};

} // namespace MCEngine