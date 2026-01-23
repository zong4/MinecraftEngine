#pragma once

#include "../Component/ParticleComponent.hpp"
#include <entt/entt.hpp>

namespace Engine
{

class ParticleSystem
{

public:
    static ParticleSystem &GetInstance();

public:
    void Update(entt::registry &registry, float deltaTime);
    void Render(entt::registry &registry) const;

private:
    ParticleSystem() = default;
    ~ParticleSystem() = default;
};

} // namespace Engine