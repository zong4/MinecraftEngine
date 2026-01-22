#pragma once

#include "../Component/AudioComponent.hpp"
#include "../Component/TransformComponent.hpp"
#include <entt/entt.hpp>

namespace Engine
{

class AudioSystem
{
public:
    static AudioSystem &GetInstance()
    {
        static AudioSystem instance;
        return instance;
    }

public:
    void Update(entt::registry &registry)
    {
        PROFILE_FUNCTION();

        auto &&view = registry.view<TransformComponent, AudioComponent>();
        for (auto &&entity : view)
        {
            auto &&[transform, audio] = view.get<TransformComponent, AudioComponent>(entity);
            audio.SetPosition(transform.GetWorldPosition());
        }
    }
    void Clear(entt::registry &registry)
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

private:
    AudioSystem() = default;
    ~AudioSystem() = default;
};

} // namespace Engine