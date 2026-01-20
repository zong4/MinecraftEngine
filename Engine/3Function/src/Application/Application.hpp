#pragma once

#include "Timer.hpp"
#include <Platform.hpp>

namespace Engine
{

class Application
{
public:
    Application(const WindowProperty &props) { Init(props); }
    virtual ~Application() { Shutdown(); }

public:
    void Run()
    {
        PROFILE_FUNCTION();

        Timer timer;
        while (m_Window->IsRunning())
        {
            m_Window->Update(timer.GetElapsedSeconds());
            m_Window->Render();
            m_Window->PostUpdate();
        }
    }

protected:
    std::shared_ptr<Window> m_Window = nullptr;

protected:
    void PushLayer(const std::shared_ptr<Layer> &layer) { m_Window->PushLayer(layer); }
    void PopLayer(const std::shared_ptr<Layer> &layer) { m_Window->PopLayer(layer); }

private:
    void Init(const WindowProperty &props) { m_Window = Window::Create(props); }
    void Shutdown() { m_Window.reset(); }
};

extern std::unique_ptr<Application> CreateApplication();

} // namespace Engine