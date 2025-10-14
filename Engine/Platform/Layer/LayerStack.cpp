#include "LayerStack.hpp"

MCEngine::LayerStack::~LayerStack()
{
    ENGINE_PROFILE_FUNCTION();

    for (const std::shared_ptr<Layer> &layer : m_Layers)
    {
        layer->OnDetach();
    }
    m_Layers.clear();
}

void MCEngine::LayerStack::PushLayer(const std::shared_ptr<Layer> &layer)
{
    ENGINE_PROFILE_FUNCTION();

    m_Layers.emplace_back(layer);
    layer->OnAttach();
    LOG_ENGINE_INFO("Layer pushed: " + layer->GetName());
}

void MCEngine::LayerStack::PopLayer(const std::shared_ptr<Layer> &layer)
{
    ENGINE_PROFILE_FUNCTION();

    auto &&it = std::find(m_Layers.begin(), m_Layers.end(), layer);
    if (it != m_Layers.end())
    {
        it->get()->OnDetach();
        m_Layers.erase(it);
        LOG_ENGINE_INFO("Layer popped: " + layer->GetName());
        return;
    }
    LOG_ENGINE_WARN("Layer not found: " + layer->GetName());
}

void MCEngine::LayerStack::OnEvent(Event &event)
{
    ENGINE_PROFILE_FUNCTION();

    for (auto &&it = m_Layers.rbegin(); it != m_Layers.rend(); it++)
    {
        if (event.IsHandled()) // So do not need to check it again in each layer
            break;
        it->get()->OnEvent(event);
    }
}

void MCEngine::LayerStack::Update(float deltaTime)
{
    ENGINE_PROFILE_FUNCTION();

    for (const std::shared_ptr<Layer> &layer : m_Layers)
    {
        layer->OnUpdate(deltaTime);
    }
}

void MCEngine::LayerStack::Render()
{
    ENGINE_PROFILE_FUNCTION();

    for (const std::shared_ptr<Layer> &layer : m_Layers)
    {
        layer->OnRender();
        layer->OnImGuiRender();
    }
}