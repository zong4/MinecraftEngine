#pragma once

#include "UI/SceneViewport.hpp"
#include "UI/Viewport.hpp"

namespace MCEditor
{

enum class EditorAction
{
    None = 0,
    NewScene,
    OpenScene,
    SaveSceneAs
};

class EditorLayer : public MCEngine::ImGuiLayer
{

public:
    EditorLayer(const std::shared_ptr<MCEngine::Window> &window);
    ~EditorLayer() override = default;

public:
    void OnEvent(MCEngine::Event &event) override;
    void OnUpdate(float deltaTime) override;
    void OnRender() override;

    bool OnKeyEvent(MCEngine::KeyEvent &event);

private:
    // Logic
    EditorAction m_Action = EditorAction::None;

    // Panels
    Viewport m_GameViewport;
    SceneViewport m_SceneViewport;

protected:
    void RenderImGui() override;

private:
    void RenderDockSpace();
    void RenderMenubar();
};

} // namespace MCEditor