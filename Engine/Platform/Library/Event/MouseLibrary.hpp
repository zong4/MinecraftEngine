#pragma once

#include "pch.hpp"

namespace MCEngine
{

class MouseLibrary
{
public:
    static MouseLibrary &GetInstance();

    // Getters
    bool IsButtonDown(int button) const;
    bool IsButtonPressed(int button) const;
    bool IsButtonReleased(int button) const;
    // const glm::vec2 &GetPosition() const { return m_Position; }
    // const glm::vec2 &GetLastPosition() const { return m_LastPosition; }
    const glm::vec2 &GetDeltaPosition() const { return m_DeltaPosition; }
    const glm::vec2 &GetScrollOffset() const { return m_ScrollOffset; }

    // Setters
    void SetButtonState(int button, int action) { m_ButtonStates[button] = action; }
    void SetPosition(double x, double y);
    void SetScrollOffset(double xOffset, double yOffset) { m_ScrollOffset = {xOffset, yOffset}; }

public:
    void PostUpdate();

private:
    std::unordered_map<int, int> m_ButtonStates;
    glm::vec2 m_Position = {0.0, 0.0};
    glm::vec2 m_LastPosition = {0.0, 0.0};
    glm::vec2 m_DeltaPosition = {0.0, 0.0};
    glm::vec2 m_ScrollOffset = {0.0, 0.0};

private:
    MouseLibrary() = default;
    ~MouseLibrary() = default;
};

} // namespace MCEngine