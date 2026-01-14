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
    void OnPostUpdate() override;
};

} // namespace Engine