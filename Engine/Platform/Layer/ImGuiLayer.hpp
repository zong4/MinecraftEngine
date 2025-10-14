#pragma once

#include "Window/Window.hpp"

namespace MCEngine
{

class ImGuiLayer : public Layer
{
public:
    ImGuiLayer(const std::shared_ptr<Window> &window, const std::string &filePath,
               const std::string &name = "ImGuiLayer");
    virtual ~ImGuiLayer() override = default;

    // Setters
    void SetBlockEvents(bool block) { m_BlockEvents = block; }

public:
    // Main loop
    void OnEvent(Event &event) override;
    void OnImGuiRender() override;

    // Called by LayerStack
    void OnAttach() override;
    void OnDetach() override;

protected:
    bool m_BlockEvents = true;
    std::shared_ptr<Window> m_Window;
    std::string m_ImGuiFilePath;

protected:
    void BeginRenderImGui() const;
    virtual void RenderImGui() = 0;
    void EndRenderImGui() const;

    void SetDarkThemeColors();
};

} // namespace MCEngine