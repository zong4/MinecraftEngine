#pragma once

#include "Event.hpp"

namespace MCEngine
{

class WindowResizeEvent : public MCEngine::Event
{
public:
    WindowResizeEvent(int width, int height) : m_Width(width), m_Height(height) {}

    int GetWidth() const { return m_Width; }
    int GetHeight() const { return m_Height; }

    std::string ToString() const override
    {
        std::stringstream ss;
        ss << "WindowResizeEvent: " << m_Width << ", " << m_Height;
        return ss.str();
    }

private:
    int m_Width;
    int m_Height;
};

} // namespace MCEngine