#include "LayerStack.hpp"

#include "Logger/Logger.hpp"

Engine::LayerStack::~LayerStack()
{
    for (const std::shared_ptr<Layer> &layer : m_Layers)
        layer->OnDetach();
    m_Layers.clear();
}

void Engine::LayerStack::PushLayer(const std::shared_ptr<Layer> &layer)
{
    m_Layers.emplace_back(layer);
    layer->OnAttach();
    LOG_ENGINE_INFO("Layer pushed: " + layer->GetName());
}

void Engine::LayerStack::PopLayer(const std::shared_ptr<Layer> &layer)
{
    auto &&it = std::find(m_Layers.begin(), m_Layers.end(), layer);
    if (it != m_Layers.end())
    {
        it->get()->OnDetach();
        m_Layers.erase(it);
        LOG_ENGINE_INFO("Layer popped: " + layer->GetName());
        return;
    }
    LOG_ENGINE_ASSERT("Layer not found in LayerStack: " + layer->GetName());
}

void Engine::LayerStack::OnEvent(Event &event)
{
    for (auto &&it = m_Layers.rbegin(); it != m_Layers.rend(); it++)
    {
        // So do not need to check it again in each layer
        if (event.IsHandled())
            break;
        it->get()->OnEvent(event);
    }
}

void Engine::LayerStack::Update(float deltaTime)
{
    for (const std::shared_ptr<Layer> &layer : m_Layers)
        layer->OnUpdate(deltaTime);

    for (const std::shared_ptr<Layer> &layer : m_Layers)
        layer->OnPostUpdate();
}

void Engine::LayerStack::Render()
{
    for (const std::shared_ptr<Layer> &layer : m_Layers)
    {
        layer->OnRender();
        layer->OnImGuiRender();
    }
}