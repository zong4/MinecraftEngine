#include "CameraComponent.hpp"

void MCEngine::CameraComponent::Resize(float width, float height)
{
    m_Width = width;
    m_Height = height;
}

void MCEngine::CameraComponent::UpdateProjectionMatrix()
{
    ENGINE_PROFILE_FUNCTION();

    if (Type == CameraType::Orthographic)
        m_ProjectionMatrix = glm::ortho(-m_Width / 200.0f * Scale, m_Width / 200.0f * Scale, -m_Height / 200.0f * Scale,
                                        m_Height / 200.0f * Scale, -1.0f, 1.0f);
    else if (Type == CameraType::Perspective)
        m_ProjectionMatrix = glm::perspective(glm::radians(FOV), m_Width / m_Height, NearClip, FarClip);
}