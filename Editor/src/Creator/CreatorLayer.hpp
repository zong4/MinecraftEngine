#pragma once

#include <Function.hpp>

namespace Editor
{

enum class SandboxMode
{
    Edit = 0,
    Play = 1,
};

class CreatorLayer : public Engine::Layer
{

public:
    CreatorLayer() : Engine::Layer("CreatorLayer") {}
    ~CreatorLayer() override = default;

public:
    void OnEvent(Engine::Event &event) override;
    void OnUpdate(float deltaTime) override;
    void OnRender() override;
    void OnImGuiRender() override;

private:
    SandboxMode m_Mode = SandboxMode::Edit;
    int m_FbWidth, m_FbHeight;

private:
    void DrawDebugUI();
};

} // namespace Editor