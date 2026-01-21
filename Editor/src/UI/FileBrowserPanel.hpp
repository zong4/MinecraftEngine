#pragma once

#include <Function.hpp>

namespace Editor
{

class FileBrowserPanel
{

public:
    static FileBrowserPanel &GetInstance();

    // Setters
    void SetCurrentDirectory(const std::filesystem::path &path) { m_CurrentDirectory = path; }

public:
    void OnImGuiRender();

private:
    std::filesystem::path m_CurrentDirectory;
    std::shared_ptr<Engine::Texture2D> m_FileIcon;
    std::shared_ptr<Engine::Texture2D> m_DirectoryIcon;

private:
    FileBrowserPanel();
    ~FileBrowserPanel() = default;
};

} // namespace Editor