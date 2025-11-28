#include "CameraComponent.hpp"

Engine::CameraComponent::CameraComponent(CameraType type, float width, float height, float fov, float nearClip,
                                         float farClip, const glm::vec4 &backgroundColor)
    : Type(type), m_Width(width), m_Height(height), FOV(fov), NearClip(nearClip), FarClip(farClip),
      BackgroundColor(backgroundColor)
{
}

glm::vec3 Engine::CameraComponent::GetRayWorld(float u, float v, const glm::mat4 &viewMatrix) const
{
    // 1. NDC space
    float ndcX = u * 2.0f - 1.0f;
    float ndcY = v * 2.0f - 1.0f;

    // 2. Clip space
    glm::vec4 rayClip = glm::vec4(ndcX, ndcY, -1.0f, 1.0f); // z = -1 = near plane

    // 2. Clip space -> Eye space
    glm::vec4 rayEye = glm::inverse(m_ProjectionMatrix) * rayClip;
    rayEye = glm::vec4(rayEye.x, rayEye.y, -1.0f, 0.0f); // w=0 表示方向向量

    // 3. Eye space -> World space
    glm::vec4 rayWorld4 = glm::inverse(viewMatrix) * rayEye;
    glm::vec3 rayWorld = glm::normalize(glm::vec3(rayWorld4));
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