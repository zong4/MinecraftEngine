#pragma once

#include "Viewport.hpp"

namespace MCEditor
{

enum class ImGuizmoType
{
    None = -1,
    Translate = 1,
    Rotate = 2,
    Scale = 3
};

class SceneViewport : public Viewport
{
public:
    SceneViewport();
    ~SceneViewport() override = default;

    // Getters
    bool IsFocused() const { return m_Focused; }
    bool IsHovered() const { return m_Hovered; }

    // Setters
    void SetCamera(const MCEngine::Entity &camera) { m_Camera = camera; }

public:
    void Render(std::shared_ptr<MCEngine::Scene> scene) override;
    void OnImGuiRender() override;

private:
    bool m_Focused = false;
    bool m_Hovered = false;
    MCEngine::Entity m_Camera;
    ImGuizmoType m_GizmoType = ImGuizmoType::Translate;
    std::unique_ptr<MCEngine::FrameBuffer> m_EntityPickingFBO =
        std::make_unique<MCEngine::FrameBuffer>(MCEngine::FrameBufferType::Integer, 1280, 720);
};

} // namespace MCEditor