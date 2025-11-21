#include "Sandbox.hpp"

#include "Layers/SandboxLayer.hpp"
#include <nlohmann/json.hpp>

Editor::Sandbox::Sandbox(const Engine::WindowProperty &props) : Application(props)
{
    AddLayer(std::make_shared<SandboxLayer>(m_Window));
    AddLayer(std::make_shared<Engine::InputLayer>());
    AddLayer(std::make_shared<Engine::ImGuiLayer>(m_Window->GetNativeWindow()));
}

std::unique_ptr<Engine::Application> Engine::CreateApplication()
{
    nlohmann::json config;
    std::ifstream configFile(std::string(PROJECT_ROOT) + "/Editor/config/sandbox.json");
    if (configFile.is_open())
    {
        configFile >> config;
        configFile.close();

        std::string title = config.value("title", "Minecraft Engine");
        int width = config.value("width", 1280);
        int height = config.value("height", 720);
        bool vsync = config.value("vsync", true);
        return std::make_unique<Editor::Sandbox>(Engine::WindowProperty(title, width, height, vsync));
    }
    else
    {
        LOG_ENGINE_WARN("Failed to open sandbox.json, using default window properties");
        return std::make_unique<Editor::Sandbox>(Engine::WindowProperty("Minecraft Engine", 1280, 720, true));
    }
}

Editor::Sandbox::~Sandbox()
{
    nlohmann::ordered_json config;
    std::ofstream configFileOut(std::string(PROJECT_ROOT) + "/Editor/config/sandbox.json");
    if (configFileOut.is_open())
    {
        Engine::WindowProperty &props = m_Window->GetProperty();
        config["title"] = props.Title;
        config["width"] = props.Width;
        config["height"] = props.Height;
        config["vsync"] = props.VSync;

        configFileOut << config.dump(4);
        configFileOut.close();
    }
    else
    {
        LOG_ENGINE_WARN("Failed to save sandbox.json");
    }
}