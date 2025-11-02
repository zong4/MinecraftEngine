#include "EditorLayer.hpp"

#include "Manager/ConfigManager.hpp"
#include "Manager/SceneManager.hpp"
#include <imgui.h>

MCEditor::EditorLayer::EditorLayer(const std::shared_ptr<MCEngine::Window> &window)
    : ImGuiLayer(window, (ConfigManager::GetInstance().GetConfigsPath() / "imgui.ini").string(), "EditorLayer")
{
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
            MCEngine::KeyCodeLibrary::GetInstance().SetKeyAction(event.GetCode(), event.GetAction());
            return false;
        });

        // MouseEvent
        dispatcher.Dispatch<MCEngine::MouseButtonEvent>([this](MCEngine::MouseButtonEvent &event) {
            MCEngine::MouseLibrary::GetInstance().SetButtonState(event.GetCode(), event.GetAction());
            return false;
        });
        dispatcher.Dispatch<MCEngine::MouseMoveEvent>([this](MCEngine::MouseMoveEvent &event) {
            MCEngine::MouseLibrary::GetInstance().SetPosition(event.GetX(), event.GetY());
            return false;
        });
        dispatcher.Dispatch<MCEngine::MouseScrollEvent>([this](MCEngine::MouseScrollEvent &event) {
            MCEngine::MouseLibrary::GetInstance().SetScrollOffset(event.GetXOffset(), event.GetYOffset());
            return false;
        });
    }

    // Handle key events for editor actions
    if (!event.IsHandled())
    {
        MCEngine::EventDispatcher dispatcher(event);
        dispatcher.Dispatch<MCEngine::KeyEvent>(std::function<bool(MCEngine::KeyEvent &)>(
            std::bind(&EditorLayer::OnKeyEvent, this, std::placeholders::_1)));
    }
}

bool MCEditor::EditorLayer::OnKeyEvent(MCEngine::KeyEvent &event)
{
    // Key Pressed for imguizmo type change
    if (event.GetAction() == 1)
    {
        switch (event.GetCode())
        {
        case ENGINE_KEY_Q:
            m_SceneViewport.SetGizmoType(ImGuizmoType::None);
            return true;
        case ENGINE_KEY_W:
            m_SceneViewport.SetGizmoType(ImGuizmoType::Translate);
            return true;
        case ENGINE_KEY_E:
            m_SceneViewport.SetGizmoType(ImGuizmoType::Rotate);
            return true;
        case ENGINE_KEY_R:
            m_SceneViewport.SetGizmoType(ImGuizmoType::Scale);
            return true;
        default:
            break;
        }
    }

    // Key Pressed for editor actions
    if (event.GetAction() == 1)
    {
        bool control = MCEngine::KeyCodeLibrary::GetInstance().IsKeyDown(ENGINE_KEY_LEFT_CONTROL) ||
                       MCEngine::KeyCodeLibrary::GetInstance().IsKeyDown(ENGINE_KEY_RIGHT_CONTROL) ||
                       MCEngine::KeyCodeLibrary::GetInstance().IsKeyDown(ENGINE_KEY_LEFT_SUPER) ||
                       MCEngine::KeyCodeLibrary::GetInstance().IsKeyDown(ENGINE_KEY_RIGHT_SUPER);
        bool shift = MCEngine::KeyCodeLibrary::GetInstance().IsKeyDown(ENGINE_KEY_LEFT_SHIFT) ||
                     MCEngine::KeyCodeLibrary::GetInstance().IsKeyDown(ENGINE_KEY_RIGHT_SHIFT);

        switch (event.GetCode())
        {
        case ENGINE_KEY_N:
            if (control)
                m_Action = EditorAction::NewScene;
            return true;
        case ENGINE_KEY_O:
            if (control)
                m_Action = EditorAction::OpenScene;
            return true;
        case ENGINE_KEY_S:
            if (control && shift)
                m_Action = EditorAction::SaveSceneAs;
            return true;
        default:
            break;
        }
    }

    return false;
}

void MCEditor::EditorLayer::OnUpdate(float deltaTime)
{
    ENGINE_PROFILE_FUNCTION();

    // Handle editor actions
    switch (m_Action)
    {
    case EditorAction::NewScene:
        SceneManager::GetInstance().NewExampleScene();
        break;
    case EditorAction::OpenScene:
        SceneManager::GetInstance().OpenSceneDialog();
        break;
    case EditorAction::SaveSceneAs:
        SceneManager::GetInstance().SaveSceneAsDialog();
        break;
    default:
        break;
    }
    m_Action = EditorAction::None;

    // Update scenes
    if (m_SceneViewport.IsFocused())
    {
        SceneManager::GetInstance().GetEditorScene()->Update(deltaTime);
    }
    SceneManager::GetInstance().GetActiveScene()->Update(deltaTime);
}

