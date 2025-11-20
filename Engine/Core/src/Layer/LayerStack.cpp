#include "LayerStack.hpp"

#include "Logger/Logger.hpp"

MCEngine::LayerStack::~LayerStack()
{
    for (const std::shared_ptr<Layer> &layer : m_Layers)
        layer->OnDetach();
    m_Layers.clear();
}

void MCEngine::LayerStack::PushLayer(const std::shared_ptr<Layer> &layer)
{
    m_Layers.emplace_back(layer);
    layer->OnAttach();
    LOG_ENGINE_INFO("Layer pushed: " + layer->GetName());
}

void MCEngine::LayerStack::PopLayer(const std::shared_ptr<Layer> &layer)
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

void MCEngine::LayerStack::OnEvent(Event &event)
{
    for (auto &&it = m_Layers.rbegin(); it != m_Layers.rend(); it++)
    {
        // So do not need to check it again in each layer
        if (event.IsHandled())
            break;
        it->get()->OnEvent(event);
    }
}

void MCEngine::LayerStack::Update(float deltaTime)
{
    for (const std::shared_ptr<Layer> &layer : m_Layers)
        layer->OnUpdate(deltaTime);
}

void MCEngine::LayerStack::Render()
{
    for (const std::shared_ptr<Layer> &layer : m_Layers)
    {
        layer->OnRender();
        layer->OnImGuiRender();
    }
}