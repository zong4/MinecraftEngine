#pragma once

#include "../Event/Event.hpp"

namespace Engine
{

class Layer
{
public:
    Layer(const std::string &name) : m_Name(name) {}
    virtual ~Layer() = default;

    const std::string &GetName() const { return m_Name; }

public:
    virtual void OnAttach() {}
    virtual void OnEvent(Event &event) = 0;
    virtual void OnUpdate(float deltaTime) {}
    virtual void OnPostUpdate() {}
    virtual void OnRender() {}
    virtual void OnImGuiRender() {}
    virtual void OnDetach() {}

private:
    std::string m_Name;
};

} // namespace Engine
