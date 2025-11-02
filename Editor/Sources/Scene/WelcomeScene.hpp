#pragma once

#include <Function.hpp>

namespace MCEditor
{

class WelcomeScene : public MCEngine::Scene
{
public:
    WelcomeScene();
    ~WelcomeScene() override = default;

private:
    void MCEngineLogo();
};

} // namespace MCEditor