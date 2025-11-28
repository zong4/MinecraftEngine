#pragma once

#include "../../Physics/BoundingBox.hpp"
#include "../../Renderers/Librarys/TextureLibrary.hpp"
#include "../../Renderers/Librarys/VertexLibrary.hpp"

namespace Engine
{

struct SpriteRendererComponent
{
    glm::vec4 Color;
    std::shared_ptr<Texture2D> Texture;
    BoundingBox WorldBBox;

public:
    SpriteRendererComponent(
        const glm::vec4 &color = glm::vec4(1.0f),
        const std::shared_ptr<Texture2D> &texture = TextureLibrary::GetInstance().GetTexture2D("DefaultTexture"))
        : Color(color), Texture(texture)
    {
        BBox = BoundingBox(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f, 1.0f, 0.0f));
        WorldBBox = BBox;
    }

    // Getters
    const BoundingBox &GetBBox() const { return BBox; }

private:
    std::vector<glm::vec3> Vertices;
    std::vector<uint32_t> Indices;
    BoundingBox BBox;
};

struct MeshRendererComponent
{
    BoundingBox WorldBBox;

public:
    MeshRendererComponent(std::span<const glm::vec3> vertices = std::span(Engine::g_CubeData.Positions),
                          std::span<const uint32_t> indices = {})
        : Vertices(vertices.begin(), vertices.end()), Indices(indices.begin(), indices.end())
    {
        for (const auto &vertex : vertices)
            BBox = BBox.Extend(vertex);
        WorldBBox = BBox;
    }

    // Getters
    const BoundingBox &GetBBox() const { return BBox; }

private:
    std::vector<glm::vec3> Vertices;
    std::vector<uint32_t> Indices;
    BoundingBox BBox;
};

} // namespace Engine
