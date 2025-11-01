#pragma once

#include <Function.hpp>

namespace MCEditor
{

class ExampleScene : public MCEngine::Scene
{
public:
    ExampleScene();
    ~ExampleScene() override = default;

private:
    void WelcomeToMCEngine();
};

} // namespace MCEditor