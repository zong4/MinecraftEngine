#include "Sandbox.hpp"

#include "Layer/EditorLayer.hpp"

Editor::Sandbox::Sandbox(const MCEngine::WindowProperty &props) : Application(props)
{
    AddLayer(std::make_shared<EditorLayer>(m_Window));
}

std::unique_ptr<MCEngine::Application> MCEngine::CreateApplication()
{
    return std::make_unique<Editor::Sandbox>(MCEngine::WindowProperty("Minecraft Engine", 1280, 720, true));
}