#pragma once

#include <Function.hpp>

namespace Editor
{

class Creator : public Engine::Application
{
public:
    Creator(const Engine::WindowProperty &props);
    ~Creator() override;
};

} // namespace Editor