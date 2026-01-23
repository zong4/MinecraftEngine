#include "AudioSystem.hpp"

Engine::AudioSystem &Engine::AudioSystem::GetInstance()
{
    static AudioSystem instance;
    return instance;
}

void Engine::AudioSystem::Update(entt::registry &registry)
{
    PROFILE_FUNCTION();

    auto &&view = registry.view<TransformComponent, AudioComponent>();
    for (auto &&entity : view)
    {
        auto &&[transform, audio] = view.get<TransformComponent, AudioComponent>(entity);
        audio.SetPosition(transform.GetGlobaldPosition());
    }
}

void Engine::AudioSystem::Clear(entt::registry &registry)
{
    PROFILE_FUNCTION();

    // Stop all audio
    auto &&view = registry.view<AudioComponent>();
    for (auto &&entity : view)
    {
        auto &&audio = view.get<AudioComponent>(entity);
        audio.Stop();
    }
}