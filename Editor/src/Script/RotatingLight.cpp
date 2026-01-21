#include "RotatingLight.hpp"

using namespace Engine;

bool Editor::RotatingLight::s_RotationEnabled = false;

void Editor::RotatingLight::OnCreate() { m_Transform = GetComponent<Engine::TransformComponent>(); }

void Editor::RotatingLight::OnUpdate(float deltaTime)
{
    PROFILE_FUNCTION();

    if (!s_RotationEnabled)
        return;

    glm::vec3 rotation = m_Transform->GetRotationEuler();
    rotation.y += m_RotationSpeed * deltaTime;
    if (rotation.y >= 360.0f)
        rotation.y -= 360.0f;
    m_Transform->SetRotationEuler(rotation);
}