void MCEditor::EditorLayer::OnRender()
{
    ENGINE_PROFILE_FUNCTION();

    auto &&sceneManager = SceneManager::GetInstance();

    // Once per frame
    sceneManager.GetActiveScene()->PreRender();
    sceneManager.GetActiveScene()->RenderShadowMap();

    // Render viewports
    m_GameViewport.Render();
    m_SceneViewport.Render();
}

void MCEditor::EditorLayer::RenderImGui()
{
    ENGINE_PROFILE_FUNCTION();

    RenderDockSpace();

    m_Toolbar.OnImGuiRender();
    m_FileBrowserPanel.OnImGuiRender();
    m_HierarchyPanel.OnImGuiRender();
    m_InspectorPanel.OnImGuiRender();

    ImGui::Begin("Game");
    m_GameViewport.OnImGuiRender();
    ImGui::End();

    ImGui::Begin("Scene");
    m_SceneViewport.OnImGuiRender();

    // Receive drag and drop
    if (ImGui::BeginDragDropTarget())
    {
        if (const ImGuiPayload *payload = ImGui::AcceptDragDropPayload("CONTENT_BROWSER_ITEM"))
        {
            if (payload->Data)
            {
                const char *path = static_cast<const char *>(payload->Data);
                std::filesystem::path filepath(path);
                if (std::filesystem::is_directory(filepath))
                {
                    m_FileBrowserPanel.SetCurrentDirectory(filepath);
                }
                else
                {
                    if (ConfigManager::IsScene(filepath))
                    {
                        SceneManager::GetInstance().OpenScene(path);
                    }
                }
            }
        }
        ImGui::EndDragDropTarget();
    }
    ImGui::End();

    // Logic
    SetBlockEvents(!m_SceneViewport.IsFocused() && !m_SceneViewport.IsHovered());
}

void MCEditor::EditorLayer::RenderDockSpace()
{
    ENGINE_PROFILE_FUNCTION();

    // Note: Switch this to true to enable dockspace
    static bool dockspaceOpen = true;
    static bool opt_fullscreen_persistant = true;
    bool opt_fullscreen = opt_fullscreen_persistant;
    static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;

    // We are using the ImGuiWindowFlags_NoDocking flag to make the parent window not dockable into,
    // because it would be confusing to have two docking targets within each others.
    ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
    if (opt_fullscreen)
    {
        ImGuiViewport *viewport = ImGui::GetMainViewport();
        ImGui::SetNextWindowPos(viewport->Pos);
        ImGui::SetNextWindowSize(viewport->Size);
        ImGui::SetNextWindowViewport(viewport->ID);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
        window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize |
                        ImGuiWindowFlags_NoMove;
        window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
    }

    // When using ImGuiDockNodeFlags_PassthruCentralNode, DockSpace() will render our background and handle the
    // pass-thru hole, so we ask Begin() to not render a background.
    if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
        window_flags |= ImGuiWindowFlags_NoBackground;

    // Important: note that we proceed even if Begin() returns false (aka window is collapsed).
    // This is because we want to keep our DockSpace() active. If a DockSpace() is inactive,
    // all active windows docked into it will lose their parent and become undocked.
    // We cannot preserve the docking relationship between an active window and an inactive docking, otherwise
    // any change of dockspace/settings would lead to windows being stuck in limbo and never being visible.
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
    ImGui::Begin("DockSpace", &dockspaceOpen, window_flags);
    ImGui::PopStyleVar();

    if (opt_fullscreen)
        ImGui::PopStyleVar(2);

    // DockSpace
    ImGuiIO &io = ImGui::GetIO();
    ImGuiStyle &style = ImGui::GetStyle();
    float minWinSizeX = style.WindowMinSize.x;
    style.WindowMinSize.x = 200.0f; // todo
    if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
    {
        ImGuiID dockspace_id = ImGui::GetID("DockSpace");
        ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
    }
    style.WindowMinSize.x = minWinSizeX;

    RenderMenubar();

    ImGui::End();
}

void MCEditor::EditorLayer::RenderMenubar()
{
    ENGINE_PROFILE_FUNCTION();

    if (ImGui::BeginMenuBar())
    {
        if (ImGui::BeginMenu("File"))
        {
            if (ImGui::MenuItem("New", "Ctrl+N"))
                SceneManager::GetInstance().NewExampleScene();
            if (ImGui::MenuItem("Open...", "Ctrl+O"))
                SceneManager::GetInstance().OpenSceneDialog();
            if (ImGui::MenuItem("Save As...", "Ctrl+Shift+S"))
                SceneManager::GetInstance().SaveSceneAsDialog();

            if (ImGui::MenuItem("Exit"))
            {
                m_Window->SetRunning(false);
            }
            ImGui::EndMenu();
        }
        ImGui::EndMenuBar();
    }
}
