#include "LightComponent.hpp"

MCEngine::LightComponent::LightComponent(LightType type, const glm::vec3 &color, float intensity, float constant,
                                         float linear, float quadratic, float innerAngle, float outerAngle)
    : m_Type(type), m_Color(color), m_Intensity(intensity), m_Constant(constant), m_Linear(linear),
      m_Quadratic(quadratic), m_InnerAngle(innerAngle), m_OuterAngle(outerAngle)
{
    LOG_ENGINE_TRACE("Light Component created with Color: " + Math::ToString(m_Color) +
                     ", Intensity: " + std::to_string(m_Intensity) + ", Constant: " + std::to_string(m_Constant) +
                     ", Linear: " + std::to_string(m_Linear) + ", Quadratic: " + std::to_string(m_Quadratic) +
                     ", InnerAngle: " + std::to_string(m_InnerAngle) + ", OuterAngle: " + std::to_string(m_OuterAngle));
}

void MCEngine::LightComponent::SetAttenuation(float constant, float linear, float quadratic)
{
    if (m_Type == LightType::Directional)
    {
        LOG_ENGINE_WARN("Trying to set attenuation on a directional light");
        return;
    }
    m_Constant = constant;
    m_Linear = linear;
    m_Quadratic = quadratic;
}

void MCEngine::LightComponent::SetAngles(float innerAngle, float outerAngle)
{
    if (m_Type != LightType::Spot)
    {
        LOG_ENGINE_WARN("Trying to set angles on a non-spot light");
        return;
    }
    m_InnerAngle = innerAngle;
    m_OuterAngle = outerAngle;
}