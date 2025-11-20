#pragma once

#include "pch.hpp"

namespace Engine
{

enum class LightType
{
    Directional,
    Point,
    Spot
};

struct LightComponent
{
    glm::vec3 Color;
    float Intensity;
    float Constant, Linear, Quadratic;
    float InnerAngle, OuterAngle;

public:
    LightComponent(LightType type, const glm::vec3 &color = glm::vec3(1.0f), float intensity = 1.0f,
                   float constant = 1.0f, float linear = 0.09f, float quadratic = 0.032f, float innerAngle = 12.5f,
                   float outerAngle = 15.0f)
        : m_Type(type), Color(color), Intensity(intensity), Constant(constant), Linear(linear), Quadratic(quadratic),
          InnerAngle(innerAngle), OuterAngle(outerAngle)
    {
    }

    // Getters
    LightType GetType() const { return m_Type; }

private:
    LightType m_Type;
};

} // namespace Engine