#pragma once

#include <Platform.hpp>

namespace Engine
{

class Input
{
public:
    static Input &GetInstance();

    // Getters
    bool IsKeyDown(int keyCode) const;
    bool IsKeyPressed(int keyCode) const;
    bool IsKeyReleased(int keyCode) const;
    bool IsControlDown() const;
    bool IsShiftDown() const;
    const glm::vec2 &GetDeltaPosition() const { return m_DeltaPosition; }
    const glm::vec2 &GetScrollOffset() const { return m_ScrollOffset; }

    // Setters
    void SetBlockEvents(bool block) { m_BlockEvents = block; }
    void SetKeyAction(int keyCode, int action);
    void SetPosition(double x, double y);
    void SetScrollOffset(double xOffset, double yOffset);
    void Clear();

private:
    bool m_BlockEvents = false;
    std::unordered_map<int, int> m_KeyActions;
    glm::vec2 m_Position = {0.0, 0.0};
    glm::vec2 m_LastPosition = {0.0, 0.0};
    glm::vec2 m_DeltaPosition = {0.0, 0.0};
    glm::vec2 m_ScrollOffset = {0.0, 0.0};

private:
    Input() = default;
    ~Input() = default;
};

} // namespace Engine