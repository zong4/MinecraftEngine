#include "Creator.hpp"

#include "Layers/CreatorLayer.hpp"
#include <nlohmann/json.hpp>

Editor::Creator::Creator(const Engine::WindowProperty &props) : Application(props)
{
    AddLayer(std::make_shared<CreatorLayer>(m_Window));
    AddLayer(std::make_shared<Engine::InputLayer>());
    AddLayer(Engine::ImGuiLayer::Create(m_Window->GetNativeWindow()));
}

std::unique_ptr<Engine::Application> Engine::CreateApplication()
{
    nlohmann::json config;
    std::ifstream configFile(std::string(PROJECT_ROOT) + "/Editor/config/Creator.json");
    if (configFile.is_open())
    {
        configFile >> config;
        configFile.close();

        std::string title = config.value("Title", "Minecraft Engine");
        int width = config.value("Width", 1280);
        int height = config.value("Height", 720);
        bool vsync = config.value("VSync", true);
        return std::make_unique<Editor::Creator>(Engine::WindowProperty(title, width, height, vsync));
    }
    else
    {
        LOG_ENGINE_WARN("Failed to open Creator.json, using default window properties");
        return std::make_unique<Editor::Creator>(Engine::WindowProperty("Minecraft Engine", 1280, 720, true));
    }
}

Editor::Creator::~Creator()
{
    nlohmann::ordered_json config;
    std::ofstream configFileOut(std::string(PROJECT_ROOT) + "/Editor/config/Creator.json");
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
        LOG_ENGINE_WARN("Failed to save Creator.json");
    }
}