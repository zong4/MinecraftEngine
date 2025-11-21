#pragma once

#include <Function.hpp>

namespace Editor
{

// How about Creator?
class Sandbox : public Engine::Application
{
public:
    Sandbox(const Engine::WindowProperty &props);
    ~Sandbox() override;
};

} // namespace Editor