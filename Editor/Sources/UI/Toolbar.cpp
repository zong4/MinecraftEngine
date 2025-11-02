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
                 ImGuiWindowFlags_NoTitleBar |            //  无标题栏
                     ImGuiWindowFlags_NoCollapse |        //  禁止折叠按钮
                     ImGuiWindowFlags_NoResize |          //  不允许缩放
                     ImGuiWindowFlags_NoMove |            //  不可移动
                     ImGuiWindowFlags_NoScrollbar |       //  无滚动条
                     ImGuiWindowFlags_NoScrollWithMouse | //  禁止鼠标滚动
                     //  ImGuiWindowFlags_NoSavedSettings |   //  不保存位置或状态
                     ImGuiWindowFlags_NoBackground | //  可选：去掉背景（半透明UI时有用）
                     ImGuiWindowFlags_NoDecoration   //  不绘制边框/标题等装饰
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
