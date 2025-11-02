#include "FileBrowserPanel.hpp"

#include "Manager/ConfigManager.hpp"
#include "Manager/SceneManager.hpp"
#include <imgui.h>

MCEditor::FileBrowserPanel::FileBrowserPanel() : m_CurrentDirectory(ConfigManager::GetInstance().GetAssetsPath())
{
    m_FileIcon =
        std::make_shared<MCEngine::Texture2D>((ConfigManager::GetInstance().GetIconsPath() / "File.png").string());
    m_DirectoryIcon =
        std::make_shared<MCEngine::Texture2D>((ConfigManager::GetInstance().GetIconsPath() / "Directory.png").string());
}

void MCEditor::FileBrowserPanel::OnImGuiRender()
{
    ENGINE_PROFILE_FUNCTION();

    ImGui::Begin("File Browser");

    // Back button
    if (!std::filesystem::equivalent(m_CurrentDirectory, ConfigManager::GetInstance().GetAssetsPath()))
    {
        if (ImGui::Button("<-"))
            m_CurrentDirectory = m_CurrentDirectory.parent_path();
    }
    else
    {
        ImGui::Button("/");
    }

    // At least one column
    static float thumbnailSize = 60.0f;
    float cellSize = thumbnailSize;
    float panelWidth = ImGui::GetContentRegionAvail().x;
    int columnCount = (int)(panelWidth / cellSize);
    if (columnCount < 1)
        columnCount = 1;
    ImGui::Columns(columnCount, 0, false);

    // Entries
    for (auto &&directoryEntry : std::filesystem::directory_iterator(m_CurrentDirectory))
    {
        const auto &path = directoryEntry.path();
        auto &&relativePath = std::filesystem::relative(path, ConfigManager::GetInstance().GetAssetsPath());

        // ID
        std::string filenameString = relativePath.filename().string();
        if (filenameString.empty() || filenameString[0] == '.')
            continue;
        ImGui::PushID(filenameString.c_str());

        // Icon
        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0));
        std::shared_ptr<MCEngine::Texture2D> icon = directoryEntry.is_directory() ? m_DirectoryIcon : m_FileIcon;
        ImGui::ImageButton(filenameString.c_str(), (ImTextureID)icon->GetRendererID(), {thumbnailSize, thumbnailSize},
                           {0, 1}, {1, 0});
        ImGui::PopStyleColor();

        // Drag and Drop Source
        if (ImGui::BeginDragDropSource())
        {
            ImGui::SetDragDropPayload("CONTENT_BROWSER_ITEM", path.c_str(),
                                      (strlen(path.string().c_str()) + 1) * sizeof(char));
            ImGui::EndDragDropSource();
        }

        // Handle double-click to open directory or select file
        if (ImGui::IsItemHovered())
        {
            if (ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left))
            {
                if (directoryEntry.is_directory())
                    m_CurrentDirectory /= path.filename();
                else
                {
                    if (ConfigManager::IsScene(path))
                        SceneManager::GetInstance().OpenScene(path.string());
                }
            }
            else if (ImGui::IsMouseClicked(ImGuiMouseButton_Left))
            {
                LOG_EDITOR_TRACE("Selected file: " + path.string());
            }
        }

        // Filename
        ImGui::PushFont(MCEngine::FontLibrary::GetInstance().GetFont("Cute-Thin"));
        float availWidth = ImGui::GetContentRegionAvail().x;
        float textWidth = ImGui::CalcTextSize(filenameString.c_str()).x;
        if (textWidth > availWidth)
        {
            while (!filenameString.empty() && ImGui::CalcTextSize((filenameString + "...").c_str()).x > availWidth)
                filenameString.pop_back();
            filenameString += "...";
        }

        // Centered text
        ImVec2 textSize = ImGui::CalcTextSize(filenameString.c_str());
        float textOffsetX = (thumbnailSize - textSize.x) * 0.5f;
        if (textOffsetX < 0.0f)
            textOffsetX = 0.0f;
        ImGui::SetCursorPosX(ImGui::GetCursorPosX() + textOffsetX);
        ImGui::TextUnformatted(filenameString.c_str());
        ImGui::PopFont();

        // Next column
        ImGui::PopID();
        ImGui::NextColumn();
    }

    // Footer
    ImGui::Columns(1);
    ImGui::SliderFloat("Thumbnail Size", &thumbnailSize, 16, 512);

    ImGui::End();
}
