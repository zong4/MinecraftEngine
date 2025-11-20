#include "SandboxLayer.hpp"

#include "Scenes/EditorScene.hpp"
#include "Scenes/ExampleScene.hpp"
#include <imgui.h>

Editor::SandboxLayer::SandboxLayer(std::shared_ptr<Engine::Window> window)
    : Engine::Layer("SandboxLayer"), m_Window(window)
{
    m_EditorScene = std::make_shared<Editor::EditorScene>();
    m_ActiveScene = std::make_shared<Editor::ExampleScene>();
}

void Editor::SandboxLayer::OnEvent(Engine::Event &event)
{
    PROFILE_FUNCTION();

    if (!event.IsHandled())
    {
        Engine::EventDispatcher dispatcher(event);
        dispatcher.Dispatch<Engine::KeyEvent>(
            std::function<bool(Engine::KeyEvent &)>(std::bind(&SandboxLayer::OnKeyEvent, this, std::placeholders::_1)));
    }
}

void Editor::SandboxLayer::OnUpdate(float deltaTime)
{
    PROFILE_FUNCTION();

    if (m_Mode == SandboxMode::Edit)
        m_EditorScene->Update(deltaTime);
    else
        m_ActiveScene->Update(deltaTime);
}

void Editor::SandboxLayer::OnRender()
{
    PROFILE_FUNCTION();

    // Pre-render
    {
        m_ActiveScene->PreRender();
        m_ActiveScene->RenderShadowMap();
    }

    // Render
    {
        uint32_t viewportWidth = m_Window->GetProperty().FbWidth;
        uint32_t viewportHeight = m_Window->GetProperty().FbHeight;
        Engine::RendererCommand::SetViewport(0, 0, viewportWidth, viewportHeight);
        m_EditorScene->Resize((float)viewportWidth, (float)viewportHeight);
        m_ActiveScene->Resize((float)viewportWidth, (float)viewportHeight);

        if (m_Mode == SandboxMode::Edit)
            m_ActiveScene->Render(m_EditorScene->GetMainCamera());
        else
            m_ActiveScene->Render(m_ActiveScene->GetMainCamera());

        // test: bvh
        Engine::BVH bvh(m_ActiveScene);
        bvh.Render(3);
    }
}

void Editor::SandboxLayer::OnImGuiRender()
{
    PROFILE_FUNCTION();

    ImGui::Begin("Sandbox Layer Debug");
    if (m_Mode == SandboxMode::Edit)
        ImGui::Text("Mode: Edit (Press SPACE to Simulate)");
    else
        ImGui::Text("Mode: Simulate (Press ESCAPE to Edit)");
    ImGui::End();
}

bool Editor::SandboxLayer::OnKeyEvent(Engine::KeyEvent &event)
{
    if (Engine::Input::GetInstance().IsKeyPressed(KEY_SPACE))
    {
        m_Mode = SandboxMode::Simulate;
        return true;
    }
    else if (Engine::Input::GetInstance().IsKeyPressed(KEY_ESCAPE))
    {
        m_Mode = SandboxMode::Edit;
        return true;
    }

    return false;
}