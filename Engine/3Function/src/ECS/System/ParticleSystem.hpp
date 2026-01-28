#pragma once

#include "../../AssetsManager/ShadersManager.hpp"
#include "../Component/ParticleComponent.hpp"
#include <entt/entt.hpp>

namespace Engine
{

class ParticleSystem
{

public:
    ParticleSystem() = default;
    ~ParticleSystem() = default;

public:
    void Update(entt::registry &registry, float deltaTime)
    {
        PROFILE_FUNCTION();

        auto &&view = registry.view<ParticleComponent>();
        for (auto &&entity : view)
        {
            auto &&particle = view.get<ParticleComponent>(entity);
            particle.Update(deltaTime);
        }
    }
    void Render(entt::registry &registry) const
    {
        PROFILE_FUNCTION();

        auto &&shader = ShadersManager::GetInstance().GetShader("Particles");
        shader->Bind();
        auto &&view = registry.view<ParticleComponent>();
        for (auto &&entity : view)
        {
            auto &&particle = view.get<ParticleComponent>(entity);
            particle.Render();
        }
        shader->Unbind();
    }
};

} // namespace Engine