#include "FileBrowserPanel.hpp"

#include <imgui.h>

Editor::FileBrowserPanel &Editor::FileBrowserPanel::GetInstance()
{
    static FileBrowserPanel instance;
    return instance;
}

void Editor::FileBrowserPanel::OnImGuiRender()
{
    PROFILE_FUNCTION();

    ImGui::Begin("File Browser");

    // Navigation Bar
    if (!std::filesystem::equivalent(m_CurrentDirectory, m_RootDirectory))
    {
        if (ImGui::Button("<-"))
            m_CurrentDirectory = m_CurrentDirectory.parent_path();
    }
    else
    {
        ImGui::Button("/");
    }

    // Calculate number of columns(at least 1)
    static float thumbnailSize = 60.0f;
    float cellSize = thumbnailSize;
    float panelWidth = ImGui::GetContentRegionAvail().x;
    int columnCount = (int)(panelWidth / cellSize);
    if (columnCount < 1)
        columnCount = 1;
    ImGui::Columns(columnCount, 0, false);

    // List files and directories
    for (auto &&directoryEntry : std::filesystem::directory_iterator(m_CurrentDirectory))
    {
        const auto &path = directoryEntry.path();
        auto &&relativePath = std::filesystem::relative(path, m_RootDirectory);

        // Skip hidden files
        std::string filenameString = relativePath.filename().string();
        if (filenameString.empty() || filenameString[0] == '.')
            continue;
        ImGui::PushID(filenameString.c_str());

        // File/Directory Icon Button
        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0));
        std::shared_ptr<Engine::Texture2D> icon = directoryEntry.is_directory() ? m_DirectoryIcon : m_FileIcon;
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

        // Handle Clicks
        if (ImGui::IsItemHovered())
        {
            if (ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left))
            {
                if (directoryEntry.is_directory())
                    m_CurrentDirectory /= path.filename();
                else
                {
                    LOG_EDITOR_TRACE("Opened file: " + path.string());
                }
            }
            else if (ImGui::IsMouseClicked(ImGuiMouseButton_Left))
            {
                LOG_EDITOR_TRACE("Selected file: " + path.string());
            }
        }

        // Filename Text (with ellipsis if too long)
        // ImGui::PushFont(Engine::FontLibrary::GetInstance().GetFont("Cute-Thin"));
        float availWidth = ImGui::GetContentRegionAvail().x;
        float textWidth = ImGui::CalcTextSize(filenameString.c_str()).x;
        if (textWidth > availWidth)
        {
            while (!filenameString.empty() && ImGui::CalcTextSize((filenameString + "...").c_str()).x > availWidth)
                filenameString.pop_back();
            filenameString += "...";
        }

        // Center the text under the icon
        ImVec2 textSize = ImGui::CalcTextSize(filenameString.c_str());
        float textOffsetX = (thumbnailSize - textSize.x) * 0.5f;
        if (textOffsetX < 0.0f)
            textOffsetX = 0.0f;
        ImGui::SetCursorPosX(ImGui::GetCursorPosX() + textOffsetX);
        ImGui::TextUnformatted(filenameString.c_str());
        // ImGui::PopFont();

        // Next Column
        ImGui::PopID();
        ImGui::NextColumn();
    }

    // Footer
    ImGui::Columns(1);
    ImGui::SliderFloat("Thumbnail Size", &thumbnailSize, 16, 512);

    ImGui::End();
}

Editor::FileBrowserPanel::FileBrowserPanel()
    : m_RootDirectory(Engine::GetAssetsRoot()), m_CurrentDirectory(m_RootDirectory)
{
    m_FileIcon = Engine::Texture2D::Create((Engine::GetResourcesRoot() / "Icons/File.png").string());
    m_DirectoryIcon = Engine::Texture2D::Create((Engine::GetResourcesRoot() / "Icons/Directory.png").string());
}