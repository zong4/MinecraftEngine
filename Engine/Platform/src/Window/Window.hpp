#pragma once

#include <Core.hpp>

namespace Engine
{

struct WindowProperty
{
    std::string Title;
    int Width;
    int Height;
    int FbWidth;
    int FbHeight;
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

    // Setters
    void SetRunning(bool running) { m_Running = running; }
    void SetVSync(bool enabled);
    void AddLayer(const std::shared_ptr<Layer> &layer) { m_LayerStack.PushLayer(layer); }
    void RemoveLayer(const std::shared_ptr<Layer> &layer) { m_LayerStack.PopLayer(layer); }

public:
    void OnEvent(Event &event) { m_LayerStack.OnEvent(event); }
    void Update(float deltaTime);
    void Render();

private:
    bool m_Running = true;
    void *m_NativeWindow = nullptr;
    WindowProperty m_Property;
    LayerStack m_LayerStack;

private:
    void Init();
    void SetCallbacks();
    void Shutdown();
};

} // namespace Engine
