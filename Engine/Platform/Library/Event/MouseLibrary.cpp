#include "MouseLibrary.hpp"

MCEngine::MouseLibrary &MCEngine::MouseLibrary::GetInstance()
{
    static MouseLibrary instance;
    return instance;
}

bool MCEngine::MouseLibrary::IsButtonDown(int button) const
{
    auto &&it = m_ButtonStates.find(button);
    if (it != m_ButtonStates.end())
    {
        return it->second == 1 || it->second == 2;
    }
    return false;
}

bool MCEngine::MouseLibrary::IsButtonPressed(int button) const
{
    auto &&it = m_ButtonStates.find(button);
    if (it != m_ButtonStates.end())
    {
        return it->second == 1;
    }
    return false;
}

bool MCEngine::MouseLibrary::IsButtonReleased(int button) const
{
    auto &&it = m_ButtonStates.find(button);
    if (it != m_ButtonStates.end())
    {
        return it->second == 0;
    }
    return false;
}

void MCEngine::MouseLibrary::SetPosition(double x, double y)
{
    m_LastPosition = m_Position;
    m_Position = {x, y};
    m_DeltaPosition = m_Position - m_LastPosition;
}

void MCEngine::MouseLibrary::Clear()
{
    m_DeltaPosition = {0.0f, 0.0f};
    m_ScrollOffset = {0.0f, 0.0f};
}