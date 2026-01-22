#pragma once

#include "../../ECS/Entity/Entity.hpp"
#include "../Collider/BoundingBox.hpp"

namespace Engine
{

class Ray
{
public:
    Ray(const glm::vec3 &origin, const glm::vec3 &direction);

public:
    glm::vec3 At(float t) const { return m_Origin + t * m_Direction; }
    bool Hit(const BoundingBox &bbox, float tMin, float tMax) const;
    bool HitRenderer(const Entity &entity, float tMin, float tMax) const;
    // bool HitCollider(const Entity &entity, float tMin, float tMax) const;

private:
    glm::vec3 m_Origin;
    glm::vec3 m_Direction;
};

} // namespace Engine