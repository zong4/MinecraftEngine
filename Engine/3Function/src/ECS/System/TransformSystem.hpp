#pragma once

#include "../Component/RelationshipComponent.hpp"
#include "../Component/TransformComponent.hpp"
#include <entt/entt.hpp>

namespace Engine
{

class TransformSystem
{
public:
    static TransformSystem &GetInstance();

public:
    void Update(entt::registry &registry);

private:
    TransformSystem() = default;
    ~TransformSystem() = default;

private:
    void UpdateTransform(TransformComponent *transform, RelationshipComponent *relationship,
                         const glm::mat4 &parentTransformMatrix, const glm::quat &parentRotationQuat);
};

} // namespace Engine