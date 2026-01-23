#pragma once

#include "../Component/AudioComponent.hpp"
#include "../Component/TransformComponent.hpp"
#include <entt/entt.hpp>

namespace Engine
{

class AudioSystem
{
public:
    static AudioSystem &GetInstance();

public:
    void Update(entt::registry &registry);
    void Clear(entt::registry &registry);

private:
    AudioSystem() = default;
    ~AudioSystem() = default;
};

} // namespace Engine