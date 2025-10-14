#pragma once

#include "Layer/LayerStack.hpp"
#include "Renderer/RendererAPIProperty.hpp"

namespace MCEngine
{

struct WindowProperty
{
    std::string Title;
    int Width;
    int Height;
    bool VSync;
    glm::vec4 ClearColor;

public:
    WindowProperty(const std::string &title, int width, int height, bool vsync, const glm::vec4 &clearColor)
        : Title(title), Width(width), Height(height), VSync(vsync), ClearColor(clearColor)
    {
    }
};

class Window
{
public:
    Window(const WindowProperty &property);
    ~Window();

    // Getters
    bool IsRunning() const;
    void *GetNativeWindow() const { return m_NativeWindow; }
    WindowProperty &GetProperty() { return m_Property; }
    const WindowProperty &GetProperty() const { return m_Property; }
    const RendererAPIProperty &GetRendererAPIProperty() const { return m_RendererAPIProperty; }

    // Setters
    void SetRunning(bool running) { m_Running = running; }
    void SetVSync(bool enabled);
    void AddLayer(const std::shared_ptr<Layer> &layer) { m_LayerStack.PushLayer(layer); }
    void RemoveLayer(const std::shared_ptr<Layer> &layer) { m_LayerStack.PopLayer(layer); }

public:
    // Main loop
    void OnEvent(Event &e);
    void Update(float deltaTime);
    void Render();

private:
    bool m_Running = true;
    void *m_NativeWindow = nullptr;
    WindowProperty m_Property;

    // Renderer
#ifdef __APPLE__
    RendererAPIProperty m_RendererAPIProperty = RendererAPIProperty(RendererAPI::OpenGL, 4, 1);
#else
    RendererAPIProperty m_RendererAPIProperty = RendererAPIProperty(RendererAPI::OpenGL, 4, 2);
#endif
    LayerStack m_LayerStack;

private:
    // Initialization
    void Init();
    void SetCallbacks();

    // Cleanup
    void Shutdown();
};

} // namespace MCEngine
