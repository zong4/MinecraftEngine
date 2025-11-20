#pragma once

#include "Input.hpp"

namespace Engine
{

class InputLayer : public Layer
{
public:
    InputLayer() : Layer("InputLayer") {}
    ~InputLayer() override = default;

public:
    void OnEvent(Event &event) override;
    void OnUpdate(float deltaTime) override {}
    void OnPostUpdate() override;
    void OnRender() override {}
    void OnImGuiRender() override {}
};

} // namespace Engine