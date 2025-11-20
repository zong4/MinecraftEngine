#include "Sandbox.hpp"

#include "Layers/SandboxLayer.hpp"

Editor::Sandbox::Sandbox(const Engine::WindowProperty &props) : Application(props)
{
    AddLayer(std::make_shared<SandboxLayer>(m_Window));
    AddLayer(std::make_shared<Engine::InputLayer>());
    AddLayer(std::make_shared<Engine::ImGuiLayer>(m_Window));
}

std::unique_ptr<Engine::Application> Engine::CreateApplication()
{
    return std::make_unique<Editor::Sandbox>(Engine::WindowProperty("Minecraft Engine", 1280, 720, true));
}