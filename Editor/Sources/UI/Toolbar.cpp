#include "Toolbar.hpp"
#include "Manager/ConfigManager.hpp"
#include <imgui.h>

MCEditor::Toolbar::Toolbar()
{
    ENGINE_PROFILE_FUNCTION();

    m_PlayButtonIcon = std::make_shared<MCEngine::Texture2D>(
        (ConfigManager::GetInstance().GetIconsPath() / "PlayButton.png").string());
    m_StopButtonIcon = std::make_shared<MCEngine::Texture2D>(
        (ConfigManager::GetInstance().GetIconsPath() / "StopButton.png").string());
}

void MCEditor::Toolbar::OnImGuiRender()
{
    ENGINE_PROFILE_FUNCTION();

    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 2));
    ImGui::PushStyleVar(ImGuiStyleVar_ItemInnerSpacing, ImVec2(0, 0));
    ImGui::Begin("##toolbar", nullptr,
                 ImGuiWindowFlags_NoTitleBar |            
                     ImGuiWindowFlags_NoCollapse |        
                     ImGuiWindowFlags_NoResize |          
                     ImGuiWindowFlags_NoMove |            
                     ImGuiWindowFlags_NoScrollbar |       
                     ImGuiWindowFlags_NoScrollWithMouse | 
                     //  ImGuiWindowFlags_NoSavedSettings |   
                     ImGuiWindowFlags_NoBackground | 
                     ImGuiWindowFlags_NoDecoration   
    );

    // Color
    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0));

    // Size and Position
    ImVec2 avail = ImGui::GetContentRegionAvail();
    float size = avail.y;
    ImVec2 center = ImVec2((avail.x - size) * 0.5f, (avail.y - size) * 0.5f);
    ImGui::SetCursorPos(center);

    // Button
    std::shared_ptr<MCEngine::Texture2D> icon = m_State == EditorState::Edit ? m_PlayButtonIcon : m_StopButtonIcon;
    if (ImGui::ImageButton("Button", (ImTextureID)icon->GetRendererID(), ImVec2(size, size), ImVec2(0, 0),
                           ImVec2(1, 1)))
    {
        if (m_State == EditorState::Edit)
            OnScenePlay();
        else if (m_State == EditorState::Play)
            OnSceneStop();
    }

    ImGui::PopStyleColor(1);
    ImGui::End();
    ImGui::PopStyleVar(2);
}

void MCEditor::Toolbar::OnScenePlay()
{
    ENGINE_PROFILE_FUNCTION();

    m_State = EditorState::Play;
    // SceneManager::GetInstance().PlayScene();
}

void MCEditor::Toolbar::OnSceneStop()
{
    ENGINE_PROFILE_FUNCTION();

    m_State = EditorState::Edit;
    // SceneManager::GetInstance().StopScene();
}
