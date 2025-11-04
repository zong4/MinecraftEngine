#pragma once

#include "Renderer/RendererAPIProperty.hpp"

namespace MCEngine
{

struct WindowProperty
{
    std::string Title;
    int Width;
    int Height;
    bool VSync;

public:
    WindowProperty(const std::string &title, int width, int height, bool vsync)
        : Title(title), Width(width), Height(height), VSync(vsync)
    {
    }
};

class Window
{
public:
    Window(const WindowProperty &property) : m_Property(property) { Init(); }
    ~Window() { Shutdown(); }

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
    void OnEvent(Event &e) { m_LayerStack.OnEvent(e); }
    void Update(float deltaTime);
    void Render() { m_LayerStack.Render(); }

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
