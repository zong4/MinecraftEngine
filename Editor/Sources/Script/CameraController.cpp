#include "CameraController.hpp"

#include <imgui.h>
// After import imgui
#include "ImGuizmo/ImGuizmo.h"

void MCEditor::CameraController::OnStart()
{
    auto &&transform = GetComponent<MCEngine::TransformComponent>();
    transform.Position = glm::vec3(0.0f, 10.0f, 20.0f);
    transform.SetRotationEuler(glm::vec3(-30.0f, 0.0f, 0.0f));
}

void MCEditor::CameraController::OnUpdate(float deltaTime)
{
    ENGINE_PROFILE_FUNCTION();

    if (ImGuizmo::IsUsing())
        return;

    auto &&transform = GetComponent<MCEngine::TransformComponent>();
    glm::vec2 mouseDelta = MCEngine::MouseLibrary::GetInstance().GetDeltaPosition();
    if (MCEngine::MouseLibrary::GetInstance().IsButtonDown(ENGINE_MOUSE_BUTTON_LEFT))
    {
        transform.Position +=
            (-transform.GetRight() * mouseDelta.x + transform.GetUp() * mouseDelta.y) * m_MoveSpeed * deltaTime;
    }
    else if (MCEngine::MouseLibrary::GetInstance().IsButtonDown(ENGINE_MOUSE_BUTTON_RIGHT))
    {
        transform.SetRotationEuler(transform.GetRotationEuler() -
                                   glm::vec3(mouseDelta.y, mouseDelta.x, 0.0f) * m_RotateSpeed * deltaTime);
    }

    const glm::vec2 &scrollOffset = MCEngine::MouseLibrary::GetInstance().GetScrollOffset();
    if (scrollOffset.y != 0.0)
    {
        transform.Position += transform.GetForward() * scrollOffset.y * m_ZoomSpeed * deltaTime;
        MCEngine::MouseLibrary::GetInstance().SetScrollOffset(0.0, 0.0);
    }
}
