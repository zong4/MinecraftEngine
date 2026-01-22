#pragma once

#include "Scene.hpp"

namespace Engine
{

class Scene2D : public Scene
{
public:
    Scene2D(const std::string &name = "Untitled") : Scene(name) {}
    virtual ~Scene2D() override = default;
};

} // namespace Engine
