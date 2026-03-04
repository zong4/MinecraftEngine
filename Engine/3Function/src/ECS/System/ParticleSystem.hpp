#pragma once

#include "../../ResourcesManager/ShadersManager.hpp"
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

        auto &&view = registry.view<TransformComponent, ParticleComponent>();
        for (auto &&entity : view)
        {
            auto &&[transform, particle] = view.get<TransformComponent, ParticleComponent>(entity);
            particle.Update(deltaTime, &transform);
        }
    }
    void Render(entt::registry &registry) const
    {
        PROFILE_FUNCTION();

        auto &&shader = ShadersManager::GetInstance().GetShader("Particle");
        shader->Bind();
        auto &&view = registry.view<ParticleComponent>();
        for (auto &&entity : view)
        {
            auto &&particle = view.get<ParticleComponent>(entity);
            shader->SetUniformVec4("u_Color", particle.Color);
            shader->SetUniformInt("u_PointSize", particle.m_PointSize);
            particle.Render();
        }
        shader->Unbind();
    }
};

} // namespace Engine