#pragma once

#include "../Component/RelationshipComponent.hpp"
#include "../Component/TransformComponent.hpp"
#include <entt/entt.hpp>

namespace Engine
{

class TransformSystem
{
public:
    TransformSystem() = default;
    ~TransformSystem() = default;

public:
    void Update(entt::registry &registry)
    {
        PROFILE_FUNCTION();

        auto &&view = registry.view<TransformComponent, RelationshipComponent>();
        for (auto &&entity : view)
        {
            auto &&[transform, relationship] = view.get<TransformComponent, RelationshipComponent>(entity);
            if (!relationship.Parent)
            {
                UpdateTransform(&transform, &relationship, glm::mat4(1.0f), glm::quat(1.0f, 0.0f, 0.0f, 0.0f));
            }
        }
    }

private:
    void UpdateTransform(TransformComponent *transform, RelationshipComponent *relationship,
                         const glm::mat4 &parentTransformMatrix, const glm::quat &parentRotationQuat)
    {
        // Update the transform component
        transform->UpdateTransformMatrix(parentTransformMatrix, parentRotationQuat);

        // Update children's transformation matrices
        for (auto &&child : relationship->GetChildren())
        {
            UpdateTransform(child.GetComponent<TransformComponent>(), child.GetComponent<RelationshipComponent>(),
                            transform->GetTransformMatrix(), transform->GetRotationQuat(TransformSpace::Global));
        }
    }
};

} // namespace Engine