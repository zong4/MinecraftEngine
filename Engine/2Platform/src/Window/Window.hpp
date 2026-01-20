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
    static std::shared_ptr<Window> Create(const WindowProperty &property);

    // Getters
    bool IsRunning() const;
    void *GetNativeWindow() const { return m_NativeWindow; }
    WindowProperty &GetProperty() { return m_Property; }

    // Setters
    void SetVSync(bool enabled);
    void PushLayer(const std::shared_ptr<Layer> &layer) { m_LayerStack.PushLayer(layer); }
    void PopLayer(const std::shared_ptr<Layer> &layer) { m_LayerStack.PopLayer(layer); }

public:
    void OnEvent(Event &event);
    void Update(float deltaTime);
    void Render();
    void PostUpdate();

protected:
    bool m_FirstFrame = true;
    void *m_NativeWindow = nullptr;
    WindowProperty m_Property;
    LayerStack m_LayerStack;

protected:
    Window(const WindowProperty &property) : m_Property(property) {}
    virtual ~Window() { Shutdown(); }

protected:
    virtual void Init() = 0;
    void SetCallbacks();
    void Shutdown();
};

} // namespace Engine