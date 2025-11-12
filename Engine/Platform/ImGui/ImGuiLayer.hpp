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

public:
    // Main loop
    void OnEvent(Event &event) override;
    void OnImGuiRender() override;

    // Called by LayerStack
    void OnAttach() override;
    void OnDetach() override;

protected:
    std::shared_ptr<Window> m_Window;
    std::string m_ImGuiFilePath;

protected:
    void BeginRenderImGui() const;
    virtual void RenderImGui() = 0;
    void EndRenderImGui() const;

    void SetDarkThemeColors();
};

} // namespace MCEngine