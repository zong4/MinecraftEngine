#pragma once

#include <Function.hpp>

namespace Editor
{

class Sandbox : public Engine::Application
{
public:
    Sandbox(const Engine::WindowProperty &props);
    ~Sandbox() override = default;
};

} // namespace Editor