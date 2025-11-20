#pragma once

#include <Platform.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace Engine
{

enum class CameraType
{
    Orthographic,
    Perspective
};

struct CameraComponent
{
    bool Primary = false;
    CameraType Type;
    glm::vec4 BackgroundColor;
    float Scale = 1.0f;
    float FOV, NearClip, FarClip;

public:
    CameraComponent(CameraType type = CameraType::Perspective, float width = 1280.0f, float height = 720.0f,
                    float fov = 45.0f, float nearClip = 0.1f, float farClip = 100.0f,
                    const glm::vec4 &backgroundColor = glm::vec4(0.01f, 0.01f, 0.01f, 1.0f))
        : Type(type), m_Width(width), m_Height(height), FOV(fov), NearClip(nearClip), FarClip(farClip),
          BackgroundColor(backgroundColor)
    {
    }

    // Getters
    float GetWidth() const { return m_Width; }
    float GetHeight() const { return m_Height; }
    const glm::mat4 &GetProjectionMatrix() const { return m_ProjectionMatrix; }

public:
    void Resize(float width, float height)
    {
        m_Width = width;
        m_Height = height;
    }

    // Call every frame before using the projection matrix
    void UpdateProjectionMatrix()
    {
        if (Type == CameraType::Orthographic)
            m_ProjectionMatrix =
                glm::ortho(-m_Width / 200.0f * Scale, m_Width / 200.0f * Scale, -m_Height / 200.0f * Scale,
                           m_Height / 200.0f * Scale, NearClip, FarClip); // Maybe it is hacky
        else if (Type == CameraType::Perspective)
            m_ProjectionMatrix = glm::perspective(glm::radians(FOV), m_Width / m_Height, NearClip, FarClip);
    }

private:
    float m_Width, m_Height;
    glm::mat4 m_ProjectionMatrix;
};

} // namespace Engine