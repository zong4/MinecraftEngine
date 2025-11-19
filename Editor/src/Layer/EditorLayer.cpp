#include "EditorLayer.hpp"

#include "Scene/EditorScene.hpp"
#include "Scene/ExampleScene.hpp"
#include <imgui.h>

MCEditor::EditorLayer::EditorLayer(const std::shared_ptr<MCEngine::Window> &window) : ImGuiLayer(window, "EditorLayer")
{
    m_EditorScene = std::make_shared<MCEditor::EditorScene>();
    m_ActiveScene = std::make_shared<MCEditor::ExampleScene>();
    m_SceneViewport.SetCamera(m_EditorScene->GetMainCamera());
}

void MCEditor::EditorLayer::OnEvent(MCEngine::Event &event)
{
    ENGINE_PROFILE_FUNCTION();

    // Capture events in ImGuiLayer first
    ImGuiLayer::OnEvent(event);

    // Store key states in KeyCodeLibrary
    if (!event.IsHandled())
    {
        MCEngine::EventDispatcher dispatcher(event);

        // KeyEvent
        dispatcher.Dispatch<MCEngine::KeyEvent>([this](MCEngine::KeyEvent &event) {
            MCEngine::Input::GetInstance().SetKeyAction(event.GetCode(), event.GetAction());
            return false;
        });

        // MouseEvent
        {
            dispatcher.Dispatch<MCEngine::MouseButtonEvent>([this](MCEngine::MouseButtonEvent &event) {
                MCEngine::Input::GetInstance().SetKeyAction(event.GetCode(), event.GetAction());
                return false;
            });
            dispatcher.Dispatch<MCEngine::MouseMoveEvent>([this](MCEngine::MouseMoveEvent &event) {
                MCEngine::Input::GetInstance().SetPosition(event.GetX(), event.GetY());
                return false;
            });
            dispatcher.Dispatch<MCEngine::MouseScrollEvent>([this](MCEngine::MouseScrollEvent &event) {
                MCEngine::Input::GetInstance().SetScrollOffset(event.GetXOffset(), event.GetYOffset());
                return false;
            });
        }
    }

    // Handle key events for editor actions
    if (!event.IsHandled())
    {
        MCEngine::EventDispatcher dispatcher(event);
        dispatcher.Dispatch<MCEngine::KeyEvent>(std::function<bool(MCEngine::KeyEvent &)>(
            std::bind(&EditorLayer::OnKeyEvent, this, std::placeholders::_1)));
    }
}

bool MCEditor::EditorLayer::OnKeyEvent(MCEngine::KeyEvent &event) { return false; }

void MCEditor::EditorLayer::OnUpdate(float deltaTime)
{
    ENGINE_PROFILE_FUNCTION();

    // Update scenes
    {
        if (m_SceneViewport.IsFocused())
            m_EditorScene->Update(deltaTime);
        m_ActiveScene->Update(deltaTime);
    }
}

void MCEditor::EditorLayer::OnRender()
{
    ENGINE_PROFILE_FUNCTION();

    // Pre-render
    {
        m_ActiveScene->PreRender();
        m_ActiveScene->RenderShadowMap();
    }

    // Render viewports
    {
        m_SceneViewport.Render(m_ActiveScene);
    }
}

void MCEditor::EditorLayer::RenderImGui()
{
    ENGINE_PROFILE_FUNCTION();

    ImGui::Begin("Scene");
    m_SceneViewport.OnImGuiRender();
    ImGui::End();
}