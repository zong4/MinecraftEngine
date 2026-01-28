#include "Creator.hpp"

#include "CreatorLayer.hpp"
#include <nlohmann/json.hpp>

Editor::Creator::Creator(const Engine::WindowProperty &props) : Application(props)
{
    PushLayer(std::make_shared<CreatorLayer>());
    PushLayer(std::make_shared<Engine::InputLayer>());
    PushLayer(Engine::ImGuiLayer::Create(m_Window->GetNativeWindow()));
}

std::unique_ptr<Engine::Application> Engine::CreateApplication()
{
    PROFILE_FUNCTION();

    // Load window properties from config file
    nlohmann::json config;
    std::ifstream configFile((Engine::GetConfigsDirectory() / "Creator.json").string());
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
    PROFILE_FUNCTION();

    // Save window properties to config file
    nlohmann::ordered_json config;
    std::ofstream configFileOut((Engine::GetConfigsDirectory() / "Creator.json").string());
    if (configFileOut.is_open())
    {
        Engine::WindowProperty &props = m_Window->GetProperty();
        config["Title"] = props.Title;
        config["Width"] = props.Width;
        config["Height"] = props.Height;
        config["VSync"] = props.VSync;

        configFileOut << config.dump(4);
        configFileOut.close();
    }
    else
    {
        LOG_ENGINE_WARN("Failed to save Creator.json");
    }
}