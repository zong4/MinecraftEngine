#pragma once

#include "../Component/LightComponent.hpp"
#include "../Component/TransformComponent.hpp"
#include <entt/entt.hpp>

namespace Engine
{

class LightSystem
{
public:
    static LightSystem &GetInstance();

public:
    void Resize(entt::registry &registry, int width, int height);
    void Render(entt::registry &registry) const;

private:
    LightSystem() = default;
    ~LightSystem() = default;
};

} // namespace Engine