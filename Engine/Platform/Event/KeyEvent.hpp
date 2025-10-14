#pragma once

#include "Event/Event.hpp"

namespace MCEngine
{

class KeyEvent : public Event
{
public:
    KeyEvent(int code, int action) : m_Code(code), m_Action(action) {}

    int GetCode() const { return m_Code; }
    int GetAction() const { return m_Action; }

public:
    std::string ToString() const override
    {
        std::stringstream ss;
        ss << "KeyEvent: " << m_Code << ", " << m_Action;
        return ss.str();
    }

protected:
    int m_Code;
    int m_Action;
};

} // namespace MCEngine