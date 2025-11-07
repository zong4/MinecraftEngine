#include "Input.hpp"

#include "KeyCode.hpp"
#include "MouseButtonCode.hpp"

MCEngine::Input &MCEngine::Input::GetInstance()
{
    static Input instance;
    return instance;
}

bool MCEngine::Input::IsKeyDown(int keyCode) const
{
    ENGINE_PROFILE_FUNCTION();

    auto &&it = m_KeyActions.find(keyCode);
    if (it != m_KeyActions.end())
        return it->second == 1 || it->second == 2;
    return false;
}

bool MCEngine::Input::IsKeyPressed(int keyCode) const
{
    ENGINE_PROFILE_FUNCTION();

    auto &&it = m_KeyActions.find(keyCode);
    if (it != m_KeyActions.end())
        return it->second == 1;
    return false;
}

bool MCEngine::Input::IsKeyReleased(int keyCode) const
{
    ENGINE_PROFILE_FUNCTION();

    auto &&it = m_KeyActions.find(keyCode);
    if (it != m_KeyActions.end())
        return it->second == 0;
    return false;
}

bool MCEngine::Input::IsControlDown() const
{
    return IsKeyDown(ENGINE_KEY_LEFT_CONTROL) || IsKeyDown(ENGINE_KEY_RIGHT_CONTROL) ||
           IsKeyDown(ENGINE_KEY_LEFT_SUPER) || IsKeyDown(ENGINE_KEY_RIGHT_SUPER);
}

bool MCEngine::Input::IsShiftDown() const
{
    return IsKeyDown(ENGINE_KEY_LEFT_SHIFT) || IsKeyDown(ENGINE_KEY_RIGHT_SHIFT);
}

void MCEngine::Input::SetPosition(double x, double y)
{
    m_LastPosition = m_Position;
    m_Position = {x, y};
    m_DeltaPosition = m_Position - m_LastPosition;
}

void MCEngine::Input::Clear()
{
    m_DeltaPosition = {0.0f, 0.0f};
    m_ScrollOffset = {0.0f, 0.0f};
}