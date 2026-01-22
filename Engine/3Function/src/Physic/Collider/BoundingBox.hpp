#pragma once

#include <Platform.hpp>

namespace Engine
{

class BoundingBox
{
public:
    BoundingBox() = default;
    BoundingBox(const glm::vec3 &min, const glm::vec3 &max) : m_Min(min), m_Max(max) {}

    // Getters
    const glm::vec3 &GetMin() const { return m_Min; }
    const glm::vec3 &GetMax() const { return m_Max; }
    int GetLongestAxis() const;
    glm::vec3 GetCenter() const { return (m_Min + m_Max) * 0.5f; }

public:
    BoundingBox Extend(const glm::vec3 &point) const;
    BoundingBox Extend(const BoundingBox &other) const;
    BoundingBox Transform(const glm::mat4 &matrix) const;

private:
    glm::vec3 m_Min;
    glm::vec3 m_Max;
};

} // namespace Engine