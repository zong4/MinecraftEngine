#include "ParticleSystem.hpp"

#include "../../AssetsManager/ShadersManager.hpp"

Engine::ParticleSystem &Engine::ParticleSystem::GetInstance()
{
    static ParticleSystem instance;
    return instance;
}

void Engine::ParticleSystem::Update(entt::registry &registry, float deltaTime)
{
    PROFILE_FUNCTION();

    auto &&view = registry.view<ParticleComponent>();
    for (auto &&entity : view)
    {
        auto &&particle = view.get<ParticleComponent>(entity);
        particle.Update(deltaTime);
    }
}

void Engine::ParticleSystem::Render(entt::registry &registry) const
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