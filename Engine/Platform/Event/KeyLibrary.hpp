#pragma once

#include "pch.hpp"

namespace MCEngine
{

class KeyLibrary
{
public:
    static KeyLibrary &GetInstance();

    // Getters
    bool IsKeyDown(int keyCode) const;
    bool IsKeyPressed(int keyCode) const;
    bool IsKeyReleased(int keyCode) const;
    bool IsControlDown() const;
    bool IsShiftDown() const;

    // Setters
    void SetKeyAction(int keyCode, int action) { m_KeyStates[keyCode] = action; }

private:
    std::unordered_map<int, int> m_KeyStates;

private:
    KeyLibrary() = default;
    ~KeyLibrary() = default;
};

} // namespace MCEngine