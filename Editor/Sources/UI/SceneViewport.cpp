#include "SceneViewport.hpp"

#include "Manager/SceneManager.hpp"
#include <imgui.h>
// After import imgui
#include "ImGuizmo/ImGuizmo.h"

MCEditor::SceneViewport::SceneViewport() { m_Camera = SceneManager::GetInstance().GetEditorScene()->GetMainCamera(); }

void MCEditor::SceneViewport::SetGizmoType(ImGuizmoType type)
{
    if (!ImGuizmo::IsUsing())
        m_GizmoType = type;
}

void MCEditor::SceneViewport::Render()
{
    ENGINE_PROFILE_FUNCTION();

    if (m_ViewportDirty)
    {
        // Resize EditorScene is also OK
        m_Camera.GetComponent<MCEngine::CameraComponent>()->Resize(m_ViewportSize.x, m_ViewportSize.y);

        m_FBO->Resize((int)m_ViewportSize.x, (int)m_ViewportSize.y);
        m_MultisampleFBO->Resize((int)m_ViewportSize.x, (int)m_ViewportSize.y);
        m_EntityPickingFBO->Resize((int)m_ViewportSize.x, (int)m_ViewportSize.y);

        m_ViewportDirty = false;
    }

    m_MultisampleFBO->Bind();
    MCEngine::RendererCommand::Clear();
    SceneManager::GetInstance().GetActiveScene()->Render(m_Camera);
    m_MultisampleFBO->Blit(m_FBO->GetRendererID());
    m_MultisampleFBO->Unbind();

    m_EntityPickingFBO->Bind();
    MCEngine::RendererCommand::Clear();
    SceneManager::GetInstance().GetActiveScene()->RenderColorID();
    m_EntityPickingFBO->Unbind();
}

void MCEditor::SceneViewport::OnImGuiRender()
{
    ENGINE_PROFILE_FUNCTION();

    m_Focused = ImGui::IsWindowFocused();
    m_Hovered = ImGui::IsWindowHovered();
    ImVec2 viewportSize = ImGui::GetContentRegionAvail();
    if (viewportSize.x > 0 && viewportSize.y > 0)
    {
        if (viewportSize.x != m_ViewportSize.x || viewportSize.y != m_ViewportSize.y)
        {
            m_ViewportDirty = true;
            m_ViewportSize = {viewportSize.x, viewportSize.y};
        }
    }
    ImGui::Image((ImTextureID)(intptr_t)m_FBO->GetTexture()->GetRendererID(), viewportSize, ImVec2(0, 1), ImVec2(1, 0));

    RenderGizmos();
    PickEntity(); // After RenderGizmos to avoid conflict with ImGuizmo
}

void MCEditor::SceneViewport::RenderGizmos()
{
    ENGINE_PROFILE_FUNCTION();

    auto &&selectedEntity = SceneManager::GetInstance().GetSelectedEntity();
    if (selectedEntity && m_GizmoType != ImGuizmoType::None)
    {
        ImGuizmo::BeginFrame();
        ImGuizmo::SetOrthographic(false);
        ImGuizmo::SetDrawlist();

        // ImGuizmo rect
        ImGuizmo::SetRect(ImGui::GetWindowPos().x + ImGui::GetWindowContentRegionMin().x,
                          ImGui::GetWindowPos().y + ImGui::GetWindowContentRegionMin().y, m_ViewportSize.x,
                          m_ViewportSize.y);

        // MVP
        auto &&transformComponent = selectedEntity.GetComponent<MCEngine::TransformComponent>();
        glm::mat4 transform = transformComponent->GetTransformMatrix();
        const glm::mat4 &view =
            glm::inverse(m_Camera.GetComponent<MCEngine::TransformComponent>()->GetTransformMatrix());
        const glm::mat4 &projection = m_Camera.GetComponent<MCEngine::CameraComponent>()->GetProjectionMatrix();

        ImGuizmo::OPERATION gizmoOperation;
        switch (m_GizmoType)
        {
        case ImGuizmoType::Translate:
            gizmoOperation = ImGuizmo::OPERATION::TRANSLATE;
            break;
        case ImGuizmoType::Rotate:
            gizmoOperation = ImGuizmo::OPERATION::ROTATE;
            break;
        case ImGuizmoType::Scale:
            gizmoOperation = ImGuizmo::OPERATION::SCALE;
            break;
        default:
            gizmoOperation = ImGuizmo::OPERATION::TRANSLATE;
            break;
        }

        // Snapping
        bool snap = ImGui::IsKeyDown(ImGuiKey_LeftShift) || ImGui::IsKeyDown(ImGuiKey_RightShift);
        float snapValue = 0.5f; // Snap to 0.5m for translation/scale
        if (gizmoOperation == ImGuizmo::OPERATION::ROTATE)
            snapValue = 45.0f; // Snap to 45 degrees for rotation
        float snapValues[3] = {snapValue, snapValue, snapValue};
        ImGuizmo::Manipulate(glm::value_ptr(view), glm::value_ptr(projection), gizmoOperation, ImGuizmo::LOCAL,
                             glm::value_ptr(transform), nullptr, snap ? snapValues : nullptr);

        if (ImGuizmo::IsUsing())
        {
            glm::mat4 localTransform = transform;
            auto &&relationshipParent = selectedEntity.GetComponent<MCEngine::RelationshipComponent>()->GetParent();
            if (relationshipParent)
            {
                glm::mat4 parentTransform =
                    relationshipParent.GetComponent<MCEngine::TransformComponent>()->GetTransformMatrix();
                localTransform = glm::inverse(parentTransform) * transform;
            }

            glm::vec3 position, rotation, scale;
            MCEngine::Math::DecomposeTransform(localTransform, position, rotation, scale);
            transformComponent->Position = position;
            transformComponent->SetRotationRadians(rotation);
            transformComponent->Scale = scale;
        }
    }
}

void MCEditor::SceneViewport::PickEntity()
{
    ENGINE_PROFILE_FUNCTION();

    if (m_Hovered && !ImGuizmo::IsUsing() && ImGui::IsMouseClicked(ImGuiMouseButton_Left))
    {
        ImVec2 mouseInViewport = {
            ImGui::GetMousePos().x - ImGui::GetWindowPos().x - ImGui::GetWindowContentRegionMin().x,
            ImGui::GetMousePos().y - ImGui::GetWindowPos().y - ImGui::GetWindowContentRegionMin().y};
        int pixelX = static_cast<int>(mouseInViewport.x);
        int pixelY = static_cast<int>(m_ViewportSize.y - mouseInViewport.y); // Invert Y axis
        SceneManager::GetInstance().SetSelectedEntity(
            (entt::entity)(m_EntityPickingFBO->PickPixel(pixelX, pixelY) - 1));
    }
}
