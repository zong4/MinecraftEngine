#include "CreatorLayer.hpp"

#include "../Scene/SceneManager.hpp"
#include "../Script/RotatingLight.hpp"
#include "../UI/FileBrowserPanel.hpp"
#include <imgui.h>

void Editor::CreatorLayer::OnEvent(Engine::Event &event)
{
    PROFILE_FUNCTION();

    Engine::EventDispatcher dispatcher(event);
    dispatcher.Dispatch<Engine::WindowResizeEvent>([this](Engine::WindowResizeEvent &event) {
        m_FbWidth = event.GetFbWidth();
        m_FbHeight = event.GetFbHeight();
        SceneManager::GetInstance().GetEditorScene()->Resize(m_FbWidth, m_FbHeight);
        SceneManager::GetInstance().GetActiveScene()->Resize(m_FbWidth, m_FbHeight);
        return true;
    });
}

void Editor::CreatorLayer::OnUpdate(float deltaTime)
{
    PROFILE_FUNCTION();

    if (Engine::Input::GetInstance().IsKeyReleased(KEY_SPACE))
    {
        m_Mode = (m_Mode == SandboxMode::Edit) ? SandboxMode::Play : SandboxMode::Edit;
    }

    if (Engine::Input::GetInstance().IsKeyReleased(KEY_R))
    {
        if (m_Mode == SandboxMode::Edit)
        {
            SceneManager::GetInstance().GetActiveScene() = std::make_shared<Editor::ExampleScene>();
            SceneManager::GetInstance().GetActiveScene()->Resize(m_FbWidth, m_FbHeight);
        }
    }

    switch (m_Mode)
    {
    case SandboxMode::Edit:
        SceneManager::GetInstance().GetEditorScene()->Update(deltaTime);
        SceneManager::GetInstance().GetEditorScene()->UpdateRuntime(deltaTime);
        SceneManager::GetInstance().GetActiveScene()->Update(deltaTime);
        break;
    case SandboxMode::Play:
        SceneManager::GetInstance().GetActiveScene()->Update(deltaTime);
        SceneManager::GetInstance().GetActiveScene()->UpdateRuntime(deltaTime);
        break;
    default:
        LOG_ENGINE_ASSERT("Unknown SandboxMode!");
        break;
    }
}

void Editor::CreatorLayer::OnRender()
{
    PROFILE_FUNCTION();

    switch (m_Mode)
    {
    case SandboxMode::Edit:
        SceneManager::GetInstance().GetActiveScene()->Render(
            SceneManager::GetInstance().GetEditorScene()->GetMainCamera());
        break;
    case SandboxMode::Play:
        SceneManager::GetInstance().GetActiveScene()->Render(
            SceneManager::GetInstance().GetActiveScene()->GetMainCamera());
        break;
    default:
        LOG_ENGINE_ASSERT("Unknown SandboxMode!");
        break;
    }
}

void Editor::CreatorLayer::OnImGuiRender()
{
    PROFILE_FUNCTION();

    FileBrowserPanel::GetInstance().OnImGuiRender();
    DrawDebugUI();
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

    ImGuiIO &io = ImGui::GetIO();
    bool mouseBlock = io.WantCaptureMouse && ImGui::IsWindowHovered(ImGuiHoveredFlags_AnyWindow);
    bool keyBlock = io.WantCaptureKeyboard && ImGui::IsAnyItemActive();
    Engine::Input::GetInstance().SetBlockEvents(mouseBlock || keyBlock);
    ImGui::Text("ImGui Blocking Events: %s", (mouseBlock || keyBlock) ? "True" : "False");

    ImGui::End();
}
