#include "EditorLayer.hpp"

#include "Scene/EditorScene.hpp"
#include "Scene/ExampleScene.hpp"
#include <imgui.h>

MCEditor::EditorLayer::EditorLayer(const std::shared_ptr<MCEngine::Window> &window) : ImGuiLayer(window, "EditorLayer")
{
    m_EditorScene = std::make_shared<MCEditor::EditorScene>();
    m_ActiveScene = std::make_shared<MCEditor::ExampleScene>();
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
        uint32_t viewportWidth = m_Window->GetProperty().Width;
        uint32_t viewportHeight = m_Window->GetProperty().Height;
        MCEngine::RendererCommand::SetViewport(0, 0, viewportWidth, viewportHeight);
        LOG_ENGINE_INFO("Viewport Size: " + std::to_string(viewportWidth) + "x" + std::to_string(viewportHeight));
        m_EditorScene->Resize((float)viewportWidth, (float)viewportHeight);
        m_ActiveScene->Resize((float)viewportWidth, (float)viewportHeight);
        m_ActiveScene->Render(m_EditorScene->GetMainCamera());

        // test: bvh
        MCEngine::BVH bvh(m_ActiveScene);
        bvh.Render(3);
    }
}

void MCEditor::EditorLayer::RenderImGui() { ENGINE_PROFILE_FUNCTION() }