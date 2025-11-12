#include "EditorLayer.hpp"

#include "Manager/ConfigManager.hpp"
#include "Manager/SceneManager.hpp"
#include <imgui.h>
// After import imgui
#include "ImGuizmo/ImGuizmo.h"

MCEditor::EditorLayer::EditorLayer(const std::shared_ptr<MCEngine::Window> &window)
    : ImGuiLayer(window, (ConfigManager::GetInstance().GetConfigsPath() / "imgui.ini").string(), "EditorLayer")
{
}

void MCEditor::EditorLayer::OnEvent(MCEngine::Event &event)
{
    ENGINE_PROFILE_FUNCTION();

    // Store key states in KeyCodeLibrary
    if (!event.IsHandled())
    {
        MCEngine::EventDispatcher dispatcher(event);

        // KeyEvent
        dispatcher.Dispatch<MCEngine::KeyEvent>([this](MCEngine::KeyEvent &event) {
            MCEngine::Input::GetInstance().SetKeyAction(event.GetCode(), event.GetAction());
            return false;
        });

        // MouseEvent
        {
            dispatcher.Dispatch<MCEngine::MouseButtonEvent>([this](MCEngine::MouseButtonEvent &event) {
                MCEngine::Input::GetInstance().SetKeyAction(event.GetCode(), event.GetAction());
                return false;
            });
            dispatcher.Dispatch<MCEngine::MouseMoveEvent>([this](MCEngine::MouseMoveEvent &event) {
                MCEngine::Input::GetInstance().SetPosition(event.GetX(), event.GetY());
                return false;
            });
            dispatcher.Dispatch<MCEngine::MouseScrollEvent>([this](MCEngine::MouseScrollEvent &event) {
                MCEngine::Input::GetInstance().SetScrollOffset(event.GetXOffset(), event.GetYOffset());
                return false;
            });
        }
    }

    // Handle key events for editor actions
    if (!event.IsHandled())
    {
        MCEngine::EventDispatcher dispatcher(event);

        dispatcher.Dispatch<MCEngine::KeyEvent>([this](MCEngine::KeyEvent &event) {
            if (event.GetAction() == 1)
            {
                // Key Pressed for gizmo types
                switch (event.GetCode())
                {
                case ENGINE_KEY_Q:
                    if (!ImGuizmo::IsUsing())
                        m_GizmoType = ImGuizmoType::None;
                    return true;
                case ENGINE_KEY_W:
                    if (!ImGuizmo::IsUsing())
                        m_GizmoType = ImGuizmoType::Translate;
                    return true;
                case ENGINE_KEY_E:
                    if (!ImGuizmo::IsUsing())
                        m_GizmoType = ImGuizmoType::Rotate;
                    return true;
                case ENGINE_KEY_R:
                    if (!ImGuizmo::IsUsing())
                        m_GizmoType = ImGuizmoType::Scale;
                    return true;
                default:
                    break;
                }

                // Key Pressed for editor actions
                switch (event.GetCode())
                {
                case ENGINE_KEY_N:
                    if (MCEngine::Input::GetInstance().IsControlDown())
                        m_Action = EditorAction::NewScene;
                    return true;
                case ENGINE_KEY_O:
                    if (MCEngine::Input::GetInstance().IsControlDown())
                        m_Action = EditorAction::OpenScene;
                    return true;
                case ENGINE_KEY_S:
                    if (MCEngine::Input::GetInstance().IsControlDown() && MCEngine::Input::GetInstance().IsShiftDown())
                        m_Action = EditorAction::SaveSceneAs;
                    return true;
                default:
                    break;
                }
            }

            return false;
        });
    }
}

void MCEditor::EditorLayer::OnUpdate(float deltaTime)
{
    ENGINE_PROFILE_FUNCTION();

    // Handle editor actions
    {
        switch (m_Action)
        {
        case EditorAction::NewScene:
            SceneManager::GetInstance().NewExampleScene();
            break;
        case EditorAction::OpenScene:
            SceneManager::GetInstance().OpenSceneDialog();
            break;
        case EditorAction::SaveSceneAs:
            SceneManager::GetInstance().SaveSceneAsDialog();
            break;
        default:
            break;
        }
        m_Action = EditorAction::None;
    }

    // Update scenes
    SceneManager::GetInstance().GetEditorScene()->Update(deltaTime);
}

void MCEditor::EditorLayer::OnRender()
{
    ENGINE_PROFILE_FUNCTION();

    // Pre-render
    {
        SceneManager::GetInstance().GetActiveScene()->PreRender();
        SceneManager::GetInstance().GetActiveScene()->RenderShadowMap();
    }

    SceneManager::GetInstance().GetActiveScene()->Render(SceneManager::GetInstance().GetEditorScene()->GetMainCamera());

    // Render entity ID to FBO
    m_EntityPickingFBO->Bind();
    SceneManager::GetInstance().GetActiveScene()->RenderColorID();
    m_EntityPickingFBO->Unbind();
}

void MCEditor::EditorLayer::RenderImGui()
{
    ENGINE_PROFILE_FUNCTION();

    RenderGizmos();
    PickEntity(); // After RenderGizmos to avoid conflict with ImGuizmo
}

void MCEditor::EditorLayer::RenderGizmos()
{
    ENGINE_PROFILE_FUNCTION();

    auto &&selectedEntity = SceneManager::GetInstance().GetSelectedEntity();
    if (selectedEntity && m_GizmoType != ImGuizmoType::None)
    {
        ImGuizmo::BeginFrame();
        ImGuizmo::SetOrthographic(false);
        ImGuizmo::SetDrawlist();

        // ImGuizmo rect
        ImGuiViewport *viewport = ImGui::GetMainViewport();
        ImGuizmo::SetRect(viewport->Pos.x, viewport->Pos.y, viewport->Size.x, viewport->Size.y);

        // MVP
        auto &&transformComponent = selectedEntity.GetComponent<MCEngine::TransformComponent>();
        glm::mat4 transform = transformComponent->GetTransformMatrix();
        const glm::mat4 &view = glm::inverse(SceneManager::GetInstance()
                                                 .GetEditorScene()
                                                 ->GetMainCamera()
                                                 .GetComponent<MCEngine::TransformComponent>()
                                                 ->GetTransformMatrix());
        const glm::mat4 &projection = SceneManager::GetInstance()
                                          .GetEditorScene()
                                          ->GetMainCamera()
                                          .GetComponent<MCEngine::CameraComponent>()
                                          ->GetProjectionMatrix();

        // Gizmo operation
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

        // Update transform component if gizmo is used
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

void MCEditor::EditorLayer::PickEntity()
{
    ENGINE_PROFILE_FUNCTION();

    if (!ImGuizmo::IsUsing() && ImGui::IsMouseClicked(ImGuiMouseButton_Left))
    {
        float mouseX = ImGui::GetMousePos().x;
        float mouseY = ImGui::GetMousePos().y;

        ImGuiViewport *viewport = ImGui::GetMainViewport();
        int pixelX = static_cast<int>(mouseX);
        int pixelY = static_cast<int>(viewport->Size.y - mouseY); // 翻转 Y

        SceneManager::GetInstance().SetSelectedEntity(
            (entt::entity)(m_EntityPickingFBO->PickPixel(pixelX, pixelY) - 1));
    }
}
