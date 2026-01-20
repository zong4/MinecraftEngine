#pragma once

#include "Layer.hpp"
#include <algorithm>
#include <vector>

namespace Engine
{

class LayerStack
{
public:
    LayerStack() = default;
    ~LayerStack();

    // Getters
    std::shared_ptr<Layer> GetTopLayer() const;

    // Setters
    void PushLayer(const std::shared_ptr<Layer> &layer);
    void PopLayer(const std::shared_ptr<Layer> &layer);

public:
    void OnEvent(Event &event);
    void Update(float deltaTime);
    void Render();
    void PostUpdate();

private:
    std::vector<std::shared_ptr<Layer>> m_Layers;
};

} // namespace Engine