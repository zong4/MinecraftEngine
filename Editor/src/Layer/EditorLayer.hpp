#pragma once

#include <Function.hpp>

namespace Editor
{

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
    std::shared_ptr<MCEngine::Scene> m_EditorScene;
    std::shared_ptr<MCEngine::Scene> m_ActiveScene;

protected:
    void RenderImGui() override;

private:
    bool OnKeyEvent(MCEngine::KeyEvent &event);
};

} // namespace Editor