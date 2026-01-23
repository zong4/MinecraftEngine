#pragma once

#include "../Component/CameraComponent.hpp"
#include "../Component/TransformComponent.hpp"

namespace Engine
{

class CameraSystem
{
public:
    static CameraSystem &GetInstance();

public:
    void Resize(entt::registry &registry, int width, int height);
    void Update(entt::registry &registry);
    void Upload(entt::registry &registry);
};

} // namespace Engine