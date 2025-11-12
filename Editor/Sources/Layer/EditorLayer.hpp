#pragma once

#include <Function.hpp>

namespace MCEditor
{

enum class EditorAction
{
    None = 0,
    NewScene,
    OpenScene,
    SaveSceneAs
};

enum class ImGuizmoType
{
    None = -1,
    Translate = 1,
    Rotate = 2,
    Scale = 3
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

private:
    EditorAction m_Action = EditorAction::None;
    ImGuizmoType m_GizmoType = ImGuizmoType::Translate;
    std::unique_ptr<MCEngine::FrameBuffer> m_EntityPickingFBO =
        std::make_unique<MCEngine::FrameBuffer>(MCEngine::FrameBufferType::Integer, 1280, 720);

protected:
    void RenderImGui() override;

private:
    void RenderGizmos();
    void PickEntity();
};

} // namespace MCEditor