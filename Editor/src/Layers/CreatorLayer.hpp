#pragma once

#include <Function.hpp>

namespace Editor
{

enum class SandboxMode
{
    Edit = 0,
    Play = 1,
    Pause = 2
};

class CreatorLayer : public Engine::Layer
{

public:
    CreatorLayer(std::shared_ptr<Engine::Window> window);
    ~CreatorLayer() override = default;

public:
    void OnUpdate(float deltaTime) override;
    void OnRender() override;
    void OnImGuiRender() override;

private:
    SandboxMode m_Mode = SandboxMode::Edit;
    uint32_t m_ViewportWidth = 0, m_ViewportHeight = 0;
    std::shared_ptr<Engine::Window> m_Window;
    std::shared_ptr<Engine::Scene> m_EditorScene;
    std::shared_ptr<Engine::Scene> m_ActiveScene;
};

} // namespace Editor