#include "SceneViewport.hpp"

#include <imgui.h>

MCEditor::SceneViewport::SceneViewport() {}

void MCEditor::SceneViewport::Render(std::shared_ptr<MCEngine::Scene> scene)
{
    ENGINE_PROFILE_FUNCTION();

    // Resize if needed
    if (m_ViewportDirty)
    {
        m_Camera.GetComponent<MCEngine::CameraComponent>()->Resize(m_ViewportSize.x,
                                                                   m_ViewportSize.y); // Resize EditorScene is also OK
        m_FBO->Resize((int)m_ViewportSize.x, (int)m_ViewportSize.y);
        m_MultisampleFBO->Resize((int)m_ViewportSize.x, (int)m_ViewportSize.y);
        m_EntityPickingFBO->Resize((int)m_ViewportSize.x, (int)m_ViewportSize.y);

        m_ViewportDirty = false;
    }

    // Render scene
    m_MultisampleFBO->Bind();
    scene->Render(m_Camera);

    // test: bvh
    MCEngine::BVH bvh(scene);
    bvh.Render(3);

    m_MultisampleFBO->Blit(m_FBO->GetRendererID());
    m_MultisampleFBO->Unbind();

    // Render entity ID to FBO
    m_EntityPickingFBO->Bind();
    scene->RenderColorID();
    m_EntityPickingFBO->Unbind();
}

void MCEditor::SceneViewport::OnImGuiRender()
{
    ENGINE_PROFILE_FUNCTION();

    MCEditor::Viewport::OnImGuiRender();

    // Update focused/hovered state
    m_Focused = ImGui::IsWindowFocused();
    m_Hovered = ImGui::IsWindowHovered();
}
