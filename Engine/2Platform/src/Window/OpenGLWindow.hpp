#pragma once

#include "Window.hpp"

namespace Engine
{

class OpenGLWindow : public Window
{
public:
    OpenGLWindow(const WindowProperty &property) : Window(property) { Init(); }
    virtual ~OpenGLWindow() override = default;

protected:
    void Init() override;
};

} // namespace Engine
