#pragma once

#include "pch.hpp"

namespace MCEngine
{

class KeyCodeLibrary
{
public:
    static KeyCodeLibrary &GetInstance();

    // Getters
    bool IsKeyDown(int keyCode) const;
    bool IsKeyPressed(int keyCode) const;
    bool IsKeyReleased(int keyCode) const;

    // Setters
    void SetKeyAction(int keyCode, int action) { m_KeyStates[keyCode] = action; }

private:
    std::unordered_map<int, int> m_KeyStates;

private:
    KeyCodeLibrary() = default;
    ~KeyCodeLibrary() = default;
};

} // namespace MCEngine