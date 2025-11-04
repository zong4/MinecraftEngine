#pragma once

#include "Event/Event.hpp"

namespace MCEngine
{

class Layer
{
public:
    Layer(const std::string &name) : m_Name(name) {}
    virtual ~Layer() = default;

    // Getters
    const std::string &GetName() const { return m_Name; }

public:
    // Main loop
    virtual void OnEvent(Event &event) = 0;
    virtual void OnUpdate(float deltaTime) = 0;
    virtual void OnRender() = 0;
    virtual void OnImGuiRender() = 0;

    // Called by LayerStack
    virtual void OnAttach() = 0;
    virtual void OnDetach() = 0;

private:
    std::string m_Name;
};

} // namespace MCEngine
