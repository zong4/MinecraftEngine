#pragma once

#include <Core.hpp>

namespace Engine
{

class ImGuiLayer : public Layer
{
public:
    virtual ~ImGuiLayer() override = default;
    static std::shared_ptr<ImGuiLayer> Create(void *nativeWindow);

    // Setters
    void SetBlockEvents(bool block) { m_BlockEvents = block; }

public:
    void OnAttach() override;
    // void OnEvent(Event &event) override;
    virtual void BeginRenderImGui() const = 0;
    virtual void EndRenderImGui() const = 0;
    void OnDetach() override;

protected:
    bool m_BlockEvents = false;
    void *m_NativeWindow;

protected:
    ImGuiLayer(void *nativeWindow) : Layer("ImGuiLayer"), m_NativeWindow(nativeWindow) {}

protected:
    virtual void InitRenderer() = 0;
    virtual void ShutdownRenderer() = 0;

private:
    void SetDarkThemeColors();
};

} // namespace Engine