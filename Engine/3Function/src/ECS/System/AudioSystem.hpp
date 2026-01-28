#pragma once

#include "../Component/AudioComponent.hpp"
#include "../Component/TransformComponent.hpp"
#include "../Entity/Entity.hpp"

namespace Engine
{

class AudioSystem
{
public:
    AudioSystem() = default;
    ~AudioSystem() = default;

    // Setters
    void Delete(const Entity &entity) { entity.GetComponent<AudioComponent>()->Stop(); }

public:
    void Update(entt::registry &registry)
    {
        PROFILE_FUNCTION();

        // Update after TransformSystem
        auto &&view = registry.view<TransformComponent, AudioComponent>();
        for (auto &&entity : view)
        {
            auto &&[transform, audio] = view.get<TransformComponent, AudioComponent>(entity);
            audio.SetPosition(transform.GetGlobalPosition());
        }
    }
    void Shutdown(entt::registry &registry)
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
};

} // namespace Engine