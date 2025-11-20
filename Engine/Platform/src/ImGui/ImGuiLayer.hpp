#pragma once

#include <Core.hpp>

namespace Engine
{

class ImGuiLayer : public Layer
{
public:
    ImGuiLayer(void *nativeWindow);
    virtual ~ImGuiLayer() override = default;

    // Setters
    void SetBlockEvents(bool block) { m_BlockEvents = block; }

public:
    void OnAttach() override;
    void OnEvent(Event &event) override;
    void OnUpdate(float deltaTime) override {}
    void OnRender() override {}
    void BeginRenderImGui() const;
    virtual void OnImGuiRender() override {}
    void EndRenderImGui() const;
    void OnDetach() override;

protected:
    bool m_BlockEvents = true;
    void *m_NativeWindow;

private:
    void SetDarkThemeColors();
};

} // namespace Engine