#include "Application.hpp"

#include "../Timer/Timer.hpp"

void Engine::Application::Run()
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

void Engine::Application::Init(const WindowProperty &props) { m_Window = Window::Create(props); }

void Engine::Application::Shutdown() { m_Window.reset(); }
