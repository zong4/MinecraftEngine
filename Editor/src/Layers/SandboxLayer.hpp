#pragma once

#include <Function.hpp>

namespace Editor
{

class SandboxLayer : public Engine::Layer
{

public:
    SandboxLayer(std::shared_ptr<Engine::Window> window);
    ~SandboxLayer() override = default;

public:
    void OnEvent(Engine::Event &event) override;
    void OnUpdate(float deltaTime) override;
    void OnRender() override;
    void OnImGuiRender() override;

private:
    std::shared_ptr<Engine::Window> m_Window;
    std::shared_ptr<Engine::Scene> m_EditorScene;
    std::shared_ptr<Engine::Scene> m_ActiveScene;

private:
    bool OnKeyEvent(Engine::KeyEvent &event);
};

} // namespace Editor