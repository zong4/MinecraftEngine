#pragma once

#include "../../Physics/BoundingBox.hpp"

namespace Engine
{

struct SpriteRendererComponent
{
    std::shared_ptr<Texture2D> TextureInstance;
    glm::vec4 Color;

public:
    SpriteRendererComponent(
        const std::shared_ptr<Texture2D> &texture = TextureLibrary::GetInstance().GetTexture2D("DefaultTexture"),
        const glm::vec4 &color = glm::vec4(1.0f))
        : TextureInstance(texture), Color(color)
    {
    }
};

struct MeshRendererComponent
{
    BoundingBox BBox;
    BoundingBox WorldBBox;

public:
    MeshRendererComponent()
    {
        BBox = BoundingBox(glm::vec3(-0.5f), glm::vec3(0.5f));
        WorldBBox = BBox;
    }
};

// todo: future use for ColliderComponent
struct ColliderComponent
{
    // BoundingBox BBox;
    // BoundingBox WorldBBox;
    // bool IsTrigger = false;

    // Extension of BoundingBox
    // ColliderType Type;
};

} // namespace Engine
