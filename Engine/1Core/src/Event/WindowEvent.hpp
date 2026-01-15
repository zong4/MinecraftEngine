#pragma once

#include "Event.hpp"

namespace Engine
{

// framebuffer size can be different from window size on high-DPI monitors
class WindowResizeEvent : public Engine::Event
{
public:
    WindowResizeEvent(int width, int height, int fbWidth, int fbHeight)
        : m_Width(width), m_Height(height), m_FbWidth(fbWidth), m_FbHeight(fbHeight)
    {
    }

    int GetWidth() const { return m_Width; }
    int GetHeight() const { return m_Height; }
    int GetFramebufferWidth() const { return m_FbWidth; }
    int GetFramebufferHeight() const { return m_FbHeight; }

    std::string ToString() const override
    {
        std::stringstream ss;
        ss << "WindowResizeEvent: " << m_Width << ", " << m_Height << ", " << m_FbWidth << ", " << m_FbHeight;
        return ss.str();
    }

private:
    int m_Width;
    int m_Height;
    int m_FbWidth;
    int m_FbHeight;
};

} // namespace Engine