#include "SandboxLayer.hpp"

#include "Scene/EditorScene.hpp"
#include "Scene/ExampleScene.hpp"
#include <imgui.h>

Editor::SandboxLayer::SandboxLayer(std::shared_ptr<MCEngine::Window> window) : ImGuiLayer(window, "SandboxLayer")
{
    m_EditorScene = std::make_shared<Editor::EditorScene>();
    m_ActiveScene = std::make_shared<Editor::ExampleScene>();
}

void Editor::SandboxLayer::OnEvent(MCEngine::Event &event)
{
    ENGINE_PROFILE_FUNCTION();

    // Capture events in ImGuiLayer first
    ImGuiLayer::OnEvent(event);

    // Then dispatch to SandboxLayer if not handled
    if (!event.IsHandled())
    {
        MCEngine::EventDispatcher dispatcher(event);
        dispatcher.Dispatch<MCEngine::KeyEvent>(std::function<bool(MCEngine::KeyEvent &)>(
            std::bind(&SandboxLayer::OnKeyEvent, this, std::placeholders::_1)));
    }
}

void Editor::SandboxLayer::OnUpdate(float deltaTime)
{
    m_EditorScene->Update(deltaTime);
    // m_ActiveScene->Update(deltaTime);
}

void Editor::SandboxLayer::OnImGuiRender()
{
    ENGINE_PROFILE_FUNCTION();

    ImGuiLayer::OnImGuiRender();

    ImGui::Begin("Sandbox Layer");
    ImGui::Text("This is the Sandbox Layer.");
    ImGui::End();
}

void Editor::SandboxLayer::OnRender()
{
    ENGINE_PROFILE_FUNCTION();

    // Pre-render
    {
        m_ActiveScene->PreRender();
        m_ActiveScene->RenderShadowMap();
    }

    // Render
    {
        uint32_t viewportWidth = m_Window->GetProperty().FbWidth;
        uint32_t viewportHeight = m_Window->GetProperty().FbHeight;
        MCEngine::RendererCommand::SetViewport(0, 0, viewportWidth, viewportHeight);
        m_EditorScene->Resize((float)viewportWidth, (float)viewportHeight);
        m_ActiveScene->Resize((float)viewportWidth, (float)viewportHeight);
        m_ActiveScene->Render(m_EditorScene->GetMainCamera());

        // test: bvh
        MCEngine::BVH bvh(m_ActiveScene);
        bvh.Render(3);
    }
}

bool Editor::SandboxLayer::OnKeyEvent(MCEngine::KeyEvent &event) { return false; }