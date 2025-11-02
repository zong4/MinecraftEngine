#include "KeyLibrary.hpp"

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