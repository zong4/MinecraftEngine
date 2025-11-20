#include "BoundingBox.hpp"

int Engine::BoundingBox::GetLongestAxis() const
{
    glm::vec3 extents = m_Max - m_Min;
    if (extents.x >= extents.y && extents.x >= extents.z)
        return 0; // Axis::X
    else if (extents.y >= extents.x && extents.y >= extents.z)
        return 1; // Axis::Y
    else
        return 2; // Axis::Z
}

Engine::BoundingBox Engine::BoundingBox::Transform(const glm::mat4 &matrix) const
{
    glm::vec3 corners[8] = {
        glm::vec3(m_Min.x, m_Min.y, m_Min.z), glm::vec3(m_Max.x, m_Min.y, m_Min.z),
        glm::vec3(m_Min.x, m_Max.y, m_Min.z), glm::vec3(m_Max.x, m_Max.y, m_Min.z),
        glm::vec3(m_Min.x, m_Min.y, m_Max.z), glm::vec3(m_Max.x, m_Min.y, m_Max.z),
        glm::vec3(m_Min.x, m_Max.y, m_Max.z), glm::vec3(m_Max.x, m_Max.y, m_Max.z),
    };

    glm::vec3 newMin(FLT_MAX), newMax(-FLT_MAX);
    for (const auto &corner : corners)
    {
        glm::vec4 transformed = matrix * glm::vec4(corner, 1.0f);
        newMin = glm::min(newMin, glm::vec3(transformed));
        newMax = glm::max(newMax, glm::vec3(transformed));
    }

    return BoundingBox(newMin, newMax);
}

void Engine::BoundingBox::Merge(const BoundingBox &other)
{
    m_Min = glm::min(m_Min, other.m_Min);
    m_Max = glm::max(m_Max, other.m_Max);
}