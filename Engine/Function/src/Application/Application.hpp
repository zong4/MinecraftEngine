#pragma once

#include "pch.hpp"

namespace Engine
{

class Application
{
public:
    Application(const WindowProperty &props) { Init(props); }
    virtual ~Application() { Shutdown(); }

public:
    void Run();

protected:
    std::shared_ptr<Window> m_Window = nullptr;
    std::shared_ptr<ImGuiLayer> m_ImGuiLayer = nullptr;

protected:
    void AddLayer(const std::shared_ptr<Layer> &layer) { m_Window->AddLayer(layer); }
    void RemoveLayer(const std::shared_ptr<Layer> &layer) { m_Window->RemoveLayer(layer); }

private:
    void Init(const WindowProperty &props);
    void Shutdown();
};

extern std::unique_ptr<Application> CreateApplication();

} // namespace Engine