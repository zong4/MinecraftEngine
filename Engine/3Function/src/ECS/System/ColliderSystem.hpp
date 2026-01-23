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
    static ColliderSystem &GetInstance();

public:
    void Update(entt::registry &registry);
    void RenderBVH(entt::registry &registry, int maxDepth) const;

private:
    std::shared_ptr<BVH> m_BVH = nullptr;

private:
    ColliderSystem() = default;
    ~ColliderSystem() = default;
};

} // namespace Engine