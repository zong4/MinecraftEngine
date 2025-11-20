#pragma once

#include <Function.hpp>

namespace Editor
{

class Sandbox : public MCEngine::Application
{
public:
    Sandbox(const MCEngine::WindowProperty &props);
    ~Sandbox() override = default;
};

} // namespace Editor