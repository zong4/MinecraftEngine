#pragma once

#include "Event.hpp"

namespace MCEngine
{

class EventDispatcher
{
public:
    EventDispatcher(Event &event) : m_Event(event) {}

public:
    // clang-format off
    template <typename T>
    // clang-format on
    void Dispatch(const std::function<bool(T &)> &func)
    {
        if (T *event = dynamic_cast<T *>(&m_Event))
        {
            m_Event.SetHandled(func(*event));
        }
    }

private:
    Event &m_Event;
};

} // namespace MCEngine