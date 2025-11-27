#pragma once

#include <Platform.hpp>

namespace Engine
{

class Ray
{
public:
    Ray(const glm::vec3 &origin, const glm::vec3 &direction) : m_Origin(origin), m_Direction(glm::normalize(direction))
    {
    }

public:
    glm::vec3 At(float t) const { return m_Origin + t * m_Direction; }

private:
    glm::vec3 m_Origin;
    glm::vec3 m_Direction;
};

} // namespace Engine