#include "CameraController.hpp"

using namespace Engine;

void Editor::CameraController::OnCreate()
{
    m_Transform = GetComponent<Engine::TransformComponent>();
    m_Transform->Position = glm::vec3(0.0f, 10.0f, 20.0f);
    m_Transform->SetRotationEuler(glm::vec3(-30.0f, 0.0f, 0.0f));
}

void Editor::CameraController::OnUpdate(float deltaTime)
{
    PROFILE_FUNCTION();

    // Get mouse input
    glm::vec2 mouseDelta = Input::GetInstance().GetDeltaPosition();
    glm::vec2 scrollOffset = Input::GetInstance().GetScrollOffset();

    // Move
    if (Input::GetInstance().IsKeyDown(MOUSE_BUTTON_LEFT))
    {
        glm::vec3 movement = -m_Transform->GetRight(TransformSpace::Global) * mouseDelta.x +
                             m_Transform->GetUp(TransformSpace::Global) * mouseDelta.y;
        m_Transform->Position += movement * m_MoveSpeed * deltaTime;
    }

    // Rotate
    if (Input::GetInstance().IsKeyDown(MOUSE_BUTTON_RIGHT))
    {
        glm::vec3 rotation = glm::vec3(mouseDelta.y, mouseDelta.x, 0.0f);
        m_Transform->SetRotationEuler(m_Transform->GetRotationEuler() - rotation * m_RotateSpeed * deltaTime);
    }

    // Zoom
    if (scrollOffset.y != 0.0)
    {
        glm::vec3 movement = m_Transform->GetForward(TransformSpace::Global) * scrollOffset.y;
        m_Transform->Position += movement * m_ZoomSpeed * deltaTime;
    }
}
