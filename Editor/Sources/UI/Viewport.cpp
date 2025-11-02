#include "Viewport.hpp"

#include "Manager/SceneManager.hpp"
#include <imgui.h>

void MCEditor::Viewport::Render()
{
    ENGINE_PROFILE_FUNCTION();

    auto &&scene = SceneManager::GetInstance().GetActiveScene();

    // Resize if needed
    if (m_ViewportDirty)
    {
        scene->Resize(m_ViewportSize.x, m_ViewportSize.y);
        m_FBO->Resize((int)m_ViewportSize.x, (int)m_ViewportSize.y);
        m_MultisampleFBO->Resize((int)m_ViewportSize.x, (int)m_ViewportSize.y);

        m_ViewportDirty = false;
    }

    // Render scene
    m_MultisampleFBO->Bind();
    MCEngine::RendererCommand::Clear();
    scene->Render(scene->GetMainCamera());
    m_MultisampleFBO->Blit(m_FBO->GetRendererID());
    m_MultisampleFBO->Unbind();
}

void MCEditor::Viewport::OnImGuiRender()
{
    ENGINE_PROFILE_FUNCTION();

    // Get the available viewport size
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
}