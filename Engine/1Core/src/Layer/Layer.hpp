#pragma once

#include "../Event/Event.hpp"

namespace Engine
{

class Layer
{
public:
    Layer(const std::string &name) : m_Name(name) {}
    virtual ~Layer() = default;

    // Getters
    const std::string &GetName() const { return m_Name; }

public:
    // Sorted by execution order
    virtual void OnAttach() {}
    virtual void OnEvent(Event &event) {}
    virtual void OnUpdate(float deltaTime) {}
    virtual void OnRender() {}
    virtual void OnImGuiRender() {}
    virtual void OnPostUpdate() {}
    virtual void OnDetach() {}

private:
    std::string m_Name;
};

} // namespace Engine
