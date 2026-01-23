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
        // Perspective camera
        float yFull = tan(glm::radians(FOV) / 2.0f);
        float xFull = ((float)m_Width / (float)m_Height) * yFull;

        // Convert (u,v) to NDC space [-1,1]
        float xDelta = (2.0f * u - 1.0f) * xFull;
        float yDelta = (1.0f - 2.0f * v) * yFull;
        return glm::normalize(glm::vec3(xDelta, yDelta, -1.0f));
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