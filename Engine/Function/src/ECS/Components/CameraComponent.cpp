#include "CameraComponent.hpp"

Engine::CameraComponent::CameraComponent(CameraType type, float width, float height, float fov, float nearClip,
                                         float farClip, const glm::vec4 &backgroundColor)
    : Type(type), m_Width(width), m_Height(height), FOV(fov), NearClip(nearClip), FarClip(farClip),
      BackgroundColor(backgroundColor)
{
}

glm::vec3 Engine::CameraComponent::GetRayWorld(float u, float v) const
{
    glm::mat4 invProj = glm::inverse(m_ProjectionMatrix);
    glm::vec4 rayClip = glm::vec4(u * 2.0f - 1.0f, v * 2.0f - 1.0f, -1.0f, 1.0f);
    glm::vec4 rayEye = invProj * rayClip;
    rayEye = glm::vec4(rayEye.x, rayEye.y, -1.0f, 0.0f); // Assuming camera is at origin looking down -Z
    glm::vec3 rayWorld = glm::normalize(glm::vec3(rayEye));
    return rayWorld;
}

void Engine::CameraComponent::Resize(int width, int height)
{
    m_Width = width;
    m_Height = height;
}

void Engine::CameraComponent::UpdateProjectionMatrix()
{
    if (Type == CameraType::Orthographic)
        m_ProjectionMatrix = glm::ortho(-m_Width / 200.0f * Scale, m_Width / 200.0f * Scale, -m_Height / 200.0f * Scale,
                                        m_Height / 200.0f * Scale, NearClip, FarClip); // Maybe it is hacky
    else if (Type == CameraType::Perspective)
        m_ProjectionMatrix = glm::perspective(glm::radians(FOV), (float)m_Width / (float)m_Height, NearClip, FarClip);
}