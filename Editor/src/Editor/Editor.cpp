#include "Editor.hpp"

#include "Layer/EditorLayer.hpp"
#include "Manager/ConfigManager.hpp"

MCEditor::Editor::Editor(const MCEngine::WindowProperty &props) : Application(props)
{
    AddLayer(std::make_shared<EditorLayer>(m_Window));
}

std::unique_ptr<MCEngine::Application> MCEngine::CreateApplication()
{
    return std::make_unique<MCEditor::Editor>(MCEditor::ConfigManager::GetInstance().GetWindowProperty());
}