#include "Ray.hpp"

#include "../../ECS/Component/RendererComponents.hpp"

Engine::Ray::Ray(const glm::vec3 &origin, const glm::vec3 &direction)
    : m_Origin(origin), m_Direction(glm::normalize(direction))
{
}

bool Engine::Ray::Hit(const BoundingBox &bbox, float tMin, float tMax) const
{
    for (int a = 0; a < 3; a++)
    {
        // Ray is parallel to slab
        if (fabs(m_Direction[a]) < 1e-8f)
        {
            if (m_Origin[a] < bbox.GetMin()[a] || m_Origin[a] > bbox.GetMax()[a])
                return false;
            else
                continue;
        }

        float invD = 1.0f / m_Direction[a];
        float t0 = (bbox.GetMin()[a] - m_Origin[a]) * invD;
        float t1 = (bbox.GetMax()[a] - m_Origin[a]) * invD;
        if (invD < 0.0f)
            std::swap(t0, t1);
        tMin = t0 > tMin ? t0 : tMin;
        tMax = t1 < tMax ? t1 : tMax;
        if (tMax <= tMin)
            return false;
    }
    return true;
}

bool Engine::Ray::Hit(const Entity &entity, float tMin, float tMax) const
{
    auto &&meshRenderer = entity.GetComponent<MeshRendererComponent>();
    if (!meshRenderer)
        return false;
    return Hit(meshRenderer->WorldBBox, tMin, tMax);
}