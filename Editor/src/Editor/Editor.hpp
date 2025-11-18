#pragma once

#include <Function.hpp>

namespace MCEditor
{

class Editor : public MCEngine::Application
{
public:
    Editor(const MCEngine::WindowProperty &props);
    ~Editor() override = default;
};

} // namespace MCEditor