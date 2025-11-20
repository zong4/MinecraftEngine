#pragma once

#include <Function.hpp>

namespace Editor
{

class WelcomeScene : public MCEngine::Scene
{
public:
    WelcomeScene();
    ~WelcomeScene() override = default;

private:
    void MCEngineLogo();
};

} // namespace Editor