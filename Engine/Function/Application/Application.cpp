#include "Application.hpp"

#include "Timer/Timer.hpp"

void MCEngine::Application::Run()
{
    ENGINE_PROFILE_FUNCTION();

    Timer timer;
    while (m_Window->IsRunning())
    {
        m_Window->Update(timer.GetElapsedSeconds());
        m_Window->Render();
    }
}

void MCEngine::Application::Init(const WindowProperty &props) { m_Window = std::make_shared<Window>(props); }

void MCEngine::Application::Shutdown() { m_Window.reset(); }
