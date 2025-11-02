#include "CameraController.hpp"

#include <imgui.h>
// After import imgui
#include "ImGuizmo/ImGuizmo.h"

using namespace MCEngine;

void MCEditor::CameraController::OnCreate()
{
    m_Transform = GetComponent<MCEngine::TransformComponent>();
    m_Transform->Position = glm::vec3(0.0f, 10.0f, 20.0f);
    m_Transform->SetRotationEuler(glm::vec3(-30.0f, 0.0f, 0.0f));
}

void MCEditor::CameraController::OnUpdate(float deltaTime)
{
    ENGINE_PROFILE_FUNCTION();

    if (ImGuizmo::IsUsing())
        return;

    // Get mouse input
    glm::vec2 mouseDelta = MouseLibrary::GetInstance().GetDeltaPosition();
    glm::vec2 scrollOffset = MouseLibrary::GetInstance().GetScrollOffset();

    // Move
    if (MouseLibrary::GetInstance().IsButtonDown(ENGINE_MOUSE_BUTTON_LEFT))
        m_Transform->Position += (-m_Transform->GetRight(TransformSpace::Global) * mouseDelta.x +
                                  m_Transform->GetUp(TransformSpace::Global) * mouseDelta.y) *
                                 m_MoveSpeed * deltaTime;

    // Rotate
    if (MouseLibrary::GetInstance().IsButtonDown(ENGINE_MOUSE_BUTTON_RIGHT))
        m_Transform->SetRotationEuler(m_Transform->GetRotationEuler() -
                                      glm::vec3(mouseDelta.y, mouseDelta.x, 0.0f) * m_RotateSpeed * deltaTime);

    // Zoom
    if (scrollOffset.y != 0.0)
        m_Transform->Position +=
            m_Transform->GetForward(TransformSpace::Global) * scrollOffset.y * m_ZoomSpeed * deltaTime;
}
