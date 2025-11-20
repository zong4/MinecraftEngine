#include "Application.hpp"

#include "Timer/Timer.hpp"

void MCEngine::Application::Run()
{
    ENGINE_PROFILE_FUNCTION();

    Timer timer;
    while (m_Window->IsRunning())
    {
        m_Window->Update(timer.GetElapsedSeconds());

        m_ImGuiLayer->BeginRenderImGui();
        m_Window->Render();
        m_ImGuiLayer->EndRenderImGui();
    }
}

void MCEngine::Application::Init(const WindowProperty &props)
{
    m_Window = std::make_shared<Window>(props);
    m_ImGuiLayer = std::make_shared<ImGuiLayer>(m_Window);
    m_ImGuiLayer->OnAttach();
}

void MCEngine::Application::Shutdown()
{
    m_ImGuiLayer->OnDetach();
    m_Window.reset();
}
