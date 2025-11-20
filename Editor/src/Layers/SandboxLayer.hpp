#pragma once

#include <Function.hpp>

namespace Editor
{

class SandboxLayer : public MCEngine::Layer
{

public:
    SandboxLayer(std::shared_ptr<MCEngine::Window> window);
    ~SandboxLayer() override = default;

public:
    void OnEvent(MCEngine::Event &event) override;
    void OnUpdate(float deltaTime) override;
    void OnRender() override;
    void OnImGuiRender() override;

private:
    std::shared_ptr<MCEngine::Window> m_Window;
    std::shared_ptr<MCEngine::Scene> m_EditorScene;
    std::shared_ptr<MCEngine::Scene> m_ActiveScene;

private:
    bool OnKeyEvent(MCEngine::KeyEvent &event);
};

} // namespace Editor