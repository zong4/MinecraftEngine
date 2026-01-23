#include "CameraComponent.hpp"

Engine::CameraComponent::CameraComponent(CameraType type, float width, float height, float fov, float nearClip,
                                         float farClip, const glm::vec4 &backgroundColor)
    : Type(type), m_Width(width), m_Height(height), FOV(fov), NearClip(nearClip), FarClip(farClip),
      BackgroundColor(backgroundColor)
{
}

glm::vec3 Engine::CameraComponent::GetDirection(float u, float v) const
{
    if (Type == CameraType::Orthographic)
    {
        return glm::vec3(0.0f, 0.0f, -1.0f);
    }
    else
    {
        float aspectRatio = (float)m_Width / (float)m_Height;
        float fovRad = glm::radians(FOV);
        float px = (2.0f * u - 1.0f) * tan(fovRad / 2.0f) * aspectRatio;
        float py = (1.0f - 2.0f * v) * tan(fovRad / 2.0f);
        glm::vec3 dir = glm::normalize(glm::vec3(px, py, -1.0f));
        return dir;
    }
}

void Engine::CameraComponent::Resize(int width, int height)
{
    m_Width = width;
    m_Height = height;
}

void Engine::CameraComponent::UpdateProjectionMatrix()
{
    if (Type == CameraType::Orthographic)
    {
        m_ProjectionMatrix =
            glm::ortho(-m_Width / 200.0f * Scale, m_Width / 200.0f * Scale, -m_Height / 200.0f * Scale,
                       m_Height / 200.0f * Scale, NearClip, FarClip); // 200.0f to convert from pixels to world units
    }
    else if (Type == CameraType::Perspective)
    {
        m_ProjectionMatrix = glm::perspective(glm::radians(FOV), (float)m_Width / (float)m_Height, NearClip, FarClip);
    }
}