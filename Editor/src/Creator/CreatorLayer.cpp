#include "CreatorLayer.hpp"

#include "../Scene/EditorScene.hpp"
#include "../Scene/ExampleScene.hpp"
#include "../Scene/SceneManager.hpp"
#include "../UI/FileBrowserPanel.hpp"
#include <imgui.h>

void Editor::CreatorLayer::OnEvent(Engine::Event &event)
{
    PROFILE_FUNCTION();

    Engine::EventDispatcher dispatcher(event);

    // Handle window resize events
    dispatcher.Dispatch<Engine::WindowResizeEvent>([this](Engine::WindowResizeEvent &event) {
        SceneManager::GetInstance().Resize(event.GetFbWidth(), event.GetFbHeight());
        return true;
    });
}

void Editor::CreatorLayer::OnUpdate(float deltaTime)
{
    PROFILE_FUNCTION();

    // Toggle Sandbox Mode
    if (Engine::Input::GetInstance().IsKeyReleased(KEY_SPACE))
    {
        m_Mode = (m_Mode == SandboxMode::Edit) ? SandboxMode::Play : SandboxMode::Edit;
    }

    // Reset Scene in Edit Mode
    if (Engine::Input::GetInstance().IsKeyReleased(KEY_R))
    {
        if (m_Mode == SandboxMode::Edit)
        {
            SceneManager::GetInstance().SetActiveScene(std::make_shared<Editor::ExampleScene>());
        }
    }

    // Update Scenes based on Mode
    switch (m_Mode)
    {
    case SandboxMode::Edit:
        SceneManager::GetInstance().UpdateInEditor(deltaTime);
        break;
    case SandboxMode::Play:
        SceneManager::GetInstance().UpdateInRuntime(deltaTime);
        break;
    default:
        LOG_ENGINE_ASSERT("Unknown SandboxMode!");
        break;
    }
}

void Editor::CreatorLayer::OnRender()
{
    PROFILE_FUNCTION();

    // Render Scenes based on Mode
    switch (m_Mode)
    {
    case SandboxMode::Edit:
        SceneManager::GetInstance().RenderInEditor();
        break;
    case SandboxMode::Play:
        SceneManager::GetInstance().RenderInRuntime();
        break;
    default:
        LOG_ENGINE_ASSERT("Unknown SandboxMode!");
        break;
    }
}

void Editor::CreatorLayer::OnImGuiRender()
{
    PROFILE_FUNCTION();

    // File Browser Panel
    FileBrowserPanel::GetInstance().OnImGuiRender();

    // Debug UI
    DrawDebugUI();

    // Capture Input Events for next frame
    ImGuiIO &io = ImGui::GetIO();
    bool mouseCapture = io.WantCaptureMouse && ImGui::IsWindowHovered(ImGuiHoveredFlags_AnyWindow);
    bool keyboardCapture = io.WantCaptureKeyboard && ImGui::IsAnyItemActive();
    Engine::Input::GetInstance().SetBlockEvents(mouseCapture || keyboardCapture);
}

void Editor::CreatorLayer::DrawDebugUI()
{
    PROFILE_FUNCTION();

    ImGui::Begin("Debug Info");

    // Sandbox Mode
    if (m_Mode == SandboxMode::Edit)
        ImGui::Text("Mode: Edit (Press SPACE to Switch to Play)");
    else
        ImGui::Text("Mode: Play (Press SPACE to Switch to Edit)");
    ImGui::Text("Press R to Reset Scene in Edit Mode");

    // Input Blocking Info
    ImGuiIO &io = ImGui::GetIO();
    bool mouseCapture = io.WantCaptureMouse && ImGui::IsWindowHovered(ImGuiHoveredFlags_AnyWindow);
    bool keyboardCapture = io.WantCaptureKeyboard && ImGui::IsAnyItemActive();
    ImGui::Text("ImGui Blocking Events: %s", (mouseCapture || keyboardCapture) ? "True" : "False");

    ImGui::End();
}
