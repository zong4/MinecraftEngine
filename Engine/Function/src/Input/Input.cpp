#include "Input.hpp"

Engine::Input &Engine::Input::GetInstance()
{
    static Input instance;
    return instance;
}

bool Engine::Input::IsKeyDown(int keyCode) const
{
    PROFILE_FUNCTION();

    auto &&it = m_KeyActions.find(keyCode);
    if (it != m_KeyActions.end())
        return it->second == 1 || it->second == 2;
    return false;
}

bool Engine::Input::IsKeyPressed(int keyCode) const
{
    PROFILE_FUNCTION();

    auto &&it = m_KeyActions.find(keyCode);
    if (it != m_KeyActions.end())
        return it->second == 1;
    return false;
}

bool Engine::Input::IsKeyReleased(int keyCode) const
{
    PROFILE_FUNCTION();

    auto &&it = m_KeyActions.find(keyCode);
    if (it != m_KeyActions.end())
        return it->second == 0;
    return false;
}

bool Engine::Input::IsControlDown() const
{
    return IsKeyDown(KEY_LEFT_CONTROL) || IsKeyDown(KEY_RIGHT_CONTROL) || IsKeyDown(KEY_LEFT_SUPER) ||
           IsKeyDown(KEY_RIGHT_SUPER);
}

bool Engine::Input::IsShiftDown() const { return IsKeyDown(KEY_LEFT_SHIFT) || IsKeyDown(KEY_RIGHT_SHIFT); }

void Engine::Input::SetKeyAction(int keyCode, int action)
{
    if (m_BlockEvents)
        return;
    m_KeyActions[keyCode] = action;
}

void Engine::Input::SetPosition(double x, double y)
{
    if (m_BlockEvents)
        return;
    m_LastPosition = m_Position;
    m_Position = {x, y};
    m_DeltaPosition = m_Position - m_LastPosition;
}

void Engine::Input::SetScrollOffset(double xOffset, double yOffset)
{
    if (m_BlockEvents)
        return;
    m_ScrollOffset = {xOffset, yOffset};
}

void Engine::Input::Clear()
{
    m_DeltaPosition = {0.0, 0.0};
    m_ScrollOffset = {0.0, 0.0};
}