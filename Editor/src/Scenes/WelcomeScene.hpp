#pragma once

#include <Function.hpp>

namespace Editor
{

class WelcomeScene : public Engine::Scene3D
{
public:
    WelcomeScene();
    ~WelcomeScene() override = default;

private:
    void MCEngineLogo();
};

} // namespace Editor