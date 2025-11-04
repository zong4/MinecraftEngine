#include "KeyLibrary.hpp"

#include "Event/KeyCode.hpp"

MCEngine::KeyLibrary &MCEngine::KeyLibrary::GetInstance()
{
    static KeyLibrary instance;
    return instance;
}

bool MCEngine::KeyLibrary::IsKeyDown(int keyCode) const
{
    ENGINE_PROFILE_FUNCTION();

    auto &&it = m_KeyStates.find(keyCode);
    if (it != m_KeyStates.end())
    {
        return it->second == 1 || it->second == 2;
    }
    return false;
}

bool MCEngine::KeyLibrary::IsKeyPressed(int keyCode) const
{
    ENGINE_PROFILE_FUNCTION();

    auto &&it = m_KeyStates.find(keyCode);
    if (it != m_KeyStates.end())
    {
        return it->second == 1;
    }
    return false;
}

bool MCEngine::KeyLibrary::IsKeyReleased(int keyCode) const
{
    ENGINE_PROFILE_FUNCTION();

    auto &&it = m_KeyStates.find(keyCode);
    if (it != m_KeyStates.end())
    {
        return it->second == 0;
    }
    return false;
}

bool MCEngine::KeyLibrary::IsControlDown() const
{
    return IsKeyDown(ENGINE_KEY_LEFT_CONTROL) || IsKeyDown(ENGINE_KEY_RIGHT_CONTROL) ||
           IsKeyDown(ENGINE_KEY_LEFT_SUPER) || IsKeyDown(ENGINE_KEY_RIGHT_SUPER);
}

bool MCEngine::KeyLibrary::IsShiftDown() const
{
    return IsKeyDown(ENGINE_KEY_LEFT_SHIFT) || IsKeyDown(ENGINE_KEY_RIGHT_SHIFT);
}