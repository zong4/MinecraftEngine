#include "LightComponent.hpp"

void MCEngine::LightComponent::SetAttenuation(float constant, float linear, float quadratic)
{
    if (m_Type == LightType::Directional)
    {
        LOG_ENGINE_WARN("Trying to set attenuation on a directional light");
        return;
    }
    Constant = constant;
    Linear = linear;
    Quadratic = quadratic;
}

void MCEngine::LightComponent::SetAngles(float innerAngle, float outerAngle)
{
    if (m_Type != LightType::Spot)
    {
        LOG_ENGINE_WARN("Trying to set angles on a non-spot light");
        return;
    }
    InnerAngle = innerAngle;
    OuterAngle = outerAngle;
}