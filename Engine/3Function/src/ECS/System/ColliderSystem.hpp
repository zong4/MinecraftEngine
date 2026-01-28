#pragma once

#include "../../Physic/Collider/BVH.hpp"
#include "../Component/RendererComponents.hpp"
#include "../Component/TransformComponent.hpp"
#include <entt/entt.hpp>

namespace Engine
{

class ColliderSystem
{
public:
    ColliderSystem() = default;
    ~ColliderSystem() = default;

public:
    void Update(entt::registry &registry);
    void RenderBVH(entt::registry &registry, int maxDepth) const;

private:
    std::shared_ptr<BVH> m_BVH = nullptr;
};

} // namespace Engine