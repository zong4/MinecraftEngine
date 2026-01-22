#pragma once

#include "../Component/RendererComponents.hpp"
#include "../Component/TransformComponent.hpp"
#include <entt/entt.hpp>

namespace Engine
{

class RendererSystem
{
public:
    static RendererSystem &GetInstance()
    {
        static RendererSystem instance;
        return instance;
    }

public:
    void Update(entt::registry &registry)
    {
        PROFILE_FUNCTION();

        // Sprites
        auto &&spriteView = registry.view<TransformComponent, SpriteRendererComponent>();
        for (auto &&entity : spriteView)
        {
            auto &&[transform, spriteRenderer] = spriteView.get<TransformComponent, SpriteRendererComponent>(entity);
            spriteRenderer.WorldBBox = spriteRenderer.GetBBox().Transform(transform.GetTransformMatrix());
        }

        // Meshes
        auto &&meshView = registry.view<TransformComponent, MeshRendererComponent>();
        for (auto &&entity : meshView)
        {
            auto &&[transform, meshRenderer] = meshView.get<TransformComponent, MeshRendererComponent>(entity);
            meshRenderer.WorldBBox = meshRenderer.GetBBox().Transform(transform.GetTransformMatrix());
        }
    }

private:
    RendererSystem() = default;
    ~RendererSystem() = default;
};

} // namespace Engine