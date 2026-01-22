#pragma once

#include "../../Physic/Collider/BoundingBox.hpp"

namespace Engine
{

struct SpriteRendererComponent
{
    BoundingBox WorldBBox;

public:
    SpriteRendererComponent()
    {
        m_BBox = BoundingBox(glm::vec3(-0.5f, -0.5f, 0.0f), glm::vec3(0.5f, 0.5f, 0.0f));
        WorldBBox = m_BBox;
    }

    // Getters
    const BoundingBox &GetBBox() const { return m_BBox; }

private:
    BoundingBox m_BBox;
};

struct MeshRendererComponent
{
    BoundingBox WorldBBox;

public:
    MeshRendererComponent()
    {
        m_BBox = BoundingBox(glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec3(0.5f, 0.5f, 0.5f));
        WorldBBox = m_BBox;
    }

    // Getters
    const BoundingBox &GetBBox() const { return m_BBox; }

private:
    BoundingBox m_BBox;
};

} // namespace Engine
