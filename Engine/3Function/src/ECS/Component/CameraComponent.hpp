#pragma once

#include "../../Physic/Ray/Ray.hpp"
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
    float Scale = 1.0f;
    float FOV, NearClip, FarClip;
    glm::vec4 BackgroundColor;

public:
    CameraComponent(CameraType type = CameraType::Perspective, float width = 1280.0f, float height = 720.0f,
                    float fov = 45.0f, float nearClip = 0.1f, float farClip = 3000.0f,
                    const glm::vec4 &backgroundColor = glm::vec4(0.01f, 0.01f, 0.01f, 1.0f));

    // Getters
    int GetWidth() const { return m_Width; }
    int GetHeight() const { return m_Height; }
    const glm::mat4 &GetProjectionMatrix() const { return m_ProjectionMatrix; }
    glm::vec3 GetLocalDir(float u, float v) const; // u is in [0,1], v is in [0,1]

public:
    void Resize(int width, int height);
    void UpdateProjectionMatrix(); // Call every frame before using the projection matrix

private:
    int m_Width, m_Height;
    glm::mat4 m_ProjectionMatrix;
};

} // namespace Engine