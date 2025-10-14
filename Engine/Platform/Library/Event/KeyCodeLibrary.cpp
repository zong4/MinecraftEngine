#include "KeyCodeLibrary.hpp"

MCEngine::KeyCodeLibrary &MCEngine::KeyCodeLibrary::GetInstance()
{
    static KeyCodeLibrary instance;
    return instance;
}

bool MCEngine::KeyCodeLibrary::IsKeyDown(int keyCode) const
{
    ENGINE_PROFILE_FUNCTION();

    auto &&it = m_KeyStates.find(keyCode);
    if (it != m_KeyStates.end())
    {
        return it->second == 1 || it->second == 2;
    }
    return false;
}

bool MCEngine::KeyCodeLibrary::IsKeyPressed(int keyCode) const
{
    ENGINE_PROFILE_FUNCTION();

    auto &&it = m_KeyStates.find(keyCode);
    if (it != m_KeyStates.end())
    {
        return it->second == 1;
    }
    return false;
}

bool MCEngine::KeyCodeLibrary::IsKeyReleased(int keyCode) const
{
    ENGINE_PROFILE_FUNCTION();

    auto &&it = m_KeyStates.find(keyCode);
    if (it != m_KeyStates.end())
    {
        return it->second == 0;
    }
    return false;
}