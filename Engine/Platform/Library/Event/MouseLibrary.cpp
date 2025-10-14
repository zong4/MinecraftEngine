#include "MouseLibrary.hpp"

MCEngine::MouseLibrary &MCEngine::MouseLibrary::GetInstance()
{
    static MouseLibrary instance;
    return instance;
}

bool MCEngine::MouseLibrary::IsButtonDown(int button) const
{
    ENGINE_PROFILE_FUNCTION();

    auto &&it = m_ButtonStates.find(button);
    if (it != m_ButtonStates.end())
    {
        return it->second == 1 || it->second == 2;
    }
    return false;
}

bool MCEngine::MouseLibrary::IsButtonPressed(int button) const
{
    ENGINE_PROFILE_FUNCTION();

    auto &&it = m_ButtonStates.find(button);
    if (it != m_ButtonStates.end())
    {
        return it->second == 1;
    }
    return false;
}

bool MCEngine::MouseLibrary::IsButtonReleased(int button) const
{
    ENGINE_PROFILE_FUNCTION();

    auto &&it = m_ButtonStates.find(button);
    if (it != m_ButtonStates.end())
    {
        return it->second == 0;
    }
    return false;
}

void MCEngine::MouseLibrary::SetPosition(double x, double y)
{
    ENGINE_PROFILE_FUNCTION();

    m_LastPosition = m_Position;
    m_Position = {x, y};
    m_DeltaPosition = m_Position - m_LastPosition;
}

void MCEngine::MouseLibrary::PostUpdate()
{
    ENGINE_PROFILE_FUNCTION();

    m_DeltaPosition = {0.0f, 0.0f};
}