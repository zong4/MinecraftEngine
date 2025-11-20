#pragma once

#include "pch.hpp"

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
    const glm::vec4 &GetColor() const { return m_Color; }
    int GetLongestAxis() const;
    glm::vec3 GetCenter() const { return (m_Min + m_Max) * 0.5f; }

    // Setters
    void SetColor(const glm::vec4 &color) { m_Color = color; }

public:
    BoundingBox Transform(const glm::mat4 &matrix) const;
    void Merge(const BoundingBox &other);

private:
    glm::vec3 m_Min;
    glm::vec3 m_Max;
    glm::vec4 m_Color;
};

} // namespace Engine